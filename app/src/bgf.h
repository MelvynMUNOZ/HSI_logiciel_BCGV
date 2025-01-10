/**
 * \file        bgf.h
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF.
 */

#ifndef BGF_H
#define BGF_H

#include <stdint.h>
#include "bcgv_api.h"

/**
 * \brief Read the serial data of the driver, set the bit ack if there are differents values
 *
 * \param drvFd : Fd driver
 * \return boolean : the execution of the function is correct
 */
bool bgf_read_serial_message(int32_t drvFd);

/**
 * \brief Check if a flag is different that the precedent flag, in this case, write a message to bgf
 *
 * \param drvFd : Fd driver
 * \return boolean : the execution of the function is correct
 */
bool bgf_write_serial_message(int32_t drvFd);

#endif /* BGF_H */
