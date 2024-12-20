/**
 * \file        mutex.h
 * \brief       Mutex coding and decoding functions declaraations
 * \details
 * \author      Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef MUTEX_H
#define MUTEX_H

#include "drv_api.h"
#include "bcgv_api.h"

/**
 * \brief Decode UDP frame from MUX and update application data
 * \param frame Pointer to the UDP frame buffer
 */
void mux_decode(const uint8_t *frame);

#endif // MUTEX_H