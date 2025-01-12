/**
 * \file fsm_windshield.h
 * \brief Finite State Machine for windshield washer and wiper control
 * \details Header file containing FSM states, events and function declarations
 * \author Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef FSM_WINDSHIELD_H
#define FSM_WINDSHIELD_H

/***** Functions *************************************************************/

/**
 * \brief Run the windshield wipers and washer FSM
 * \return int Status of transition callback (-1 for error, 0 for success)
 */
int fsm_windshield_run(void);

#endif /* FSM_WINDSHIELD_H */
