#include "compat_ace.h"

#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

#include "log.h"

acebt_abi acebt_abi_version(void) {
    static acebt_abi cached_abi = PRE_5170;
    static bool initialized = false;

    if (!initialized) {
        void* handle = dlopen(NULL, RTLD_LAZY);
        if (handle) {
            dlerror();
            void* sym = dlsym(handle, "aceBt_bleRegisterGattClient");
            if (sym) {
                cached_abi = SINCE_5170;
            }
        } else {
            fprintf(stderr, "dlopen failed: %s\n", dlerror());
        }
        initialized = true;
    }

    return cached_abi;
}

void hex_dump(const void* ptr, size_t size) {
    const unsigned char* data = (const unsigned char*)ptr;
    for (size_t i = 0; i < size; ++i) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Line break every 16 bytes
    }
    printf("\n");
}

void pre5170_gattc_cb_handler(aceAipc_parameter_t* task) {
    if (task == NULL) {
        log_info("COMPAT: Server handler callback: data is null");
        return;
    }

    printf("Dumping aceAipc_parameter_t memory:\n");
    hex_dump(task, sizeof(aceAipc_parameter_t));

    /* In AIPC callback this runs in server side callback context. Hence use the
       callback id to retrive the session info*/
    sessionHandle session_handle = getSessionForTask(task);
    bleGattClientCallbacks_t* p_client_callbacks =
        getBTClientData(session_handle, CALLBACK_INDEX_BLE_GATT_CLIENT);
    if (session_handle == NULL || p_client_callbacks == NULL) {
        log_error(
            "Error invalid handle, session %p callback %p", session_handle, p_client_callbacks
        );
        return;
    }

    // TODO: All the callback cases

    return;
}

status_t pre5170_bleRegisterGattClient(
    sessionHandle session_handle, bleGattClientCallbacks_t* callbacks, bleAppId_t app_id
) {
    status_t status;
    uint16_t mask = 0;
    aipcHandles_t aipc_handle;
    registerCbackGattcData_t manager_callbacks;

    status = getSessionInfo(session_handle, &aipc_handle);
    if (status != ACE_STATUS_OK) {
        log_error("COMPAT Couldn't get session info. Result %d", status);
        return ACE_STATUS_BAD_PARAM;
    }

    log_warn(
        "COMPAT aipc_handle {\n  callback_server_id: %u (0x%04x),\n  server_id: %u (0x%04x)\n}",
        aipc_handle.callback_server_id, aipc_handle.callback_server_id, aipc_handle.server_id,
        aipc_handle.server_id
    );

    if (callbacks == NULL || callbacks->size == 0) {
        mask = 0;
    } else {
        if (callbacks->on_ble_gattc_service_registered_cb != NULL) mask |= 0x01;
        if (callbacks->on_ble_gattc_service_discovered_cb != NULL) mask |= 0x02;
        if (callbacks->on_ble_gattc_read_characteristics_cb != NULL) mask |= 0x04;
        if (callbacks->on_ble_gattc_write_characteristics_cb != NULL) mask |= 0x08;
        if (callbacks->notify_characteristics_cb != NULL) mask |= 0x10;
        if (callbacks->on_ble_gattc_write_descriptor_cb != NULL) mask |= 0x20;
        if (callbacks->on_ble_gattc_read_descriptor_cb != NULL) mask |= 0x40;
        if (callbacks->on_ble_gattc_get_gatt_db_cb != NULL) mask |= 0x80;
        if (callbacks->on_ble_gattc_execute_write_cb != NULL) mask |= 0x100;
    }

    uint32_t packed_aipc_handle =
        ((uint32_t)aipc_handle.server_id << 16) | ((uint32_t)aipc_handle.callback_server_id);
    aceBt_serializeGattcRegisterData(&manager_callbacks, packed_aipc_handle, mask, app_id);
    log_info(
        "COMPAT [aceBt_bleRegisterGattClient()]: Register GATTS Client session handle %p",
        session_handle
    );
    registerBTClientData(session_handle, CALLBACK_INDEX_BLE_GATT_CLIENT, callbacks);
    registerBTEvtHandler(
        session_handle, pre5170_gattc_cb_handler, ACE_BT_CALLBACK_PM_CONN_LIST,
        ACE_BT_CALLBACK_PM_MAX
    );

    status_t aipc_status = aipc_invoke_sync_call(
        session_handle, ACE_BT_BLE_REGISTER_GATT_CLIENT_API, &manager_callbacks,
        manager_callbacks.size
    );
    if (aipc_status != ACE_STATUS_OK) {
        log_info(
            "COMPAT [aceBt_bleRegisterGattClient()]: fail to register gatt client callbacks with "
            "server! result: %d",
            aipc_status
        );
        registerBTClientData(session_handle, CALLBACK_INDEX_BLE_GATT_CLIENT, NULL);
        registerBTEvtHandler(
            session_handle, NULL, ACE_BT_CALLBACK_PM_CONN_LIST, ACE_BT_CALLBACK_PM_MAX
        );
    }

    return status;
}
