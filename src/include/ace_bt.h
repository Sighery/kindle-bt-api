#ifndef ACE_BT_H
#define ACE_BT_H

#include "ace_status.h"
#include "bluetooth_common_api.h"
#include "bluetooth_session_api.h"
#include "bluetooth_ble_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function for clients to identify if BLE is supported by the current
 * Bluetooth adapter.
 *
 * @return TRUE if BLE is supported by current adapter; FALSE otherwise
 */
bool aceBT_isBLESupported(void);

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

// /**
//  * @brief ble connection state changed callback\n
//  * Invoked on @ref aceBT_bleRegister
//  *
//  * @param[in] status ACEBT_STATUS_SUCCESS if success; error state otherwise
//  */
// typedef void (*on_ble_registered_callback)(ace_status_t status);

// /**
//  * @brief ble connection state changed callback\n
//  * Invoked on @ref aceBT_bleConnect, @ref aceBT_bleCancelConnect, @ref
//  * aceBT_bleDisconnect
//  * @param[in] state Connection state
//  * @param[in] status Appropriate gatt status code
//  * @param[in] conn_handle ble connection handle
//  * @param[in] p_addr BD Address of the device for connection state event
//  */
// typedef void (*on_ble_connection_state_changed_callback)(
//     aceBT_bleConnState_t state, aceBT_gattStatus_t status,
//     const aceBT_bleConnHandle conn_handle, aceBT_bdAddr_t* p_addr);

// /**
//  * @brief MTU Updated callback
//  *
//  * @param[in] status ACEBT_STATUS_SUCCESS if success; error state otherwise
//  * @param[in] conn_handle ble connection handle
//  * @param[in] mtu Updated MTU size
//  */
// typedef void (*on_ble_mtu_updated_callback)(ace_status_t status,
//                                             aceBT_bleConnHandle conn_handle,
//                                             int mtu);

// /** @brief Basic BLE GAP callback struct */
// typedef struct {
//     size_t size;                                  /**< Size*/
//     aceBT_commonCallbacks_t common_cbs;           /**< Common callbacks */
//     on_ble_registered_callback ble_registered_cb; /**< BLE registered callback*/
//     on_ble_connection_state_changed_callback
//         connection_state_change_cb; /**< Connection state changes callback */
//     on_ble_mtu_updated_callback
//         on_ble_mtu_updated_cb; /**< BLE MTU updated callback*/
// } aceBT_bleCallbacks_t;

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

// /**
//  * @brief Function to register BLE client API.
//  * Without having a successful registration no other API can be invoked.
//  * @ref on_ble_registered_callback is used to notify a succesfull registration
//  *
//  * @param[in] session_handle Session handle received when opening the session
//  * @param[in] callbacks callbacks
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleRegister(aceBT_sessionHandle session_handle,
//                                  aceBT_bleCallbacks_t* callbacks);


#ifdef __cplusplus
}
#endif

#endif  // ACE_BT_H
