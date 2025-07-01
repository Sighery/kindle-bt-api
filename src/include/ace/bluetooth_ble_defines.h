/**
 * @file bluetooth_ble_defines.h
 *
 * @brief ACE BLE defines
 */

#ifndef BT_BLE_DEFINES_H
#define BT_BLE_DEFINES_H

#include <stdbool.h>
#include "queue.h"
#include "bluetooth_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BLE_DS BLE Data structures
 * @brief Data structures for Bluetooth LE
 * @{
 *@ingroup ACE_BT_DS
 */

/**
 * @defgroup ACE_BLE_DS_DEFS Common LE Defines
 * @brief Enum/Struct definitions for Common BLE modules.
 * @{
 * @ingroup ACE_BLE_DS
 */

#define ACEBT_MTU_DEFAULT 20 /**< Default MTU value */

#define ACEBT_OOB_DATA_LEN 16   /**< OOB data length */
#define ACEBT_PIN_LENGTH_MAX 16 /**< PIN maximum length */

#define ACEBT_BEACON_CLIENT 0x01 /**< Beacon client */
#define ACEBT_GATTS_CLIENT 0x02  /**< GATTS client */
#define ACEBT_GATTC_CLIENT 0x03  /**< GATTC client*/

#define ACEBT_ATT_READ_REQUEST 0x00  /**< Attribute read request*/
#define ACEBT_ATT_WRITE_REQUEST 0x01 /**< Attribute write request*/
#define ACEBT_ATT_WRITE_COMMAND 0x02 /**< Attribute write command*/

/** @brief Opaque handle representing a Connection Interface */
typedef struct aceBT_bleConnectionHandle* aceBT_bleConnHandle;

/** @brief Opaque handle representing a GATT Interface */
typedef struct aceBT_bleGattInstance* aceBT_bleGattInstanceHandle;

/** @brief Bluetooth BLE write type */
typedef enum {
    ACEBT_BLE_WRITE_TYPE_RESP_NO = 0,      /**< Write Response not required */
    ACEBT_BLE_WRITE_TYPE_RESP_REQUIRED = 1 /**< Write Response required */
} aceBT_responseType_t;

/** @brief Bluetooth BLE connection state */
typedef enum {
    ACEBT_BLE_STATE_CONNECTED = 0,     /**< BLE State Connected */
    ACEBT_BLE_STATE_CONNECTING = 1,    /**< BLE State Connecting */
    ACEBT_BLE_STATE_DISCONNECTING = 2, /**< BLE State Disconnecting */
    ACEBT_BLE_STATE_DISCONNECTED = 3,  /**< BLE State Disconnected*/
} aceBT_bleConnState_t;

/** @brief BLE connection parameter set as part of Connection parameter update
 */
typedef enum {
    /** Max Parameters: High priority, ultra-low latency param. This should
     * be used to transfer large amount of data quickly. Applications should
     * switch to @ref ACE_BT_BLE_CONN_PARAM_BALANCED or
     * @ref ACE_BT_BLE_CONN_PARAM_LOW once done. */
    ACE_BT_BLE_CONN_PARAM_MAX = 0,
    /** High Parameters: high priority, low latency param */
    ACE_BT_BLE_CONN_PARAM_HIGH = 1,
    /** Balanced Parameters: Uses default conn param recommended by Bluetooth
     * SIG */
    ACE_BT_BLE_CONN_PARAM_BALANCED = 2,
    /** Low Parameters: Low priority, high latency param */
    ACE_BT_BLE_CONN_PARAM_LOW = 3,
    /** Ultra Low Parameters: Ultra low priority, high latency param */
    ACE_BT_BLE_CONN_PARAM_ULTRA_LOW = 4
} aceBt_bleConnParam_t;

/** @brief BLE connection priority set as part of Connection request */
typedef enum {
    /** Low Priority */
    ACE_BT_BLE_CONN_PRIO_LOW = 5,
    /** Medium Priority */
    ACE_BT_BLE_CONN_PRIO_MEDIUM = 10,
    /** High Priority */
    ACE_BT_BLE_CONN_PRIO_HIGH = 15,
    /** Maximum Priority. Policy manager determines if an
     * application is allowed a dedicated connection.
     * If allowed, the application can have at most one dedicated connection at
     * a time. */
    ACE_BT_BLE_CONN_PRIO_DEDICATED = 20,
} aceBt_bleConnPriority_t;

/**
 * @brief BLE Application identifier.
 * Based on the application ID, connection policy will be applied as defined in
 * the policy manager.
 * Unknown application should use @ref ACE_BT_BLE_APPID_GENERIC
 */
typedef enum {
    /** Generic Application */
    ACE_BT_BLE_APPID_GENERIC = 0,
    /** FFS Application */
    ACE_BT_BLE_APPID_FFS = 1,
    /** Reserved Application */
    ACE_BT_BLE_APPID_RESERVED = 2,
    /** Smart Home Application */
    ACE_BT_BLE_APPID_SMARTHOME = 3,
    /** Gadget Application */
    ACE_BT_BLE_APPID_GADGETS = 4,
    /** HOGP Application */
    ACE_BT_BLE_APPID_HOGP = 5,
    /** Amazon Remote Application */
    ACE_BT_BLE_APPID_AMZN_REMOTE = 6,
    /** BLE Mesh Application */
    ACE_BT_BLE_APPID_BLE_MESH = 7,
} aceBt_bleAppId_t;

/** @brief BLE connection role */
typedef enum {
    ACEBT_BLE_GATT_CLIENT_ROLE = 0, /**< GATT Client role  */
    ACEBT_BLE_SOCKET = 1            /**< LE Socket role */
} aceBT_bleConnRole_t;

/** @brief BLE GATT discovery type */
typedef enum {
    ACEBT_GATT_DISC_SRVC_ALL = 1,     /**< Discover all services */
    ACEBT_GATT_DISC_SRVC_BY_UUID = 2, /**< Discover service of a special type */
    ACEBT_GATT_DISC_INC_SRVC = 3,    /**< Discover the included service within a
                                    service */
    ACEBT_GATT_DISC_CHAR = 4,        /**< Discover characteristics of a service
                                    with/without type requirement */
    ACEBT_GATT_DISC_CHAR_DSCPT = 5,  /**< Discover characteristic descriptors of
                                    a character */
    ACEBT_GATT_DISC_TYPE_INVALID = 6 /**< Maximum discover type */
} aceBT_bleGattDiscoveryType;

/** @brief BLE GATT status */
typedef enum {
    ACEBT_GATT_STATUS_SUCCESS = 0,             /**< Success */
    ACEBT_GATT_STATUS_READ_NOT_PERMITTED = 1,  /**< Characteristic does not
                                              support  read */
    ACEBT_GATT_STATUS_WRITE_NOT_PERMITTED = 2, /**< Characteristic does not
                                              support write */
    ACEBT_GATT_STATUS_INSUFFICIENT_AUTHENTICATION = 3, /**< Link is not properly
                                                      Authenticated */
    ACEBT_GATT_STATUS_REQUEST_NOT_SUPPORTED = 4, /**< Operation not supported */
    ACEBT_GATT_STATUS_INVALID_OFFSET =
        5, /**< Invalid offset (long writes/reads) */
    ACEBT_GATT_STATUS_ERROR_CONN_TIMEOUT = 6,       /**< Connection Timed out */
    ACEBT_GATT_STATUS_INVALID_ATTRIBUTE_LENGTH = 7, /**< Bad Attribute Length */
    ACEBT_GATT_STATUS_INSUFFICIENT_ENCRYPTION = 8,  /**< Link is not properly
                                                   Encrypted */
    ACEBT_GATT_STATUS_LOCAL_HOST_TERMINATED_CONNECTION = 9, /**< Disconnect from
                                                           Local Host */
    ACEBT_GATT_STATUS_ERROR = 10,                  /**< Generic GATT Error */
    ACEBT_GATT_STATUS_CONNECTION_CONGESTED = 11,   /**< Congested connection */
    ACEBT_GATT_STATUS_ERROR_CONN_EST_FAIL = 12,    /**< Failed to establish gatt
                                                 connection */
    ACEBT_GATT_STATUS_INSUFFICIENT_RESOURCES = 13, /**< Not enough resources */
} aceBT_gattStatus_t;

/**
 * @name GATT characteristic properties
 * @{
 */
#define ACEBT_BLE_GATT_PROP_BROADCAST 0x0001 /**< Broadcast property */
#define ACEBT_BLE_GATT_PROP_READ 0x0002      /**< Read only property */
#define ACEBT_BLE_GATT_PROP_WRITE_NO_RESPONSE \
    0x0004 /**< Write but no reponse for the write property*/
#define ACEBT_BLE_GATT_PROP_WRITE 0x0008          /**< Write property */
#define ACEBT_BLE_GATT_PROP_NOTIFY 0x0010         /**< Notify property */
#define ACEBT_BLE_GATT_PROP_INDICATE 0x0020       /**< Indicate property */
#define ACEBT_BLE_GATT_PROP_SIGNED_WRITE 0x0040   /**< Signed write property */
#define ACEBT_BLE_GATT_PROP_EXTENDED_PROPS 0x0080 /**< Exitended properties */
/** @} */

/**
 * @name GATT permissions
 * @{
 */
/** Readable attribute */
#define ACEBT_BLE_GATT_PERM_READ 0x0001
/** Encrypted Readable attribute */
#define ACEBT_BLE_GATT_PERM_READ_ENCRYPTED 0x0002
/** Encrypted Readable attribute that requires authentication*/
#define ACEBT_BLE_GATT_PERM_READ_ENCRYPTED_MITM 0x0004
/** Writable attribute */
#define ACEBT_BLE_GATT_PERM_WRITE 0x0010
/** Encrypted writable attribute */
#define ACEBT_BLE_GATT_PERM_WRITE_ENCRYPTED 0x0020
/** Encrypted writable attribute that requires authentication*/
#define ACEBT_BLE_GATT_PERM_WRITE_ENCRYPTED_MITM 0x0040
/** Writable signed attribute */
#define ACEBT_BLE_GATT_PERM_WRITE_SIGNED 0x0080
/** Encrypted writable signed attribute that requires authentication*/
#define ACEBT_BLE_GATT_PERM_WRITE_SIGNED_MITM 0x0100
/** @} */

/**
 * @name Write types
 * @{
 */
/** Write type*/
#define ACEBT_BLE_WRITE_TYPE_NO_RESPONSE 0x0001
#define ACEBT_BLE_WRITE_TYPE_DEFAULT 0x0002
#define ACEBT_BLE_WRITE_TYPE_SIGNED 0x0004
/** @} */

/**
 * @name Attribute format
 * @brief Attribute form used for @ref aceBT_bleGattAttributeFormat
 * @{
 */
/** BLE attribute data format*/
#define ACEBT_BLE_FORMAT_UINT8 0x11
#define ACEBT_BLE_FORMAT_UINT16 0x12
#define ACEBT_BLE_FORMAT_UINT32 0x14

#define ACEBT_BLE_FORMAT_SINT8 0x21
#define ACEBT_BLE_FORMAT_SINT16 0x22
#define ACEBT_BLE_FORMAT_SINT32 0x24

#define ACEBT_BLE_FORMAT_SFLOAT 0x32
#define ACEBT_BLE_FORMAT_FLOAT 0x34
#define ACEBT_BLE_FORMAT_BLOB 0xFF
/** @} */

/**
 * @name Authentication mode
 * @{
 */
#define AUTHENTICATION_NONE 0    /**< No authentication */
#define AUTHENTICATION_NO_MITM 1 /**< Authentication with no MITM*/
#define AUTHENTICATION_MITM 2    /**< Authentication with MITM*/
/** @} */

/** @brief BLE Attribute permission */
typedef uint16_t aceBT_bleGattAttributePermission;

/** @brief BLE Attribute property */
typedef uint8_t aceBT_bleGattAttributeProperty;

/** @brief BLE Attribute format */
typedef uint8_t aceBT_bleGattAttributeFormat;

/** @brief BLE GATT service type */
typedef enum {
    ACEBT_BLE_GATT_SERVICE_TYPE_PRIMARY,   /**< Primary service */
    ACEBT_BLE_GATT_SERVICE_TYPE_SECONDARY, /**< Secondary service */
    ACEBT_BLE_GATT_SERVICE_TYPE_INCLUDED,  /**< Included service */
} aceBT_bleGattServiceType_t;

/** @brief BLE GATT callback request type */
typedef enum {
    ACEBT_BLE_GATT_REQ_READ, /**< Read request */
    ACEBT_BLE_GATT_REQ_WRITE /**< Write request */
} aceBT_bleGattCallbackRequestType_t;

/** @brief ATT handle value notification structure. */
typedef struct {
    uint16_t attHandle; /**< Attribute handle */
    uint8_t attVal[1];  /**< Attribute value */
} aceBT_bleGattNotificationValue_t;

/** @brief BLE GATT record */
typedef struct {
    aceBT_uuid_t uuid;                      /**< Attribute uuid */
    aceBT_bleGattAttributeProperty attProp; /**< Attribure Property */
    aceBT_bleGattAttributePermission
        attPerm;     /**< Format of the characteristics value */
    uint16_t handle; /**< GATT characteristcs handle */
} aceBT_bleGattRecord_t;

/** @brief BLE Blob value */
typedef struct {
    uint16_t size;   /**< Data size */
    uint16_t offset; /**< Data offset */
    uint8_t* data;   /**< Data pointer */
} aceBT_bleGattBlobValue_t;

/** @brief BLE GATT Descriptor */
typedef struct {
    aceBT_bleGattRecord_t gattRecord;   /**< GATT record */
    aceBT_bleGattBlobValue_t blobValue; /**< Blob value */
    bool is_set;                        /**< True if the descriptor is set */
    bool is_notify;                     /**< True if notify is enabled */
    uint8_t desc_auth_retry;            /**< Attribute uuid */
    uint8_t write_type;                 /**< Write type */
} aceBT_bleGattDescriptor_t;

/** @brief BLE GATT Descriptor record */
struct aceBT_gattDescRec_t {
    STAILQ_ENTRY(aceBT_gattDescRec_t) link; /**< Descriptor record */
    aceBT_bleGattDescriptor_t value;        /**< Descriptor value */
};

STAILQ_HEAD(gattDescList, aceBT_gattDescRec_t);

/** @brief BLE GATT Characteristics */
typedef struct {
    union {
        uint8_t uint8Val;
        uint16_t uint16Val;
        uint32_t uint32Val;
        int8_t int8Val;
        int16_t int16Val;
        int32_t int32Val;
        aceBT_bleGattBlobValue_t blobValue;
    };
    aceBT_bleGattAttributeFormat
        format; /**< Format of the characteristics value 0x00FF is a blob*/
    aceBT_bleGattRecord_t gattRecord;         /**< GATT record */
    aceBT_bleGattDescriptor_t gattDescriptor; /**< GATT descriptor */
    uint8_t auth_retry;                       /**< Authentication entry */
    uint8_t read_auth_retry;                  /**< Read authentication entry */
    uint8_t write_type;                       /**< Write type */
    struct gattDescList descList;             /**< Descriptor list */
    uint8_t multiDescCount; /**< Number of descriptors in @ref descList */
} aceBT_bleGattCharacteristicsValue_t;

STAILQ_HEAD(gattCharsList, aceBT_gattCharRec_t);
STAILQ_HEAD(gattIncludedSvcList, aceBT_gattIncSvcRec_t);

/** @brief Service structure of the GATTS. */
typedef struct {
    aceBT_bleGattServiceType_t serviceType; /**< Service type */
    aceBT_uuid_t uuid;                      /**< UUID value */
    uint16_t handle;                        /**< Service handle */
    uint16_t no_characteristics; /**< Number of characteristic records in @ref
                                    charsList */
    uint16_t no_desc; /**< Total number of descriptors in @ref charsList */
    uint16_t no_included_svc; /**< Number of services in @ref incSvcList */
    struct gattIncludedSvcList incSvcList; /**< Included services list */
    struct gattCharsList charsList;        /**< Characteristics list */
    bool continue_decleration;             /**< True for continue decleration */
} aceBT_bleGattsService_t;

/** @brief BLE GATT Characteristics record */
struct aceBT_gattCharRec_t {
    STAILQ_ENTRY(aceBT_gattCharRec_t) link;    /**< Next record */
    aceBT_bleGattCharacteristicsValue_t value; /**< Charactieristic value */
    bool descriptor_added; /**< True if descriptor is added */
};

/** @brief BLE GATT included service record */
struct aceBT_gattIncSvcRec_t {
    STAILQ_ENTRY(aceBT_gattIncSvcRec_t) link; /**< Next record */
    aceBT_bleGattsService_t value;            /**< Charactieristic value */
};

/** @brief BLE GATT descriptor service record */
struct aceBT_gattDecRec_t {
    STAILQ_ENTRY(aceBT_gattDecRec_t) link; /**< Next record */
    aceBT_bleGattDescriptor_t value;       /**< Charactieristic value */
};

/** @brief BLE GATT Characteristics */
typedef struct {
    ace_status_t status;              /**< @deprecated Response status*/
    int requestId;                      /**< Request ID for the response */
    aceBT_bleGattBlobValue_t blobValue; /**< Blob value */
    uint8_t errorStatus;                /**< General GATT Error Status */
} __attribute__((packed)) aceBT_bleGattResp_t;

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BLE_DEFINES_H
