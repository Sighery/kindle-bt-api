/**
 * @file bluetooth_defines.h
 *
 * @brief ACE BT defines
 */

#ifndef BT_DEFINES_H
#define BT_DEFINES_H

#include <stdint.h>
#include <stddef.h>
#include "ace_status.h"
// #include "ace_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BT_DS_COMMON Common Bluetooth Data structures and defines
 * @{
 * @ingroup ACE_BT_DS
 */

/**
 * @brief Length of MAC addresses.
 */
#define ACEBT_MAC_ADDR_LEN 6
/**
 * @brief BT Device name buffer size.
 */
#define ACEBT_BD_NAME_LEN 249
/**
 * @brief BT Device short name buffer size.
 */
#define ACEBT_BD_NAME_SHORT_LEN 80
/**
 * @brief Maximum length of BT pairing pin.
 */
#define ACEBT_PAIRING_PIN_LENGTH_MAX 16
/**
 * @brief Maximum UUID length.
 */
#define ACEBT_UUID_LENGTH_MAX 16
/**
 * @brief Maximum number of UUIDs.
 */
#define ACEBT_MAX_NUM_UUIDS 32
/**
 * @brief Length of MAC address string.
 */
#define ACEBT_MAC_ADDR_STR_LEN 18  // Format XX:XX:XX:XX:XX:XX
/**
 * @brief Length 128 bit UUID string.
 * @note Format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
 */
#define ACEBT_128_BIT_UUID_STR_LEN 37
/**
 * @brief Length 32 bit UUID string.
 * @note Format XXXXXXXX
 */
#define ACEBT_32_BIT_UUID_STR_LEN 9
/**
 * @brief Length 16 bit UUID string.
 * @note Format XXXX
 */
#define ACEBT_16_BIT_UUID_STR_LEN 5
/**
 * @brief Used to prevent unused variable compiler warnings.
 */
#define UNUSED(x) (void)(x)

/**
 * @brief Bluetooth adapter states.
 */
typedef enum {
    ACEBT_STATE_DISABLED = 0, /**< Adapter state is Radio Disabled */
    ACEBT_STATE_ENABLED = 1,  /**< Adapter state is Radio Enabled */
    ACEBT_STATE_ENABLING = 2, /**< Adapter state is Radio being enabled */
    ACEBT_STATE_DISABLING = 3 /**< Adapter state is Radio being disabled */
} aceBT_state_t;

/**
 * @brief Bluetooth device connection state.
 * These states represent the device level connection states.
 */
typedef enum {
    ACEBT_CONN_STATE_CONNECTED = 0,    /**< Connected state */
    ACEBT_CONN_STATE_DISCONNECTED = 1, /**< Disconnected state */
    ACEBT_CONN_STATE_CONNECTING = 2,   /**< Connecting state */
    ACEBT_CONN_STATE_DISCONNECTING = 3 /**< Disconnecting state */
} aceBT_connState_t;

// /**
//  * @brief Bluetooth HFP Connection State.
//  */
// typedef enum {
//     ACEBT_HFP_CONN_STATE_DISCONNECTED =
//         0,                               /**< HFP profile Disconnected state */
//     ACEBT_HFP_CONN_STATE_CONNECTING = 1, /**< HFP profile Connecting state */
//     ACEBT_HFP_CONN_STATE_CONNECTED = 2,  /**< HFP profile Connected state */
//     ACEBT_HFP_CONN_STATE_SLC_CONNECTED =
//         3, /**< HFP profile SLC Connected state */
//     ACEBT_HFP_CONN_STATE_DISCONNECTING =
//         4 /**< HFP profile Disconnecting state */
// } aceBT_hfpConnState_t;

/**
 * @brief Discovery states
 */
typedef enum {
    ACEBT_DISCOVERY_STATE_STOPPED = 0, /**< Discovery stopped state */
    ACEBT_DISCOVERY_STATE_STARTED = 1  /**< Discovery started state */
} aceBT_discoveryState_t;

// /**
//  * @name Pairing States
//  * @brief BT Device Type @ref aceBT_pairingState_t
//  * @{
//  */
// /** Not Paired */
// #define ACEBT_PAIRING_STATE_NONE 0
// /** Pairing */
// #define ACEBT_PAIRING_STATE_PAIRING 1
// /** Pairing State typedef*/
// typedef uint8_t aceBT_pairingState_t;
// /** @} */

// /**
//  * @brief Hands Free Audio states
//  */
// typedef enum {
//     ACEBT_HFP_AUDIO_STATE_DISCONNECTED = 0, /**< HFP audio Disconnected state */
//     ACEBT_HFP_AUDIO_STATE_CONNECTING = 1,   /**< HFP audio Connecting state */
//     ACEBT_HFP_AUDIO_STATE_CONNECTED = 2,    /**< HFP audio Connected state */
//     ACEBT_HFP_AUDIO_STATE_DISCONNECTING =
//         3 /**< HFP audio Disconnecting state */
// } aceBT_hfpAudioState_t;

/**
 * @brief Audio states
 */
typedef enum {
    ACEBT_AUDIO_STATE_SUSPENDED = 0, /**< Audio suspended state */
    ACEBT_AUDIO_STATE_STOPPED = 1,   /**< Audio stopped state */
    ACEBT_AUDIO_STATE_STARTED = 2    /**< Audio started state */
} aceBT_audioState_t;

// /**
//  * @brief Profile connection states.
//  * The profile level connection states.
//  */
// typedef enum {
//     ACEBT_PROFILE_STATE_CONNECTED = 0,    /**< Profile Connected state */
//     ACEBT_PROFILE_STATE_DISCONNECTED = 1, /**< Profile Disconnected state */
//     ACEBT_PROFILE_STATE_CONNECTING = 2,   /**< Profile Connecting state */
//     ACEBT_PROFILE_STATE_DISCONNECTING = 3 /**< Profile Disconnecting state*/
// } aceBT_profileState_t;

/**
 * @brief Bluetooth Address
 */
typedef struct {
    uint8_t address[ACEBT_MAC_ADDR_LEN];
} __attribute__((packed)) aceBT_bdAddr_t;

/**
 * @brief Bluetooth Device Name
 */
typedef struct {
    uint8_t name[ACEBT_BD_NAME_LEN];
} __attribute__((packed)) aceBT_bdName_t;

/**
 * @brief Bluetooth Adapter Visibility Modes
 */
typedef enum {
    ACEBT_SCAN_MODE_NONE = 0,        /**< ScanMode is not connectable and not
                                    discoverable*/
    ACEBT_SCAN_MODE_CONNECTABLE = 1, /**< ScanMode is connectable but not
                                    discoverable */
    ACEBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE = 2 /**< ScanMode is connectable
                                                and discoverable */
} aceBT_scanMode_t;

// /**
//  * @brief Bluetooth Page-Scan intervals
//  */
// typedef enum {
//     ACEBT_PAGE_SCAN_INTERVAL_200MS = 0, /**< Page scan interval of 200ms */
//     ACEBT_PAGE_SCAN_INTERVAL_1280MS = 1 /**< Page scan interval of 1280ms */
// } aceBT_scanParam_t;

// /**
//  * @brief Bluetooth Scan types
//  */
// typedef enum {
//     ACEBT_SCAN_TYPE_STANDARD = 0,  /**< Standard Scan */
//     ACEBT_SCAN_TYPE_INTERLACED = 1 /**< Interlaced Scan for BT 1.2+ devices */
// } aceBT_scanType_t;

// /**
//  * @name BT Device Types
//  * @brief BT Device Type @ref aceBT_deviceType_t
//  * @{
//  */
// /** Bluetooth Classic Device Type */
// #define ACEBT_DEVICE_DEVTYPE_BREDR 0x01
// /** Bluetooth Low Energy Device Type */
// #define ACEBT_DEVICE_DEVTYPE_BLE 0x02
// /** Dual Bluetooth Type */
// #define ACEBT_DEVICE_DEVTYPE_DUAL 0x03
// /** Bluetooth Device Type */
// typedef uint8_t aceBT_deviceType_t;
// /** @} */

/**
 * @name BT Transport Types
 * @brief BT Transport Type @ref aceBT_transportType_t
 * @{
 */
/** Bluetooth Transport Automatic Type. */
#define ACEBT_TRANSPORT_AUTO 0x00
/** Bluetooth Transport Enhanced Data Rate Type. */
#define ACEBT_TRANSPORT_EDR 0x01
/** Bluetooth Transport Low Energy Type. */
#define ACEBT_TRANSPORT_LE 0x02
/** Bluetooth Transport Type */
typedef uint8_t aceBT_transportType_t;
/** @} */

/**
 * @name Bond States
 * @brief BT Bond States @ref aceBT_bondState_t
 * @{
 */
/** Bluetooth Bond State - not bonded. */
#define ACEBT_BOND_STATE_NONE 0x00
/** Bluetooth Bond State - bonding. */
#define ACEBT_BOND_STATE_BONDING 0x01
/** Bluetooth Bond State - bonded. */
#define ACEBT_BOND_STATE_BONDED 0x02
/** Bluetooth Bond State */
typedef uint8_t aceBT_bondState_t;
/** @} */

/**
 * @brief Bluetooth SSP Bonding Variant
 */
typedef enum {
    ACEBT_SSP_VARIANT_PASSKEY_CONFIRMATION = 1, /**< Passkey Confirmation */
    ACEBT_SSP_VARIANT_PASSKEY_ENTRY = 2,        /**< Passkey entry */
    ACEBT_SSP_VARIANT_CONSENT = 3,              /**< Consent */
    ACEBT_SSP_VARIANT_PASSKEY_NOTIFICATION = 4  /**< Passkey notification */
} aceBT_sspVariant_t;

/**
 * @brief Bluetooth ACL Disconnect Reason
 * From Bluetooth Core Spec Vol2, Part D Error Codes
 * Unknown disconnect reason will be treated as @ref ACE_BT_ACL_UNSPECIFIED
 */
typedef enum {
    ACE_BT_ACL_SUCCESS = 0x00,            /**< Success */
    ACE_BT_ACL_ILLEGAL_COMMAND = 0x01,    /**< Unknown HCI Command */
    ACE_BT_ACL_NO_CONNECTION = 0x02,      /**< Unknown Connection Identifier */
    ACE_BT_ACL_HW_FAILURE = 0x03,         /**< Hardware Failure */
    ACE_BT_ACL_PAGE_TIMEOUT = 0x04,       /**< Page Timeout */
    ACE_BT_ACL_AUTH_FAILURE = 0x05,       /**< Authentication Failure */
    ACE_BT_ACL_KEY_MISSING = 0x06,        /**< PIN or Key Missing */
    ACE_BT_ACL_MEM_FULL = 0x07,           /**< Memory Capacity Exceeded */
    ACE_BT_ACL_CONNECTION_TIMEOUT = 0x08, /**< Connection Timeout */
    ACE_BT_ACL_MAX_NUM_OF_CONNECTIONS = 0x09, /**< Connection Limit Exceeded */
    ACE_BT_ACL_MAX_NUM_OF_SCOS =
        0x0A, /**< Synchronous Connection Limit To A Device Exceeded */
    ACE_BT_ACL_CONNECTION_EXISTS = 0x0B,  /**< Connection Already Exists */
    ACE_BT_ACL_COMMAND_DISALLOWED = 0x0C, /**< Command Disallowed */
    ACE_BT_ACL_HOST_REJECT_RESOURCE =
        0x0D, /**< Connection Rejected due to Limited Resources */
    ACE_BT_ACL_HOST_REJECT_SECURITY =
        0x0E, /**< Connection Rejected Due To Security Reasons */
    ACE_BT_ACL_HOST_REJECT_DEVICE =
        0x0F, /**< Connection Rejected due to Unacceptable BD_ADDR */
    ACE_BT_ACL_HOST_TIMEOUT = 0x10, /**< Connection Accept Timeout Exceeded */
    ACE_BT_ACL_UNSUPPORTED_VALUE =
        0x11, /**< Unsupported Feature or Parameter Value */
    ACE_BT_ACL_ILLEGAL_PARAMETER_FMT =
        0x12,                    /**< Invalid HCI Command Parameters */
    ACE_BT_ACL_PEER_USER = 0x13, /**< Remote User Terminated Connection */
    ACE_BT_ACL_PEER_LOW_RESOURCES =
        0x14, /**< Remote Device Terminated Connection due to Low Resources */
    ACE_BT_ACL_PEER_POWER_OFF =
        0x15, /**< Remote Device Terminated Connection due to Power Off */
    ACE_BT_ACL_HOST_USER = 0x16, /**< Connection Terminated By Local Host */
    ACE_BT_ACL_REPEATED_ATTEMPTS = 0x17,   /**< Repeated Attempts */
    ACE_BT_ACL_PAIRING_NOT_ALLOWED = 0x18, /**< Pairing Not Allowed */
    ACE_BT_ACL_UNKNOWN_LMP_PDU = 0x19,     /**< Unknown LMP PDU */
    ACE_BT_ACL_UNSUPPORTED_REM_FEATURE =
        0x1A, /**< Unsupported Remote Feature / Unsupported LMP Feature */
    ACE_BT_ACL_SCO_OFFSET_REJECTED = 0x1B,   /**< SCO Offset Rejected */
    ACE_BT_ACL_SCO_INTERVAL_REJECTED = 0x1C, /**< SCO Interval Rejected */
    ACE_BT_ACL_SCO_AIR_REJECTED = 0x1D,      /**< SCO Air Mode Rejected */
    ACE_BT_ACL_INVALID_LMP_PARAM =
        0x1E, /**< Invalid LMP Parameters / Invalid LL Parameters */
    ACE_BT_ACL_UNSPECIFIED = 0x1F, /**< Unspecified Error */
    ACE_BT_ACL_UNSUPPORTED_LMP_FEATURE =
        0x20, /**< Unsupported LMP Parameter Value / Unsupported LL Parameter
                 Value */
    ACE_BT_ACL_ROLE_CHANGE_NOT_ALLOWED = 0x21, /**< Role Change Not Allowed */
    ACE_BT_ACL_LMP_RESPONSE_TIMEOUT =
        0x22, /**< LMP Response Timeout / LL Response Timeout */
    ACE_BT_ACL_LMP_ERR_TRANS_COLLISION =
        0x23, /**< LMP Error Transaction Collision / LL Procedure Collision */
    ACE_BT_ACL_LMP_PDU_NOT_ALLOWED = 0x24, /**< LMP PDU Not Allowed */
    ACE_BT_ACL_ENCRY_MODE_NOT_ACCEPTABLE =
        0x25,                            /**< Encryption Mode Not Acceptable */
    ACE_BT_ACL_UNIT_KEY_USED = 0x26,     /**< Link Key cannot be Changed */
    ACE_BT_ACL_QOS_NOT_SUPPORTED = 0x27, /**< Requested QoS Not Supported */
    ACE_BT_ACL_INSTANT_PASSED = 0x28,    /**< Instant Passed */
    ACE_BT_ACL_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED =
        0x29, /**< Pairing With Unit Key Not Supported */
    ACE_BT_ACL_DIFF_TRANSACTION_COLLISION =
        0x2A, /**< Different Transaction Collision */
    ACE_BT_ACL_QOS_UNACCEPTABLE_PARAM = 0x2C, /**< QoS Unacceptable Parameter */
    ACE_BT_ACL_QOS_REJECTED = 0x2D,           /**< QoS Rejected */
    ACE_BT_ACL_CHAN_CLASSIF_NOT_SUPPORTED =
        0x2E, /**< Channel Classification Not Supported */
    ACE_BT_ACL_INSUFFCIENT_SECURITY = 0x2F, /**< Insufficient Security */
    ACE_BT_ACL_PARAM_OUT_OF_RANGE =
        0x30, /**< Parameter Out Of Mandatory Range */
    ACE_BT_ACL_ROLE_SWITCH_PENDING = 0x32,     /**< Role Switch Pending */
    ACE_BT_ACL_RESERVED_SLOT_VIOLATION = 0x34, /**< Reserved Slot Violation */
    ACE_BT_ACL_ROLE_SWITCH_FAILED = 0x35,      /**< Role Switch Failed */
    ACE_BT_ACL_INQ_RSP_DATA_TOO_LARGE =
        0x36, /**< Extended Inquiry Response Too Large */
    ACE_BT_ACL_SIMPLE_PAIRING_NOT_SUPPORTED =
        0x37, /**< Secure Simple Pairing Not Supported By Host */
    ACE_BT_ACL_HOST_BUSY_PAIRING = 0x38, /**< Host Busy - Pairing */
    ACE_BT_ACL_REJ_NO_SUITABLE_CHANNEL =
        0x39, /**< Connection Rejected due to No Suitable Channel Found */
    ACE_BT_ACL_CONTROLLER_BUSY = 0x3A, /**< Controller Busy */
    ACE_BT_ACL_UNACCEPT_CONN_INTERVAL =
        0x3B, /**< Unacceptable Connection Parameters */
    ACE_BT_ACL_DIRECTED_ADVERTISING_TIMEOUT = 0x3C, /**< Advertising Timeout */
    ACE_BT_ACL_CONN_TOUT_DUE_TO_MIC_FAILURE =
        0x3D, /**< Connection Terminated due to MIC Failure */
    ACE_BT_ACL_CONN_FAILED_ESTABLISHMENT =
        0x3E, /**< Connection Failed to be Established */
    ACE_BT_ACL_MAC_CONNECTION_FAILED = 0x3F, /**< MAC Connection Failed */
} aceBt_aclDisconnectReason_t;

/**
 * @brief Bluetooth ACL callback data structure
 */
typedef struct {
    aceBT_transportType_t transport;    /**< ACL transport type */
    aceBt_aclDisconnectReason_t reason; /**< HCI reason for ACL disconnect */
} aceBt_aclData_t;

// /**
//  * @cond DEPRECATED
//  * @deprecated Bluetooth status codes. Please use @ref ace_status_t .
//  * @{
//  */
// typedef ace_status_t aceBT_status_t;

// /** @brief Success status. */
// #define ACEBT_STATUS_SUCCESS ACE_STATUS_OK
// /** @brief Error status due to Generic failure. */
// #define ACEBT_STATUS_FAIL ACE_STATUS_GENERAL_ERROR
// /** @brief Error status due to BT middleware not ready. */
// #define ACEBT_STATUS_NOT_READY ACE_STATUS_UNINITIALIZED
// /** @brief Error status due to no memory in system.  */
// #define ACEBT_STATUS_NOMEM ACE_STATUS_OUT_OF_MEMORY
// /** @brief Error status due to BT middleware being
//  *  busy with another operation.
//  */
// #define ACEBT_STATUS_BUSY ACE_STATUS_BUSY
// /** @brief Error status due to operation not being supported */
// #define ACEBT_STATUS_UNSUPPORTED ACE_STATUS_NOT_SUPPORTED
// /** @brief Error status due to passed parameter being invalid  */
// #define ACEBT_STATUS_PARM_INVALID ACE_STATUS_BAD_PARAM
// /** @brief Error status due to authentication failure */
// #define ACEBT_STATUS_AUTH_FAILURE ACE_STATUS_NET_AUTH_FAILURE
// /** @brief Error status due to operation not found */
// #define ACEBT_STATUS_NOT_FOUND ACE_STATUS_NOT_FOUND
// /** @brief Error status due to maximum buffer being utilized */
// #define ACEBT_STATUS_MAX_BUFF ACE_STATUS_OUT_OF_RESOURCES
// /** @brief Error status due to link loss */
// #define ACEBT_STATUS_LINK_LOSS ACE_STATUS_NET_CONNECTION_TIMEOUT_ERROR

// /**!< Below error codes are redundant and will be removed.*/
// /** @brief Error status due to JNI environment.
//  *  Unused and to be removed.
//  */
// #define ACEBT_STATUS_JNI_ENVIRONMENT_ERROR ACE_STATUS_BT_JNI_ENVIRONMENT_ERROR
// /** @brief Error status due to JNI thread malfunction.
//  *  Unused and to be removed.
//  */
// #define ACEBT_STATUS_JNI_THREAD_ATTACH_ERROR \
//     ACE_STATUS_BT_JNI_THREAD_ATTACH_ERROR
// /** @brief Error status due to wakelock.
//  * Unused and to be removed.
//  */
// #define ACEBT_STATUS_WAKELOCK_ERROR ACE_STATUS_BT_WAKELOCK_ERROR
// /** @brief Status for a pending connection.
//  *  Unused and to be removed.
//  */
// #define ACEBT_STATUS_CONN_PENDING ACE_STATUS_BT_CONN_PENDING
// /** @brief Error status due to connection timeout
//  *  in auth procedure. Unused and to be removed.
//  */
// #define ACEBT_STATUS_AUTH_FAIL_CONN_TIMEOUT ACE_STATUS_BT_AUTH_FAIL_CONN_TIMEOUT
// /** @brief Error status due remote device disconnection.
//  *  Unused and to be removed.
//  */
// #define ACEBT_STATUS_RMT_DEV_DOWN ACE_STATUS_BT_RMT_DEV_DOWN
// /** @brief request already completed.
//  *  To be removed, use ACE_STATUS_OK instead.
//  */
// #define ACEBT_STATUS_DONE ACE_STATUS_BT_DONE
// /** @brief Error status due to SMP failure.
//  *  To be removed, use ACE_STATUS_NET_AUTH_FAILURE instead.
//  */
// #define ACEBT_STATUS_AUTH_FAIL_SMP_FAIL ACE_STATUS_BT_AUTH_FAIL_SMP_FAIL
// /** @brief Error status due to unhandled operation.
//  *  To be removed, use ACE_STATUS_GENERAL_ERROR instead.
//  */
// #define ACEBT_STATUS_UNHANDLED ACE_STATUS_BT_UNHANDLED
// /** @brief Error status due to authentication rejected by remote device
//  * To be removed, use ACE_STATUS_NET_AUTH_FAILURE instead.
//  */
// #define ACEBT_STATUS_AUTH_REJECTED ACE_STATUS_BT_AUTH_REJECTED

// /**
//  * @}
//  * @endcond
//  */ // cond DEPRECATED

/**
 * @brief Bluetooth Profiles
 */
typedef enum {
    ACEBT_PROFILE_HEADSET = 0,          /**< Headset and Handsfree profile */
    ACEBT_PROFILE_A2DP_SOURCE = 1,      /**< A2DP Source profile */
    ACEBT_PROFILE_AVRCP_TARGET = 2,     /**< AVRCP Target profile */
    ACEBT_PROFILE_A2DP_SINK = 3,        /**< A2DP Sink profile */
    ACEBT_PROFILE_AVRCP_CONTROLLER = 4, /**< AVRCP Controller profile */
    ACEBT_PROFILE_INPUT_DEVICE =
        5, /**< Human Input Device (HID) Host profile */
    ACEBT_PROFILE_HEADSET_CLIENT =
        6,                         /**< Headset Client (HFP-HF role) profile */
    ACEBT_PROFILE_GATT_CLIENT = 7, /**< GATT Client profile */
    ACEBT_PROFILE_GATT_SERVER = 8, /**< GATT Server profile */
    ACEBT_PROFILE_MAX_ID = 9       /**< Max profile index */
} aceBT_profile_t;

// /**
//  * @name Profile Masks
//  * @brief Profile Masks @ref aceBT_profilesMask_t
//  * @{
//  */
// /** @brief Mask for HFP. */
// #define ACEBT_PROFILE_MASK_HEADSET (1 << ACEBT_PROFILE_HEADSET)
// /** @brief Mask for A2DP. */
// #define ACEBT_PROFILE_MASK_A2DP (1 << ACEBT_PROFILE_A2DP_SOURCE)
// /** @brief Mask for AVRCP Target. */
// #define ACEBT_PROFILE_MASK_AVRCP_TARGET (1 << ACEBT_PROFILE_AVRCP_TARGET)
// /** @brief Mask for A2DP Sink. */
// #define ACEBT_PROFILE_MASK_A2DP_SINK (1 << ACEBT_PROFILE_A2DP_SINK)
// /** @brief Mask for AVRCP Controller. */
// #define ACEBT_PROFILE_MASK_AVRCP_CONTROLLER \
//     (1 << ACEBT_PROFILE_AVRCP_CONTROLLER)
// /** @brief Mask for HID Profile. */
// #define ACEBT_PROFILE_MASK_HID (1 << ACEBT_PROFILE_INPUT_DEVICE)
// /** @brief Mask for HFP Client. */
// #define ACEBT_PROFILE_MASK_HEADSET_CLIENT (1 << ACEBT_PROFILE_HEADSET_CLIENT)
// /** @brief Mask for GATT Client. */
// #define ACEBT_PROFILE_MASK_GATT_CLIENT (1 << ACEBT_PROFILE_GATT_CLIENT)
// /** @brief Mask for GATT Server. */
// #define ACEBT_PROFILE_MASK_GATT_SERVER (1 << ACEBT_PROFILE_GATT_SERVER)
// /** @brief Macro to create a profile mask. */
// #define ACEBT_CREATE_PROFILE_MASK(x) (x < ACEBT_PROFILE_MAX_ID) ? (1 << x) : 0
// /** @brief Bluetooth profile mask typedef */
// typedef uint32_t aceBT_profilesMask_t;
// /** @} */

/* @brief Bluetooth Adapter and Remote Device property types */
typedef enum {
    /* Properties common to both adapter and remote device */
    /**
     * Description - Bluetooth Device Name
     * Access mode - Adapter name can be GET/SET. Remote device can be GET
     * Data type   - bt_bdname_t
     */
    ACEBT_PROPERTY_BDNAME = 0x01,
    /**
     * Description - Bluetooth Device Address
     * Access mode - Only GET.
     * Data type   - bt_bdaddr_t
     */
    ACEBT_PROPERTY_BDADDR = 0x02,
    /**
     * Description - Bluetooth Service 128-bit UUIDs
     * Access mode - Only GET.
     * Data type   - Array of bt_uuid_t (Array size inferred from property
     * length).
     */
    ACEBT_PROPERTY_UUIDS = 0x03,
    /**
     * Description - Bluetooth Class of Device as found in Assigned Numbers
     * Access mode - Only GET.
     * Data type   - uint32_t.
     */
    ACEBT_PROPERTY_CLASS_OF_DEVICE = 0x04,
    /**
     * Description - Device Type - BREDR, BLE or DUAL Mode
     * Access mode - Only GET.
     * Data type   - bt_device_type_t
     */
    ACEBT_PROPERTY_TYPE_OF_DEVICE = 0x05,
    /**
     * Description - Bluetooth Service Record
     * Access mode - Only GET.
     * Data type   - bt_service_record_t
     */
    ACEBT_PROPERTY_SERVICE_RECORD = 0x06,
    /* Properties unique to adapter */
    /**
     * Description - Bluetooth Adapter scan mode
     * Access mode - GET and SET
     * Data type   - bt_scan_mode_t.
     */
    ACEBT_PROPERTY_ADAPTER_SCAN_MODE = 0x07,
    /**
     * Description - List of bonded devices
     * Access mode - Only GET.
     * Data type   - Array of bt_bdaddr_t of the bonded remote devices
     *               (Array size inferred from property length).
     */
    ACEBT_PROPERTY_ADAPTER_BONDED_DEVICES = 0x08,
    /**
     * Description - Bluetooth Adapter Discovery timeout (in seconds)
     * Access mode - GET and SET
     * Data type   - uint32_t
     */
    ACEBT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT = 0x09,

    /* Properties unique to remote device */
    /**
     * Description - User defined friendly name of the remote device
     * Access mode - GET and SET
     * Data type   - bt_bdname_t.
     */
    ACEBT_PROPERTY_REMOTE_FRIENDLY_NAME = 0x0A,
    /**
     * Description - RSSI value of the inquired remote device
     * Access mode - Only GET.
     * Data type   - int32_t.
     */
    ACEBT_PROPERTY_REMOTE_RSSI = 0x0B,
    /**
     * Description - Remote version info
     * Access mode - SET/GET.
     * Data type   - bt_remote_version_t.
     */
    ACEBT_PROPERTY_REMOTE_VERSION_INFO = 0x0C,
    /**
     * Description - Local LE features
     * Access mode - GET.
     * Data type   - bt_local_le_features_t.
     */
    ACEBT_PROPERTY_LOCAL_LE_FEATURES = 0x0D,
    /**
     * Description - Manufacturer specific info on EIR
     * Access mode - GET
     * Data type   - bt_eir_manf_info_t.
     */
    ACEBT_PROPERTY_EIR_MANF_INFO = 0xFE,
    /**
     * Description - Time at which remote device had bonded first time
     * Access mode - GET
     * Data type   - int32_t.
     */
    ACEBT_PROPERTY_REMOTE_DEVICE_TIMESTAMP = 0xFF,
    /* Properties unique to adapter */
    /**
     * Description - Bluetooth Adapter scan parameters
     * Access mode - SET
     * Data type   - bt_scan_param_t.
     */
    ACEBT_PROPERTY_ADAPTER_SCAN_PARAM = 0x100,
    /**
     * Description - Bluetooth Adapter's connectability
     * Access mode - GET and SET
     * Data type   - 2 * uint32_t
     */
    ACEBT_PROPERTY_CONNECTABILITY = 0x101,
    /**
     * Description - Bluetooth Adapter's scan type (Standard or Interlaced)
     * Access mode - GET and SET
     * Data type   - bt_scan_type_t
     */
    ACEBT_PROPERTY_SCAN_TYPE = 0x102,

    ACEBT_PROPERTY_GADGET_EIR_MANF_INFO = 0x103,
} aceBT_propertyType_t;

/**
 * @brief Bluetooth Adapter Property
 */
typedef struct {
    aceBT_propertyType_t type;
    size_t len;
    void* val;
} aceBT_property_t;

// /**
//  * @brief PIN Code Array
//  */
// typedef struct {
//     uint8_t pin[ACEBT_PAIRING_PIN_LENGTH_MAX];
// } __attribute__((packed)) aceBT_pinCode_t;

/**
 * @brief Bluetooth UUID Types
 */
typedef enum {
    ACEBT_UUID_TYPE_16 = 0,  /**< 16 bit UUID */
    ACEBT_UUID_TYPE_32 = 1,  /**< 32 bit UUID */
    ACEBT_UUID_TYPE_128 = 2, /**< 128 bit UUID */
} aceBT_UUIDType_t;

/**
 * @brief Struct for Bluetooth UUID Representation
 */
typedef struct {
    uint8_t uu[ACEBT_UUID_LENGTH_MAX];
    aceBT_UUIDType_t type;
} __attribute__((packed)) aceBT_uuid_t;

// /**
//  * @brief EIR Informations
//  */
// typedef struct {
//     uint32_t vendor;
//     uint32_t product;
// } aceBT_eir_manf_info_t;

// /**
//  * @brief Gadget EIR Information
//  */
// typedef struct {
//     uint32_t vendor;
//     uint32_t product;
//     uint32_t uuid;
// } aceBT_gadget_eir_manf_info_t;

// /**
//  * @brief EIR Manufacturer Information
//  */
// typedef struct {
//     aceBT_propertyType_t type;
//     union {
//         aceBT_eir_manf_info_t eir;
//         aceBT_gadget_eir_manf_info_t gadget_eir;
//     };
// } aceBT_eirManfInfo_t;

// /**
//  * @brief Struct for Local Device Properties
//  */
// typedef struct {
//     aceBT_bdAddr_t addr;
//     aceBT_bdName_t name;
//     aceBT_deviceType_t type;
// } __attribute__((packed)) aceBT_deviceProperties_t;

// /**
//  * @brief Struct for Device List
//  */
// typedef struct {
//     uint16_t num_devices;
//     aceBT_bdAddr_t p_devices[];
// } __attribute__((packed)) aceBT_deviceList_t;

// /**
//  * @brief HFP Voice Recognition
//  */
// typedef enum {
//     ACEBT_HFP_VR_STATE_STOPPED = 0, /**< Stopped */
//     ACEBT_HFP_VR_STATE_STARTED = 1  /**< Started */
// } aceBT_hfpVrState_t;

// /**
//  * @brief HFP Volume Type
//  */
// typedef enum {
//     ACEBT_HFP_VOLUME_TYPE_SPK = 0, /**< Speaker */
//     ACEBT_HFP_VOLUME_TYPE_MIC = 1  /**< MIC */
// } aceBT_hfpVolumeType_t;

// /**
//  * @brief HFP Noise Reduction and Echo Cancellation
//  */
// typedef enum {
//     ACEBT_HFP_NREC_STOP = 0, /**< NREC Stop */
//     ACEBT_HFP_NREC_START = 1 /**< NREC Start */
// } aceBT_hfpNrec_t;

// /**
//  * @brief HFP WBS Code Setting
//  */
// typedef enum {
//     ACEBT_HFP_WBS_NONE = 0, /**< WBS NONE */
//     ACEBT_HFP_WBS_NO = 1,   /**< WBS Disabled */
//     ACEBT_HFP_WBS_YES = 2   /**< WBS Enabled */
// } aceBT_hfpWbsConfig_t;

// /**
//  * @brief HFP Call Hold Handling
//  */
// typedef enum {
//     ACEBT_HFP_CHLD_TYPE_RELEASEHELD = 0, /**< Release held call */
//     ACEBT_HFP_CHLD_TYPE_RELEASEACTIVE_ACCEPTHELD =
//         1, /**< Release active call and accept Hheld call */
//     ACEBT_HFP_CHLD_TYPE_HOLDACTIVE_ACCEPTHELD =
//         2, /**< Hold active call and accept held call */
//     ACEBT_HFP_CHLD_TYPE_ADDHELDTOCONF = 3 /**< Add Held call to conference */
// } aceBT_hfpChldType_t;

// /**
//  * @brief HFP Indicators
//  */
// typedef enum {
//     ACEBT_HFP_IND_ENHANCED_DRIVER_SAFETY =
//         1, /**< Enhanced driver safety indicator */
//     ACEBT_HFP_IND_BATTERY_LEVEL_STATUS =
//         2 /**< Battery level status indicator */
// } aceBT_hfpIndType_t;

// /**
//  * @brief HFP Indicator State
//  */
// typedef enum {
//     ACEBT_HFP_IND_DISABLED = 0, /**< Indicator disabled */
//     ACEBT_HFP_IND_ENABLED = 1,  /**< Indicator enabled */
// } aceBT_hfpIndStatus_t;

// /**
//  * @brief Bluetooth HID Protocol Type
//  */
// typedef enum {
//     ACEBT_HID_HOST_PROTOCOL_REPORT = 0,        /**< Report protocol */
//     ACEBT_HID_HOST_PROTOCOL_BOOT = 1,          /**< Boot protocol */
//     ACEBT_HID_HOST_PROTOCOL_UNSUPPORTED = 0XFF /**< Unsupported protocol */
// } aceBT_hidHostProtocolMode_t;

// /**
//  * @brief Bluetooth HID Report Type
//  */
// typedef enum {
//     ACEBT_HID_HOST_REPORT_INPUT = 1,  /**< Input report */
//     ACEBT_HID_HOST_REPORT_OUTPUT = 2, /**< Output report */
//     ACEBT_HID_HOST_REPORT_FEATURE = 3 /**< Feature report */
// } aceBT_hidHostReportType_t;

// /** @} */

// /** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_DEFINES_H
