/**
 * \file mux.h
 * \brief
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef MUX_H
#define MUX_H

#include "drv_api.h"

/**
 * \brief Decode UDP frame from MUX and update application data
 * \param frame : Pointer to the UDP frame buffer
 * \return bool : true if the frame was successfully decoded, false otherwise
 */
bool mux_decode(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE]);

#endif /* MUX_H */