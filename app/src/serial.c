/**
 * \file serial.c
 * \brief Serial shared read/write buffers.
 * \author Raphael CAUSSE
 */

/***** Includes **************************************************************/

#include <stdio.h>
#include "serial.h"

/***** Extern Variables ******************************************************/

serial_frame_t serial_buffer_read[DRV_MAX_FRAMES] = {0};
serial_frame_t serial_buffer_write[DRV_MAX_FRAMES] = {0};

/***** Functions *************************************************************/

void serial_buffer_dump(const serial_frame_t *serial_buffer)
{
    for (size_t i = 0; i < DRV_MAX_FRAMES; i++)
    {
        printf("Serial Channel %lu\n", i);
        printf("  serNum: %u\n", serial_buffer[i].serNum);
        printf("  frame: [ ");
        for (size_t j = 0; j < SER_MAX_FRAME_SIZE; j++)
        {
            printf(" %02X", serial_buffer[i].frame[j]);
        }
        printf(" ]\n");
        printf("  frameSize: %lu\n", serial_buffer[i].frameSize);
    }
    printf("\n");
}