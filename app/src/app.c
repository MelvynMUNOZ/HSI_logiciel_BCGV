/**
 * \file app.c
 * \brief Entry point of the application.
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "drv_api.h"
#include "bcgv_api.h"

int main(void)
{
    int32_t ret;
    int32_t driver_fd;
    uint8_t udp_frame[DRV_UDP_100MS_FRAME_SIZE];

    /***** Starting application *****/

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
            printf("ERROR: failed to read upd frame 100ms\n");
            continue;
        }

        printf("MUX > [");
        for (size_t i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++)
        {
            printf("%c", udp_frame[i]); // TODO: decode frame received from MUX. Now just prints as char each byte.
        }
        printf("]\n");
    }

    /***** Closing application *****/

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        printf("ERROR: failed to close driver\n");
        return EXIT_FAILURE;
    }

    printf("INFO: driver closed");

    return EXIT_SUCCESS;
}