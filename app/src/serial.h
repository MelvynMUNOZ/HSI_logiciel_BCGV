/**
 * \file serial.h
 * \brief Serial shared read/write buffers.
 * \author Raphael CAUSSE
 */

#ifndef SERIAL_H
#define SERIAL_H

/***** Includes **************************************************************/

#include "drv_api.h"

/***** Extern Variables ******************************************************/

extern serial_frame_t serial_buffer_read[DRV_MAX_FRAMES];
extern serial_frame_t serial_buffer_write[DRV_MAX_FRAMES];

/***** Functions *************************************************************/

/**
 * \brief Dump serial buffer
 * \param serial_buffer : Serial buffer to dump
 */
void serial_buffer_dump(const serial_frame_t *serial_buffer);

#endif /* SERIAL_H */