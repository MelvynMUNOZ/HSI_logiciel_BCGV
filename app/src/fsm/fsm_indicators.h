/**
 * \file fsm_indicators.h
 * \brief Finite state machine for indicators (hazard, left and right).
 * \details
 * \author Raphael CAUSSE
 */

#ifndef FSM_INDICATORS_H
#define FSM_INDICATORS_H

/**
 * \brief Run the FSM for indicators.
 * \param state : Current state of the FSM.
 * \return int : New state after the FSM execution.
 */
int fsm_indicators_run(int state);

#endif /* FSM_INDICATORS_H */