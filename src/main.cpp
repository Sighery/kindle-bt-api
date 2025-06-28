#include <stdio.h>
#include <inttypes.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#include "ace_bt.h"
#include "bluetooth_session_api.h"
#include "bluetooth_api.h"
#include "bluetooth_common_api.h"
#include "bluetooth_ble_api.h"
#include "bluetooth_ble_gatt_client_api.h"
// #include "bluetooth_manager_ble_gattc_util.h"

#define ADDR_WITH_COLON_LEN 17
#define ADDR_WITHOUT_COLON_LEN 12
#define PRINT_UUID_STR_LEN 49


aceBT_sessionHandle bt_session = NULL;
aceBT_bleConnHandle ble_conn_handle = NULL;
bool registered_ble = false;
bool registered_gatt_client = false;

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
    printf("connHandle %p\n", connHandle);

    // char buff[256];
    // aceBtCli_utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    // CLI_LOG("UUID:: %s", buff);

    // for (int idx = 0; idx < charsValue.blobValue.size; idx++)
    //     CLI_LOG("%02x", charsValue.blobValue.data[idx]);

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
    // char buff[256];
    // aceBtCli_utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    // CLI_LOG("UUID:: %s", buff);
    // for (int idx = 0; idx < charsValue.blobValue.size; idx++)
    //     CLI_LOG("%x", charsValue.blobValue.data[idx]);
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

    // char buff[256];
    // aceBtCli_utilsPrintUuid(buff, &charsValue.gattRecord.uuid, 256);
    // CLI_LOG("Char UUID:: %s", buff);

    // /* Make sure data is not null before printing */
    // if (charsValue.blobValue.data != NULL) {
    //     if (sprintf(buff, "%x", *(uint8_t*)charsValue.blobValue.data) > 0) {
    //         CLI_LOG("Descriptor Value is: %s", buff);
    //     }
    // }

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
    // connection_handle = connHandle;
    // gNo_svc = no_svc;
    // ace_status_t status =
    //     aceBT_bleCloneGattService(&pGgatt_service, gatt_service, gNo_svc);
    // if (status == ACE_STATUS_OK) {
    //     for (uint32_t i = 0; i < no_svc; i++) {
    //         CLI_LOG("Gatt Database index :%" PRIu32 " %p ", i,
    //                 &pGgatt_service[i]);
    //         aceBtCli_utilsDumpServer(&pGgatt_service[i]);
    //     }

    //     if (callback_vars.gattc_svc_discovered && (no_svc > 0)) {
    //         CLI_SET_CB_VAR(callback_vars.got_gatt_db, true);
    //     }

    //     CLI_SEM_POST(sync_cmd_sem, ACEBT_CLI_BLE_GET_DB_CB);
    // } else {
    //     CLI_LOG("Error copying GATT database %d", status);
    // }
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

// ACE_STATUS_OK = 0

int main() {
    printf("Hello World from Kindle!\n");

    bool isBLE = aceBT_isBLESupported();
    printf("Is BLE enabled: %d\n", isBLE);

    aceBT_sessionType_t supported_session = aceBT_getSupportedSession();
    printf("Supported session type: %d\n", supported_session);

    ace_status_t session_result = aceBT_openSession(
        ACEBT_SESSION_TYPE_DUAL_MODE, &session_callbacks, &bt_session
    );
    printf("session_result: %d\n", session_result);
    printf("BT Client :: opened session with %" PRIx32 "\n", (uint32_t)bt_session);

    aceBT_state_t radio_state = ACEBT_STATE_DISABLED;
    ace_status_t radio_status = aceBT_getRadioState(&radio_state);
    printf("getRadioState() state:%u status:%d\n", radio_state, radio_status);

    while(radio_state != ACEBT_STATE_ENABLED) {
        printf("Radio not yet enabled. Waiting...\n");
        sleep(5);
    }
    printf("Radio is enabled\n");

    // BT Classic stuff?
    aceBT_registerClientCallbacks(bt_session, &bt_callbacks);
    aceBT_registerAsSecurityClient(bt_session, &bt_security_callbacks);

    // Register BLE
    aceBT_registerAsSecurityClient(bt_session, &bt_security_callbacks);
    ace_status_t bleregister_status = aceBT_bleRegister(bt_session, &ble_callbacks);
    printf("BLE registration status %d\n", bleregister_status);

    while(!registered_ble) {
        printf("BLE not yet registered. Waiting\n");
        sleep(5);
    }
    printf("BLE registered\n");

    printf("Deregister GATT clients?\n");
    ace_status_t gattc_deregister_status = aceBT_bleDeRegisterGattClient(bt_session);
    printf("GATT Client deregistration %d\n", gattc_deregister_status);

    sleep(5);

    ace_status_t gattc_status = aceBt_bleRegisterGattClient(
        bt_session, &gatt_client_callback
    );
    // ace_status_t gattc_status = aceBt_bleRegisterGattClient(
    //     bt_session, &gatt_client_callback, ACE_BT_BLE_APPID_GENERIC
    // );
    printf("Gatt Client registration %d\n", gattc_status);

    // If I don't comment out this bit here, it will hang forever

    // while(!registered_gatt_client) {
    //     printf("Gatt Client not yet registered. Waiting\n");
    //     sleep(5);
    // }
    sleep(5);
    printf("Registered as GATT client\n");

    printf("BLE Connection\n");

    aceBT_bdAddr_t bdaddr;
    char bdaddrStr[] = "2C:CF:67:B8:DC:3F";

    if (utilsConvertStrToBdAddr(bdaddrStr, &bdaddr) != ACE_STATUS_OK) {
        printf("Failed to convert string BT ADDR\n");
        return -1;
    }

    aceBt_bleConnParam_t conn_param = ACE_BT_BLE_CONN_PARAM_BALANCED;
    aceBt_bleConnPriority_t conn_priority = ACE_BT_BLE_CONN_PRIO_MEDIUM;
    bool auto_connect = true;

    ace_status_t connect_status = aceBt_bleConnect(
        bt_session, &bdaddr, conn_param, ACEBT_BLE_GATT_CLIENT_ROLE,
        auto_connect, conn_priority
    );
    printf("Connection attempt status %d\n", connect_status);

    // pthread_t thread1;
    // pthread_create(&thread1, NULL, ble_connection, NULL);
    // pthread_join(thread1, NULL);

    printf("Sleeping now...\n");
    sleep(5);

    printf("Another sleep...\n");
    sleep(5);

    return 0;
}
