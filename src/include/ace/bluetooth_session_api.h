/**
 * @file
 *
 * @brief ACE BT Session APIs provide interface for a Bluetooth client to
 * instantiate, use and destroy a Bluetooth session.
 *
 * BT Session is the starting point for any middleware/application (referred as
 * Bluetooth client from here on) on ACE that needs to use Bluetooth based
 * services/communication.
 * A ACE based platform may have either Bluetooth dual mode support or Bluetooth
 * LE only support.
 * A Bluetooth client can invoke aceBT_getSupportedSession() to find session
 * types supported on any ACE based platform.
 * Based on the Application need and underlying platform support, a Bluetooth
 * client can call into aceBT_openSession() API to open a Bluetooth dual mode
 * or Bluetooth LE session.
 * If a Bluetooth session is successfully opened, client will receive session
 * handle as output parameter in aceBT_openSession() API.
 * Clients should open a Bluetooth session only once until the session is closed
 * via aceBT_closeSession() API.
 * Once the session is opened clients should call into callback registration
 * APIs of Bluetooth Classic interface and/or Bluetooth LE interface based on
 * the client preferences.
 */

#ifndef BLUETOOTH_SESSION_API_H
#define BLUETOOTH_SESSION_API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ACE_BT_DS_Session Session manager's Data Structures
 * @{
 * @ingroup ACE_BT_DS
 */

/** @brief Opaque handle representing a Bluetooth session */
typedef struct aceBT_session* aceBT_sessionHandle;

/** @brief ACE BT Session types */
typedef enum {
    /** BLE only session. Use this type if application only needs to use BLE*/
    ACEBT_SESSION_TYPE_BLE = 0,
    /** Classic and BLE session. Use this type if application needs to use both
       classic BT and BLE*/
    ACEBT_SESSION_TYPE_DUAL_MODE = 1,
    /** Classic only session. Use this type if application only needs to use
       classic BT*/
    ACEBT_SESSION_TYPE_CLASSIC = 2,
    /** Neiher classic BT nor BLE is supported*/
    ACEBT_SESSION_TYPE_NONE = 3,
} aceBT_sessionType_t;

/** @brief ACE BT Session states
 * @see acebt_session_state_callback
 */
typedef enum {
    /** ACE BT Session has successfully opened*/
    ACEBT_SESSION_STARTED,
    /** ACE BT Session has successfully closed*/
    ACEBT_SESSION_CLOSED,
    /** ACE BT Session has got restarted due to underlying server*/
    ACEBT_SESSION_RESTARTED
} aceBT_sessionState_t;

/** @} */

/**
 * @defgroup ACE_BT_SESSION_API_CB Bluetooth session callbacks
 * @{
 * @ingroup ACE_BT_CB
 */

/**
 * @brief Session state callback, Sent to session client whenever session state
 *changes CURRENTLY NOT IN USE
 *@param session_handle  session handle.
 *@param state current state of session.
 */
typedef void (*acebt_session_state_callback)(aceBT_sessionHandle session_handle,
                                             aceBT_sessionState_t state);

/** @brief Bluetooth session callback struct */
typedef struct {
    size_t size;
    acebt_session_state_callback session_state_cb;
} aceBT_sessionCallbacks_t;

/** @} */

/**
 * @defgroup ACE_BT_SESSION_API Session Manager APIs for Bluetooth clients
 * @{
 * @ingroup ACE_BT_API
 */

/**
 * @brief Returns the supported Bluetooth session type by underlying platform
 *
 * @return : aceBT_sessionType_t session type supported by underlying platform.
 */
aceBT_sessionType_t aceBT_getSupportedSession(void);

/**
 * @brief Opens/Creates a Bluetooth session for an Application
 * Main entry point for Bluetooth APIs. Clients will need to invoke this API
 * to create a Bluetooth session.
 * Once the session is successfuly opened,client application should register
 * into
 * Bluetooth classic or LE modules based on the session availablity.
 * Use aceBT_getSupportedSession API to retrieve supported Bluetooth session
 * type by underlying platform
 *
 * @param[in] session_type Type of session (dual mode, classic only or LE only)
 * to be opened
 * @param[in] callbacks pointer to session callbacks
 * @param[out] session_handle will be updated with session handle if session
 * creation was successful.
 *
 * @return ACEBT_STATUS_SUCCESS if success
 * @return ACEBT_STATUS_NOMEM if ran out of memory
 * @return ACEBT_STATUS_BUSY if profile is busy connecting another device
 * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 * @return ACEBT_STATUS_NOT_READY if server is not ready
 * @return ACEBT_STATUS_FAIL for all other errors
 */
ace_status_t aceBT_openSession(aceBT_sessionType_t session_type,
                               aceBT_sessionCallbacks_t* callbacks,
                               aceBT_sessionHandle* session_handle);

/**
 * @brief Closes a Bluetooth session for an Application
 * Function to close a Bluetooth session. Clients will need to invoke this API
 * to end the session and also stop receiving the callbacks.
 *
 * @param[in] session_handle session handle for the session to be closed
 * @return ACEBT_STATUS_SUCCESS if success
 * @return ACEBT_STATUS_PARM_INVALID if request contains invalid parameters
 */
ace_status_t aceBT_closeSession(aceBT_sessionHandle session_handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BLUETOOTH_SESSION_API_H
