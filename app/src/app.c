/**
 * \file app.c
 * \brief Entry point of the application.
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include <stdio.h>

#include "drv_api.h"
#include "bcgv_api.h"
#include "mux.h"
#include "fsm_windshield_washer.h"

int main(void)
{
    int32_t ret;
    int32_t driver_fd;
    uint8_t udp_frame[DRV_UDP_100MS_FRAME_SIZE];

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

    /* Initialize FSM */
    fsm_windshield_washer_init();
    printf("INFO: FSM initialized\n");

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

        /* Decode frame and update application data */
        mux_decode(udp_frame);

        /* Update FSM */
        fsm_windshield_washer_update();

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