/**
 * \file        fsm_windshield_wiper.c
 * \brief       A Finite State Machine for the windshield wiper.
 * \details     Contains states and callback functions to control windshield wiper.
 * \author      Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#include "fsm_windshield_washer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* States */
// typedef enum {
//     ST_ANY = -1,                            /* Any state */
//     ST_INIT = 0,                            /* Init state */
//     ST_ALL_OFF = 1,
//     ST_WIPER_ON = 2,
// 	ST_ALL_ON = 3,
//     ST_WAIT = 4,
//     ST_TERM = 255                           /* Final state */
// } fsm_windshield_state_t;

/* Events */
// typedef enum {
//     EV_ANY = -1,                            /* Any event */
//     EV_NONE = 0,                            /* No event */
//     EV_CMD_EG_0 = 1,
//     EV_CMD_EG_1 = 2,
// 	EV_CMD_LG_0 = 3,
// 	EV_CMD_LG_1 = 4,
// 	EV_T_OVER2SEC = 5,
// 	EV_T_UNDER2SEC = 6,
//     EV_ERR = 255                            /* Error event */
// } fsm_windshield_event_t;

/**
* \brief Function to print state machine status info.
* \details Only for debug, message written in french
*/
/* Callback functions called on transitions */
static int8_t callback_ALL_OFF (void) {printf("Passage à l'état TOUS ETEINTS\n");};
static int8_t callback_WIPER_ON (void) {printf("Passage à l'état ESSUIE-GLACES ACTIVES\n");};
static int8_t callback_ALL_ON (void) {printf("Passage à l'état LAVE GLACES + ESSUIE-GLACES ACTIVES\n");};
static int8_t callback_WAIT (void) {printf("Passage à l'état TIMER ESSUIE GLACES & LAVE GLACES ETEINTS\n");};
static int8_t FsmError(void) {printf("Erreur. Passage à l'état TERM\n");};

/* Transition structure */
typedef struct {
    fsm_windshield_state_t state;
    fsm_windshield_event_t event;
    int8_t (*callback)(void);
    int8_t next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    /* Transitions */
    { ST_ALL_OFF, EV_CMD_EG_0, &callback_ALL_OFF, ST_ALL_OFF},
	{ ST_ALL_OFF, EV_CMD_LG_0, &callback_ALL_OFF, ST_ALL_OFF},
	{ ST_ALL_OFF, EV_CMD_EG_1, &callback_WIPER_ON, ST_WIPER_ON},
	{ ST_ALL_OFF, EV_CMD_LG_1, &callback_ALL_ON, ST_ALL_ON},
	
    { ST_WIPER_ON, EV_CMD_EG_1, &callback_WIPER_ON, ST_WIPER_ON},
	{ ST_WIPER_ON, EV_CMD_EG_0, &callback_ALL_OFF, ST_ALL_OFF},
	{ ST_WIPER_ON, EV_CMD_LG_1, &callback_ALL_ON, ST_ALL_ON},
	
	{ ST_ALL_ON, EV_CMD_LG_1, &callback_ALL_ON, ST_ALL_ON},
	{ ST_ALL_ON, EV_CMD_LG_0, &callback_WAIT, ST_WAIT},

	{ ST_WAIT, EV_T_UNDER2SEC, &callback_WAIT, ST_WAIT},
	{ ST_WAIT, EV_CMD_LG_1, &callback_ALL_ON, ST_ALL_ON},
	{ ST_WAIT, EV_T_OVER2SEC, &callback_ALL_OFF, ST_ALL_OFF},
	
	{ ST_ANY, EV_ERR, &FsmError, ST_TERM}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

/**
* \brief Function to find the current event for the state machine.
* \details 
* \param focus_point : 0 to evaluate eg_status, 1 to evaluate lg_status, 2 to evaluate t_status, anything else trigger "EV_NONE" event
* \param eg_status : 0 to trigger "EV_CMD_EG_1", 1 to trigger "EV_CMD_EG_1"
* \param lg_status : 0 to trigger "EV_CMD_LG_0", 1 to trigger "EV_CMD_LG_1"
* \param t_status : 0 to trigger "EV_T_UNDER2SEC", 1 to trigger "EV_T_OVER2SEC"
* \return int8_t : Event returned as an integer for the state machine
*/
int8_t get_next_event (int8_t focus_point,int8_t eg_status,int8_t lg_status, int8_t t_status)
{
	int8_t event = EV_NONE;
	
	if (focus_point == 0){
		if (eg_status == 0){
			event = EV_CMD_EG_0;
		}
		else if (eg_status == 1){
			event = EV_CMD_EG_1;
		}
	} else if (focus_point == 1){
		if (lg_status == 0){
			event = EV_CMD_LG_0;
		}
		else if (lg_status == 1){
			event = EV_CMD_LG_1;
		}
	} else if (focus_point == 2){
		if (lg_status == 0){
			event = EV_T_UNDER2SEC;
		}
		else if (lg_status == 1){
			event = EV_T_OVER2SEC;
		}
	}
	
	return event;
}

/**
* \brief Function to trigger an event on the state machine.
* \details 
* \param state : Current state of the state machine
* \param focus_point : food for get_next_event()
* \param eg_status : food for get_next_event()
* \param lg_status : food for get_next_event()
* \param t_status : food for get_next_event()
* \return int8_t : New state of the state machine returned as an integer
*/
int8_t fsm_windshield_trigger_event (int8_t state,int8_t focus_point,int8_t eg_status,int8_t lg_status, int8_t t_status)
{
	int8_t i = 0;
    int8_t ret = 0; 
    int8_t event = EV_NONE;
	
	/* Check if FSM hasn't reach end state */
    if (state != ST_TERM) {
		/* Get event */
		event = get_next_event(focus_point,eg_status,lg_status,t_status);
		
		/* For each transitions */
		for (i = 0; i < TRANS_COUNT; i++) {
			/* If State is current state OR The transition applies to all states ...*/
			if ((state == trans[i].state) || (ST_ANY == trans[i].state)) {
				/* If event is the transition event OR the event applies to all */
				if ((event == trans[i].event) || (EV_ANY == trans[i].event)) {
					/* Apply the new state */
					state = trans[i].next_state;
					if (trans[i].callback != NULL) {
						/* Call the state function */
						ret = (trans[i].callback)();
					}
					break;
				}
			}
		}
	}
	
	return state;
}

/**
* \brief Function to initialize and start the state machine.
* \details 
* \return int8_t : Finite state machine state returned as an integer
*/
int main(void)
{
    int i = 0;
    int ret = 0; 
    int event = EV_NONE;
    int state = ST_INIT;
    
    /* While FSM hasn't reach end state */
    while (state != ST_TERM) {
        
        /* Get event */
        event = get_next_event(0, 0, 0, 0);
        
        /* For each transitions */
        for (i = 0; i < TRANS_COUNT; i++) {
            /* If State is current state OR The transition applies to all states ...*/
            if ((state == trans[i].state) || (ST_ANY == trans[i].state)) {
                /* If event is the transition event OR the event applies to all */
                if ((event == trans[i].event) || (EV_ANY == trans[i].event)) {
                    /* Apply the new state */
                    state = trans[i].next_state;
                    if (trans[i].callback != NULL) {
                        /* Call the state function */
                        ret = (trans[i].callback)();
                    }
                    break;
                }
            }
        }
    }

    return ret;
}