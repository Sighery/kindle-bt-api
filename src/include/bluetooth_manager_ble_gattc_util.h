#ifndef BT_MGR_BLE_GATTC_UTIL_H
#define BT_MGR_BLE_GATTC_UTIL_H

#include "bluetooth_defines.h"
#include "bluetooth_ble_defines.h"

// #define PRINT_ERROR_ON_FAIL(ret_val, msg)  \
//     {                                      \
//         if (!(ret_val)) {                  \
//             BT_LOGE("%s: " msg, __func__); \
//         }                                  \
//     }

void aceBT_init_gattc(void);

// void aceBT_deinit_gattc(void);

// void aceBtUtils_gattcSvcListLock(void);

// void aceBtUtils_gattcSvcListUnlock(void);

// void gatt_utilc_add_service(int conn_id, aceBT_bleGattsService_t* gattService,
//                             int no_svc);

// void gatt_utilc_remove_service_by_conn_id(int conn_id,
//                                           aceBT_bleGattsService_t** gattService,
//                                           int* no_svc);

// bool gatt_utilc_update_prepare_write(int conn_id, bool prep_write);

// bool gatt_utilc_is_prep_write(int conn_id);

// bool gatt_utilc_is_gatt_busy(int conn_id);

// bool gatt_utilc_set_gatt_busy(int conn_id, bool busy_state);

// uint32_t aceBtUtils_gattcUuidToSessionHandle(aceBT_uuid_t uuid);

// aceBT_bleGattCharacteristicsValue_t* aceBtUtils_gattcGetCharsByHandleByRef(
//     int conn_id, int handle);

// aceBT_bleGattDescriptor_t* aceBtUtils_gattcGetDescByHandle(int conn_id,
//                                                            int handle);

// void aceBtUtils_gattcRemoveGattServer(uint16_t conn_id);

// BTUuid_t aceBtUtils_gattcUuidToBtUuid(uint32_t gattc_uuid);

// void aceBtUtils_gattcUuidToAceBtUuid(uint32_t gattc_uuid, aceBT_uuid_t* uuid);

// bool aceBtUtils_gattcCpyCharsVal(aceBT_bleGattCharacteristicsValue_t* src,
//                                  aceBT_bleGattCharacteristicsValue_t* dst);
// void aceBtUtils_gattcFreeCharsVal(aceBT_bleGattCharacteristicsValue_t* val);
// bool aceBtUtils_gattcCpyDescVal(aceBT_bleGattDescriptor_t* src,
//                                 aceBT_bleGattDescriptor_t* dst);
// void aceBtUtils_gattcFreeDescVal(aceBT_bleGattDescriptor_t* desc);

// #ifdef ACEBT_POLICY_MANAGER
// bool gatt_utilc_update_gatt_db_properties(int old_conn_id, int conn_id);
// #endif

#endif  // BT_MGR_BLE_GATTC_UTIL_H
