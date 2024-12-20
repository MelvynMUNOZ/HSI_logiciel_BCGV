/**
 * \file        fsm_classic_light.c
 * \author      Melvyn MUNOZ
 * \date        2024-12-16
 * \brief       This is a template file to create a Finite State Machine.
 */

#include "bcgv_api.h"
#include "fsm_lights.h"

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
    EV_POSITION_ON = 1,
    EV_POSITION_OFF = 2,
    EV_POSITION_ON_ACK = 3,
    EV_CROSSING_ON = 4,
    EV_CROSSING_OFF = 5,
    EV_CROSSING_ON_ACK = 6,
    EV_HIGHBEAM_ON = 7,
    EV_HIGHBEAM_OFF = 8,
    EV_HIGHBEAM_ON_ACK = 9,
    EV_ERR = 255 /* Error event */
} fsm_event_t;

/* Callback functions called on transitions */

// ALL LIGHTS

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
}

// POSITION LIGHT

/**
 * \brief   Change position light to true
 */
static int callback_position_ON(void)
{
    set_flag_position_light(true);
}
/**
 * \brief   Change position light to false
 */
static int callback_position_OFF(void)
{
    set_flag_position_light(false);
}

static int callback_position_ON_ACK(void)
{
    // TODO
}

// CROSSING LIGHT

/**
 * \brief   Change crossing light to true
 */
static int callback_crossing_ON(void)
{
    set_flag_crossing_light(true);
}
/**
 * \brief   Change crossing light to false
 */
static int callback_crossing_OFF(void)
{
    set_flag_crossing_light(false);
}

static int callback_crossing_ON_ACK(void)
{
    // TODO
}

// HIGHBEAM LIGHT

/**
 * \brief   Change highbeam light to true
 */
static int callback_highbeam_ON(void)
{
    set_flag_highbeam_light(true);
}
/**
 * \brief   Change highbeam light to false
 */
static int callback_highbeam_OFF(void)
{
    set_flag_highbeam_light(false);
}

static int callback_highbeam_ON_ACK(void)
{
    // TODO
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

    {ST_ALL_OFF, EV_POSITION_ON, &callback_position_ON, ST_ONE_ON},
    {ST_ONE_ON, EV_POSITION_ON_ACK, &callback_position_ON_ACK, ST_ONE_ON_ACK},
    {ST_ONE_ON, EV_POSITION_OFF, &callback_init, ST_ALL_OFF},
    {ST_ONE_ON_ACK, EV_POSITION_OFF, &callback_init, ST_ALL_OFF},

    {ST_ALL_OFF, EV_CROSSING_ON, &callback_crossing_ON, ST_ONE_ON},
    {ST_ONE_ON, EV_CROSSING_ON_ACK, &callback_crossing_ON_ACK, ST_ONE_ON_ACK},
    {ST_ONE_ON, EV_CROSSING_OFF, &callback_init, ST_ALL_OFF},
    {ST_ONE_ON_ACK, EV_CROSSING_OFF, &callback_init, ST_ALL_OFF},

    {ST_ALL_OFF, EV_HIGHBEAM_ON, &callback_highbeam_ON, ST_ONE_ON},
    {ST_ONE_ON, EV_HIGHBEAM_ON_ACK, &callback_highbeam_ON_ACK, ST_ONE_ON_ACK},
    {ST_ONE_ON, EV_HIGHBEAM_OFF, &callback_init, ST_ALL_OFF},
    {ST_ONE_ON_ACK, EV_HIGHBEAM_OFF, &callback_init, ST_ALL_OFF},

    {ST_ONE_ON, EV_ERR, &FsmError, ST_TERM},
    {ST_ANY, EV_ERR, &FsmError, ST_TERM}

};

#define TRANS_COUNT (sizeof(trans) / sizeof(*trans))

/**
 * \brief Verified is all command light are OFF
 *
 * \param cmd_position_light
 * \param cmd_crossing_light
 * \param cmd_highbeam_light
 * \return boolean
 */
bool is_all_light_OFF(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_highbeam_light)
{
    return cmd_position_light == 0 && cmd_crossing_light == 0 && cmd_highbeam_light == 0;
}
/**
 * \brief Verified is only position light is ON
 *
 * \param cmd_position_light
 * \param cmd_crossing_light
 * \param cmd_highbeam_light
 * \return boolean
 */
bool is_position_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_highbeam_light)
{
    return cmd_position_light == 1 && cmd_crossing_light == 0 && cmd_highbeam_light == 0;
}
/**
 * \brief Verified is only crossing light is ON
 *
 * \param cmd_position_light
 * \param cmd_crossing_light
 * \param cmd_highbeam_light
 * \return boolean
 */
bool is_crossing_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_highbeam_light)
{
    return cmd_position_light == 0 && cmd_crossing_light == 1 && cmd_highbeam_light == 0;
}
/**
 * \brief Verified is only crossing light is ON
 *
 * \param cmd_position_light
 * \param cmd_crossing_light
 * \param cmd_highbeam_light
 * \return boolean
 */
bool is_highbeam_light_ON(cmd_light_t cmd_position_light, cmd_light_t cmd_crossing_light, cmd_light_t cmd_highbeam_light)
{
    return cmd_position_light == 0 && cmd_crossing_light == 0 && cmd_highbeam_light == 1;
}

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
    cmd_light_t cmd_position_light = get_cmd_position_light();
    cmd_light_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_light_t cmd_highbeam_light = get_cmd_highbeam_light();

    flag_t flag_position_light = get_flag_position_light();
    flag_t flag_crossing_light = get_flag_crossing_light();
    flag_t flag_highbeam_light = get_flag_highbeam_light();

    /* Build all the events */

    switch (current_state)
    {
    case ST_ALL_OFF:
        if (is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_NONE;
        }
        else if (is_position_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_POSITION_ON;
        }
        else if (is_crossing_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_CROSSING_ON;
        }
        else if (is_highbeam_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_HIGHBEAM_ON;
        }
        break;
    case ST_ONE_ON:
        if (is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            if (flag_position_light == true)
            {
                event = EV_POSITION_OFF;
            }
            else if (flag_crossing_light == true)
            {
                event = EV_CROSSING_OFF;
            }
            else if (flag_highbeam_light == true)
            {
                event = EV_HIGHBEAM_OFF;
            }
        }
        else if (flag_position_light == true && is_position_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_POSITION_ON;
        }
        else if (flag_crossing_light == true && is_crossing_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_CROSSING_ON;
        }
        else if (flag_highbeam_light == true && is_highbeam_light_ON(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            event = EV_HIGHBEAM_ON;
        }
        else
        {
            event = EV_ERR;
        }
        break;
    case ST_ONE_ON_ACK:
        if (is_all_light_OFF(cmd_position_light, cmd_crossing_light, cmd_highbeam_light))
        {
            if (flag_position_light == true)
            {
                event = EV_POSITION_OFF;
            }
            else if (flag_crossing_light == true)
            {
                event = EV_CROSSING_OFF;
            }
            else if (flag_highbeam_light == true)
            {
                event = EV_HIGHBEAM_OFF;
            }
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
