/**
 * \file        fsm_classic_light.c
 * \author      Melvyn MUNOZ
 * \date        2024-12-16
 * \brief       This is a template file to create a Finite State Machine.
 */

#include <stdlib.h>
#include "bcgv_api.h"
#include "fsm_lights.h"

#define TIMER_1S_COUNT_100MS (10)

/* States */
typedef enum
{
    ST_ANY = -1, /* Any state */
    ST_INIT = 0, /* Init state */
    ST_ALL_OFF = 1,
    ST_ONE_ON = 2,
    ST_ONE_ON_ACK = 3,
    ST_TERM = 255 /* Final state */
} fsm_state_t;

/* Events */
typedef enum
{
    EV_ANY = -1, /* Any event */
    EV_NONE = 0, /* No event */
    EV_CMD_ON = 1,
    EV_CMD_OFF = 2,
    EV_CMD_ON_ACK = 3,
    EV_ERR = 255 /* Error event */
} fsm_event_t;

/* Static variable */
static uint8_t timer_counter = 0;

/* Callback functions called on transitions */

/**
 * \brief    Initialise all light flags to false
 */
static int callback_init(void)
{
    set_flag_position_light(false);
    set_flag_crossing_light(false);
    set_flag_highbeam_light(false);
}
/**
 * \brief   Change position light to true, crossing light to true, highbeam light to true,
 */
static int FsmError(void)
{
    set_flag_position_light(false);
    set_flag_crossing_light(false);
    set_flag_highbeam_light(false);
    return -1;
}

/**
 * \brief   Change one type of light to true
 */
static int callback_cmd_ON(void)
{
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();

    if (cmd_crossing_light == true)
    {
        set_flag_position_light(true);
    }
    if (cmd_crossing_light == true)
    {
        set_flag_crossing_light(true);
    }
    if (cmd_highbeam_light == true)
    {
        set_flag_highbeam_light(true);
    }
}

/**
 * \brief   Change one type of light to false
 */
static int callback_cmd_OFF(void)
{
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();

    if (cmd_crossing_light == false)
    {
        set_flag_position_light(false);
    }
    if (cmd_crossing_light == false)
    {
        set_flag_crossing_light(false);
    }
    if (cmd_highbeam_light == false)
    {
        set_flag_highbeam_light(false);
    }

    timer_counter = 0;

    return 0;
}

/**
 * \brief   Callback ON ack
 */
static int callback_cmd_ON_ACK(void)
{
    return 0;
}

/**
 * \brief   Callback for waiting ON ack
 */
static int callback_cmd_ON_wait_ACK(void)
{
    timer_counter++;
}

/* Transition structure */
typedef struct
{
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    {ST_INIT, EV_NONE, &callback_init, ST_ALL_OFF},

    {ST_ALL_OFF, EV_CMD_ON, &callback_cmd_ON, ST_ONE_ON},
    {ST_ONE_ON, EV_CMD_OFF, &callback_cmd_OFF, ST_ALL_OFF},
    {ST_ONE_ON, EV_NONE, &callback_cmd_ON_wait_ACK, ST_ONE_ON},
    {ST_ONE_ON, EV_CMD_ON_ACK, &callback_cmd_ON_ACK, ST_ONE_ON_ACK},
    {ST_ONE_ON_ACK, EV_CMD_OFF, &callback_init, ST_ALL_OFF},

    {ST_ONE_ON, EV_ERR, &FsmError, ST_TERM},
    {ST_ANY, EV_ERR, &FsmError, ST_TERM}

};

#define TRANS_COUNT (sizeof(trans) / sizeof(*trans))

/**
 * \brief Get the next event object
 *
 * \param current_state
 * \return int
 */
int get_next_event(int current_state)
{
    int event = EV_NONE;

    /* Here, you can get the parameters of your FSM */
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();

    flag_t flag_position_light = get_flag_position_light();
    flag_t flag_crossing_light = get_flag_crossing_light();
    flag_t flag_highbeam_light = get_flag_highbeam_light();

    bit_flag_t bgf_ack = get_bit_flag_bgf_ack();

    bool position_ON = (cmd_crossing_light == true);
    bool crossing_ON = (cmd_crossing_light == true);
    bool highbeam_ON = (cmd_crossing_light == true);
    bool position_OFF = (cmd_crossing_light == false);
    bool crossing_OFF = (cmd_crossing_light == false);
    bool highbeam_OFF = (cmd_crossing_light == false);

    bool flag_position_ON = (flag_crossing_light == true);
    bool flag_crossing_ON = (flag_crossing_light == true);
    bool flag_highbeam_ON = (flag_crossing_light == true);
    bool flag_position_OFF = (flag_crossing_light == false);
    bool flag_crossing_OFF = (flag_crossing_light == false);
    bool flag_highbeam_OFF = (flag_crossing_light == false);

    /* Build all the events */

    switch (current_state)
    {
    case ST_ALL_OFF:
        if (position_ON || crossing_ON || highbeam_ON)
        {
            event = EV_CMD_ON;
        }
        break;
    case ST_ONE_ON:
        if (position_OFF && crossing_OFF && highbeam_OFF)
        {
            if (flag_position_ON || flag_crossing_ON || flag_highbeam_ON)
            {
                event = EV_CMD_OFF;
            }
        }
        else if (timer_counter < TIMER_1S_COUNT_100MS)
        {
            if (position_ON && flag_position_ON)
            {
                if (BGF_ACK_POSITION_LIGHT)
                {
                    event = EV_CMD_ON_ACK;
                }
            }
            else if (crossing_ON && flag_crossing_ON)
            {
                if (BGF_ACK_CROSSING_LIGHT)
                {
                    event = EV_CMD_ON_ACK;
                }
            }
            else if (highbeam_ON && flag_highbeam_ON)
            {
                if (BGF_ACK_HIGHBEAM_LIGHT)
                {
                    event = EV_CMD_ON_ACK;
                }
            }
        }
        else
        {
            event = EV_ERR;
        }
        break;
    case ST_ONE_ON_ACK:
        if (position_OFF && crossing_OFF && highbeam_OFF)
        {
            event = EV_CMD_OFF;
        }
        break;
    case ST_TERM:
        event = EV_ERR;
        break;
    }
    return event;
}

int fsm_lights_run(void)
{
    int i = 0;
    int ret = 0;
    int event = EV_NONE;
    int state = ST_INIT;

    /* While FSM hasn't reach end state */
    while (state != ST_TERM)
    {
        /* Get event */
        event = get_next_event(state);
        /* For each transitions */
        for (i = 0; i < TRANS_COUNT; i++)
        {
            /* If State is current state OR The transition applies to all states ...*/
            if ((state == trans[i].state) || (ST_ANY == trans[i].state))
            {
                /* If event is the transition event OR the event applies to all */
                if ((event == trans[i].event) || (EV_ANY == trans[i].event))
                {
                    /* Apply the new state */
                    state = trans[i].next_state;
                    if (trans[i].callback != NULL)
                    {
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
