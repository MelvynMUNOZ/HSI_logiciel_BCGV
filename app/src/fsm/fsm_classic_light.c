/**
 * \file        fsm_classic_light.c
 * \author      Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 * \date        November 16 2024
 * \brief       This is a template file to create a Finite State Machine.
 * \details
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "bcgv_api.h"


/* States */
typedef enum {
    ST_ANY = -1,                            /* Any state */
    ST_INIT = 0,                            /* Init state */
    // ST_STATE1 = 1,
    // ST_STATE2 = 2,
    // ST_STATE3 = 3,

    ST_ALL_OFF = 1,
    ST_ONE_ON = 2,
    ST_ONE_ON_VERIFIED = 3,

    ST_TERM = 255                           /* Final state */
} fsm_state_t;

/* Events */
typedef enum {
    EV_ANY = -1,                            /* Any event */
    EV_NONE = 0,                            /* No event */
    EV_EVENT_POSITION_ON = 1,
    EV_EVENT_POSITION_OFF = 2,
    EV_EVENT_CROSSING_ON = 3,
    EV_EVENT_CROSSING_OFF = 4,
    EV_EVENT_HIGH_ON = 5,
    EV_EVENT_HIGH_OFF = 6,
    EV_ERR = 255                            /* Error event */
} fsm_event_t;

/* Callback functions called on transitions */
static int callback_init (void) { 
    set_flag_position_light(0);
    set_flag_crossing_light(0);
    set_flag_high_light(0);
}
static int callback_position_ON (void) { 
    set_flag_position_light(1);
    // set_flag_crossing_light(0);
    // set_flag_high_light(0);
}
static int callback_position_OFF (void) { 
    set_flag_position_light(0);
    // set_flag_crossing_light(0);
    // set_flag_high_light(0);
}
static int callback_crossing_ON (void) { 
    // set_flag_position_light(0);
    set_flag_crossing_light(1);
    // set_flag_high_light(0);
}
static int callback_crossing_OFF (void) { 
    // set_flag_position_light(0);
    set_flag_crossing_light(0);
    // set_flag_high_light(0);
}
static int callback_high_ON (void) { 
    // set_flag_position_light(0);
    // set_flag_crossing_light(0);
    set_flag_high_light(1);
}
static int callback_high_OFF (void) { 
    // set_flag_position_light(0);
    // set_flag_crossing_light(0);
    set_flag_high_light(0);
}
static int FsmError(void) { 
    set_flag_position_light(1);
    set_flag_crossing_light(1);
    set_flag_high_light(1);
}

/* Transition structure */
typedef struct {
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    // { ST_INIT, EV_NONE, &callback_init, ST_STATE1},
    // { ST_STATE1, EV_EVENT1, &callback2, ST_STATE2},
    // { ST_STATE2, EV_EVENT1, &callback3, ST_STATE3},
    // { ST_STATE2, EV_EVENT2, &callback1, ST_STATE1},
    // { ST_STATE2, EV_NONE, &callback4, ST_TERM},
    // { ST_STATE3, EV_EVENT2, &callback1, ST_STATE1},
    // { ST_ANY, EV_ERR, &FsmError, ST_TERM}


    { ST_INIT, EV_NONE, &callback_init, ST_ALL_OFF},
    
    { ST_ALL_OFF, EV_EVENT_POSITION_ON, &callback_position_ON, ST_ONE_ON},
    { ST_ONE_ON, EV_EVENT_POSITION_ON, &callback_position_ON, ST_ONE_ON_VERIFIED}, // a voir pour la callback
    { ST_ONE_ON, EV_EVENT_POSITION_OFF, &callback_init, ST_ALL_OFF},

    { ST_ALL_OFF, EV_EVENT_CROSSING_ON, &callback_crossing_ON, ST_ONE_ON},
    { ST_ONE_ON, EV_EVENT_CROSSING_ON, &callback_crossing_ON, ST_ONE_ON_VERIFIED}, // a voir pour la callback
    { ST_ONE_ON, EV_EVENT_CROSSING_OFF, &callback_init, ST_ALL_OFF},
    
    { ST_ALL_OFF, EV_EVENT_HIGH_ON, &callback_high_ON, ST_ONE_ON},
    { ST_ONE_ON, EV_EVENT_HIGH_ON, &callback_high_ON, ST_ONE_ON_VERIFIED}, // a voir pour la callback
    { ST_ONE_ON, EV_EVENT_HIGH_OFF, &callback_init, ST_ALL_OFF},

    { ST_ONE_ON, EV_NONE, &FsmError, ST_TERM},
    { ST_ONE_ON_VERIFIED, EV_NONE, &callback_init, ST_ALL_OFF},
    { ST_ANY, EV_ERR, &FsmError, ST_TERM}

};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

bool is_all_light_OFF(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_high_light){
    return cmd_position_light == 0 && cmd_crossing_light == 0 && cmd_high_light == 0;
}

bool is_position_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_high_light){
    return cmd_position_light == 1 && cmd_crossing_light == 0 && cmd_high_light == 0;
}

bool is_crossing_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_high_light){
    return cmd_position_light == 0 && cmd_crossing_light == 1 && cmd_high_light == 0;
}

bool is_high_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_high_light){
    return cmd_position_light == 0 && cmd_crossing_light == 0 && cmd_high_light == 1;
}

int get_next_event(int current_state)
{
    int event = EV_NONE; // a voir si on met par default a NONE ou a ERR

    /* Here, you can get the parameters of your FSM */
    cmd_light_t cmd_position_light = get_cmd_position_light();
    cmd_light_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_light_t cmd_high_light = get_cmd_high_light();

    flag_t flag_position_light = get_flag_position_light();
    flag_t flag_crossing_light = get_flag_crossing_light();
    flag_t flag_high_light = get_flag_high_light();

    /* Build all the events */
    
    // if (current_state == ST_INIT){
    //     if(cmd_position_light != 0 && cmd_crossing_light != 0 && cmd_high_light != 0){
    //         event = EV_ERR;
    //     }
        
    // }
    if (current_state == ST_ALL_OFF){
        if(is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_high_light)){
            event = EV_NONE;
        }
        else if (is_position_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_POSITION_ON;
        }
        else if (is_crossing_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_CROSSING_ON;
        }
        else if (is_high_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_HIGH_ON;
        }
        // else{
        //     event = EV_ERR;
        // }
    }
    else if (current_state == ST_ONE_ON){
        if(is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_high_light)){
            if (flag_position_light == 1){
                event = EV_EVENT_POSITION_OFF;
            }else if (flag_crossing_light == 1){
                event = EV_EVENT_CROSSING_OFF;
            }else if(flag_high_light == 1){
                event = EV_EVENT_HIGH_OFF;
            }
        }

        else if (flag_position_light == 1 && is_position_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_POSITION_ON;
        }
        else if (flag_crossing_light == 1 && is_crossing_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_CROSSING_ON;
        }
        else if (flag_high_light == 1 && is_high_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        {
            event = EV_EVENT_HIGH_ON;
        }
        else{ 
            event = EV_ERR; //a voir
        }
    }
    else if (current_state == ST_ONE_ON_VERIFIED){
        if(is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_high_light)){
            if (flag_position_light == 1){
                event = EV_EVENT_POSITION_OFF;
            }else if (flag_crossing_light == 1){
                event = EV_EVENT_CROSSING_OFF;
            }else if(flag_high_light == 1){
                event = EV_EVENT_HIGH_OFF;
            }
        }
        // TODO : voir si fait cette version là ou alors on fait les cas d'erreurs et le reste c'est NONE
        // else if (flag_position_light == 1 && is_position_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        // {
        //     event = EV_EVENT_POSITION_ON;
        // }
        // else if (flag_crossing_light == 1 && is_crossing_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        // {
        //     event = EV_EVENT_CROSSING_ON;
        // }
        // else if (flag_high_light == 1 && is_high_light_ON(cmd_position_light, cmd_crossing_light, cmd_high_light))
        // {
        //     event = EV_EVENT_HIGH_ON;
        // }
        // else{
        //     event = EV_ERR;
        // }
    }
    else if (current_state == ST_TERM){
        event = EV_ERR;
    }
    
    return event;
}

int main(void)
{
    int i = 0;
    int ret = 0; 
    int event = EV_NONE;
    int state = ST_INIT;
    
    /* While FSM hasn't reach end state */
    while (state != ST_TERM) {
        
        /* Get event */
        event = get_next_event(state);
        
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
