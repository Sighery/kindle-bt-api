/**
 * @file bluetooth_ble_api.h
 *
 * @brief ACE BLE interface provice interface to exercise BLE GAP functionality
 * on this platform
 */

#ifndef BT_BLE_API_H
#define BT_BLE_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
// #include <toolchain/toolchain.h>
#include "bluetooth_ble_defines.h"
#include "bluetooth_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BLE_CB BLE callbacks
 * @{
 * @ingroup ACE_BT_CB
 */

/**
 * @defgroup ACE_BLE_CB_COMMON BLE common callbacks
 * @{
 * @ingroup ACE_BLE_CB
 */

/**
 * @brief ble connection state changed callback\n
 * Invoked on @ref aceBT_bleRegister
 *
 * @param[in] status ACEBT_STATUS_SUCCESS if success; error state otherwise
 */
typedef void (*on_ble_registered_callback)(ace_status_t status);

/**
 * @brief ble connection state changed callback\n
 * Invoked on @ref aceBT_bleConnect, @ref aceBT_bleCancelConnect, @ref
 * aceBT_bleDisconnect
 * @param[in] state Connection state
 * @param[in] status Appropriate gatt status code
 * @param[in] conn_handle ble connection handle
 * @param[in] p_addr BD Address of the device for connection state event
 */
typedef void (*on_ble_connection_state_changed_callback)(
    aceBT_bleConnState_t state, aceBT_gattStatus_t status,
    const aceBT_bleConnHandle conn_handle, aceBT_bdAddr_t* p_addr);

/**
 * @brief MTU Updated callback
 *
 * @param[in] status ACEBT_STATUS_SUCCESS if success; error state otherwise
 * @param[in] conn_handle ble connection handle
 * @param[in] mtu Updated MTU size
 */
typedef void (*on_ble_mtu_updated_callback)(ace_status_t status,
                                            aceBT_bleConnHandle conn_handle,
                                            int mtu);

/** @brief Basic BLE GAP callback struct */
typedef struct {
    size_t size;                                  /**< Size*/
    aceBT_commonCallbacks_t common_cbs;           /**< Common callbacks */
    on_ble_registered_callback ble_registered_cb; /**< BLE registered callback*/
    on_ble_connection_state_changed_callback
        connection_state_change_cb; /**< Connection state changes callback */
    on_ble_mtu_updated_callback
        on_ble_mtu_updated_cb; /**< BLE MTU updated callback*/
} aceBT_bleCallbacks_t;

/** @} */
/** @} */

/**
 * @defgroup ACE_BLE_API BLE APIs
 * @brief APIs for invoking BLE functaionalities.
 * @{
 * @ingroup ACE_BT_API
 */

/**
 * @defgroup ACE_BLE_API_COMMON Common APIs
 * @{
 * @ingroup ACE_BLE_API
 */

/**
 * @brief Function to register BLE client API.
 * Without having a successful registration no other API can be invoked.
 * @ref on_ble_registered_callback is used to notify a succesfull registration
 *
 * @param[in] session_handle Session handle received when opening the session
 * @param[in] callbacks callbacks
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
 * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
 * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
 * @return @ref ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_bleRegister(aceBT_sessionHandle session_handle,
                               aceBT_bleCallbacks_t* callbacks);

// /**
//  * @brief Function to de-register
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_bleDeRegister(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function to create a BLE connection.
//  * Once the connection is created, this handle can be used for all further
//  * operations.\n
//  * Triggers @ref on_ble_connection_state_changed_callback
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] p_devices Bluetooth address of the device to which the connection
//  needs to be initiated
//  * @param[in] conn_priority pritority of the BLE connection
//  * @param[in] conn_role connection role
//  * @param[in] auto_connect If set to false, apps will be responsible for
//  * reconnection.\n
//  * If true, offload reconnection to controller. Note that this behavior depends
//  * on capability of the BT controller too.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  * @deprecated
//  * @see aceBt_bleConnect
//  */
// deprecated__ aceBT_status_t
// aceBT_bleConnect(aceBT_sessionHandle session_handle, aceBT_bdAddr_t* p_devices,
//                  aceBT_bleConnPriority_t conn_priority,
//                  aceBT_bleConnRole_t conn_role, bool auto_connect);

/**
 * @brief Create a BLE connection.
 * Once the connection is created, this handle can be used for all further
 * operations.\n
 * Triggers @ref on_ble_connection_state_changed_callback
 *
 * @param[in] session_handle Session handle received when opening the session
 * @param[in] p_device Bluetooth address of the device to which the connection
 needs to be initiated
 * @param[in] conn_param parameters of the BLE connection
 * @param[in] conn_role connection role
 * @param[in] auto_connect If set to false, apps will be responsible for
 * reconnection.\n
 * If true, offload reconnection to controller. Note that this behavior depends
 * on capability of the BT controller too.
 * @param[in] conn_priority Based on the connection priority and available
 * resources, the middleware will decide whether it can initiate the connection
 * or not.
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
 * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
 * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
 * @return @ref ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBt_bleConnect(aceBT_sessionHandle session_handle,
                              aceBT_bdAddr_t* p_device,
                              aceBt_bleConnParam_t conn_param,
                              aceBT_bleConnRole_t conn_role,
                              bool auto_connect,
                              aceBt_bleConnPriority_t conn_priority);

// /**
//  * @brief Function to cancel a BLE create connection\n
//  * Triggers @ref on_ble_connection_state_changed_callback
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] p_devices Bluetooth address of the device to which the connection
//  * needs to be initiated
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_bleCancelConnect(aceBT_sessionHandle session_handle,
//                                       aceBT_bdAddr_t* p_devices);

// /**
//  * @brief Function to disconnect BLE connection\n
//  * Triggers @ref on_ble_connection_state_changed_callback
//  *
//  * @param[in] conn_handle connection handle
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_bleDisconnect(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief Function to modify the connection priority
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] conn_handle connection handle
//  * @param[in] connection_priority pritority of the BLE connection
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  * @deprecated
//  * @see aceBt_bleSetConnectionParam
//  */
// deprecated__ aceBT_status_t aceBT_bleRequestConnectionPriority(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleConnPriority_t connection_priority);

// /**
//  * @deprecated
//  * @see aceBt_bleReadRssi
//  */
// deprecated__ aceBT_status_t aceBT_bleReadRssi(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief Function to get RSSI of connected BLE device
//  *
//  * @param[in] conn_handle Handle that was received during the BLE connection
//  * @param[out] rssi Retrieves RSSI value
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBt_bleReadRssi(aceBT_bleConnHandle conn_handle, int8_t* rssi);

// /**
//  * @brief Modify the connection parameters for a requested connection
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] p_addr BD address of the connection request
//  * @param[in] conn_param parameters of the BLE connection
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBt_bleSetConnParam(aceBT_sessionHandle session_handle,
//                                      const aceBT_bdAddr_t* p_addr,
//                                      aceBt_bleConnParam_t conn_param);

// /**
//  * @brief Modify the connection priority for a requested connection.
//  * This API requires ACEBT_POLICY_MANAGER to be defined
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] p_addr BD address of the connection request
//  * @param[in] conn_priority priority of the BLE connection
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if policy manager isn't supported
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBt_bleSetConnPriority(aceBT_sessionHandle session_handle,
//                                         const aceBT_bdAddr_t* p_addr,
//                                         aceBt_bleConnPriority_t conn_priority);

// /**
//  * @brief Function to create a BLE connection
//  *
//  * @param[in] conn_handle handle that was received during the BLE connection
//  * @return Connection states in aceBT_bleConnState_t
//  */
// aceBT_bleConnState_t aceBT_bleGetCurrentState(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief Request a change of MTU for the connection
//  *
//  * @param[in] conn_handle handle that was received during the BLE connection
//  * @param[in] mtu
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_bleRequestMtu(aceBT_bleConnHandle conn_handle, int mtu);

// /**
//  * @brief Request a the current MTU for the connection
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] conn_handle Connection handle
//  * @param[out] mtu maximum transmission unit
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getMtu(aceBT_sessionHandle session_handle,
//                             aceBT_bleConnHandle conn_handle, int* mtu);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BLE_API_H
