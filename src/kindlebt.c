#include <pthread.h>
#include <stdbool.h>

#include "ace/bluetooth_ble_api.h"
#include "ace/bluetooth_ble_gatt_client_api.h"
#include "ace/bluetooth_common_api.h"
#include "ace/bluetooth_session_api.h"

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
        printf("Already registered BLE\n");
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

status_t bleRegisterGattClient(sessionHandle session_handle) {
    return aceBt_bleRegisterGattClient(session_handle, NULL, ACE_BT_BLE_APPID_GADGETS);
}

status_t bleDeregisterGattClient(sessionHandle session_handle) {
    return aceBT_bleDeRegisterGattClient(session_handle);
}

status_t bleConnect(
    sessionHandle session_handle, bleConnHandle* connHandle, bdAddr_t* p_device,
    bleConnParam_t conn_param, bleConnRole_t conn_role, bleConnPriority_t conn_priority
) {
    status_t conn_status =
        aceBt_bleConnect(session_handle, p_device, conn_param, conn_role, false, conn_priority);
    if (conn_status != ACE_STATUS_OK) return conn_status;

    status_t cond_status =
        waitForCondition(&callback_vars_lock, &callback_vars_cond, &callback_vars.gattc_connected);

    if (cond_status == ACE_STATUS_OK) *connHandle = conn_handle;

    return cond_status;
}

status_t bleDisconnect(bleConnHandle connHandle) { return aceBT_bleDisconnect(connHandle); }
