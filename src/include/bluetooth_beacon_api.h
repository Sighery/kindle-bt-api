/**
 * @file bluetooth_beacon_api.h
 *
 * @brief ACE Bluetooth Beacon Manager APIs provide APIs to advertise and listen
 * to Bluetooth LE beacons
 */

#ifndef BT_BEACON_API_H
#define BT_BEACON_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "bluetooth_beacon_clients.h"
#include "bluetooth_defines.h"
#include "bluetooth_beacon_defines.h"
#include "bluetooth_session_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BLE_DS_BEACON_1 Beacon opaque handle definitions
 * @{
 * @ingroup ACE_BLE_DS_BEACON
 */

// /**
//  * @name Beacon Features Checkers
//  * @brief Macros to check support for individual beacon features
//  * @{
//  */
// /** Check whether multi advertisement is supported */
// #define ACEBT_BEACONMGR_IS_MULTI_ADV_SUPPORTED(features) \
//     features& ACEBT_BEACON_MGR_MULTI_ADV_SUPPORTED
// /** Check whether extended advertisement is supported */
// #define ACEBT_BEACONMGR_IS_EXTENDED_ADV_SUPPORTED(features) \
//     features& ACEBT_BEACON_MGR_EXTENDED_ADV_SUPPORTED
// /** Check whether periodic advertisement is supported */
// #define ACEBT_BEACONMGR_IS_PERIODIC_ADV_SUPPORTED(features) \
//     features& ACEBT_BEACON_MGR_PERIODIC_ADV_SUPPORTED
// /** @} */

// /** @brief Max beacon per instance */
// #define ACEBT_MAX_BEACON_SUPPORTED 1

/** @brief Opaque handle representing a BLE Advertising instance */
typedef struct aceBT_AdvInstance* aceBT_advInstanceHandle;

/** @brief Opaque handle representing a BLE Advertising instance  */
typedef struct aceBT_ScanInstance* aceBT_scanInstanceHandle;

/** @} */

/**
 * @defgroup ACE_BLE_CB_BEACON BLE Beacon callbacks
 * @{
 * @ingroup ACE_BLE_CB
 */

/**
 * @brief callback to notifiy a change in advertisment instance\n
 * Invoked on @ref aceBT_startBeacon, @ref aceBT_startBeaconWithScanResponse,
 * and @ref aceBT_stopBeacon
 *
 * @param[in] adv_instance Advertisement instance
 * @param[in] state Current advertisement state
 * @param[in] power_mode Current power mode used for this advertisement
 * @param[in] beacon_mode Beacon mode in which this adv instance is being
 * broadcasted
 */
typedef void (*beacon_advChangeCallback)(aceBT_advInstanceHandle adv_instance,
                                         aceBT_beaconAdvState_t state,
                                         aceBT_beaconPowerMode_t power_mode,
                                         aceBT_beaconAdvMode_t beacon_mode);

/**
 * @brief callback to notifiy a change in advertisment instance\n
 * Invoked on @ref aceBT_startBeaconScan, @ref
 * aceBT_startBeaconScanWithDefaultParams, @ref aceBT_stopBeaconScan
 *
 * @param[in] scan_instance Scan instance
 * @param[in] state Current advertisement state
 * @param[in] interval Interval in in untis of 1.25 ms at which this scan is
 * performed currently
 * @param[in] window length of scan procedure / scan interval in untis of 1.25
 * ms
 */
typedef void (*beacon_scanChangeCallback)(
    aceBT_scanInstanceHandle scan_instance, aceBT_beaconScanState_t state,
    uint32_t interval, uint32_t window);

/**
 * @brief callback to notifiy a change in advertisment instance\n
 * Invoked in response of @ref aceBT_startBeaconScan and @ref
 * aceBT_startBeaconScanWithDefaultParams
 *
 * @param[in] scan_instance Scan instance
 * @param[in] state Current advertisement state
 * @param[in] scanResult Scan result(s)
 */
typedef void (*beacon_scanResultCallback)(
    aceBT_scanInstanceHandle scan_instance, aceBT_BeaconScanRecord_t* record);

/**
 * @brief callback to notifiy that beacon client registration status\n
 * Invoked on @ref aceBT_RegisterBeaconClient
 *
 * @param[in] status status of the beacon client registration
 */
typedef void (*beacon_onBeaconClientRegistered)(ace_status_t status);

/**
 * @brief Beacon manager callback struct. Clients need to set the struct field
 * to NULL if callback is not needed
 */
typedef struct {
    size_t size; /**< Size of the struct */
    beacon_advChangeCallback
        advStateChanged; /**< Advertisement state changed */
    beacon_scanChangeCallback scanStateChanged; /**< Scan state changed */
    beacon_scanResultCallback scanResults;      /**< Scan results callback */
    beacon_onBeaconClientRegistered
        onclientRegistered; /**< Beacon client registration callback */
} aceBT_beaconCallbacks_t;

/** @} */

// /**
//  * @defgroup ACE_BLE_API_BEACON BLE Beacon Manager APIs
//  * @{
//  * @ingroup ACE_BLE_API
//  */

// /**
//  * @brief Utility function to reset the Beacon data param.
//  *
//  * @param[in] beacon_data Beacon data to reset
//  */
// void aceBT_initBeaconData(aceBT_BeaconData_t* beacon_data);

// /**
//  * @brief Utility function which helps in forming mesh data packet.\n
//  * Packet structure: adv_data_type (1 byte), opt:beacon_type (1 byte), data
//  * (data_len bytes)
//  *
//  * @param[out] beacon formatted and appended to beacon filled with the mesh
//  * data
//  * @param[in] data mesh data
//  * @param[in] data_len length of the mesh data
//  * @param[in] adv_data_type @ref aceBT_meshAdvType_t
//  * @param[in] beacon_type Type of the beacon
//  * @return None
//  */
// void aceBT_appendMeshData(aceBT_BeaconData_t* beacon,
//                           aceBT_meshAdvType_t adv_data_type,
//                           aceBT_meshBeaconType_t beacon_type, uint8_t* data,
//                           uint16_t data_len);

// /**
//  * @brief Utility function which helps in forming the manufacture specific data
//  *
//  * @param[out] beacon formatted and appended with the manu id, data and len
//  * @param[in] manu_id manufacture id
//  * @param[in] data manufacturer data
//  * @param[in] data_len length of the manufacture data
//  * @return None
//  */
// void aceBT_appendManufactureId(aceBT_BeaconData_t* beacon, uint16_t manu_id,
//                                uint8_t* data, uint16_t data_len);

// /**
//  * @brief Utility function which helps in forming the service data
//  *
//  * @param[out] beacon formatted and appended with the service uuid and data
//  * @param[in] uuid service uuid
//  * @param[in] data service data
//  * @param[in] data_len length of the service data
//  * @return None
//  */
// void aceBT_appendServiceData(aceBT_BeaconData_t* beacon, aceBT_uuid_t uuid,
//                              uint8_t* data, uint16_t data_len);

// /**
//  * @brief Utility function which helps in forming the service uuid
//  *
//  * @param[out] beacon formatted and appended with the service ids
//  * @param[in] uuid_list List of one or more service uuids
//  * @param[in] count number of uuids in uuid array
//  * @return None
//  */
// void aceBT_appendServiceUUIDs(aceBT_BeaconData_t* beacon,
//                               aceBT_uuid_t* uuid_list, uint8_t count);

// /**
//  * @brief Returns bitmask value of all supported features by local BLE device
//  * Features are defined in bluetooth_beacon_defines.h.
//  * Use ACEBT_BEACONMGR_IS_xxx_SUPPORTED macro to check support for individual
//  * features.
//  *
//  * @param[out] features filled with supported features
//  * @return @ref ACEBT_STATUS_SUCCESS
//  */
// aceBT_status_t aceBT_beaconMgr_getSupportedFeatures(uint32_t* features);

/**
 * @brief Interface to register as a beacon manager client with Beacon
 * manager.\n Only one beacon manager client is allowed per client session.\n
 * Triggers @ref beacon_onBeaconClientRegistered
 *
 * @param[in] session_handle  This handle should be same as handle returned by
 * OpenSession() API
 * @param[in] callbacks Callback structure of type aceBT_beacon_callbacks_t.
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_FAIL if request fails
 * @return @ref ACEBT_STATUS_UNSUPPORTED if BLE is not supported
 */
ace_status_t aceBT_RegisterBeaconClient(aceBT_sessionHandle session_handle,
                                        aceBT_beaconCallbacks_t* callbacks);

// /**
//  * @brief Interface to deregister a beacon manager client with Beacon manager
//  *
//  * @param[in] session_handle This handle should be same as handle returned by
//  * OpenSession() API
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if BLE is not supported
//  */
// aceBT_status_t aceBT_deregisterBeaconClient(aceBT_sessionHandle session_handle);

// /**
//  * @brief Returns maximum length of advertisement payload an application can
//  * send for a specific advertisement type
//  *
//  * @param[in] beacon_type Beacon type
//  * @return maximum length of allowed advertisement payload
//  */
// uint32_t aceBT_getMaximumAdvertisingDataLength(aceBT_beaconType_t beacon_type);

// /**
//  * @brief Interface to request beacon manager to initiate an
//  * advertisement/Beaconing on behalf of a client.\n
//  * If API returns BT_SUCCESS, a new advertise instance will be created and
//  * filled on adv_instance.\n
//  * Advertise instance should be used to refer to this advertisement in all
//  * subsequent calls.\n
//  * Triggers @ref beacon_advChangeCallback.
//  *
//  * @note This API should only be called after beacon_onBeaconClientRegistered
//  * is received.
//  *
//  * @param[in] session_handle This handle should be same as handle returned by
//  * OpenSession() API
//  * @param[in] settings aceBT_BeaconAdvSettings filled with the application
//  * requested Beacon settings for this advertisement
//  * @param[in] client_id client identification assigned for this client assigned
//  * by Beacon manager If no ID is currently assigned, pass 0xFFFF.
//  * @param[in] payload instance of aceBT_BeaconPayload_t structure filled with
//  * beacon type and application payload that needs to be advertised
//  * @param[out] adv_instance Filled with reference to this advertisement
//  * instance.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  */
// aceBT_status_t aceBT_startBeacon(aceBT_sessionHandle session_handle,
//                                  aceBT_BeaconAdvSettings settings,
//                                  aceBT_BeaconClientId client_id,
//                                  aceBT_BeaconPayloadList_t payload,
//                                  aceBT_advInstanceHandle* adv_instance);

// /**
//  * @brief Interface to request beacon manager to initiate an
//  * advertisement/Beaconing on behalf of a client along with a scan response
//  * data that will be returned to remote scanning server when a scan request is
//  * received.\n
//  * If API returns BT_SUCCESS, a new advertise instance will be created and
//  * filled on adv_instance.\n
//  * Advertise instance should be used to control to this advertisement in all
//  * subsequent calls.\n
//  * Triggers @ref beacon_advChangeCallback.
//  *
//  * @note This API should only be called after beacon_onBeaconClientRegistered
//  * is received.
//  *
//  * @param[in] session_handle This handle should be same as handle returned by
//  * OpenSession() API
//  * @param[in] settings filled with the application requested Beacon settings
//  * for this advertisement
//  * @param[in] client_id client identification assigned for this client assigned
//  * by Beacon manager If no ID is currently assigned, pass 0xFFFF.
//  * @param[in] payload  instance of aceBT_BeaconPayload_t structure filled with
//  * beacon type and application payload that needs to be advertised
//  * @param[in] scan_response Payload to be sent as scan response
//  * @param[out] adv_instance filled with reference to this advertisement
//  * instance.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  */
// aceBT_status_t aceBT_startBeaconWithScanResponse(
//     aceBT_sessionHandle session_handle, aceBT_BeaconAdvSettings settings,
//     aceBT_BeaconClientId client_id, aceBT_BeaconPayloadList_t payload,
//     aceBT_BeaconPayloadList_t scan_response,
//     aceBT_advInstanceHandle* adv_instance);

// /**
//  * @brief Interface to set periodic advertisement parameters for an extended
//  * advertisement instance.\n
//  * Note that periodic advertisement parameters are applicable only if the
//  * advertisement instance is of type extended advertisement.
//  *
//  * @param[in,out] adv_instance Advertisement instance
//  * @param[in] periodic_adv_interval Preferred interval for periodic
//  * advertisements
//  * @param[in] power_mode Preferred power mode for periodic advertisements
//  * @param[in] client_id client identification assigned for this client assigned
//  * by beacon manager. If no ID is currently assigned, pass 0xFFFF.
//  * @param[in] include_tx_power True if periodic advertisement data needs to
//  * include Tx power as part of payload
//  * @param[in] periodic_adv_payload Periodic advertisement payload.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if BLE is not supported
//  */
// aceBT_status_t aceBT_setPeriodicAdvParams(
//     aceBT_advInstanceHandle adv_instance, uint32_t periodic_adv_interval,
//     aceBT_beaconPowerMode_t power_mode, aceBT_BeaconClientId client_id,
//     bool include_tx_power, aceBT_BeaconPayloadList_t periodic_adv_payload);

// /**
//  * @brief Permanently stops an ongoing BLE advertisement.\n
//  * Triggers @ref beacon_advChangeCallback
//  *
//  * @param[in] session_handle This handle should be same as handle returned by
//  * OpenSession() API
//  * @param[in] adv_instance Advertisement instance
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_stopBeacon(aceBT_sessionHandle session_handle,
//                                 aceBT_advInstanceHandle adv_instance);

// /**
//  * @brief Retrieves Beacon Settings associated with an Advertisement instance
//  *
//  * @param[in] adv_instance Advertisement instance
//  * @param[out] settings filled the beacon settings
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if BLE is not supported
//  */
// aceBT_status_t aceBT_getBeacontSettings(aceBT_advInstanceHandle adv_instance,
//                                         aceBT_BeaconAdvSettings* settings);

// /**
//  * @brief Retrieves Beacon payload associated with an Advertisement instance
//  *
//  * @param[in] adv_instance Advertisement instance
//  * @param[out] data beacon payload
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_FAIL if request fails
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if BLE is not supported
//  */
// aceBT_status_t aceBT_getAdvertisementPayload(
//     aceBT_advInstanceHandle adv_instance, aceBT_BeaconPayloadList_t* data);

// /**
//  * @brief Interface to request beacon manager to initiate a BLE scan on behalf
//  * of a client.\n
//  * If API returns BT_SUCCESS, a new scaninstance will be created and filled in
//  * scan_instance.\n
//  * This scan instance should be used to control this scan  in all subsequent
//  * calls.\n
//  * Triggers @ref beacon_scanChangeCallback.
//  *
//  * @note This API should only be called after beacon_onBeaconClientRegistered
//  * is received.
//  *
//  * @param[in] session_handle  Session handle for the current Bluetooth client
//  * @param[in] client_id client identification assigned for this client assigned
//  * by Beacon manager. If no ID is currently assigned, pass 0xFFFF.
//  * @param[in] settings aceBT_BeaconScanSettings_t filled with the application
//  * requested scan settings for this scan request
//  * @param[in] filter_lists aceBT_BeaconScanFilterList_t filled with the
//  * application requested scan filters for this scan request
//  * @param[in] num_filter_lists Number of filter lists in @ref filter_lists
//  * pointer
//  * @param[out] scan_instance filled with reference
//  * to this scanning instance.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_startBeaconScan(aceBT_sessionHandle session_handle,
//                                      aceBT_BeaconClientId client_id,
//                                      aceBT_BeaconScanSettings_t settings,
//                                      aceBT_BeaconScanFilterList_t* filter_lists,
//                                      uint8_t num_filter_lists,
//                                      aceBT_scanInstanceHandle* scan_instance);

// /**
//  * @brief Interface to request beacon manager to initiate a BLE scan on behalf
//  * of a client with default settings without any scan filters.\n
//  * If API returns BT_SUCCESS, a new scaninstance will be created and filled on
//  * out parameter scan_instance.\n
//  * This scan instance should be used to control this scan in all
//  * subsequent calls.\n
//  * Triggers @ref beacon_scanChangeCallback
//  *
//  * @param[in] session_handle This handle should be same as handle returned by
//  * OpenSession() API
//  * @param[in] client_id client identification assigned for this client assigned
//  * by Beacon manager. If no ID is currently assigned, pass 0xFFFF.
//  * @param[out] scan_instance filled with reference to this scanning instance.
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_startBeaconScanWithDefaultParams(
//     aceBT_sessionHandle session_handle, aceBT_BeaconClientId client_id,
//     aceBT_scanInstanceHandle* scan_instance);

// /**
//  * @brief Stops an ongoing BLE Scan\n
//  * Triggers @ref beacon_scanChangeCallback
//  *
//  * @param[in] scan_instance Scan instance
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// aceBT_status_t aceBT_stopBeaconScan(aceBT_scanInstanceHandle scan_instance);

// /**
//  * @brief Utility function which extracts Flags from an advertisment packet
//  *
//  * @param[in] scan_record Advertisement packet
//  * @param[out] flags Flags present in advertsement packet
//  * @return length of flags field (1) or 0 if not present
//  */
// uint8_t aceBT_scanRecordExtractFlags(aceBT_BeaconScanRecord_t* scan_record,
//                                      uint8_t* flags);

// /**
//  * @brief Utility function which extracts Flags from an advertisment packet
//  *
//  * @param[in] scan_record advertisement packet
//  * @param[in] size size of uuids
//  * @param[out] uuids uuids present in advertsement packet
//  * @return number of uuids extracted or 0 if not present
//  */
// uint8_t aceBT_scanRecordExtractUuid(aceBT_BeaconScanRecord_t* scan_record,
//                                     aceBT_uuid_t* uuids, uint8_t size);

// /**
//  * @brief Utility function which extracts name from an advertisment packet
//  *
//  * @param[in] scan_record in advertisement packet
//  * @param[out] name out name
//  * @return length of name or 0 if not present
//  */
// uint8_t aceBT_scanRecordExtractName(aceBT_BeaconScanRecord_t* scan_record,
//                                     aceBT_bdName_t* name);

// /**
//  * @brief Utility function which extracts tx power from an advertisment packet
//  *
//  * @param[in] scan_record advertisement packet
//  * @param[out] tx tx power
//  * @return length of tx power (1) or 0 if not present
//  */
// uint8_t aceBT_scanRecordExtractTxPower(aceBT_BeaconScanRecord_t* scan_record,
//                                        int* tx);

// /**
//  * @brief Utility function which extracts Service Data from an advertisment
//  * packet
//  *
//  * @param[in] scan_record Advertisement Packet
//  * @param[in,out] size Size of data array and length of service data extracted
//  * @param[out] data Extracted Service Data
//  * @return @ref ACEBT_STATUS_SUCCESS if service data extracted
//  * @return @ref ACEBT_STATUS_PARM_INVALID if size of data array is less than
//  * service data length
//  * @return @ref ACEBT_STATUS_NOT_FOUND for if service data is not found
//  */
// aceBT_status_t aceBT_scanRecordExtractServiceData(
//     aceBT_BeaconScanRecord_t* scan_record, uint8_t* data, uint8_t* size);

// /**
//  * @brief Utility function which extracts Service Data Uuid from an advertisment
//  * packet
//  *
//  * @param[in] scan_record advertisement packet
//  * @param[out] data service data uuid
//  * @return length of service data or 0 if not present
//  */
// uint8_t aceBT_scanRecordExtractServiceDataUuid(
//     aceBT_BeaconScanRecord_t* scan_record, aceBT_uuid_t* data);

// /**
//  * @brief Utility function which extracts Manufacturer Specific Data from an
//  * advertisment packet
//  *
//  * @param[in] scan_record Advertisement Packet
//  * @param[out] data Extracted Manufacturer Data
//  * @param[in,out] size Size of data array and length of manufacturer data
//  * extracted
//  * @return @ref ACEBT_STATUS_SUCCESS if manugacturer data extracted
//  * @return @ref ACEBT_STATUS_PARM_INVALID if size of data array is less than
//  * manufacturer data length
//  * @return @ref ACEBT_STATUS_NOT_FOUND for if service data is not found
//  */
// aceBT_status_t aceBT_scanRecordExtractManufacturerData(
//     aceBT_BeaconScanRecord_t* scan_record, uint8_t* data, uint8_t* size);

// /**
//  * @brief Utility function to free manufacturer data array allocated by @ref
//  * aceBt_scanRecordExtractManufacturerDataArray
//  *
//  * @param[in] data Pointer to manufacturer data array to be freed
//  * @return @ref ACEBT_STATUS_SUCCESS if pointer was freed
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if API isn't supported
//  */
// aceBT_status_t aceBt_scanRecordFreeManufacturerDataArray(
//     aceBt_beaconManufacturerData_t* data);

// /**
//  * @brief Utility function which extracts multiple manufacturer specific data
//  * from an advertisment packet.
//  * manu_data array will be allocated by this function if the return is
//  * ACEBT_STATUS_SUCCESS. User must free the allocated memory by calling
//  * @ref aceBt_scanRecordFreeManufacturerDataArray
//  *
//  * @param[in] scan_record Advertisement Packet
//  * @param[out] manu_data Pointer to pointer to extracted manufacturer data
//  * @param[out] data_count Number of manufacturer data extracted
//  * @return @ref ACEBT_STATUS_SUCCESS if manugacturer data extracted
//  * @return @ref ACEBT_STATUS_NOMEM if memory allocation failed
//  * @return @ref ACEBT_STATUS_NOT_FOUND for if service data is not found
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if API isn't supported
//  */
// aceBT_status_t aceBt_scanRecordExtractManufacturerDataArray(
//     aceBT_BeaconScanRecord_t* scan_record,
//     aceBt_beaconManufacturerData_t** manu_data, uint8_t* data_count);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BEACON_API_H
