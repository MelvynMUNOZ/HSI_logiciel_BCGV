/**
 * \file comodo.h
 * \brief Header file for COMODO handling functions.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef COMODO_H
#define COMODO_H

#include <stdint.h>
#include <stdbool.h>
#include "bcgv_api.h"

#define COMODO_SERIAL_FRAME_SIZE 1

/**
 * \brief Decode the COMODO serial frame and update application data.
 * \param frame Pointer to the frame buffer.
 * \return bool true if the frame was successfully decoded, false otherwise.
 */
bool comodo_decode(const uint8_t frame[COMODO_SERIAL_FRAME_SIZE]);

/**
 * \brief Get the value of a specific bit from a byte.
 * \param byte The byte from which to extract the bit.
 * \param bit The bit position (0-7) to extract.
 * \return bool true if the bit is set, false otherwise.
 */
bool get_bit(uint8_t byte, uint8_t bit);

#endif // COMODO_H
