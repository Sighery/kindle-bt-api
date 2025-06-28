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
 * @file bluetooth_beacon_clients.h
 *
 * @brief ACE Bluetooth Beacon Manager header defining identifiers for known
 * Beacon clients
 */

#ifndef BT_BEACON_CLIENTS_H
#define BT_BEACON_CLIENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <ace/bluetooth_defines.h>

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

/**
 * @name Beacon Client Types
 * @brief List of all internal Beacon clients
 * @{
 */
#define ACE_BEACON_CLIENT_TYPE_FFS 0x01        /**< Client Type FFS */
#define ACE_BEACON_CLIENT_TYPE_MONEYPENNY 0x02 /**< Client Type MoneyPenny*/
#define ACE_BEACON_CLIENT_TYPE_UNKNOWN 0xFF    /**< Client Type Unknown */
/** @} */

/**
 * @cond DEPRECATED
 * @deprecated Please use renamed client type instead.
 * @{
 */
#define BEACON_CLIENT_TYPE_FFS \
    ACE_BEACON_CLIENT_TYPE_FFS /**< Client Type FFS */
#define BEACON_CLIENT_TYPE_MONEYPENNY \
    ACE_BEACON_CLIENT_TYPE_MONEYPENNY /**< Client Type MoneyPenny*/
#define BEACON_CLIENT_TYPE_UNKNOWN \
    ACE_BEACON_CLIENT_TYPE_UNKNOWN /**< Client Type Unknown */
/**
 * @}
 * @endcond
 */ // cond DEPRECATED

/**
 * @brief Beacon client Identifier
 */
typedef uint16_t aceBT_BeaconClientId;

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BT_BEACON_CLIENTS_H
