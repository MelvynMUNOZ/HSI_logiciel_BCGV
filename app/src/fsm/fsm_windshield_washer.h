/**
 * \file        fsm_windshield_washer.h
 * \brief       Finite State Machine for windshield washer and wiper control
 * \details     Header file containing FSM states, events and function declarations
 */

#ifndef FSM_WINDSHIELD_WASHER_H
#define FSM_WINDSHIELD_WASHER_H

#include "bcgv_api.h"

/* States for debug prints */
typedef enum
{
    ST_ANY = -1,                /* Any state */
    ST_ALL_OFF = 0,             /* Initial state: everything off */
    ST_WIPER_ON = 1,            /* Only wipers active */
    ST_WASHER_AND_WIPER_ON = 2, /* Both washer and wipers active */
    ST_WIPER_TIMER = 3,         /* Wipers active with timer */
    ST_TERM = 255               /* Terminal state (not used) */
} fsm_state_t;

/* Events for debug prints */
typedef enum
{
    EV_ANY = -1,       /* Any event */
    EV_NONE = 0,       /* No event */
    EV_CMD_WIPER = 1,  /* Wiper command change */
    EV_CMD_WASHER = 2, /* Washer command change */
    EV_TIMER_2SEC = 3, /* 2-second timer expired */
    EV_ERR = 255       /* Error event */
} fsm_event_t;

/**
 * \brief Initialize the windshield washer FSM
 * \details Must be called once before using the FSM
 */
void fsm_windshield_washer_init(void);

/**
 * \brief Periodic update function for the windshield washer FSM
 * \details Should be called every 100ms by the main application
 */
void fsm_windshield_washer_update(void);

/**
 * \brief Get current state name for debug prints
 * \param state State enum value
 * \return String representation of the state
 */
const char *fsm_get_state_name(fsm_state_t state);

/**
 * \brief Get event name for debug prints
 * \param event Event enum value
 * \return String representation of the event
 */
const char *fsm_get_event_name(fsm_event_t event);

#endif // FSM_WINDSHIELD_WASHER_H