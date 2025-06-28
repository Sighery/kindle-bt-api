/**
 * @file bluetooth_ble_gatt_client_api.h
 *
 * @brief ACE Bluetooth Beacon Manager header defining identifiers for known
 * Beacon clients
 */

#ifndef BT_BLE_GATT_CLIENT_API_H
#define BT_BLE_GATT_CLIENT_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "bluetooth_defines.h"
#include "bluetooth_ble_defines.h"
#include "bluetooth_session_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BLE_CB_GATT_CLIENT GATT Client callbacks
 * @{
 * @ingroup ACE_BLE_CB
 */

/**
 * @brief Callback for @ref aceBT_bleRegisterGattClient.\n
 * It shall not be possible to perform any opertaion without the registration
 * to invoke any GATT interfaces.\n
 * Invoking this api multiple time for the same session would result error.
 *
 * @param[in] status status of the registration BT_SUCCESS or appropriate
 * failure
 * @return None
 */
typedef void (*on_ble_gattc_service_registered_callback)(ace_status_t status);

/**
 * @brief Callback to indicate that service discovery is complete, GATT service
 * discovery may take time to complete.\n
 * Triggered on @ref aceBT_bleDiscoverAllServices or @ref
 * aceBT_bleDiscoverPrimaryServicesByUuid
 *
 * @param[in] conn_handle connection handle
 * @param[in] status status of the service discovery BT_SUCCESS or appropriate
 * failure
 * @return None
 */
typedef void (*on_ble_gattc_service_discovered_callback)(
    aceBT_bleConnHandle conn_handle, ace_status_t status);

/**
 * @brief Callback to indicate read characteristics request complete\n
 * Triggered on @ref aceBT_bleReadCharacteristics
 *
 * @param[in] conn_handle connection handle for which the read operation was
 * requested
 * @param[in] chars_value characteristics value of the read
 * @param[in] status status of the read characteristic operation BT_SUCCESS or
 * appropriate failure
 * @return None
 */
typedef void (*on_ble_gattc_read_characteristics_callback)(
    aceBT_bleConnHandle conn_handle,
    aceBT_bleGattCharacteristicsValue_t chars_value, ace_status_t status);

/**
 * @brief Callback to indicate write characteristics request complete\n
 * Triggered on @ref aceBT_bleWriteCharacteristics
 *
 * @param[in] conn_handle connection handle for which the write operation was
 * requested
 * @param[in] gatt_characteristics characteristics value of the write
 * @param[in] status status of the write characteristic operation BT_SUCCESS or
 * appropriate failure
 * @return None
 */
typedef void (*on_ble_write_characteristics_callback)(
    aceBT_bleConnHandle conn_handle,
    aceBT_bleGattCharacteristicsValue_t gatt_characteristics,
    ace_status_t status);

/**
 * @brief Callback to indicate read descriptor request complete.\n
 * The read descriptor value will be filled as part of the
 * gattDescriptorblobValue @ref in aceBT_bleGattCharacteristicsValue_t \n
 * Triggered on @ref aceBT_bleReadDescriptor
 *
 * @param[in] conn_handle connection handle for which the read operation was
 * requested
 * @param[in] chars_value characteristics value of the read descriptor operation
 * @param[in] status status of the read descriptor operation BT_SUCCESS or
 * appropriate failure
 * @return None
 */
typedef void (*on_ble_gattc_read_descriptor_callback)(
    aceBT_bleConnHandle conn_handle,
    aceBT_bleGattCharacteristicsValue_t chars_value, ace_status_t status);

/**
 * @brief Callback to indicate GATT Client write descriptor request complete.\n
 * Triggered on @ref aceBT_bleWriteDescriptor
 *
 * @param[in] conn_handle connection handle for which the write operation was
 * requested
 * @param[in] gatt_characteristics characteristics value of the write descriptor
 * operation
 * @param[in] status status of the write descriptor operation BT_SUCCESS or
 * appropriate failure
 * @return None
 */
typedef void (*on_ble_gattc_write_descriptor_callback)(
    aceBT_bleConnHandle conn_handle,
    aceBT_bleGattCharacteristicsValue_t gatt_characteristics,
    ace_status_t status);

/**
 * @brief Callback to indicate that change in characteristics\n
 * Triggered in response to @ref aceBT_bleSetNotification
 *
 * @param[in] conn_handle connection handle
 * @param[in] chars_value characteristics value of the read descriptor operation
 * @return None
 */
typedef void (*on_ble_gattc_notify_characteristics_callback)(
    aceBT_bleConnHandle conn_handle,
    aceBT_bleGattCharacteristicsValue_t gatt_characteristics);

/**
 * @brief Callback to indicate retrival of GATT Client DB.\n
 * Triggered on @ref aceBT_bleGetService or @ref aceBT_bleGetServiceByUuid
 *
 * @param[in] conn_handle connection handle
 * @param[in] gatt_service Pointer to array of GATT Services. Since the memory
 * is allocated from BT middleware and will be freed after callback execution,
 * clients should deep-copy this pointer array using @ref
 * aceBT_bleCloneGattService.
 * @param[in] no_svc Number of GATT Services
 * @return None
 */
typedef void (*on_ble_gattc_get_gatt_db_callback)(
    aceBT_bleConnHandle conn_handle, aceBT_bleGattsService_t* gatt_service,
    uint32_t no_svc);

/**
 * @brief Callback to indicate GATT Client Execute write operation.\n
 * Triggered on @ref aceBT_bleExecuteReliableWrite
 *
 * @param[in] conn_handle connection handle for which the operation was
 * requested
 * @param[in] status status of the execute write operation BT_SUCCESS or
 * appropriate failure
 * @return None
 */
typedef void (*on_ble_gattc_execute_write_callback)(
    aceBT_bleConnHandle conn_handle, ace_status_t status);

/** @brief GATT Client Data structures */
typedef struct {
    /** Size of the struct */
    size_t size;
    /** Service registered callback */
    on_ble_gattc_service_registered_callback on_ble_gattc_service_registered_cb;
    /** Service discovered callback */
    on_ble_gattc_service_discovered_callback on_ble_gattc_service_discovered_cb;
    /** Read characteristics callback */
    on_ble_gattc_read_characteristics_callback
        on_ble_gattc_read_characteristics_cb;
    /** Read characteristics callback */
    on_ble_write_characteristics_callback on_ble_gattc_write_characteristics_cb;
    /** Characteristics notification callback */
    on_ble_gattc_notify_characteristics_callback notify_characteristics_cb;
    /** Read characteristics callback */
    on_ble_gattc_write_descriptor_callback on_ble_gattc_write_descriptor_cb;
    /** Read characteristics callback for GATTC descriptor */
    on_ble_gattc_read_descriptor_callback on_ble_gattc_read_descriptor_cb;
    /** Get GATT database callback */
    on_ble_gattc_get_gatt_db_callback on_ble_gattc_get_gatt_db_cb;
    /** Ececute write callback */
    on_ble_gattc_execute_write_callback on_ble_gattc_execute_write_cb;
} aceBT_bleGattClientCallbacks_t;

/** @} */

/**
 * @defgroup ACE_BLE_API_GATT_CLIENT GATT Client APIs
 * @{
 * @ingroup ACE_BLE_API
 */
/**
 * @brief Interface to register as a GATT client, this is a synchronous call
 * that returns the status immediately. This function should be called before
 * invoking any GATT interfaces.\n
 * Invoking this api multiple times for the same session would result in error\n
 * Triggers @ref on_ble_gattc_service_registered_callback
 *
 * @param[in] session_handle session handle for the GATT client session
 * @param[in] callbacks of type aceBT_bleGattClientCallbacks_t
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
 * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
 * @return @ref ACEBT_STATUS_FAIL for all other errors
 * @deprecated
 * @see aceBt_bleRegisterGattClient
 */
ace_status_t aceBt_bleRegisterGattClient(
    aceBT_sessionHandle session_handle,
    aceBT_bleGattClientCallbacks_t* callbacks);

/**
 * @brief Register as a GATT client. This is a synchronous call
 * that returns the status immediately. This function should be called before
 * invoking any GATT APIs.\n
 * Invoking this API multiple times for the same session would result in error\n
 * Triggers @ref on_ble_gattc_service_registered_callback
 *
 * @param[in] session_handle session handle for the GATT client session
 * @param[in] callbacks of type aceBT_bleGattClientCallbacks_t
 * @param[in] app_id If policy manager is enabled, connection policy will be
 * applied as defined in the policy manager based on the application ID,
 * For unknown applications @ref ACE_BT_BLE_APPID_GENERIC should be used.
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
 * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
 * @return @ref ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_bleRegisterGattClient(
    aceBT_sessionHandle session_handle,
    aceBT_bleGattClientCallbacks_t* callbacks, aceBt_bleAppId_t app_id);

/**
 * @brief API to deregister GATT client, this is a synchronous call returns the
 * status immdiately,this function should be called before after finishing all
 * GATT operation, this would clean up all the internal data structures
 *
 * @param[in] session_handle session handle for the GATT client session
 * @return @ref ACEBT_STATUS_SUCCESS if success
 * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
 * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
 * @return @ref ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_bleDeRegisterGattClient(
    aceBT_sessionHandle session_handle);

// /**
//  * @brief API to discover all services of the remote device, it is important to
//  * have a GATT connection before initiating the discover service.\n
//  * This is a synchronous call that returns the status immediately.\n
//  * @ref on_ble_gattc_service_discovered_callback will return when discovery is
//  * completed
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection of the BLE connection what was established
//  * using the BLE api's
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_UNSUPPORTED if does not support BLE
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleDiscoverAllServices(aceBT_sessionHandle session_handle,
//                                             aceBT_bleConnHandle conn_handle);

// /**
//  * @brief API to discover all primary services by uuid GATT client,
//  * this is a synchronous call returns status immediately.\n
//  * @ref on_ble_gattc_service_discovered_callback will return when discovery is
//  * completed
//  *
//  * @param[in] conn_handle connection handle
//  * @param[in] uuid that need to be discovered
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleDiscoverPrimaryServicesByUuid(
//     aceBT_bleConnHandle conn_handle, aceBT_uuid_t uuid);
// /**
//  * @brief API to get the discovered service and filter the primary service
//  * based on the UUID. This api will return the service discovered list only if
//  * @ref on_ble_gattc_service_discovered_callback returns success\n
//  * Triggers on_ble_gattc_get_gatt_db_callback
//  *
//  * @param[in] conn_handle connection handle
//  * @param[in] uuid of the discovered primary service service. If there is no
//  * match then a null service will be return with appropriate error
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleGetServiceByUuid(aceBT_bleConnHandle conn_handle,
//                                          aceBT_uuid_t uuid);

// /**
//  * @brief  API to get the discovered service and filter the primary service\n
//  * This api will return the service discovered list only if
//  * @ref on_ble_gattc_service_discovered_callback returns success\n
//  * Triggers @ref on_ble_gattc_get_gatt_db_callback
//  *
//  * @param[in] conn_handle connection handle
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleGetService(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief API to read GATT characteristics value from the server.
//  * The read will be performed on the server for the requested gattRecord.uuid
//  * in @ref aceBT_bleGattCharacteristicsValue_t upon receiving the data.\n
//  * @ref on_ble_gattc_read_characteristics_callback will be invoked to notify the
//  * the application.\n
//  * Reading Blob Value:
//  * It may not be possible to read the complete blob in one request. It would be
//  * required to pass the offset received so far in blobValue.offset of @ref
//  * aceBT_bleGattCharacteristicsValue_t for the same uuid
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] chars_value GATT characteristics value
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleReadCharacteristics(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t chars_value);

// /**
//  * @brief API to write GATT characteristics value to the GATT server.
//  * Write will be performed on the server for the uuid that is passed in
//  * gattRecord.uuid in @ref aceBT_bleGattCharacteristicsValue_t upon completion
//  * of the write based on the property of the attribute @ref
//  * on_ble_write_characteristics_callback will be invoked to notify the same to
//  * application\n
//  * The write property property gattRecord.attProp in @ref
//  * aceBT_bleGattCharacteristicsValue_t\n
//  *
//  * Reading Blob Value:
//  * It may not be possible to write the complete blob in one request. It would be
//  * required to pass the offset received so far in blobValue.offset in @ref
//  * aceBT_bleGattCharacteristicsValue_t for the same handle
//  * characteristicsValue.handle @ref aceBT_bleGattCharacteristicsValue_t.
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] chars_value GATT characteristics value
//  * @param[in] request_type Request type
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleWriteCharacteristics(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t* chars_value,
//     aceBT_responseType_t request_type);

// /**
//  * @brief API to read descriptor characteristics value from the server.
//  * The read will be performed on the descriptor associated with the
//  * characteristics gattRecord.uuid in @ref aceBT_bleGattCharacteristicsValue_t
//  * .\n
//  * Upon receiving the data @ref on_ble_gattc_read_descriptor_callback will be
//  * invoked to notify the the application.\n
//  * Reading Blob Value:
//  * It may not be possible to read the complete blob in one request. It would be
//  * required to pass the offset received so far in blobValue.offset in @ref
//  * aceBT_bleGattCharacteristicsValue_t for the same handle in @ref
//  * aceBT_bleGattCharacteristicsValue_t
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] char_value GATT characteristics value
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleReadDescriptor(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t char_value);

// /**
//  * @brief API to write GATT descriptor value to the GATT server.
//  * Write will be performed on the server for the handle that is passed in
//  * caceBT_bleGattCharacteristicsValue_t.gattDescriptor.\n
//  * Upon completion of the write based on the property of the attribute,
//  * @ref on_ble_gattc_write_descriptor_callback will be invoked to
//  * notify the same to application.\n
//  *
//  * Write Blob Value:
//  * It may not be possible to write the complete blob in one request. The clients
//  * are required to track and pass the offset received so far in
//  * gattDescriptor.blobValue.offset of  @ref aceBT_bleGattCharacteristicsValue_t
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] chars_value Pointer to GATT characteristics value
//  * @param[in] request_type Request type
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleWriteDescriptor(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t* chars_value,
//     aceBT_responseType_t request_type);

// /**
//  * @brief API to enable/disable Notification on the server. Invoking this API
//  * will set the notification on the remote device for passed characteristics
//  * handle. @ref on_ble_gattc_notify_characteristics_callback would be invoked
//  * on any change in characteristics value on the server.\n
//  * @ref on_ble_gattc_write_descriptor_callback will be called to notify that
//  * notification is set on the server
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] chars_value GATT characteristics value
//  * @param[in] enable boolean to enable or disable notification
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleSetNotification(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t chars_value, bool enable);

// /**
//  * @brief API to enable/disable Indication on the server. Invoking this API
//  * will set the indication on the remote device for passed characteristics
//  * handle. @ref on_ble_gattc_notify_characteristics_callback
//  * would be invoked on any change in characteristics value on the server.
//  * @ref on_ble_gattc_write_descriptor_callback will called to notify that
//  * indication is set on the server
//  *
//  * @param[in] session_handle session handle for the GATT client session
//  * @param[in] conn_handle connection handle
//  * @param[in] chars_value GATT characteristics value
//  * @param[in] enable boolean to enable or disable indication
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleSetIndication(
//     aceBT_sessionHandle session_handle, aceBT_bleConnHandle conn_handle,
//     aceBT_bleGattCharacteristicsValue_t chars_value, bool enable);

// /**
//  * @brief API Initiates a reliable write transaction for a given remote device.
//  *
//  * All reliable write operation performed using this API
//  * will be acknowledged with @ref on_ble_write_characteristics_callback, the
//  * server will echo the data that is originally written so that it can be
//  * validate to continue the operation. Once the write operation is completed
//  * the the transaction is committed by invoking
//  * @ref aceBT_bleExecuteReliableWrite or aborted using
//  * @ref aceBT_bleAbortReliableWrite
//  *
//  * @param[in] conn_handle connection handle
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleBeginReliableWriteCharacteristics(
//     aceBT_bleConnHandle conn_handle);

// /**
//  * @brief API Initiates a execute the ongoing reliable write transaction for a
//  * given remote device.
//  *
//  * @param[in] conn_handle connection handle
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleExecuteReliableWrite(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief API to abort the ongoing reliable write transaction
//  *
//  * @param[in] conn_handle connection handle
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleAbortReliableWrite(aceBT_bleConnHandle conn_handle);

// /**
//  * @brief API to Clean up GATT service
//  *
//  * @param[in] service Pointer to array of GATT Services. The client allocates
//  * this memory and hence take care of freeing up the memory.
//  * @param[in] no_svc Number of GATT Services
//  * @return @ref ACEBT_STATUS_SUCCESS if success
//  * @return @ref ACEBT_STATUS_NOMEM if ran out of memory
//  * @return @ref ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return @ref ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return @ref ACEBT_STATUS_NOT_READY if server is not ready
//  * @return @ref ACEBT_STATUS_FAIL for all other errors
//  */
// ace_status_t aceBT_bleCleanupGattService(aceBT_bleGattsService_t* service,
//                                            int no_svc);

// /**
//  * @brief  API to copy GATT service(s)
//  *
//  * @param[out] dst_gatt_service destination GATT service pointer
//  * @param[in] src_gatt_service source GATT service pointer
//  * @param[in] no_svc number of services to be copied
//  * @return ACEBT_STATUS_SUCCESS if success
//  * @return ACEBT_STATUS_NOMEM if ran out of memory
//  * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
//  * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
//  * @return ACEBT_STATUS_NOT_READY if server is not ready
//  * @return ACEBT_STATUS_FAIL for all other errors
//  */
// extern ace_status_t aceBT_bleCloneGattService(
//     aceBT_bleGattsService_t** dst_gatt_service,
//     const aceBT_bleGattsService_t* src_gatt_service, int no_svc);

// /** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BLE_GATT_CLIENT_API_H
