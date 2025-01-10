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
 * \brief Read UDP 100ms frame (blocking call).
 * \details Wait for next UDP 100ms frame and returns it.
 * \param drv_fd : File descriptor of channel with driver (get with open)
 * \param frame : Pointer to the MUX frame buffer
 * \return bool :
 */
bool mux_read_frame_100ms(int32_t drv_fd);

/**
 * \brief
 * \details
 * \param drv_fd
 * \param frame
 * \return bool :
 */
bool mux_write_frame_200ms(int32_t drv_fd);

/**
 * \brief Check for a valid frame number in the frame.
 * \details If there the frame number is not equal to the expected frame number (incremental and looping at 100), a warning is logged.
 */
void mux_check_frame_number();

/**
 * \brief Decode UDP frame from MUX and update application data.
 * \details Frame format (15 bytes, Big endian):
 *          - 1 byte (MSB): Frame number (1-100)
 *          - 4 bytes : Distance (km)
 *          - 1 byte : Speed (km/h)
 *          - 1 byte : Chassis issues
 *          - 1 byte : Motor issues
 *          - 1 byte : Fuel level (litres)
 *          - 4 bytes: Engine RPM
 *          - 1 byte (LSB): Battery issues
 *          - 1 byte : CRC8
 * \return bool : true if the frame crc8 is valid and frame has been decoded, false otherwise
 */
bool mux_decode_frame_100ms();

/**
 * \brief
 */
void mux_encode_frame_200ms();

/**
 * \brief Print raw bytes of MUX frame.
 * \param frame : Pointer to the MUX frame buffer
 */
void mux_print_raw(const uint8_t *frame, size_t length);

/**
 * \brief
 */
void mux_print_decoded(void);

#endif /* MUX_H */