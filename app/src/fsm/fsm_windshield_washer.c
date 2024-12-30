/**
 * \file        fsm_windshield_washer.c
 * \brief       Finite State Machine for windshield washer and wiper control
 * \details     Implementation of the FSM controlling wipers and washer states
 * \author      Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "fsm_windshield_washer.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Static variables */
static uint32_t timer_counter = 0;
static const uint32_t TIMER_2SEC = 20; // 20 * 100ms = 2 seconds

/* Debug string arrays for states and events */
static const char *STATE_NAMES[] = {
    [ST_ALL_OFF] = "ALL_OFF",
    [ST_WIPER_ON] = "WIPER_ON",
    [ST_WASHER_AND_WIPER_ON] = "WASHER_AND_WIPER_ON",
    [ST_WIPER_TIMER] = "WIPER_TIMER"};

static const char *EVENT_NAMES[] = {
    [EV_NONE] = "NONE",
    [EV_CMD_WIPER] = "CMD_WIPER",
    [EV_CMD_WASHER] = "CMD_WASHER",
    [EV_TIMER_2SEC] = "TIMER_2SEC"};

/* Callback functions for state transitions */
static int handle_all_off(void)
{
    printf("DEBUG: FSM - Executing ALL_OFF actions\n");
    set_cmd_wiper(WIPER_OFF);
    set_cmd_washer(WASHER_OFF);
    timer_counter = 0;
    return 0;
}

static int handle_wiper_on(void)
{
    printf("DEBUG: FSM - Executing WIPER_ON actions\n");
    set_cmd_wiper(WIPER_LOW);
    set_cmd_washer(WASHER_OFF);
    timer_counter = 0;
    return 0;
}

static int handle_washer_and_wiper_on(void)
{
    printf("DEBUG: FSM - Executing WASHER_AND_WIPER_ON actions\n");
    set_cmd_wiper(WIPER_LOW);
    set_cmd_washer(WASHER_ON);
    timer_counter = 0;
    return 0;
}

static int handle_wiper_timer(void)
{
    printf("DEBUG: FSM - Executing WIPER_TIMER actions (counter: %lu)\n", (unsigned long)timer_counter);
    set_cmd_wiper(WIPER_LOW);
    set_cmd_washer(WASHER_OFF);
    timer_counter++;
    return 0;
}

static int handle_error(void)
{
    printf("ERROR: FSM - Error condition triggered\n");
    set_cmd_wiper(WIPER_OFF);
    set_cmd_washer(WASHER_OFF);
    timer_counter = 0;
    return -1;
}

/* Transition structure */
typedef struct
{
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    fsm_state_t next_state;
} tTransition;

/* Transition table */
static tTransition trans[] = {
    {ST_ALL_OFF, EV_CMD_WIPER, &handle_wiper_on, ST_WIPER_ON},
    {ST_ALL_OFF, EV_CMD_WASHER, &handle_washer_and_wiper_on, ST_WASHER_AND_WIPER_ON},
    {ST_WIPER_ON, EV_CMD_WIPER, &handle_all_off, ST_ALL_OFF},
    {ST_WIPER_ON, EV_CMD_WASHER, &handle_washer_and_wiper_on, ST_WASHER_AND_WIPER_ON},
    {ST_WASHER_AND_WIPER_ON, EV_CMD_WASHER, &handle_wiper_timer, ST_WIPER_TIMER},
    {ST_WIPER_TIMER, EV_TIMER_2SEC, &handle_all_off, ST_ALL_OFF},
    {ST_WIPER_TIMER, EV_CMD_WIPER, &handle_wiper_on, ST_WIPER_ON},
    {ST_WIPER_TIMER, EV_CMD_WASHER, &handle_washer_and_wiper_on, ST_WASHER_AND_WIPER_ON},
    {ST_ANY, EV_ERR, &handle_error, ST_ALL_OFF}};

#define TRANS_COUNT (sizeof(trans) / sizeof(*trans))

/* Static variables for FSM state */
static fsm_state_t current_state = ST_ALL_OFF;
static cmd_wiper_t last_wiper_cmd = WIPER_OFF;
static cmd_washer_t last_washer_cmd = WASHER_OFF;

const char *fsm_get_state_name(fsm_state_t state)
{
    if (state == ST_ANY)
        return "ANY";
    if (state == ST_TERM)
        return "TERM";
    if (state >= 0 && state <= ST_WIPER_TIMER)
        return STATE_NAMES[state];
    return "UNKNOWN";
}

const char *fsm_get_event_name(fsm_event_t event)
{
    if (event == EV_ANY)
        return "ANY";
    if (event == EV_ERR)
        return "ERROR";
    if (event >= 0 && event <= EV_TIMER_2SEC)
        return EVENT_NAMES[event];
    return "UNKNOWN";
}

static fsm_event_t get_next_event(fsm_state_t current_state)
{
    fsm_event_t event = EV_NONE;

    cmd_wiper_t current_wiper_cmd = get_cmd_wiper();
    cmd_washer_t current_washer_cmd = get_cmd_washer();

    if (current_wiper_cmd != last_wiper_cmd)
    {
        printf("DEBUG: FSM - Wiper command changed: %d -> %d\n", last_wiper_cmd, current_wiper_cmd);
        last_wiper_cmd = current_wiper_cmd;
        if (current_wiper_cmd != WIPER_OFF)
        {
            event = EV_CMD_WIPER;
        }
    }
    else if (current_washer_cmd != last_washer_cmd)
    {
        printf("DEBUG: FSM - Washer command changed: %d -> %d\n", last_washer_cmd, current_washer_cmd);
        last_washer_cmd = current_washer_cmd;
        if (current_washer_cmd != WASHER_OFF)
        {
            event = EV_CMD_WASHER;
        }
    }

    if (current_state == ST_WIPER_TIMER && timer_counter >= TIMER_2SEC)
    {
        printf("DEBUG: FSM - 2-second timer expired\n");
        event = EV_TIMER_2SEC;
    }

    return event;
}

void fsm_windshield_washer_init(void)
{
    printf("INFO: FSM - Initializing windshield washer FSM\n");
    current_state = ST_ALL_OFF;
    last_wiper_cmd = WIPER_OFF;
    last_washer_cmd = WASHER_OFF;
    timer_counter = 0;
    handle_all_off();
}

void fsm_windshield_washer_run(void)
{
    uint32_t i;
    fsm_event_t event;

    event = get_next_event(current_state);

    if (event != EV_NONE)
    {
        printf("DEBUG: FSM - State: %s, Event: %s\n",
               fsm_get_state_name(current_state),
               fsm_get_event_name(event));
    }

    for (i = 0; i < TRANS_COUNT; i++)
    {
        if ((current_state == trans[i].state) || (ST_ANY == trans[i].state))
        {
            if ((event == trans[i].event) || (EV_ANY == trans[i].event))
            {
                fsm_state_t next_state = trans[i].next_state;
                if (event != EV_NONE)
                {
                    printf("INFO: FSM - Transition: %s -> %s\n",
                           fsm_get_state_name(current_state),
                           fsm_get_state_name(next_state));
                }
                current_state = next_state;
                if (trans[i].callback != NULL)
                {
                    trans[i].callback();
                }
                break;
            }
        }
    }
}