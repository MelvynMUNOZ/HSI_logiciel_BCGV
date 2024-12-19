#ifndef FSM_H
#define FSM_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* States */
typedef enum {
    ST_ANY = -1,                            /* Any state */
    ST_INIT = 0,                            /* Init state */
    ST_ALL_OFF = 1,
    ST_WIPER_ON = 2,
	ST_ALL_ON = 3,
    ST_WAIT = 4,
    ST_TERM = 255                           /* Final state */
} fsm_windshield_state_t;

/* Events */
typedef enum {
    EV_ANY = -1,                            /* Any event */
    EV_NONE = 0,                            /* No event */
    EV_CMD_EG_0 = 1,
    EV_CMD_EG_1 = 2,
	EV_CMD_LG_0 = 3,
	EV_CMD_LG_1 = 4,
	EV_T_OVER2SEC = 5,
	EV_T_UNDER2SEC = 6,
    EV_ERR = 255                            /* Error event */
} fsm_windshield_event_t;

/**
* \brief Function to initialize and start the state machine.
* \details 
* \return int8_t : Finite state machine state returned as an integer
*/
int8_t fsm_windshield_start(void);


/**
* \brief Function to trigger an event on the state machine.
* \details 
* \param state : Current state of the state machine
* \param focus_point : 0 to evaluate eg_status, 1 to evaluate lg_status, 2 to evaluate t_status, anything else trigger "EV_NONE" event
* \param eg_status : 0 to trigger "EV_CMD_EG_1", 1 to trigger "EV_CMD_EG_1"
* \param lg_status : 0 to trigger "EV_CMD_LG_0", 1 to trigger "EV_CMD_LG_1"
* \param t_status : 0 to trigger "EV_T_UNDER2SEC", 1 to trigger "EV_T_OVER2SEC"
* \return int8_t : New state of the state machine returned as an integer
*/
/* Function to trigger an event in the state machine */
int8_t fsm_windshield_trigger_event(int8_t state,int8_t focus_point,int8_t eg_status,int8_t lg_status,int8_t t_status);

#endif /* FSM_H f*/
