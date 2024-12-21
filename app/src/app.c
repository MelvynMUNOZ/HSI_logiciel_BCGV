/**
 * \file app.c
 * \brief Entry point of the application.
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include <stdio.h>

#include "drv_api.h"
#include "bcgv_api.h"
#include "comodo.h"

int main(void)
{
    int32_t ret;
    int32_t driver_fd;
    uint8_t udp_frame[DRV_UDP_100MS_FRAME_SIZE];
    uint8_t comodo_frame[COMODO_SERIAL_FRAME_SIZE];

    /***** Starting application *****/

    printf("INFO: Starting application\n");

    driver_fd = drv_open();
    if (driver_fd == DRV_ERROR)
    {
        printf("ERROR: failed to open driver\n");
        return EXIT_FAILURE;
    }
    else if (driver_fd == DRV_VER_MISMATCH)
    {
        printf("ERROR: driver version mismatch\n");
        return EXIT_FAILURE;
    }

    printf("INFO: driver opened\n");

    /* bcgv_init(); */
    // printf("INFO: BCGV initialized\n");

    /***** Main loop *****/

    printf("INFO: Entering main loop\n");
    while (1)
    {
        ret = drv_read_udp_100ms(driver_fd, udp_frame);
        if (ret == DRV_ERROR)
        {
            printf("ERROR: failed to read UDP frame 100ms\n");
            continue;
        }

        printf("\nDEBUG: Received new UDP frame:\n");
        printf("Raw data: ");
        for (size_t i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++)
        {
            printf("%02X ", udp_frame[i]);
        }
        printf("\n\n");

        /* Decode and display comodo frame */
        comodo_frame[0] = udp_frame[0]; // Example extraction logic, adjust as needed
        if (comodo_decode(comodo_frame))
        {
            printf("DEBUG: Successfully decoded COMODO frame.\n");
        }
        else
        {
            printf("ERROR: Failed to decode COMODO frame.\n");
        }

        printf("\n----------------------------------------\n");
    }

    /***** Closing application *****/

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        printf("ERROR: failed to close driver\n");
        return EXIT_FAILURE;
    }

    printf("INFO: driver closed\n");

    return EXIT_SUCCESS;
}
