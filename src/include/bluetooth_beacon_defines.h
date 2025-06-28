/*
 * Copyright 2018-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * AMAZON PROPRIETARY/CONFIDENTIAL
 *
 * You may not use this file except in compliance with the terms and
 * conditions set forth in the accompanying LICENSE.TXT file.
 *
 * THESE MATERIALS ARE PROVIDED ON AN "AS IS" BASIS. AMAZON SPECIFICALLY
 * DISCLAIMS, WITH RESPECT TO THESE MATERIALS, ALL WARRANTIES, EXPRESS,
 * IMPLIED, OR STATUTORY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
 */
/**
 * @file bluetooth_beacon_defines.h
 *
 * @brief ACE Bluetooth Beacon Manager header defining all constants and data
 * types
 */

#ifndef BT_BEACON_DEFINES_H
#define BT_BEACON_DEFINES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <ace/bluetooth_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BLE_DS_BEACON BLE Beacon manager Data structures
 * @{
 * @ingroup ACE_BLE_DS
 */

/**
 * @name Advertising Capability Bitmasks
 * @brief Local device advertising capability bitmasks
 * @{
 */
/** bitmask representing support for multiple advertisements */
#define ACEBT_BEACON_MGR_MULTI_ADV_SUPPORTED 0x0001
/** bitmask representing support for extended advertisements */
#define ACEBT_BEACON_MGR_EXTENDED_ADV_SUPPORTED 0x0002
/** bitmask representing support for periodic advertisements */
#define ACEBT_BEACON_MGR_PERIODIC_ADV_SUPPORTED 0x0004
/** @} */

/**
 * @brief Max size of beacon data supported by interfaces.\n
 * Note that underlying BT controller may not support.\n
 * Actual supported length can be retrieved by
 * aceBT_getMaximumAdvertisingDataLength() API
 */
#define ACEBT_BEACON_MAX_DATA_LEN 250

/**
 * @name BT Addres Types
 * @brief Adress Types used in aceBT_BeaconAdvSettings
 * @{
 */
#define ACEBT_ADDRESS_TYPE_PUBLIC 0
#define ACEBT_ADDRESS_TYPE_RANDOM 1
#define ACEBT_ADDRESS_TYPE_STATIC_RANDOM 2
#define ACEBT_ADDRESS_TYPE_RESOLVABLE 3
/** @} */

/**
 * @name Advertise Data Max Length
 * @brief Maximum length of advertise data as defined in BT spec 5.0, vol 6,
 * part B sec. 2.3.1
 * @{
 */
#define ACE_BT_MAX_ADV_LEN 31
/** @} */

/**
 * @brief Beacon advertising states. Represents current state of an
 * advertisement
 * @see beacon_advChangeCallback
 */
typedef enum {
    /** Advertisement has been queued successfully */
    ACEBT_BEACON_ADV_QUEUED = 0,
    /** Advertisement started/re started successfully */
    ACEBT_BEACON_ADV_STARTED = 1,
    /** Advertisement was started earlier but paused now due to an event that
     * needs higher priority and cannot be handled by scheduling priorities in
     * the controller.
     */
    ACEBT_BEACON_ADV_PAUSED = 2,
    /** Advertisement has been permanently stopped */
    ACEBT_BEACON_ADV_STOPPED = 3
} aceBT_beaconAdvState_t;

/** @brief Beacon ADV/Scan priority */
typedef enum {
    /** Represents a critical priority beacon operation */
    ACEBT_BEACON_PRIORITY_CRITICAL = 0,
    /** Represents a high priority beacon operation */
    ACEBT_BEACON_PRIORITY_HIGH = 1,
    /** Represents a normal priority beacon operation */
    ACEBT_BEACON_PRIORITY_NORMAL = 2,
    /** Represents a low priority beacon operation */
    ACEBT_BEACON_PRIORITY_LOW = 3
} aceBT_beaconPriority_t;

/** @brief Beacon types */
typedef enum {
    /** Advertisement data is composed by client App.
     * Beacon manager will send the same data as passthrough to underlying
     * Bluetooth stack
     */
    ACEBT_BEACON_TYPE_PASSTHROUGH = 0,
    /** Advertisement data is composed by beacon manager as amazon specific
     * manufacturer specific data format.
     * Application can pass application specific payload to be part of
     * manufacturer specific data
     */
    ACEBT_BEACON_TYPE_AMZN_MANF_DATA = 1,
    /** Advertisement data is composed by beacon manager according to Apple's
     * IBeacon advertising data format.
     * Application can pass application specific payload to be part of IBeacon
     * data format
     * For IBeacon Application specific data format consists of major number
     * and minor number.
     */
    ACEBT_BEACON_TYPE_IBEACON = 2,
    /** Advertisement data is composed by beacon manager as amazon specific
     * 16 bit UUID  data format.
     * Application can pass application specific payload to be part of
     * service data for 16 bit UUID
     */
    ACEBT_BEACON_TYPE_AMZN_16BIT_UUID = 3,
} aceBT_beaconType_t;

/** @brief Beacon operation transmit power level settings */
typedef enum {
    /** Perform Beacon operation in high transmit power */
    ACEBT_BEACON_MODE_HIGH_TX_POWER = 0,
    /** Perform Beacon operation in low transmit power */
    ACEBT_BEACON_MODE_LOW_TX_POWER = 1,
    /** Perform Beacon operation in ultra low transmit power */
    ACEBT_BEACON_MODE_ULTRA_LOW_TX_POWER = 2,
} aceBT_beaconPowerMode_t;

/** @brief Beacon broadcast modes. These modes dictates broadcast/scan
 * intervals. */
typedef enum {
    /** Perform beacon broadcast in ultra low latency, high power mode */
    ACEBT_BEACON_MODE_ULTRA_LOW_LATENCY = 0,
    /** Perform beacon broadcast in low latency, high power mode */
    ACEBT_BEACON_MODE_LOW_LATENCY = 1,
    /** Perform beacon broadcast in balanced power mode */
    ACEBT_BEACON_MODE_BALANCED = 2,
    /** Perform beacon broadcast in low power mode */
    ACEBT_BEACON_MODE_LOW_POWER = 3,
} aceBT_beaconAdvMode_t;

/** @brief Beacon physical transport types */
typedef enum {
    /* when transmitting on the LE Coded PHY. */
    PHY_OPTION_NO_PREFERRED = 0x0000,
    /** Bluetooth LE 1M PHY. Used to refer to LE 1M Physical Channel
     * for advertising, scanning or connection.*/
    ACEBT_BEACON_PHY_IM = 0x0001,
    /** Bluetooth LE 2M PHY. Used to refer to LE 2M Physical Channel for
     * advertising, scanning or connection.*/
    PHY_LE_2M = 0x00002,
    /** Bluetooth LE Coded PHY (S2 preferred). Used to refer to LE Coded
     Physical Channel for advertising, scanning or connection **/
    PHY_LE_CODED_S2 = 0x0004,
    /** Bluetooth LE Coded PHY (S8 preferred). Used to refer to LE Coded
     * Physical Channel for advertising, scanning or connection */
    PHY_LE_CODED_S8 = 0x0008,
} aceBT_beaconPhyTypes_t;

/**
 * @brief Type of remote/local device address used during advertisement,
 * scanning and connection
 */
typedef enum {
    /** Address type is Public */
    ACEBT_BEACON_ADDR_TYPE_PUBLIC = 0,
    /** Address type is random but statically generated */
    ACEBT_BEACON_ADDR_TYPE_RANDOM_STATIC = 1,
    /** Address type is private */
    ACEBT_BEACON_MODE_TYPE_RANODM_PRIVATE = 2,
} aceBT_beaconAddrType_t;

/** @brief Beacon scan states */
typedef enum {
    /** Scan request has failed due to some reason */
    ACEBT_BEACON_SCAN_FAILED = 0,
    /** Scan has been queued successfully */
    ACEBT_BEACON_SCAN_QUEUED = 1,
    /** Scan started/re-started successfully */
    ACEBT_BEACON_SCAN_STARTED = 2,
    /** Scan was started earlier but paused now due to an event that needs
     * higher priority and cannot be handled by scheduling priorities in the
     * Controller.
     */
    ACEBT_BEACON_SCAN_PAUSED = 3,
    /** Scan has been permanently stopped */
    ACEBT_BEACON_SCAN_STOPPED = 4
} aceBT_beaconScanState_t;

/** @brief Beacon Scan Mode types */
typedef enum {
    /** Perform Bluetooth LE scan in balanced power mode */
    ACEBT_BEACON_SCAN_MODE_BALANCED = 0,
    /** Scan using highest duty cycle possible */
    ACEBT_BEACON_SCAN_MODE_LOW_LATENCY = 1,
    /** Perform Bluetooth LE scan in low power mode. */
    ACEBT_BEACON_SCAN_MODE_LOW_POWER = 2,
    /** A special Bluetooth LE scan mode.
     * Clients using this scan mode will passively listen for other scan
     * results without starting BLE scans themselves */
    ACEBT_BEACON_SCAN_MODE_OPPORTUNISTIC = -1
} aceBT_beaconScanMode_t;

/** @brief Beacon Callback types */
typedef enum {
    /** Trigger callback for every Bluetooth advertisement found that matches
     * the filter criteria */
    ACEBT_BEACON_SCAN_CALLBACK_TYPE_ALL_MATCHES = 1,
    /** Trigger callback only for first advertisement found that matches the
     * filter criteria */
    ACEBT_BEACON_SCAN_CALLBACK_TYPE_FIRST_MATCH = 2,
    /** Trigger callback when advertisements are no longer received from a
     * device that has been previously reported by a first match callback */
    ACEBT_BEACON_SCAN_CALLBACK_TYPE_MATCH_LOST = 4
} aceBT_beaconScanCallbackType_t;

/** @brief Beacon Scan result types */
typedef enum {
    /** Partial scan result which contains device and rssi*/
    ACEBT_BEACON_SCAN_RESULT_TYPE_TRUNCATED = 1,
    /** Full scan result which contains device, rssi, advertising data and scan
     * response */
    ACEBT_BEACON_SCAN_RESULT_TYPE_FULL = 2
} aceBT_beaconScanResultType_t;

/** @brief Beacon Match Number types */
typedef enum {
    /** Match one advertisement per filter */
    ACEBT_BEACON_SCAN_MATCH_ONE = 1,
    /** Match few advertisements per filter. Decided by hardware */
    ACEBT_BEACON_SCAN_MATCH_FEW = 2,
    /** Match as many advertisements as possible per flter. Decided by hardware
     */
    ACEBT_BEACON_SCAN_MATCH_MAX = 3
} aceBT_beaconScanMatchNum_t;

/** @brief Match Mode types */
typedef enum {
    /** In Aggressive mode, hw will determine a match sooner even with feeble
     * signal strength and few number of sightings/match in a duration. */
    ACEBT_BEACON_SCAN_MATCH_MODE_AGGRESIVE = 1,
    /** For sticky mode, higher threshold of signal strength and sightings is
     * required before reporting by hw */
    ACEBT_BEACON_SCAN_MATCH_MODE_STICKY = 2
} aceBT_beaconScanMatchMode_t;

/**
 * @brief Scan Filter types.\n
 * Used to configure filters passed to @ref aceBT_startBeaconScan
 */
typedef enum {
    /** Filter advertisements based on address */
    ACEBT_BEACON_SCAN_FILTER_DEV_ADDR,
    /** Filter advertisements based on service data pattern UNSUPPORTED */
    ACEBT_BEACON_SCAN_FILTER_SERVICE_DATA_PATTERN,
    /** Filter advertisements based on service uuid */
    ACEBT_BEACON_SCAN_FILTER_SERVICE_UUID,
    /** Filter advertisements based on service sol uuid UNSUPPORTED */
    ACEBT_BEACON_SCAN_FILTER_SERVICE_SOL_UUID,
    /** Filter advertisements based on device name */
    ACEBT_BEACON_SCAN_FILTER_DEV_NAME,
    /** Filter advertisements based on manufacturer data */
    ACEBT_BEACON_SCAN_FILTER_MANU_DATA,
    /** Filter advertisements based on service data */
    ACEBT_BEACON_SCAN_FILTER_SERVICE_DATA,
    /** Filter advertisements based on only service data uuid */
    ACEBT_BEACON_SCAN_FILTER_SERVICE_DATA_UUID,
    /** Filter advertisements based on advertisement type */
    ACEBT_BEACON_SCAN_FILTER_ADV_TYPE
} aceBT_beaconScanFilterType_t;

/** @brief Scan filter for manufacturer data type */
typedef struct {
    /** length of the manufacturer type data */
    uint8_t len;
    /** manufacturer data.
     * First 2 bytes are for company ID. Example: Company ID = 0x0171,
     * manufactData[0] = 0x01, manufactData[1] = 0x71. Payload of manufacturer
     * data start at byte 2.
     */
    uint8_t* manufactData;
    /** manufacturer data mask.
     * For any bit in the mask,set it the 1 if it needs to match the one in
     * manufacturerdata, otherwise set it to 0. */
    uint8_t* manufactDataMask;
} __attribute__((packed)) aceBT_ScanFilterManuFactData_t;

/** @brief Scan filter for service UUID type */
typedef struct {
    /** UUID length Will always be 128 bit (16 byte) uuid*/
    uint8_t len;
    /** UUID on which advertisement to be filtered */
    aceBT_uuid_t uuid;
    /** UUID data mask.
     * For any bit in the mask,set it the 1 if it needs to match the one in
     * serviceData, otherwise set it to 0. */
    aceBT_uuid_t uuidMask;
} __attribute__((packed)) aceBT_ScanFilterServiceUUID_t;

/** @brief Scan filter for service data type */
typedef struct {
    /** Service data length */
    uint8_t len;
    /** Service data */
    uint8_t* serviceData;
    /** service data mask.
     * For any bit in the mask,set it the 1 if it needs to match the one in
     * serviceData, otherwise set it to 0. */
    uint8_t* serviceDataMask;
} __attribute__((packed)) aceBT_ScanFilterServiceData_t;

/**
 * @brief Scan filter for service data uuid type.\n
 * This service data uuid MUST be 2 bytes (16 bits)
 */
typedef struct {
    /** Service data length */
    uint8_t len;
    /** Service data */
    aceBT_uuid_t serviceDataUuid;
    /** service data mask.
     * For any bit in the mask,set it the 1 if it needs to match the one in
     * serviceDataUuid, otherwise set it to 0. */
    aceBT_uuid_t serviceDataUuidMask;
} __attribute__((packed)) aceBT_ScanFilterServiceDataUuid_t;

/**
 * @brief Scan Record Fields.\n
 * BT GAP Assigned Numbers
 */
typedef enum {
    /** Flags */
    ACEBT_BEACON_SCAN_DATA_TYPE_FLAGS = 0x01,
    /** Incomplete List of 16-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_16_BIT_PARTIAL = 0x02,
    /** Complete List of 16-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_16_BIT_COMPLETE = 0x03,
    /** Incomplete List of 32-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_32_BIT_PARTIAL = 0x04,
    /** Complete List of 32-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_32_BIT_COMPLETE = 0x05,
    /** Incomplete List of 128-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_128_BIT_PARTIAL = 0x06,
    /** Complete List of 128-bit Service Class UUIDs */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_UUID_128_BIT_COMPLETE = 0x07,
    /** Shortened Local Name */
    ACEBT_BEACON_SCAN_DATA_TYPE_LOCAL_NAME_SHORT = 0x08,
    /** Complete Local Name */
    ACEBT_BEACON_SCAN_DATA_TYPE_LOCAL_NAME_COMPLETE = 0x09,
    /** Tx Power Level */
    ACEBT_BEACON_SCAN_DATA_TYPE_TX_POWER_LEVEL = 0x0A,
    /** Service Data */
    ACEBT_BEACON_SCAN_DATA_TYPE_SERVICE_DATA = 0x16,
    /** PB-ADV */
    ACEBT_BEACON_SCAN_DATA_TYPE_MESH_PB_ADV = 0x29,
    /** Mesh Message */
    ACEBT_BEACON_SCAN_DATA_TYPE_MESH_MSG = 0x2A,
    /** Mesh Beacon */
    ACEBT_BEACON_SCAN_DATA_TYPE_MESH_BEACON = 0x2B,
    /** Manufacturer Specific Data */
    ACEBT_BEACON_SCAN_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA = 0xFF
} aceBT_BeaconScanRecordFields_t;

/** @brief Union of all scan Filter values */
typedef union {
    /** Fill this value when filter type is ACEBT_BEACON_SCAN_FILTER_DEV_ADDR */
    aceBT_bdAddr_t addr;
    /** Fill this value when filter type is ACEBT_BEACON_SCAN_FILTER_DEV_NAME */
    aceBT_bdName_t name;
    /** Fill this value when filter type is ACEBT_BEACON_SCAN_FILTER_MANU_DATA
     */
    aceBT_ScanFilterManuFactData_t manufactData;
    /** Fill this value when filter type is
     * ACEBT_BEACON_SCAN_FILTER_SERVICE_DATA */
    aceBT_ScanFilterServiceData_t serviceData;
    /** Fill this value when filter type is
     * ACEBT_BEACON_SCAN_FILTER_SERVICE_DATA_UUID */
    aceBT_ScanFilterServiceDataUuid_t serviceDataUuid;
    /** Fill this value when filter type is
     * ACEBT_BEACON_SCAN_FILTER_SERVICE_UUID */
    aceBT_ScanFilterServiceUUID_t uuid;
    /** Fill this value when filter type is
     * ACEBT_BEACON_SCAN_FILTER_ADV_TYPE */
    aceBT_BeaconScanRecordFields_t advType;
} __attribute__((packed)) aceBT_beaconScanFilterValue_t;

/**  @brief Extended advertisement specific settings */
typedef struct {
    /** Primary PHY type to be used for extended advertising */
    aceBT_beaconPhyTypes_t primaryPhy;
    /** secondary PHY type to be used for extended advertising */
    aceBT_beaconPhyTypes_t secondaryPhy;
    /** Set to TRUE if advertisement type should be scannable */
    bool scanable;
} __attribute__((packed)) aceBT_beaconExtendedAdvSettings_t;

/**
 * @brief Advertisement type options for @ref aceBT_BeaconAdvSettings
 */
typedef enum {
    /** Connectable undirected advertising */
    ACEBT_BLE_ADV_CONN_IND = 0,
    /** Connectable directed advertising */
    ACEBT_BLE_ADV_CONN_DIR_IND = 1,
    /** Non connectable undirected scannable advertising */
    ACEBT_BLE_ADV_SCAN_IND = 2,
    /** Non connectable undirected advertising */
    ACEBT_BLE_ADV_NONCONN_UNDIR_IND = 3
} aceBT_advType_t;

/** @brief Beacon Advertisement settings */
typedef struct {
    /** @deprecated use instead advType */
    bool connectable;
    /** Set this flag to TRUE if random private address to be used in
     * advertisement */
    bool enablePrivacy;
    /** @deprecated Use timeout_ticks instead. Timeout value in seconds when
     * this advertisement to be stopped */
    uint32_t timeout;
    /** Timeout value in 10 milliseconds steps when this advertisement to be
     * stopped. If timeout and timeout_ticks are both set, timeout_ticks will
     * have precedence.*/
    uint16_t timeout_ticks;
    /** Priority level for this advertisement */
    aceBT_beaconPriority_t priority;
    /** Advertisement mode */
    aceBT_beaconAdvMode_t mode;
    /** @deprecated Preferred interval in multiples of 1.25ms */
    uint32_t preferredInterval;
    /** minimum interval requested for reasonable operation of client feature */
    uint32_t QOSPreference;
    /** Use BLE 5.0 extended advertisements */
    bool useExtended;
    /** Extended advertisement settings. valid only when useExtended field set
     * to TRUE */
    aceBT_beaconExtendedAdvSettings_t extSettings;
    /** Address Type. Valid only when enablePrivacy field set to TRUE */
    uint8_t addrType;
    /** Set property to configure advertisement connectability or
     * directability*/
    aceBT_advType_t advType;
} __attribute__((packed)) aceBT_BeaconAdvSettings;

/** @brief Manufacturer data */
typedef struct {
    uint16_t length;
    uint8_t data[];
} __attribute__((packed)) aceBT_ManuData_t;

/** @brief Service data */
typedef struct {
    uint16_t length; /**< Service data ength */
    uint8_t data[];  /**< Service data */
} __attribute__((packed)) aceBT_ServiceData_t;

/** @brief Service UUID */
typedef struct {
    uint16_t noServiceUUID; /**< Number of service UUIDs in @ref serviceUuid*/
    aceBT_uuid_t serviceUuid[]; /**< service UUID list*/
} __attribute__((packed)) aceBT_ServiceUuid_t;

/** @brief Advertisment Data */
typedef enum {
    ACEBT_BEACON_MANU_DATA = 0,     /**< Manufacturer data */
    ACEBT_BEACON_SRVC_DATA = 1,     /**< Service data */
    ACEBT_BEACON_SRCV_UUID_DATA = 2 /**< Service UUID data */
} aceBT_beaconAdvDataType_t;

/** @brief Advertisment Transmission Power */
typedef enum {
    ACE_BT_ADVERTISE_TX_POWER_DEFAULT =
        0,                                    /**< Default Adv Tx Power.
                                                   Applications that do not care about
                                                   the Tx Power can use this option.
                                                   Will be set to ACE_BT_ADVERTISE_TX_POWER_HIGH */
    ACE_BT_ADVERTISE_TX_POWER_ULTRA_LOW = 1,  /**< Ultra Low Adv Tx Power */
    ACE_BT_ADVERTISE_TX_POWER_LOW = 2,        /**< Low Adv Tx Power */
    ACE_BT_ADVERTISE_TX_POWER_MEDIUM = 3,     /**< Medium Adv Tx Power */
    ACE_BT_ADVERTISE_TX_POWER_HIGH = 4,       /**< High Adv Tx Power */
    ACE_BT_ADVERTISE_TX_POWER_ULTRA_HIGH = 5, /**< Ultra High Adv Tx Power */
} aceBt_advTxPowerType_t;

/**
 * @brief Beacon Type Values.\n
 * Values from Mesh Profile Bluetooth Specification v1.0.1: 3.9 - Mesh Beacons
 */
typedef enum {
    /** Unprovisioned device beacon */
    ACEBT_MESH_BEACON_UNPROV_DEV = 0x00,
    /** Secure Network beacon  */
    ACEBT_MESH_BEACON_SECURE_NTWK = 0x01,
    /** Reserved for future use */
    ACEBT_MESH_BEACON_UNUSED = 0xFF
} aceBT_meshBeaconType_t;

/**
 * @brief Mesh Advertisement Types.\n
 * Values from Bluetooth GAP Specification
 */
typedef enum {
    /** Provisioning Bearer AD Type */
    ACEBT_MESH_PV_ADV_DATA = 0x29,
    /** Mesh Message AD Type */
    ACEBT_MESH_MESSAGE_DATA = 0x2A,
    /** Mesh Beacon AD Type */
    ACEBT_MESH_BEACON_DATA = 0x2B
} aceBT_meshAdvType_t;

/** @brief Advertisment Data */
typedef struct {
    aceBT_beaconAdvDataType_t advDataType; /**< Advertisment data*/
    union {
        aceBT_ServiceUuid_t serviceuuids; /**< Service UUID data */
        aceBT_ManuData_t manuFactureData; /**< Manufacturer data */
        aceBT_ServiceData_t serviceData;  /**< Service data */
    };
} __attribute__((packed)) aceBT_AdvertimentData_t;

/** @brief Beacon Data */
typedef struct {
    /** type of beacon to be advertised */
    aceBT_beaconType_t beaconType;
    /* Length of application payload */
    uint16_t length;
    /* application payload*/
    uint8_t data[ACEBT_BEACON_MAX_DATA_LEN];
    /** Set this flag to TRUE if TX power level to be included in advertisement
     */
    bool includeTxPower;
    /** Set this flag to TRUE if Device Name need to be included in
     * advertisement */
    bool includeDeviceName;
    /** Advertisement Tx Power Value desired */
    aceBt_advTxPowerType_t adv_tx_power_type;
} __attribute__((packed)) aceBT_BeaconData_t;

/** @brief Beacon Data list */
typedef struct {
    /** Max parameter can be retrieved using ACEBT_MAX_BEACON_SUPPORTED */
    uint8_t numberOfBeacons;
    /** Array of aceBT_BeaconData_t structures */
    aceBT_BeaconData_t* beaconArray;
} __attribute__((packed)) aceBT_BeaconPayloadList_t;

/** @brief Scan Settings */
typedef struct {
    /** Priority for this scan*/
    aceBT_beaconPriority_t priority;
    /** If set true, scan results will include only BT 4.2 based
     * advertisements*/
    bool legacyOnly;
    /** Delay of report in milliseconds, set 0 for immediate report */
    uint32_t delay;
    /** Scan mode */
    aceBT_beaconScanMode_t scanMode;
    /** Callback mode */
    aceBT_beaconScanCallbackType_t callbackType;
    /** LE Scan result type */
    aceBT_beaconScanResultType_t resultType;
    /** Match Num */
    aceBT_beaconScanMatchNum_t matchNum;
    /** Match Mode */
    aceBT_beaconScanMatchMode_t matchMode;
    /** when legacyOnly set to FALSE, select the PHY for scan */
    aceBT_beaconPhyTypes_t phyType;
    /** minimum number of interval (1.25 ms) by swhich scan needs to be
     * scheduled for resonable operation */
    uint32_t QOSPreference;
    /** set this to true if client is interested only in device addr,name and
     * RSSI*/
    bool shortResult;
} __attribute__((packed)) aceBT_BeaconScanSettings_t;

/** @brief Scan Filter */
typedef struct {
    /** Scan Filter type */
    aceBT_beaconScanFilterType_t filterType;
    /** Scan filter value, one of the member of aceBT_beaconScanFilterValue_t
     * union */
    aceBT_beaconScanFilterValue_t filterValue;
    /** How many ADV packets needs to match this scan filter before client needs
     * to be called back*/
    uint32_t minimumMatch;
} __attribute__((packed)) aceBT_BeaconScanFilter_t;

/** @brief Scan Filter list */
typedef struct {
    /** Number of filters that needs to me matched */
    uint32_t numberOfFilters;
    /** List of aceBT_BeaconScanFilter_t structures */
    aceBT_BeaconScanFilter_t* filters;
} __attribute__((packed)) aceBT_BeaconScanFilterList_t;

/** @brief Scan Record */
typedef struct {
    /** Device address */
    aceBT_bdAddr_t addr;
    /** Device address type */
    aceBT_beaconAddrType_t addrType;
    /** Scan data  */
    aceBT_BeaconData_t scanRecords;
    /** RSSI of the remote advertisement */
    int rssi;
} __attribute__((packed)) aceBT_BeaconScanRecord_t;

/** @brief Manufacturer Data */
typedef struct {
    /** Manufacturer Data*/
    uint8_t data[ACE_BT_MAX_ADV_LEN];
    /** Manufacturer Data Size*/
    uint8_t data_len;
} aceBt_beaconManufacturerData_t;

/**  @brief Scan Result List */
typedef struct {
    /** Number of scan result */
    uint8_t numRecords;
    /* List of individual scan results */
    aceBT_BeaconScanRecord_t* scanRecords;
} __attribute__((packed)) aceBT_scanResultList_t;

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BEACON_DEFINES_H
