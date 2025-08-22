#include "kindlebt.h"

#include <pthread.h>
#include <stdbool.h>

#include <ace/bluetooth_ble_api.h>
#include <ace/bluetooth_ble_gatt_client_api.h>
#include <ace/bluetooth_common_api.h>
#include <ace/bluetooth_session_api.h>

#include "log.h"

#include "compat_ace.h"
#include "kindlebt_defines.h"

#include "kindlebt_callbacks.c"

static bleCallbacks_t ble_callbacks = {
    .size = sizeof(bleCallbacks_t),
    .common_cbs =
        {
            .size = sizeof(commonCallbacks_t),
            .adapter_state_cb = adapterStateCallback,
            .bond_state_cb = bondStateCallback,
        },
    .ble_registered_cb = bleRegCallback,
    .connection_state_change_cb = bleConnStateChangedCallback,
    .on_ble_mtu_updated_cb = bleMtuUpdatedCallback,
};

static bleGattClientCallbacks_t gatt_client_callbacks = {
    .size = sizeof(bleGattClientCallbacks_t),
    .on_ble_gattc_get_gatt_db_cb = bleGattcGetDbCallbackWrapper,
    .notify_characteristics_cb = bleGattcNotifyCharsCallbackWrapper,
    .on_ble_gattc_read_characteristics_cb = bleGattcReadCharsCallbackWrapper,
    .on_ble_gattc_write_characteristics_cb = bleGattcWriteCharsCallbackWrapper,
};

bool isBLESupported(void) { return aceBT_isBLESupported(); };

status_t enableRadio(sessionHandle session_handle) { return aceBT_enableRadio(session_handle); }
status_t disableRadio(sessionHandle session_handle) { return aceBT_disableRadio(session_handle); }
status_t getRadioState(state_t* p_out_state) { return aceBT_getRadioState(p_out_state); }

sessionType_t getSupportedSession(void) { return aceBT_getSupportedSession(); }

status_t openSession(sessionType_t session_type, sessionHandle* session_handle) {
    return aceBT_openSession(session_type, NULL, session_handle);
}

status_t closeSession(sessionHandle session_handle) { return aceBT_closeSession(session_handle); }

status_t bleRegister(sessionHandle session_handle) {
    if (callback_vars.ble_registered) {
        log_warn("BLE is already registered\n");
        return ACE_STATUS_OK;
    }

    status_t ble_status = aceBT_bleRegister(session_handle, &ble_callbacks);
    if (ble_status != ACE_STATUS_OK) return ble_status;

    return waitForCondition(
        &callback_vars_lock, &callback_vars_cond, &callback_vars.ble_registered
    );
}

status_t bleDeregister(sessionHandle session_handle) {
    status_t ble_status = aceBT_bleDeRegister(session_handle);
    if (ble_status != ACE_STATUS_OK) return ble_status;

    setCallbackVariable(
        &callback_vars_lock, &callback_vars_cond, &callback_vars.ble_registered, false
    );

    return ble_status;
}

status_t bleRegisterGattClient(sessionHandle session_handle, bleGattClientCallbacks_t* callbacks) {
    // If provided a callback struct, replace the local copy
    // Applications can also just set the application callback directly
    if (callbacks != NULL) {
        application_gatt_client_callbacks = *callbacks;
    }

    // Yes, I know
    log_error("Trying to detect ABI version");
    acebt_abi abi_version = acebt_abi_version();
    log_error("ABI version is %d", abi_version);

    log_error("Calling the pre 5.17.0 interface");
    status_t status = pre5170_bleRegisterGattClient(
        session_handle, &gatt_client_callbacks, ACE_BT_BLE_APPID_GADGETS
    );
    log_error("Result %d", status);

    // return aceBt_bleRegisterGattClient(
    //     session_handle, &gatt_client_callbacks, ACE_BT_BLE_APPID_GADGETS
    // );
}

status_t bleDeregisterGattClient(sessionHandle session_handle) {
    return aceBT_bleDeRegisterGattClient(session_handle);
}

status_t bleGetDatabase(bleConnHandle conn_handle, bleGattsService_t* p_gatt_service) {
    status_t db_status = aceBT_bleGetService(conn_handle);
    if (db_status != ACE_STATUS_OK) return db_status;

    status_t cond_status =
        waitForCondition(&callback_vars_lock, &callback_vars_cond, &callback_vars.got_gatt_db);

    if (cond_status == ACE_STATUS_OK) p_gatt_service = pGgatt_service;

    return cond_status;
}

status_t bleCloneGattService(
    bleGattsService_t** dst_gatt_service, const bleGattsService_t* src_gatt_service, int no_svc
) {
    return aceBT_bleCloneGattService(dst_gatt_service, src_gatt_service, no_svc);
}

status_t bleConnect(
    sessionHandle session_handle, bleConnHandle* conn_handle, bdAddr_t* p_device,
    bleConnParam_t conn_param, bleConnRole_t conn_role, bleConnPriority_t conn_priority
) {
    status_t conn_status =
        aceBt_bleConnect(session_handle, p_device, conn_param, conn_role, false, conn_priority);
    if (conn_status != ACE_STATUS_OK) return conn_status;

    status_t cond_status =
        waitForCondition(&callback_vars_lock, &callback_vars_cond, &callback_vars.gattc_connected);

    if (cond_status == ACE_STATUS_OK) *conn_handle = ble_conn_handle;

    return cond_status;
}

status_t bleDisconnect(bleConnHandle conn_handle) { return aceBT_bleDisconnect(conn_handle); }

status_t bleSetNotification(
    sessionHandle session_handle, bleConnHandle conn_handle,
    bleGattCharacteristicsValue_t chars_value, bool enable
) {
    return aceBT_bleSetNotification(session_handle, conn_handle, chars_value, enable);
}

status_t bleReadCharacteristic(
    sessionHandle session_handle, bleConnHandle conn_handle,
    bleGattCharacteristicsValue_t chars_value
) {
    return aceBT_bleReadCharacteristics(session_handle, conn_handle, chars_value);
}

status_t bleWriteCharacteristic(
    sessionHandle session_handle, bleConnHandle conn_handle,
    bleGattCharacteristicsValue_t* chars_value, responseType_t request_type
) {
    return aceBT_bleWriteCharacteristics(session_handle, conn_handle, chars_value, request_type);
}
