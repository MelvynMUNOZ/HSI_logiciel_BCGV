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

    bcgv_ctx_init();

    /***** Main loop *****/

    while (1)
    {
        ret = drv_read_udp_100ms(driver_fd, udp_frame);
        if (ret == DRV_ERROR)
        {
            printf("ERROR: failed to read udp frame 100ms\n");
            continue;
        }

        /* TMP: print each byte for now */
        printf("MUX > [");
        for (size_t i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++)
        {
            printf("%02X ", udp_frame[i]);
        }
        printf("]\n");

        /* TODO: real implementation */
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
