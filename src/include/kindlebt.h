#ifndef KINDLE_BT_H
#define KINDLE_BT_H

#include <stdbool.h>

#include "kindlebt_defines.h"
#include "kindlebt_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

bool isBLESupported(void);

status_t enableRadio(sessionHandle session_handle);
status_t disableRadio(sessionHandle session_handle);
status_t getRadioState(state_t* p_out_state);

sessionType_t getSupportedSession(void);
status_t openSession(sessionType_t session_type, sessionHandle* session_handle);
status_t closeSession(sessionHandle session_handle);

status_t bleRegister(sessionHandle session_handle);
status_t bleDeregister(sessionHandle session_handle);

status_t bleRegisterGattClient(sessionHandle session_handle);
status_t bleDeregisterGattClient(sessionHandle session_handle);

status_t bleConnect(
    sessionHandle session_handle, bleConnHandle* conn_handle, bdAddr_t* p_device,
    bleConnParam_t conn_param, bleConnRole_t conn_role, bleConnPriority_t conn_priority
);
status_t bleDisconnect(bleConnHandle conn_handle);

#ifdef __cplusplus
}
#endif

#endif // KINDLE_BT_H
