/**
 * @file bluetooth_common_api.h
 *
 * @brief ACE Bluetooth APIs provide the interfaces to use Bluetooth
 * functionality on the current platform.
 */

#ifndef BT_COMMON_API_H
#define BT_COMMON_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "bluetooth_defines.h"
#include "bluetooth_session_api.h"
// #include "bluetooth_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BT_CB_COMMON Bluetooth common callbacks
 * @brief The common Bluetooth callback typedefs used by clients to register
 * for common Bluetooth events.
 * @{
 * @ingroup ACE_BT_CB
 */

/**
 * @brief Bluetooth adapter state (ENABLE/DISABLE) callback\n
 * Triggered on @ref aceBT_enableRadio or @ref aceBT_disableRadio
 *
 * @param[in] state Adapter state @ref aceBT_state_t
 */
typedef void (*adapter_state_callback)(aceBT_state_t state);

/**
 * @brief Remote device bond state callback.\n
 * Triggered on @ref aceBT_pair, @ref aceBT_unpair, or @ref aceBT_cancelPair
 *
 * @param[in] status Status of device connection. @ref aceBT_status_t
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] state Bond state @ref aceBT_bondState_t
 */
typedef void (*bond_state_callback)(ace_status_t status,
                                    aceBT_bdAddr_t* p_remote_addr,
                                    aceBT_bondState_t state);

/**
 * @brief Callback for PIN request
 *
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] p_remote_name pointer to device name @ref aceBT_bdName_t
 * @param[in] cod Class of device
 * @param[in] is_min_16_digit_pin Whether PIN requested is 16 digit or not
 */
typedef void (*bond_pin_request_callback)(aceBT_bdAddr_t* p_remote_addr,
                                          aceBT_bdName_t* p_remote_name,
                                          int cod, bool is_min_16_digit_pin);

/**
 * @brief Callback for SSP (Secure Simple pairing) request
 *
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] p_remote_name pointer to device name @ref aceBT_bdName_t
 * @param[in] cod Class of device
 * @param[in] ssp_type SSP Type
 * @param[in] pass_key Pass key
 */
typedef void (*bond_ssp_request_callback)(aceBT_bdAddr_t* p_remote_addr,
                                          aceBT_bdName_t* p_remote_name,
                                          int cod, aceBT_sspVariant_t ssp_type,
                                          uint32_t pass_key);

/**
 * @brief Callback invoked in response to ACL connection state change.
 * @note Disconnect reason is only filled when state
 *        is @ref ACEBT_CONN_STATE_DISCONNECTED. Otherwise, it will be
 *        @ref ACE_BT_ACL_SUCCESS.
 * @warning If corresponding HAL callback @ref BTAclStateChangedCallback_t
 *          is not implemented and supported, then this callback will not
 *          be invoked.
 *
 * @param[in] status Returns ACEBT_STATUS_SUCCESS if operation succeeded.
 * @param[in] state ACL connection state.
 * @param[in] p_remote_addr Pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] data Contains additional data associated with ACL connection
 */
typedef void (*aceBt_aclStateChangedCallback_t)(
    ace_status_t status, aceBT_connState_t state,
    const aceBT_bdAddr_t* p_remote_addr, aceBt_aclData_t data);

/** @brief Common Bluetooth callback struct */
typedef struct {
    size_t size; /**< Set to size of @ref aceBT_commonCallbacks_t */
    adapter_state_callback adapter_state_cb; /**< Adapter state callback */
    bond_state_callback bond_state_cb;       /**< Bond state callback */
    aceBt_aclStateChangedCallback_t
        acl_state_changed_cb; /**< ACL state changed callback */
} aceBT_commonCallbacks_t;

/** @brief Bluetooth security callback struct */
typedef struct {
    size_t size; /**< Set to size of @ref aceBT_securityCallbacks_t */
    bond_pin_request_callback pin_req_cb; /**< PIN Request callback */
    bond_ssp_request_callback ssp_req_cb; /**< SSP Request callback */
} aceBT_securityCallbacks_t;

/** @} */

/** Local device management APIs */

/**
 * @defgroup ACE_BT_API_COMMON Common/Device-management APIs
 * @{
 * @ingroup ACE_BT_API
 */
/**
 * @brief Function to register as a privileged client for handling security
 * callbacks. Clients registering as security agents will receive callbacks when
 * a remote device attempts to bond.
 *
 * @param[in] session_handle session handle for the current Bluetooth session
 * @param[in] p_callbacks Pointer to struct of security related callbacks
 * @return ACEBT_STATUS_SUCCESS if success
 * @return ACEBT_STATUS_NOMEM if ran out of memory
 * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return ACEBT_STATUS_NOT_READY if server is not ready
 * @return ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_registerAsSecurityClient(
    aceBT_sessionHandle session_handle, aceBT_securityCallbacks_t* p_callbacks);

// /**
//  * @brief Function which turns on the Bluetooth radio asynchronously. If the API
//  * succeeds,
//  * it will immediately return a BT_SUCCESS status for the API invocation.
//  * The actual state is known by the clients via
//  * bt_callbacks_t->adapter_state_cb()
//  * callback.
//  *
//  * @note If the radio is already enabled and this API is called,
//  * ACEBT_STATUS_SUCCESS will be returned, however, the
//  * adapter_state_cb will not be sent to the client.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success or radio already enabled
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_enableRadio(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function which turns off the Bluetooth radio asynchronously. If the
//  * API succeeds, it will immediately return a BT_SUCCESS status for the API
//  * invocation. The actual state is known by the clients via
//  * bt_callbacks_t->adapter_state_cb()
//  * callback.
//  *
//  * @note If the radio is already disabled and this API is called,
//  * ACEBT_STATUS_SUCCESS will be returned, however, the
//  * adapter_state_cb will not be sent to the client.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success or radio already disabled
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_disableRadio(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function which turns on hci logging (btsnoop). If the API succeeds,
//  * it will immediately return a BT_SUCCESS status for the API invocation.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_enableHCI(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function which turns off hci logging (btsnoop). If the API succeeds,
//  * it will immediately return a BT_SUCCESS status for the API invocation.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_disableHCI(aceBT_sessionHandle session_handle);

/**
 * @brief Function to retrieve the current Bluetooth radio state. If the BT
 * manager is not ready or if IPC call fails, this API will return an
 * appropriate error. Otherwise, the adapter state will be filled and returned.
 *
 * @param[out] p_out_state Adapter state @ref aceBT_state_t
 * @return ACEBT_STATUS_SUCCESS if success
 * @return ACEBT_STATUS_NOMEM if ran out of memory
 * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return ACEBT_STATUS_NOT_READY if server is not ready
 * @return ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_getRadioState(aceBT_state_t* p_out_state);

// /**
//  * @brief Function to synchronously retrieves the count of bonded devices.
//  *
//  * @param[out] p_count Count of devices in BONDED state
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getBondedDevicesCount(uint16_t* p_count);

// /**
//  * @brief Function to synchronously retrieves the count of connected devices.
//  *
//  * @param[out] p_count int count of devices in CONNECTED state
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getConnectedDevicesCount(uint16_t* p_count);

// /**
//  * @brief Function to retrieve all bonded devices. Must call @ref
//  * aceBT_freeDeviceList after user is done with list. If this function returns
//  * anything other than ACEBT_STATUS_SUCCESS, *p_bonded_devices will point to
//  * invalid memory.
//  *
//  * @param[out] p_bonded_devices Pointer to pointer of struct of list of devices
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getBondedDevices(aceBT_deviceList_t** p_bonded_devices);

// /**
//  * @brief Function to retrieve all connected devices. Must call @ref
//  * aceBT_freeDeviceList after user is done with list.
//  * If this function returns anything other than ACEBT_STATUS_SUCCESS,
//  * *p_connected_devices will point to invalid memory.
//  *
//  * @param[out] p_connected_devices Pointer to pointer of struct of list of
//  * devices
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getConnectedDevices(
//     aceBT_deviceList_t** p_connected_devices);

// /**
//  * @brief Function to free Device List for @ref aceBT_getBondedDevices and @ref
//  * aceBT_getConnectedDevices
//  *
//  * @param[in] p_device_list Pointer to struct of list of devices to free
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_freeDeviceList(aceBT_deviceList_t* p_device_list);

// /**
//  * @brief API to retrieve the bond state of the remote device for a given
//  * transport
//  *
//  * @param[in] p_remote_device MAC Address of remote device
//  * @param[out] p_bond_state Bond state
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getBondState(aceBT_bdAddr_t* p_remote_device,
//                                   aceBT_bondState_t* p_bond_state);

// /**
//  * @brief API to retrieve the connection state of the remote device.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[out] p_conn_state Connection state
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getConnectionState(aceBT_bdAddr_t* p_remote_device,
//                                         aceBT_connState_t* p_conn_state);

// /*
//  * @brief API to read the rssi of a remote device.
//  * Currently rssi will be logged in logcat.
//  *
//  * @param[in] p_remote_device MAC Address of remote device
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_readRssi(aceBT_bdAddr_t* p_remote_device);

// /**
//  * @brief Function for clients to identify if BLE is supported by the current
//  * Bluetooth adapter.
//  *
//  * @return TRUE if BLE is supported by current adapter; FALSE otherwise
//  */
// bool aceBT_isBLESupported(void);

// /** Remote device management APIs */

// /**
//  * @brief API to retrieve the local name of the Bluetooth device. Clients
//  * can change the name of the Bluetooth device in local storage.
//  *
//  * @param[out] p_device_name Device name
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getAdapterName(aceBT_bdName_t* p_device_name);

// /**
//  * @brief API to retrieve the properties of the local Bluetooth device.
//  *
//  * @param[out] p_device_prop structure
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getAdapterProperties(
//     aceBT_deviceProperties_t* p_device_prop);

// /**
//  * @brief API to set a new local name for the remote Bluetooth device.
//  *
//  * @param[in] p_device_name Device name to be set
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_setAdapterName(aceBT_bdName_t* p_device_name);

// /**
//  * @brief API to initiate a bond to the remote Bluetooth device on a given
//  * transport. Once bonding is done,the Bluetooth middleware may proceed to do
//  * the profile connections as well if the bonding is done over classic (BR/EDR)
//  * transport.
//  * The bond state and the connection state events are broadcasted to all clients
//  * registered in aceBT_openSession() call.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[in] transport Transport type for pairing
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_DONE if remote device is already bonded
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_pair(aceBT_bdAddr_t* p_remote_device,
//                           aceBT_transportType_t transport);

// /**
//  * @brief API to unpair a Bluetooth device. This will disconnect the existing
//  * link and remove the link key associated with the device. This API will
//  * asynchronously send the unbond event to bt_callbacks_t->bond_state_cb().
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_unpair(aceBT_bdAddr_t* p_remote_device);

// /**
//  * @brief API to cancel an on-going bonding process for the remote Bluetooth
//  * device. This API will asynchronously send the unbond event to
//  * bt_callbacks_t->bond_state_cb(bt_addr_t bd_addr).
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_cancelPair(aceBT_bdAddr_t* p_remote_device);

// /**
//  * @brief API to set the PIN for bonding a device. When the client recieves the
//  * bond_pin_request_callback(), it can respond with a valid PIN by
//  * invoking this API.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[in] is_accepted bool Whether incoming command is accepted
//  * @param[in] length number of digits in PIN Code (MAX is 16)
//  * @param[in] p_pincode aceBT_pinCode_t PIN Code
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_setPairingPin(aceBT_bdAddr_t* p_remote_device,
//                                    bool is_accepted, uint8_t length,
//                                    aceBT_pinCode_t* p_pincode);

// /**
//  * @brief API to set the SSP reply for bonding a device. When the client
//  * receives the bond_ssp_request_callback(), it can respond with a valid PIN by
//  * invoking this API.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[in] is_accepted bool Whether incoming command is accepted
//  * @param[in] variant SSP variant defined in aceBT_sspVariant_t
//  * @param[in] passkey uint32_t passkey
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_setSSPReply(aceBT_bdAddr_t* p_remote_device,
//                                  bool is_accepted, aceBT_sspVariant_t variant,
//                                  uint32_t passkey);

// /**
//  * @brief API to accept pairing confirmation. When the client receives the
//  * bond_ssp_request_callback(), it can respond by calling this API.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[in] is_accepted bool TRUE if pairing is being accepted; FALSE
//  * otherwise
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_acceptPairingConfirmation(aceBT_bdAddr_t* p_remote_device,
//                                                bool is_accepted);

// /**
//  * @brief API to retrieve the original name of the Bluetooth device.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[out] p_remote_device_name Remote device name
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getName(aceBT_bdAddr_t* p_remote_device,
//                              aceBT_bdName_t* p_remote_device_name);

// /**
//  * @brief API to retrieve device type of a bonded remote device.
//  *
//  * @param[in] p_remote_device MAC Address of remote device
//  * @param[out] p_device_type device type (BREDR/BLE/Dual)
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_NOT_FOUND if eir doesn't exist for remote device
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getDeviceType(aceBT_bdAddr_t* p_remote_device,
//                                    aceBT_deviceType_t* p_device_type);

// /**
//  * @brief API to retrieve the device properties of the Bluetooth device.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[out] p_remote_device_prop Out param to fill remote device properties
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getDeviceProperties(
//     aceBT_bdAddr_t* p_remote_device,
//     aceBT_deviceProperties_t* p_remote_device_prop);

// /** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_COMMON_API_H
