/**
 * \file crc8.h
 * \brief CRC-8 checksum computation using a lookup table.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef CRC8_H
#define CRC8_H

#include <stdint.h>
#include <stddef.h>

#define CRC8_INIT (0x00)

/**
 * \brief Compute CRC-8 checksum for a given data buffer.
 * \param data : Pointer to the input data buffer.
 * \param length : Number of bytes in the data buffer.
 * \return uint8_t : The computed CRC-8 checksum.
 */
uint8_t crc8_compute(const uint8_t *data, size_t length);

/**
 * \brief Update an existing CRC-8 checksum with a new byte.
 * \param current_crc : Current CRC-8 checksum value.
 * \param new_byte : New byte to include in the checksum.
 * \return uint8_t : Updated CRC-8 checksum.
 */
uint8_t crc8_update(uint8_t current_crc, uint8_t new_byte);

#endif /* CRC8_H */