#include <stdio.h>
#include <inttypes.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>

#include "ace/ace_status.h"
#include "ace/bluetooth_session_api.h"
#include "ace/bluetooth_api.h"
#include "ace/bluetooth_common_api.h"
#include "ace/bluetooth_ble_api.h"
#include "ace/bluetooth_ble_gatt_client_api.h"
#include "ace/bluetooth_beacon_api.h"
#include "ace/osal_alloc.h"
// #include "bluetooth_manager_ble_gattc_util.h"

#include "core/defines.h"


#define ADDR_WITH_COLON_LEN 17
#define ADDR_WITHOUT_COLON_LEN 12
#define PRINT_UUID_STR_LEN 49


aceBT_sessionHandle bt_session = NULL;
aceBT_bleConnHandle ble_conn_handle = NULL;
bool registered_ble = false;
bool registered_beacon_client = false;
// Actually never used since the callback never gets called
bool registered_gatt_client = false;
bool pico_led_status = false;

uint32_t gNo_svc;
aceBT_bleGattsService_t* pGgatt_service = NULL;

/** Callback for PIN request */
void aceBt_pinRequestCallback(
    aceBT_bdAddr_t* p_remote_addr,
    aceBT_bdName_t* p_remote_name, int cod,
    bool isMin16DigitPin
) {
    printf("aceBt_pinRequestCallback");
    // UNUSED(p_remote_name);
    // UNUSED(cod);

    // // Set new bdaddr
    // pair_cb.incoming_bdaddr = (aceBT_bdAddr_t*)aceAlloc_alloc(
    //     ACE_MODULE_BT, ACE_ALLOC_BUFFER_GENERIC, sizeof(aceBT_bdAddr_t));
    // if (pair_cb.incoming_bdaddr != NULL) {
    //     memcpy(pair_cb.incoming_bdaddr, p_remote_addr, sizeof(aceBT_bdAddr_t));
    // }

    // // Set incoming pin request flag
    // pair_cb.is_inc_pin_req_len = (isMin16DigitPin) ? 16 : 4;
    // CLI_LOG("CLI callback : bond_pin_request_callback() required length: %d",
    //         pair_cb.is_inc_pin_req_len);
}

/** Callback for SSP (Secure Simple pairing) request */
void aceBt_sspRequestCallback(
    aceBT_bdAddr_t* p_remote_addr,
    aceBT_bdName_t* p_remote_name, int cod,
    aceBT_sspVariant_t ssp_type,
    uint32_t pass_key
) {
    printf("aceBt_sspRequestCallback");
    // UNUSED(p_remote_name);
    // UNUSED(cod);

    // // For incoming SSP request, cache all the values and wait for user input to
    // // accept/reject connection
    // aceBtCli_utilsClearPairCb();

    // pair_cb.incoming_bdaddr = (aceBT_bdAddr_t*)aceAlloc_alloc(
    //     ACE_MODULE_BT, ACE_ALLOC_BUFFER_GENERIC, sizeof(aceBT_bdAddr_t));
    // if (pair_cb.incoming_bdaddr != NULL) {
    //     memcpy(pair_cb.incoming_bdaddr, p_remote_addr, sizeof(aceBT_bdAddr_t));
    // }

    // pair_cb.incoming_pass_key = pass_key;
    // pair_cb.incoming_ssp_variant = ssp_type;
    // CLI_LOG("CLI callback : bond_ssp_request_callback() passkey: %d",
    //         (int)pair_cb.incoming_pass_key);
}

void aceBt_adapterStateCallback(aceBT_state_t state) {
    if (state == ACEBT_STATE_ENABLED) {
        printf("CLI callback : cli_adapter_state_callback() state: STATE_ENABLED");
    } else if (state == ACEBT_STATE_DISABLED) {
        printf(
            "CLI callback : cli_adapter_state_callback() state: "
            "STATE_DISABLED"
        );
    }
}

void utilsConvertBdAddrToStr(aceBT_bdAddr_t* paddr, char* outStr) {
    sprintf(outStr, "%02X:%02X:%02X:%02X:%02X:%02X", paddr->address[0],
            paddr->address[1], paddr->address[2], paddr->address[3],
            paddr->address[4], paddr->address[5]);
}

static void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

uint8_t utilsConvertCharToHex(char input) {
    if (input >= '0' && input <= '9') {
        return (uint8_t)(input - '0');
    } else if (input >= 'A' && input <= 'F') {
        return (uint8_t)(input - 'A') + 10;
    } else if (input >= 'a' && input <= 'f') {
        return (uint8_t)(input - 'a') + 10;
    } else {
        return 0;
    }
}

uint16_t utilsConvertHexStrToByteArray(char* input, uint8_t* output) {
    uint8_t length = 0;
    char* hex_string = input;
    uint8_t hex_length = strlen(input);

    for (int i = 0; i < hex_length; i += 2) {
        uint8_t value = utilsConvertCharToHex(hex_string[i]) << 4;

        if (i + 1 < hex_length) {
            value |= utilsConvertCharToHex(hex_string[i + 1]);
        }
        output[length] = value;
        length++;
    }

    return length;
}

ace_status_t utilsConvertStrToBdAddr(char* str, aceBT_bdAddr_t* pAddr) {
    if (str == NULL || pAddr == NULL) {
        return ACE_STATUS_BAD_PARAM;
    }

    int length = strlen(str);
    if (length != ADDR_WITH_COLON_LEN && length != ADDR_WITHOUT_COLON_LEN) {
        printf("Invalid string format. Must be xx:xx:xx:xx:xx:xx or xxxxxxxxxxxx\n");
        return ACE_STATUS_BAD_PARAM;
    }
    // Check if string is in : format
    if (length == ADDR_WITH_COLON_LEN && str[2] == ':' && str[5] == ':' &&
        str[8] == ':' && str[11] == ':' && str[14] == ':') {
        remove_all_chars(str, ':');
    }

    if (strlen(str) != ADDR_WITHOUT_COLON_LEN) {
        printf("Invalid string format. Must be xx:xx:xx:xx:xx:xx or xxxxxxxxxxxx\n");
        return ACE_STATUS_BAD_PARAM;
    }

    for (int i = 0; i < ADDR_WITHOUT_COLON_LEN; i++) {
        if (!((str[i] >= '0' && str[i] <= '9') ||
              (str[i] >= 'A' && str[i] <= 'F') ||
              (str[i] >= 'a' && str[i] <= 'f'))) {
            printf("Contains non-hex character at index %d\n", i);
            return ACE_STATUS_BAD_PARAM;
        }
    }

    printf("str: %s\n", str);

    length = utilsConvertHexStrToByteArray(str, pAddr->address);
    if (length != ACEBT_MAC_ADDR_LEN) {
        return ACE_STATUS_BAD_PARAM;
    }

    return ACE_STATUS_OK;
}

void utilsPrintUuid(char* uuid_str, aceBT_uuid_t* uuid, int max) {
    snprintf(
        uuid_str, max,
        "%02x %02x %02x %02x %02x %02x %02x %02x %02x"
        " %02x %02x %02x %02x %02x %02x %02x",
        uuid->uu[0], uuid->uu[1], uuid->uu[2], uuid->uu[3], uuid->uu[4],
        uuid->uu[5], uuid->uu[6], uuid->uu[7], uuid->uu[8], uuid->uu[9],
        uuid->uu[10], uuid->uu[11], uuid->uu[12], uuid->uu[13],
        uuid->uu[14], uuid->uu[15]
    );
}

void aceBt_utilsDumpServer(aceBT_bleGattsService_t* server) {
    if (!server)
        return;

    struct list_head* svc_list;
    struct list_head* char_list;
    int inc_svc_count = 0;
    char buff[PRINT_UUID_STR_LEN];
    memset(buff, 0, sizeof(char) * PRINT_UUID_STR_LEN);
    utilsPrintUuid(buff, &server->uuid, PRINT_UUID_STR_LEN);
    printf("Service 0 uuid %s serviceType %d\n", buff, server->serviceType);

    struct aceBT_gattIncSvcRec_t* svc_rec;
    STAILQ_FOREACH(svc_rec, &server->incSvcList, link) {
        memset(buff, 0, sizeof(char) * PRINT_UUID_STR_LEN);
        utilsPrintUuid(buff, &svc_rec->value.uuid, PRINT_UUID_STR_LEN);
        printf(
            "Included Services %d service Type %d uuid %s\n",
            inc_svc_count++, svc_rec->value.serviceType, buff
        );
    }
    uint8_t char_count = 0;
    struct aceBT_gattCharRec_t* char_rec = NULL;
    STAILQ_FOREACH(char_rec, &server->charsList, link) {
        memset(buff, 0, sizeof(char) * PRINT_UUID_STR_LEN);
        utilsPrintUuid(buff, &char_rec->value.gattRecord.uuid, PRINT_UUID_STR_LEN);
        if (char_rec->value.gattDescriptor.is_notify && char_rec->value.gattDescriptor.is_set
        ) {
            printf(
                "\tGatt Characteristics with Notifications %d uuid %s\n",
                char_count++, buff
            );
        } else {
            printf("\tGatt Characteristics %d uuid %s\n", char_count++, buff);
        }

        if (char_rec->value.gattDescriptor.is_set) {
            utilsPrintUuid(
                buff, &char_rec->value.gattDescriptor.gattRecord.uuid,
                PRINT_UUID_STR_LEN
            );
            printf("\t\tDescriptor UUID %s\n", buff);

        } else if (char_rec->value.multiDescCount) {
            uint8_t desc_num = 1;
            struct aceBT_gattDescRec_t* desc_rec = NULL;
            /* Traverse descriptor linked list */
            STAILQ_FOREACH(desc_rec, &char_rec->value.descList, link) {
                utilsPrintUuid(
                    buff, &desc_rec->value.gattRecord.uuid,
                    PRINT_UUID_STR_LEN
                );
                printf("\t\tDescriptor %d UUID %s\n", desc_num++, buff);
            }
        }
    }
}

struct aceBT_gattCharRec_t* utilsFindCharRec(
    aceBT_uuid_t uuid, uint8_t uuid_len
) {
    struct aceBT_gattCharRec_t* char_rec = NULL;

    if (!pGgatt_service) {
        printf("GATT DB has not been populated yet!\n");
        return (NULL);
    }

    // Iterate through all services
    for (uint32_t i = 0; i < gNo_svc; i++) {
        aceBT_bleGattsService_t* services = &pGgatt_service[i];

        // Iterate through all characteristics and look for char uuid
        STAILQ_FOREACH(char_rec, &services->charsList, link) {
            // If char uuid matches, read characteristic
            if (!memcmp(char_rec->value.gattRecord.uuid.uu, &uuid.uu, uuid_len)) {
                return (char_rec);
            }
        }
    }
    printf("GATT Characteristic UUID could not be found!\n");
    return (NULL);
}

/** Remote device bond state callback */
void aceBt_bondStateCallback(
    ace_status_t status, aceBT_bdAddr_t* p_remote_addr, aceBT_bondState_t state
) {
    char addr[ACEBT_MAC_ADDR_STR_LEN];
    memset(addr, 0, ACEBT_MAC_ADDR_STR_LEN);
    utilsConvertBdAddrToStr(p_remote_addr, addr);
    switch (state) {
        case ACEBT_BOND_STATE_NONE:
            printf(
                "CLI callback : bond state changed() status: %d addr: %s "
                "state: NONE",
                status, addr
            );
            break;
        case ACEBT_BOND_STATE_BONDING:
            printf(
                "CLI callback : bond state changed() status: %d addr: %s "
                "state: BONDING",
                status, addr
            );
            break;
        case ACEBT_BOND_STATE_BONDED:
            printf(
                "CLI callback : bond state changed() status: %d addr: %s "
                "state: BONDED",
                status, addr
            );
            break;
        default:
            printf(
                "CLI callback : bond state changed() status: %d addr: %s "
                "state: UNKNOWN(%d)",
                status, addr, state
            );
            break;
    }
}

void aceBt_bleRegCallback(ace_status_t status) {
    printf("CLI callback : aceBt_bleRegCallback() status: %d\n", status);
    registered_ble = true;

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.ble_registered, true);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_REG_CB);
}

void aceBt_bleConnStateChangedCallback(
    aceBT_bleConnState_t state,
    aceBT_gattStatus_t status,
    const aceBT_bleConnHandle connHandle,
    aceBT_bdAddr_t* p_addr
) {
    printf("CLI callback : aceBt_bleConnStateChangedCallback()\n");
    printf(
        "state %d status %d connHandle %p addr %02x\n",
        state, status, connHandle, p_addr->address[5]
    );

    // if (status == ACEBT_GATT_STATUS_SUCCESS) {
    //     if (state == ACEBT_BLE_STATE_CONNECTED) {
    //         CLI_SET_CB_VAR(callback_vars.gattc_connected, true);
    //     } else if (state == ACEBT_BLE_STATE_DISCONNECTED) {
    //         CLI_SET_CB_VAR(callback_vars.gattc_disconnected, true);
    //     }
    // }

    ble_conn_handle = connHandle;
    // connection_handle = connHandle;
    // bool is_post_sem = false;
    // aceMutex_acquire(&cli_cb_mutex);
    // if (callback_vars.expected_state == state) {
    //     // if address matches for connected state, or connection handle
    //     // matches for disconnected state, post the semaphore
    //     if ((state == ACEBT_BLE_STATE_CONNECTED &&
    //          memcmp(&callback_vars.expected_conn_bda, p_addr,
    //                 sizeof(aceBT_bdAddr_t)) == 0) ||
    //         (state == ACEBT_BLE_STATE_DISCONNECTED &&
    //          callback_vars.expected_disconn_handle == connHandle)) {
    //         is_post_sem = true;
    //     }
    // }
    // aceMutex_release(&cli_cb_mutex);

    // if (is_post_sem) {
    //     CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_CONN_STATE_CB);
    // }
}

// Never actually called/used
void aceBt_bleGattcRegCallback(ace_status_t status) {
    printf("aceBt_bleGattcRegCallback\n");
    printf("state %d\n", status);
    registered_gatt_client = true;
}

void aceBt_bleMtuUpdatedCallback(
    ace_status_t status, aceBT_bleConnHandle connHandle, int mtu
) {
    printf(
        "CLI callback : aceBt_bleMtuUpdatedCallback() status: %d\n",
        status
    );
    printf("mtu %d, connHandle %p\n", mtu, connHandle);
}

void aceBt_aclStateChangedCallback(
    ace_status_t status, aceBT_connState_t state, const aceBT_bdAddr_t* p_remote_addr, aceBt_aclData_t data
) {
    char addr[ACEBT_MAC_ADDR_STR_LEN] = {0};
    utilsConvertBdAddrToStr((aceBT_bdAddr_t*)p_remote_addr, addr);
    printf(
        "%s() status:%d addr:%s state:%d, transport:%d, reason:%d\n",
        __func__, status, addr, state, data.transport, data.reason
    );
}

static aceBT_callbacks_t bt_callbacks = {
    .size = sizeof(aceBT_callbacks_t),
    .common_cbs = {
        .size = sizeof(aceBT_commonCallbacks_t),
        .adapter_state_cb = aceBt_adapterStateCallback,
        .bond_state_cb = aceBt_bondStateCallback,
        .acl_state_changed_cb = aceBt_aclStateChangedCallback,
    },
    // .classic_cbs = {
    //     .size = sizeof(aceBT_classicCallbacks_t),
    //     .adapter_discovery_state_cb = aceBtCli_discoveryStateCallback,
    //     .conn_state_cb = aceBtCli_connStateCallback,
    //     .device_discovered_cb = aceBtCli_deviceDiscoveredCallback,
    //     .profile_state_cb = aceBtCli_profileStateCallback,
    //     .audio_state_cb = aceBtCli_audioStateCallback,
    //     .acl_priority_cb = aceBtCli_aclPriorityCallback
    // }
};

static aceBT_securityCallbacks_t bt_security_callbacks = {
    .size = sizeof(aceBT_securityCallbacks_t),
    .pin_req_cb = aceBt_pinRequestCallback,
    .ssp_req_cb = aceBt_sspRequestCallback,
};
static aceBT_bleCallbacks_t ble_callbacks = {
    .size = sizeof(aceBT_bleCallbacks_t),
    .common_cbs = {
        .size = sizeof(aceBT_commonCallbacks_t),
        .adapter_state_cb = aceBt_adapterStateCallback,
        .bond_state_cb = aceBt_bondStateCallback,
    },
    .ble_registered_cb = aceBt_bleRegCallback,
    .connection_state_change_cb = aceBt_bleConnStateChangedCallback,
    .on_ble_mtu_updated_cb = aceBt_bleMtuUpdatedCallback,
};

void aceBt_bleGattcServiceDiscoveredCallback(
    aceBT_bleConnHandle connHandle, ace_status_t status
) {
    printf(
        "CLI callback : aceBt_bleGattcServiceDiscoveredCallback() status: %d\n",
        status
    );
    printf("conndle %p\n", connHandle);

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_svc_discovered, true);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_SVC_DISC_CB);
}

void aceBt_bleGattcReadCharsCallback(
    aceBT_bleConnHandle connHandle,
    aceBT_bleGattCharacteristicsValue_t charsValue,
    ace_status_t status
) {
    printf(
        "CLI callback : aceBtCli_bleGattcReadCharsCallback() status: %d\n",
        status
    );
    printf("aceBtCli_bleGattcReadCharsCallback - connHandle %p\n", connHandle);

    char buff[256];
    utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    printf("aceBtCli_bleGattcReadCharsCallback - Characteristic UUID:: %s\n", buff);

    printf("aceBtCli_bleGattcReadCharsCallback - Characteristic value: ");
    for (int idx = 0; idx < charsValue.blobValue.size; idx++)
        printf("%02x", charsValue.blobValue.data[idx]);
    printf("\n");

    // Remove this for final release. This is just for my Pico LED usecase
    printf("aceBtCli_bleGattcReadCharsCallback - Test char casting: %s\n", (char*) charsValue.blobValue.data);

    uint8_t pico_char_uuid[] = {
        0xff, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    // char pico_char_uuid_str[] = "ff 12 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    // utilsConvertHexStrToByteArray(pico_char_uuid_str, pico_char_uuid);
    printf("Compare %d\n", memcmp(&charsValue.gattRecord.uuid.uu, pico_char_uuid, sizeof(&charsValue.gattRecord.uuid.uu)));
    if (!memcmp(&charsValue.gattRecord.uuid.uu, pico_char_uuid, sizeof(&charsValue.gattRecord.uuid.uu))) {
        // Otherwise I get extra values from previous run? Like ONF
        char ledstatus[512] = {0};
        memcpy(ledstatus, charsValue.blobValue.data, charsValue.blobValue.size);
        printf("aceBtCli_bleGattcReadCharsCallback - Characteristic value str: %s\n", ledstatus);

        if (!strcmp(ledstatus, "ON")) {
            printf("aceBtCli_bleGattcReadCharsCallback - Pico LED is on!\n");
            pico_led_status = true;
        } else if (!strcmp(ledstatus, "OFF")) {
            printf("aceBtCli_bleGattcReadCharsCallback - Pico LED is off!\n");
            pico_led_status = false;
        } else {
            printf("Not matched any of the two LED statuses?");
        }
        // char* ledstatus_str;
        // for (int idx = 0; idx < charsValue.blobValue.size; idx++) {
        //     // sprintf(ledstatus_str + strlen(ledstatus_str), "%02x", charsValue.blobValue.data[idx]);
        // }
        // printf("Final buffer value is: %s\n", ledstatus_str);
    }

    // memset(charsValue.blobValue.data, 0, strlen(ledstatus));

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_read_chars, true);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_READ_CHAR_CB);
}

void aceBt_bleGattcWriteCharsCallback(
    aceBT_bleConnHandle connHandle,
    aceBT_bleGattCharacteristicsValue_t gattCharacteristics,
    ace_status_t status
) {
    printf("CLI callback : aceBt_bleGattcWriteCharsCallback()\n");
    printf("connHandle %p gatt format %u\n", connHandle,
            gattCharacteristics.format);

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_write_chars, true);
    // } else {
    //     CLI_LOG("error status %d\n", status);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_WRITE_CHAR_CB);
}

void aceBt_bleGattcNotifyCharsCallback(
    aceBT_bleConnHandle connHandle,
    aceBT_bleGattCharacteristicsValue_t charsValue
) {
    printf("CLI callback : %s()\n", __func__);
    printf("connHandle %p\n", connHandle);
    char buff[256];
    utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    printf("UUID:: %s\n", buff);
    for (int idx = 0; idx < charsValue.blobValue.size; idx++)
        printf("%x", charsValue.blobValue.data[idx]);
    printf("\n");
}

void aceBt_bleGattcWriteDescCallback(
    aceBT_bleConnHandle connHandle,
    aceBT_bleGattCharacteristicsValue_t gattCharacteristics,
    ace_status_t status
) {
    printf("CLI callback : aceBt_bleGattcWriteDescCallback()\n");
    printf("connHandle %p status %d\n", connHandle, status);

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_write_desc, true);
    // } else {
    //     CLI_LOG("error status %d\n", status);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_WRITE_DESC_CB);
}

void aceBt_bleGattcReadDescCallback(
    aceBT_bleConnHandle connHandle,
    aceBT_bleGattCharacteristicsValue_t charsValue, ace_status_t status
) {
    printf(
        "CLI callback : aceBt_bleGattcReadDescCallback() status: %d, "
        "connHandle: %p\n",
        status, connHandle
    );

    char buff[256];
    utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    printf("Char UUID:: %s\n", buff);

    /* Make sure data is not null before printing */
    if (charsValue.blobValue.data != NULL) {
        if (sprintf(buff, "%x", *(uint8_t*)charsValue.blobValue.data) > 0) {
            printf("Descriptor Value is: %s\n", buff);
        }
    }

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_read_desc, true);
    // } else {
    //     CLI_LOG("error status %d\n", status);
    // }
    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_READ_DESC_CB);
}



void aceBt_bleGattcGetDbCallback(aceBT_bleConnHandle connHandle,
                                 aceBT_bleGattsService_t* gatt_service,
                                 uint32_t no_svc) {
    printf("CLI callback : aceBt_bleGattcGetDbCallback()\n");
    printf("connHandle %p no_svc %" PRIu32 "\n", connHandle, no_svc);
    gNo_svc = no_svc;
    ace_status_t status = aceBT_bleCloneGattService(&pGgatt_service, gatt_service, gNo_svc);
    if (status == ACE_STATUS_OK) {
        for (uint32_t i = 0; i < no_svc; i++) {
            printf(
                "Gatt Database index :%" PRIu32 " %p \n",
                i, &pGgatt_service[i]
            );
            aceBt_utilsDumpServer(&pGgatt_service[i]);
        }

        // if (callback_vars.gattc_svc_discovered && (no_svc > 0)) {
        //     CLI_SET_CB_VAR(callback_vars.got_gatt_db, true);
        // }

        // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_GET_DB_CB);
    } else {
        printf("Error copying GATT database %d\n", status);
    }
}

void aceBt_bleGattcExecuteWriteCallback(aceBT_bleConnHandle connHandle,
                                        ace_status_t status) {
    printf("CLI callback : aceBt_bleGattcExecuteWriteCallback()\n");
    printf("connHandle %p status %d\n", connHandle, status);
    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.gattc_exec_write, true);
    // } else {
    //     CLI_LOG("error status %d\n", status);
    // }
    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_EXEC_WRITE_CB);
}

aceBT_bleGattClientCallbacks_t gatt_client_callback = {
    .size = sizeof(aceBT_bleGattClientCallbacks_t),
    .on_ble_gattc_service_registered_cb = aceBt_bleGattcRegCallback,
    /*service discovered callback*/
    // .on_ble_gattc_service_discovered_cb = NULL,
    // .on_ble_gattc_read_characteristics_cb = NULL,
    // .on_ble_gattc_write_characteristics_cb = NULL,
    // .notify_characteristics_cb = NULL,
    // .on_ble_gattc_write_descriptor_cb = NULL,
    // .on_ble_gattc_read_descriptor_cb = NULL,
    // .on_ble_gattc_get_gatt_db_cb = NULL,
    // .on_ble_gattc_execute_write_cb = NULL,

    .on_ble_gattc_service_discovered_cb =
        aceBt_bleGattcServiceDiscoveredCallback,
    .on_ble_gattc_read_characteristics_cb = aceBt_bleGattcReadCharsCallback,
    .on_ble_gattc_write_characteristics_cb =
        aceBt_bleGattcWriteCharsCallback,
    .notify_characteristics_cb = aceBt_bleGattcNotifyCharsCallback,
    .on_ble_gattc_write_descriptor_cb = aceBt_bleGattcWriteDescCallback,
    .on_ble_gattc_read_descriptor_cb = aceBt_bleGattcReadDescCallback,
    .on_ble_gattc_get_gatt_db_cb = aceBt_bleGattcGetDbCallback,
    .on_ble_gattc_execute_write_cb = aceBt_bleGattcExecuteWriteCallback,
};

void aceBt_beaconAdvStateCallback(
    aceBT_advInstanceHandle advInstance,
    aceBT_beaconAdvState_t state,
    aceBT_beaconPowerMode_t powerMode,
    aceBT_beaconAdvMode_t beaconMode
) {
    // UNUSED(advInstance);
    // UNUSED(powerMode);
    // UNUSED(beaconMode);
    if (state == ACEBT_BEACON_ADV_STARTED) {
        printf(
            "CLI callback : aceBtCli_beaconAdvStateCallback() state: "
            "ADV_STARTED\n");
        // CLI_SET_CB_VAR(callback_vars.adv_started, true);
    } else if (state == ACEBT_BEACON_ADV_STOPPED) {
        printf(
            "CLI callback : aceBtCli_beaconAdvStateCallback() state: "
            "ADV_STOPPED\n");
        // CLI_SET_CB_VAR(callback_vars.adv_started, false);
    } else if (state == ACEBT_BEACON_ADV_STOP_FAILED) {
        printf(
            "CLI callback : aceBtCli_beaconAdvStateCallback() state: "
            "ADV_STOP_FAILED\n");
    } else {
        return;
    }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_ADV_STATE_CB);
}

void aceBt_scanStateCallback(
    aceBT_scanInstanceHandle scanInstance,
    aceBT_beaconScanState_t state,
    uint32_t interval, uint32_t window
) {
    printf("aceBT_beaconScanState_t scan: %" PRIu32
            " state: %u interval: %" PRIu32 " window: %" PRIu32 "\n",
            (uint32_t)scanInstance, state, interval, window);

    if (state == ACEBT_BEACON_SCAN_STARTED) {
        printf(
            "CLI callback : aceBtCli_scanStateCallback() state: "
            "BEACON_SCAN_STARTED\n");
        // CLI_SET_CB_VAR(callback_vars.beacon_scan_started, true);
    } else if (state == ACEBT_BEACON_SCAN_STOPPED) {
        printf(
            "CLI callback : aceBtCli_scanStateCallback() state: "
            "BEACON_SCAN_STOPPED\n");
        // CLI_SET_CB_VAR(callback_vars.beacon_scan_started, false);
    } else {
        return;
    }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_SCAN_STATE_CB);
}

void aceBt_beaconClientRegisteredCallback(ace_status_t status) {
    printf(
        "CLI callback : aceBtCli_beaconClientRegisteredCallback() status: %d\n",
        status);

    registered_beacon_client = true;

    // if (status == ACE_STATUS_OK) {
    //     CLI_SET_CB_VAR(callback_vars.beacon_registered, true);
    // }

    // CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_BEACON_REG_CB);
}

aceBT_beaconCallbacks_t beacon_callbacks = {
    .size = sizeof(aceBT_beaconCallbacks_t),
    .advStateChanged = aceBt_beaconAdvStateCallback,
    .scanStateChanged = aceBt_scanStateCallback,
    // .scanResults = aceBtCli_scanResultsCallback,
    .onclientRegistered = aceBt_beaconClientRegisteredCallback
};

void session_state_callback(
    aceBT_sessionHandle sessionHandle, aceBT_sessionState_t state
) {
    printf(
        "Session_callback() handle:0x%" PRIx32 " state:%d\n",
        (uint32_t)sessionHandle, state
    );

    // // BT Classic stuff?
    // aceBT_registerClientCallbacks(bt_session, &bt_callbacks);
    // aceBT_registerAsSecurityClient(bt_session, &bt_security_callbacks);

    // // Register BLE
    // ace_status_t bleregister_status = aceBT_bleRegister(bt_session, &ble_callbacks);
    // printf("BLE registration status %d\n", bleregister_status);
}

static aceBT_sessionCallbacks_t session_callbacks = {
    sizeof(aceBT_sessionCallbacks_t), session_state_callback
};

void* ble_connection(void* arg) {
    printf("BLE Connection\n");

    aceBT_bdAddr_t bdaddr;
    char bdaddrStr[] = "2C:CF:67:B8:DC:3F";

    if (utilsConvertStrToBdAddr(bdaddrStr, &bdaddr) != ACE_STATUS_OK) {
        printf("Failed to convert string BT ADDR\n");
        return NULL;
    }

    char addr[ACEBT_MAC_ADDR_STR_LEN];
    memset(addr, 0, ACEBT_MAC_ADDR_STR_LEN);
    utilsConvertBdAddrToStr(&bdaddr, addr);
    printf("ble_connection BD addr back to str: %s\n", addr);

    aceBt_bleConnParam_t conn_param = ACE_BT_BLE_CONN_PARAM_BALANCED;
    aceBt_bleConnPriority_t conn_priority = ACE_BT_BLE_CONN_PRIO_MEDIUM;
    bool auto_connect = true;

    ace_status_t connect_status = aceBt_bleConnect(
        bt_session, &bdaddr, conn_param, ACEBT_BLE_GATT_CLIENT_ROLE,
        auto_connect, conn_priority
    );
    printf("Connection attempt status %d\n", connect_status);

    return NULL;
}

void* gattc_registration(void* arg) {
    printf("gattc_registration - start");

    ace_status_t gattc_status = aceBT_bleRegisterGattClient(
        bt_session, &gatt_client_callback
    );
    // ace_status_t gattc_status = aceBt_bleRegisterGattClient(
    //     bt_session, &gatt_client_callback, ACE_BT_BLE_APPID_GENERIC
    // );
    printf("gattc_registration - Gatt Client registration %d\n", gattc_status);

    while(!registered_gatt_client) {
        printf("gattc_registration - Gatt Client not yet registered. Waiting\n");
        sleep(5);
    }
    sleep(5);
    printf("gattc_registration - Registered as GATT client\n");

    return NULL;
}

int main() {
    // Testing ASCII string to HEX str conversion
    // char test1[] = "OFF";
    // char test2[200];
    // for (int i = 0; i < strlen(test1); i++) {
    //     sprintf(test2 + 2 * i, "%.2x", test1[i]);
    // }
    // printf("HEX str: %s\n", test2);
    // return 0;

    // The ACE BT stuff won't run under root user
    if (setgid((gid_t)BLUETOOTH_GROUP_ID) || setuid((uid_t)BLUETOOTH_USER_ID)) {
        fprintf(stderr, "Can't drop privileges to bluetooth user/group\n");
        return -1;
    }

    printf("MAIN - User ID %d\n", getuid());
    printf("MAIN - Group ID %d\n", getgid());

    printf("MAIN - Hello World from Kindle!\n");

    bool isBLE = aceBT_isBLESupported();
    printf("MAIN - Is BLE enabled: %d\n", isBLE);

    aceBT_sessionType_t supported_session = aceBT_getSupportedSession();
    printf("MAIN - Supported session type: %d\n", supported_session);

    ace_status_t session_result = aceBT_openSession(
        // ACEBT_SESSION_TYPE_DUAL_MODE, &session_callbacks, &bt_session
        ACEBT_SESSION_TYPE_BLE, &session_callbacks, &bt_session
    );
    printf("MAIN - session_result: %d\n", session_result);
    printf("MAIN - BT Client :: opened session with %" PRIx32 "\n", (uint32_t)bt_session);

    aceBT_state_t radio_state = ACEBT_STATE_DISABLED;
    ace_status_t radio_status = aceBT_getRadioState(&radio_state);
    printf("MAIN - getRadioState() state:%u status:%d\n", radio_state, radio_status);

    while(radio_state != ACEBT_STATE_ENABLED) {
        printf("MAIN - Radio not yet enabled. Waiting...\n");
        sleep(5);
    }
    printf("MAIN - Radio is enabled\n");

    // // BT Classic stuff?
    // aceBT_registerClientCallbacks(bt_session, &bt_callbacks);
    // aceBT_registerAsSecurityClient(bt_session, &bt_security_callbacks);

    // Register BLE
    aceBT_registerAsSecurityClient(bt_session, &bt_security_callbacks);
    ace_status_t bleregister_status = aceBT_bleRegister(bt_session, &ble_callbacks);
    printf("MAIN - BLE registration status %d\n", bleregister_status);

    while(!registered_ble) {
        printf("MAIN - BLE not yet registered. Waiting\n");
        sleep(5);
    }
    printf("MAIN - BLE registered\n");

    // printf("MAIN - Deregister GATT clients?\n");
    // ace_status_t gattc_deregister_status = aceBT_bleDeRegisterGattClient(bt_session);
    // printf("MAIN - GATT Client deregistration %d\n", gattc_deregister_status);

    // sleep(5);

    // printf("MAIN - 2nd radio state check\n");
    // radio_state = ACEBT_STATE_DISABLED;
    // radio_status = aceBT_getRadioState(&radio_state);
    // printf("MAIN - getRadioState() state:%u status:%d\n", radio_state, radio_status);

    // ace_status_t beaconc_status = aceBT_RegisterBeaconClient(
    //     bt_session, &beacon_callbacks
    // );
    // printf("MAIN - beaconc_status %d\n", beaconc_status);

    // while(!registered_beacon_client) {
    //     printf("MAIN - Beacon Client not yet registered. Waiting\n");
    //     sleep(5);
    // }
    // printf("MAIN - Beacon Client registered!\n");

    ace_status_t gattc_status = aceBT_bleRegisterGattClient(
        bt_session, &gatt_client_callback
    );
    printf("MAIN - Gatt Client registration %d\n", gattc_status);
    // ace_status_t gattc_status = aceBt_bleRegisterGattClient(
    //     bt_session, &gatt_client_callback, ACE_BT_BLE_APPID_GENERIC
    // );

    // If I don't comment out this bit here, it will hang forever
    // while(!registered_gatt_client) {
    //     printf("MAIN - Gatt Client not yet registered. Waiting\n");
    //     sleep(5);
    // }
    // sleep(5);
    printf("MAIN - Registered as GATT client\n");

    // pthread_t thread1;
    // pthread_create(&thread1, NULL, gattc_registration, NULL);
    // pthread_join(thread1, NULL);

    // sleep(10);

    printf("MAIN - BLE Connection\n");

    aceBT_bdAddr_t bdaddr;
    char bdaddrStr[] = "2C:CF:67:B8:DC:3F";

    if (utilsConvertStrToBdAddr(bdaddrStr, &bdaddr) != ACE_STATUS_OK) {
        printf("MAIN - Failed to convert string BT ADDR\n");
        return -1;
    }

    aceBt_bleConnParam_t conn_param = ACE_BT_BLE_CONN_PARAM_BALANCED;
    aceBt_bleConnPriority_t conn_priority = ACE_BT_BLE_CONN_PRIO_MEDIUM;
    // False or it won't connect to Pico
    bool auto_connect = false;

    ace_status_t connect_status = aceBt_bleConnect(
        bt_session, &bdaddr, conn_param, ACEBT_BLE_GATT_CLIENT_ROLE,
        auto_connect, conn_priority
    );
    printf("MAIN - Connection attempt status %d\n", connect_status);
    while(ble_conn_handle == NULL) {
        printf("MAIN - Still not connected to BLE device. Waiting...\n");
        sleep(2);
    }

    printf("MAIN - Get BLE DB\n");
    ace_status_t bledb_status = aceBT_bleGetService(ble_conn_handle);
    // Potential race condition with using this variable for readiness check
    // Probably want to switch all of these checks to bespoke bools
    while(pGgatt_service == NULL) {
        printf("MAIN - Still not gotten GATT DB. Waiting...\n");
        sleep(2);
    }
    printf("MAIN - Got GATT DB status %d\n", bledb_status);

    // Pico LED characteristic
    aceBT_uuid_t charac_uuid;
    char charac_str[] = "ff120000000000000000000000000000";
    if (utilsConvertHexStrToByteArray(charac_str, charac_uuid.uu) == 0) {
        printf("MAIN - Failed to convert string to GATT Characteristic UUID\n");
        return -2;
    }

    struct aceBT_gattCharRec_t* charac_rec = utilsFindCharRec(
        charac_uuid, strlen(charac_str) / 2
    );

    if (charac_rec == NULL) {
        printf("Couldn't find GATT Characteristic UUID\n");
        return -3;
    }

    // BLE notification
    printf("MAIN - Enabling notification on PICO LED Characteristic\n");
    ace_status_t notification_status = aceBT_bleSetNotification(
        bt_session, ble_conn_handle, charac_rec->value, true
    );
    printf("MAIN - Notification status: %d\n", notification_status);

    sleep(3);

    printf("MAIN - Disabling notification on PICO LED Characteristic\n");
    notification_status = aceBT_bleSetNotification(
        bt_session, ble_conn_handle, charac_rec->value, false
    );
    printf("MAIN - Notification status: %d\n", notification_status);

    sleep(3);


    // Read and write LED characteristic infinite block
    while(true) {
        ace_status_t char_read_status = aceBT_bleReadCharacteristics(
            bt_session, ble_conn_handle, charac_rec->value
        );
        printf("MAIN - Read characteristic. Status %d\n", char_read_status);
        sleep(2);


        printf("MAIN - Write characteristic\n");
        // Switch the LED status. If currently ON, set to OFF
        char write_msg_str[10];
        snprintf(write_msg_str, sizeof(write_msg_str), (pico_led_status) ? "OFF" : "ON");
        printf("MAIN - About to write %s. Len: %d\n", write_msg_str, strlen(write_msg_str));

        char write_msg[200];
        for (int i = 0; i < strlen(write_msg_str); i++)
            sprintf(write_msg + 2 * i, "%.2x", write_msg_str[i]);
        printf("MAIN - Convert ASCII str to HEX str: %s\n", write_msg);

        // If odd length characteristic, add extra byte
        uint8_t add_val = (strlen(write_msg) % 2 != 0);

        // charac_rec->value.blobValue.data = aceAlloc_alloc(
        //     ACE_MODULE_BT, ACE_ALLOC_BUFFER_GENERIC, strlen(write_msg) / 2 + add_val
        // );
        // if (charac_rec->value.blobValue.data == NULL) {
        //     printf("MAIN - Memory allocation failure in blewriteCharacteristics.\n");
        //     return -4;
        // }

        void* alloc_result = aceAlloc_alloc(
            ACE_MODULE_BT, ACE_ALLOC_BUFFER_GENERIC, strlen(write_msg) / 2 + add_val
        );
        if (alloc_result == NULL) {
            printf("MAIN - Memory allocation failure in blewriteCharacteristics.\n");
            return -4;
        } else {
            printf("MAIN - Write alloc result: %" PRIu8 "\n", (uint8_t *) alloc_result);
            charac_rec->value.blobValue.data = (uint8_t *) alloc_result;
        }

        if (add_val) {
            charac_rec->value.blobValue.data[0] = utilsConvertCharToHex(write_msg[0]);
        }

        uint16_t i;
        uint16_t offset = add_val;
        for (i = add_val; i <= strlen(write_msg) / 2; i++) {
            charac_rec->value.blobValue.data[i] = utilsConvertCharToHex(write_msg[offset++]) << 4;
            charac_rec->value.blobValue.data[i] |= utilsConvertCharToHex(write_msg[offset++]);
        }

        charac_rec->value.blobValue.size = strlen(write_msg) / 2 + add_val;

        printf("MAIN - Record blobValue data int: %u\n", charac_rec->value.blobValue.data);
        printf("MAIN - Record blobValue data str: %s\n", (char *) charac_rec->value.blobValue.data);
        printf("MAIN - Record blobValue data hex: ");
        for (int idx = 0; idx < charac_rec->value.blobValue.size; idx++) {
            printf("%02x ", charac_rec->value.blobValue.data[idx]);
        }
        printf("\n");

        ace_status_t char_write_status = aceBT_bleWriteCharacteristics(
            bt_session, ble_conn_handle, &charac_rec->value,
            ACEBT_BLE_WRITE_TYPE_RESP_REQUIRED
            // ACEBT_BLE_WRITE_TYPE_RESP_NO
        );
        aceAlloc_free(ACE_MODULE_BT, ACE_ALLOC_BUFFER_GENERIC, charac_rec->value.blobValue.data);
        printf("MAIN - Wrote characteristic. Status %d\n", char_write_status);

        // memset(charac_rec->value.blobValue.data, 0, strlen(write_msg));
        // char val[512];
        // memset(val, 0, sizeof(val) / sizeof(val[0]));
        // charac_rec->value.blobValue.size = (charac_rec->value.blobValue.size > 512) ? 512 : charac_rec->value.blobValue.size;

        sleep(2);
    }

    // char charac_value[] = "";

    // pthread_t thread1;
    // pthread_create(&thread1, NULL, ble_connection, NULL);
    // pthread_join(thread1, NULL);

    // printf("MAIN - Sleeping now...\n");
    // sleep(5);

    // printf("MAIN - Another sleep...\n");
    // sleep(5);

    printf("MAIN - Final infinite sleep. Use Ctrl-C to exit\n");
    sleep(600);

    return 0;
}
