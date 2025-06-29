/**
 * @file bluetooth_api.h
 *
 * @brief ACE Bluetooth APIs provides the interfaces to use Bluetooth
 * functionality on the current platform. The API supports multiple
 * clients in allowing them to concurrently access Bluetooth APIs.
 *
 * USAGE
 * -----
 * A client starts by opening a Bluetooth classic or BLE or Classic/BLE session
 * via @ref aceBT_openSession() call which returns a session handle. This handle
 * will be used by the client to invoke some of the Bluetooth APIs.
 *
 * Before enabling the Bluetooth radio, check the radio state using
 * @ref aceBT_getRadioState(). This is to make sure that the clients take
 * into account that other clients can enable Bluetooth radio before
 * they can initialize.
 *
 * Most of the Bluetooth APIs return the status of the call. But the actual
 * response of the APIs are asynchronously dispatched to the respective
 * clients via registered callbacks.
 *
 * Clients need to register for @ref aceBT_registerClientCallbacks(). In order
 * to be able to receive bonding callbacks, clients will also
 * register for @ref aceBT_registerAsSecurityClient().
 */
#ifndef BT_API_H
#define BT_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "bluetooth_defines.h"
#include "bluetooth_session_api.h"
#include "bluetooth_common_api.h"
// #include "bluetooth_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BT_CB_CLASSIC Bluetooth Classic callbacks
 * @{
 * @ingroup ACE_BT_CB
 */

/**
 * @brief Bluetooth adapter's discovery state callback.\n
 * Triggered on @ref aceBT_startDeviceDiscovery or
 * @ref aceBT_stopDeviceDiscovery
 *
 * @param[in] state Discovery state @ref aceBT_discoveryState_t
 */
typedef void (*adapter_discovery_state_callback)(aceBT_discoveryState_t state);

/**
 * @brief Bluetooth adapter's scan mode changed callback.\n
 * Triggered on @ref aceBT_setScanMode
 *
 * @param[in] scan_mode Scan mode @ref aceBT_scanMode_t
 */
typedef void (*adapter_scanmode_changed_callback)(aceBT_scanMode_t scan_mode);

/**
 * @brief Remote device connection state callback. This is invoked once all the
 * profiles are connected
 *
 * \dot
 * digraph states {
 *     graph [ranksep="1.25", nodesep="1.15"];
 *     forcelabel=true;
 *     ACEBT_CONN_STATE_CONNECTED -> ACEBT_CONN_STATE_DISCONNECTED [xlabel =
 * "Last profile\ndisconnected"];
 *     ACEBT_CONN_STATE_DISCONNECTED -> ACEBT_CONN_STATE_CONNECTED [xlabel =
 * "First profile\nconnected"];
 * }
 * \enddot
 * @param[in] status Status of device connection. @ref aceBT_status_t
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] state Connection state @ref aceBT_connState_t
 */
typedef void (*conn_state_callback)(ace_status_t status,
                                    aceBT_bdAddr_t* p_remote_addr,
                                    aceBT_connState_t state);

/**
 * @brief Remote device Profile connection state callback
 *
 * \dot
 *  digraph remote {
 *     graph [ranksep="1.3", nodesep="1.25"];
 *     label="Remote Device Initiated Connection";
 *     a [label="ACEBT_CONN_STATE_CONNECTED"];
 *     b [label="ACEBT_CONN_STATE_DISCONNECTED"];
 *     c [label="ACEBT_CONN_STATE_CONNECTING"];
 *
 *     forcelabel=true;
 *     b->c [xlabel = "Remote device is attempting to connect a profile"];
 *     c->b [xlabel = "Profile setup failed"];
 *     c->a [xlabel = "Profile setup succeeded, \nready for use"];
 *     a->b [xlabel = "Profile is no longer valid\ndisconnection or profile
 * error"]
 *     { rank=same; b,c }
 * }
 * \enddot
 * \dot
 * digraph local {
 *     graph [ranksep="1.3", nodesep="1.25"];
 *     label="Local Device Initiated Connection (DUT)";
 *     d [label="ACEBT_CONN_STATE_CONNECTED"];
 *     e [label="ACEBT_CONN_STATE_DISCONNECTED"];
 *     f [label="ACEBT_CONN_STATE_CONNECTING"];
 *
 *     forcelabel=true;
 *     e->f [xlabel = "DUT is attempting to connect a profile\nif ACL is not
 * established,\nthis will establish ACL"];
 *     f->e [xlabel = "Profile setup failed\nor Remote Device out-of-range"];
 *     f->d [xlabel = "Profile setup succeeded\nready for use"];
 *     d->e [xlabel = "Profile is no longer valid\ndisconnection or profile
 * error"]
 *     { rank=same; e,f }
 * }
 * \enddot
 * @param[in] status Status of profile connect @ref aceBT_status_t
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] profile The local device's profile @ref aceBT_profile_t
 * @param[in] state Connection state @ref aceBT_connState_t
 */
typedef void (*profile_state_callback)(ace_status_t status,
                                       aceBT_bdAddr_t* p_remote_addr,
                                       aceBT_profile_t profile,
                                       aceBT_connState_t state);

/**
 * @brief Audio state changed callback. This callback will be sent
 * for both A2DP Source and Sink profile.
 *
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] state Audio state @ref aceBT_audioState_t
 */
typedef void (*audio_state_callback)(aceBT_bdAddr_t* p_remote_addr,
                                     aceBT_profile_t profile,
                                     aceBT_audioState_t state);

/**
 * @brief Callback for new device discovered. Included properties:\n
 *  ACEBT_PROPERTY_BDNAME (optional)\n
 *  ACEBT_PROPERTY_BDADDR\n
 *  ACEBT_PROPERTY_EIR_MANF_INFO or ACEBT_PROPERTY_GADGET_EIR_MANF_INFO
 * (optional)\n
 *  ACEBT_PROPERTY_CLASS_OF_DEVICE (optional)\n
 *  ACEBT_PROPERTY_TYPE_OF_DEVICE (optional)
 *
 * @param[in] p_remote_addr pointer to remote device address @ref aceBT_bdAddr_t
 * @param[in] prop_count Count of properties being returned
 * @param[in] p_properties pointer to array of @ref aceBT_property_t
 */
typedef void (*device_discovered_callback)(aceBT_bdAddr_t* p_remote_addr,
                                           int prop_count,
                                           aceBT_property_t* p_properties);

/**
 *@brief Defines the callback pointers for Classic Bluetooth events.
 */
typedef struct {
    size_t size; /**< Set to size of @ref aceBT_classicCallbacks_t */
    adapter_discovery_state_callback
        adapter_discovery_state_cb; /**< BT Discovery state callback */
    adapter_scanmode_changed_callback
        adapter_scanmode_cb;           /**< Scan mode changed callback */
    conn_state_callback conn_state_cb; /**< Connection state callback */
    device_discovered_callback
        device_discovered_cb;                /**< Device discovered callback */
    profile_state_callback profile_state_cb; /**< Profile state callback */
    audio_state_callback audio_state_cb;     /**< Audio state callback */
} aceBT_classicCallbacks_t;

/**
 * @brief Bluetooth callback struct. This struct includes common and classic
 * callbacks for clients to register. Note that clients can choose to register
 * whichever callback event they need and setting rest of the pointers to NULL.
 */
typedef struct {
    size_t size; /**< Set to size of @ref aceBT_classicCallbacks_t */
    aceBT_commonCallbacks_t
        common_cbs; /**< Structure containing Common Bluetooth callbacks */
    aceBT_classicCallbacks_t
        classic_cbs; /**< Structure containing Classic Bluetooth callbacks*/
} aceBT_callbacks_t;

/** @} */

/**
 * @defgroup ACE_BT_API_CLASSIC Classic Bluetooth APIs
 * @{
 * @ingroup ACE_BT_API
 */
/**
 * @brief Function to register client callbacks. Clients can choose to decide
 * if they want to listen for the broadcast/multicast/unicast events (ie) -\n
 * a. Not register at all;\n
 * b. Register with a NULL pointer;\n
 * c. Set some of the callbacks as NULL in |p_callback|
 *
 * @param[in] session_handle session handle for the current Bluetooth session
 * @param[in] p_callbacks Pointer to client callbacks struct (@ref
 * aceBT_callbacks_t)
 * @return ACEBT_STATUS_SUCCESS if success
 * @return ACEBT_STATUS_NOMEM if ran out of memory
 * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
 * @return ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_registerClientCallbacks(aceBT_sessionHandle session_handle,
                                             aceBT_callbacks_t* p_callbacks);

// /**
//  * @brief Function to asynchronously start a Bluetooth device discovery
//  * procedure. The state of the discovery will be known by the clients via
//  * @ref adapter_discovery_state_cb(). The devices found via discovery will be
//  * sent as individual callbacks of @ref device_discovered_callback()
//  * passed as param.
//  *
//  * Clients should note that the @ref device_discovered_callback() will be
//  * invoked multiple times as the local Bluetooth device discovers each device.
//  * Also, the callback can also be triggered after stop discovery is called
//  * since RNR (HCI remote name request) can take longer sometimes.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  *
//  */
// aceBT_status_t aceBT_startDeviceDiscovery(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function to asynchronously stop the device discovery.
//  * The state of the discovery will be known by the clients via
//  * @ref adapter_discovery_state_cb().
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_stopDeviceDiscovery(aceBT_sessionHandle session_handle);

// /**
//  * @brief Function to set the scan mode for @ref ACEBT_SCAN_MODE_CONNECTABLE
//  * and/or @ref ACEBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE modes. Once the scan
//  * mode is changed by the HAL, @ref adapter_scanmode_changed_callback will be
//  * invoked.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @param[in] scan_mode aceBT_scanMode_t value
//  * @param[in] duration Duration for discoverability timeout. Should be > 0.
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_setScanMode(aceBT_sessionHandle session_handle,
//                                  aceBT_scanMode_t scan_mode, int duration);

// /**
//  * @brief Function to retrieve the current scan mode set.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @param[out] p_out_scan_mode Retrieves the scan mode @ref aceBT_scanMode_t
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getScanMode(aceBT_sessionHandle session_handle,
//                                  aceBT_scanMode_t* p_out_scan_mode);

// /**
//  * @brief Function to set the scan type set (@ref ACEBT_SCAN_TYPE_STANDARD or
//  * @ref ACEBT_SCAN_TYPE_INTERLACED) modes. There are no callbacks for setting
//  * the scan type.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @param[in] scan_type aceBT_scanType_t value
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_setScanType(aceBT_sessionHandle session_handle,
//                                  aceBT_scanType_t scan_type);

// /**
//  * @brief Function to retrieve the current scan type set.
//  *
//  * @param[in] session_handle session handle for the current Bluetooth session
//  * @param[out] p_out_scan_type Retrieves the @ref aceBT_scanType_t value
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getScanType(aceBT_sessionHandle session_handle,
//                                  aceBT_scanType_t* p_out_scan_type);

// /**
//  * @brief Function to identify all the supported Bluetooth profiles by the local
//  * adapter. The result is returned as a profile mask.
//  *
//  * @return int mask of all Bluetooth profiles currently supported by local
//  * adapter
//  */
// aceBT_profilesMask_t aceBT_getSupportedProfiles(void);

// /**
//  * @brief API to retrieve the UUIDs for the remote device. Note that this API
//  * will return the cached UUIDs from the previous SDP record and it will not
//  * explicitly invoke an SDP process again.
//  * @note If SDP is in progress, the API will return 0 count of UUIDs.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[in] buf_size Max Buffer size of p_uuids
//  * @param[out] p_uuids Array of aceBT_uuid_t
//  * @param[out] p_uuid_count Length of aceBT_uuid_t array
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_MAX_BUFF if reached buf_size
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getUuids(aceBT_bdAddr_t* p_remote_device, uint8_t buf_size,
//                               aceBT_uuid_t* p_uuids, uint8_t* p_uuid_count);

// /**
//  * @brief API to retrieve the number of UUIDs for the remote device.
//  * This API follows the behavior of @ref aceBT_getUuids and returns only
//  * the count of UUIDs.
//  *
//  * @param[in] p_remote_device MAC Address of remote device name
//  * @param[out] p_uuid_count Number of Uuids found for remote device
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getUuidsCount(aceBT_bdAddr_t* p_remote_device,
//                                    uint8_t* p_uuid_count);

// /**
//  * @brief API to retrieve manufacturer specific EIR information for a bonded
//  * remote device.
//  *
//  * @param[in] p_remote_device MAC Address of remote device
//  * @param[out] p_eir_manf EIR info @ref aceBT_eirManfInfo_t
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_UNSUPPORTED if does not support classic BT
//  * @return ACEBT_STATUS_NOT_FOUND if eir doesn't exist for remote device
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_getEirManfInfo(aceBT_bdAddr_t* p_remote_device,
//                                     aceBT_eirManfInfo_t* p_eir_manf);

// /**
//  * @brief Retrieves classic property for a bonded remote device.\n
//  * Supported property type @ref aceBT_propertyType_t:\n
//  * @ref ACEBT_PROPERTY_CLASS_OF_DEVICE (uint32_t)\n
//  *
//  * Caller is responsible for allocating sufficient memory for p_property->val
//  * based on property type (p_property->type). The allocated memory size should
//  * be assigned to p_property->len.
//  *
//  * @param[in] p_remote_device MAC address of remote device
//  * @param[in,out] p_property Classic property @ref aceBT_property_t
//  * p_property.
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_UNSUPPORTED if request provides unsupported property
//  * type
//  * @return ACEBT_STATUS_NOT_FOUND remote device not found
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBt_getClassicRemoteDeviceProperty(
//     const aceBT_bdAddr_t* p_remote_device, aceBT_property_t* p_property);

// /** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_API_H
