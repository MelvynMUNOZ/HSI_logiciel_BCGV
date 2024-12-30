/**
 * \file fsm_indicators.h
 * \brief Finite state machine for indicators (hazard, left and right).
 * \details Handle states, events and transitions for the FSM.
 * \author Raphael CAUSSE
 */

#ifndef FSM_INDICATORS_H
#define FSM_INDICATORS_H

/**
 * \brief Run the indicators FSM.
 * \return int : Status of transition callback.
 */
int fsm_indicators_run(void);

#endif /* FSM_INDICATORS_H */