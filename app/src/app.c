/**
 * \file app.c
 * \brief Entry point of the application.
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

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

    context_init(); // Initialize the context

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

    /***** Main loop *****/

    while (1)
    {
        ret = drv_read_udp_100ms(driver_fd, udp_frame);
        if (ret == DRV_ERROR)
        {
            printf("ERROR: failed to read udp frame 100ms\n");
            continue;
        }

        printf("MUX > [");
        for (size_t i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++)
        {
            printf("%c", udp_frame[i]); // Print each byte as a character for now
        }
        printf("]\n");

        // Assuming the COMODO frame is part of the UDP frame, extract and decode it
        comodo_frame[0] = udp_frame[0]; // Example extraction logic
        if (!comodo_decode(comodo_frame))
        {
            printf("ERROR: failed to decode COMODO frame\n");
        }

        // Decode the rest of the UDP frame and update application data
        if (!mux_decode(udp_frame))
        {
            printf("ERROR: failed to decode MUX frame\n");
        }
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
