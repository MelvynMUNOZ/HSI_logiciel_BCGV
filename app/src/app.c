/**
 * \file app.c
 * \brief Entry point of the application.
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "drv_api.h"
#include "bcgv_api.h"
#include "mux.h"
#include "log.h"

int main(void)
{
    bool quit = false;
    bool success = false;
    int32_t ret = 0;
    int32_t driver_fd = 0;
    uint8_t mux_frame[DRV_UDP_100MS_FRAME_SIZE] = {0};

    /***** Starting application *****/

    driver_fd = drv_open();
    if (driver_fd == DRV_ERROR)
    {
        log_error("failed to open driver", NULL);
        return EXIT_FAILURE;
    }
    else if (driver_fd == DRV_VER_MISMATCH)
    {
        log_error("driver version mismatch", NULL);
        return EXIT_FAILURE;
    }
    log_info("driver opened", NULL);

    bcgv_ctx_init();

    /***** Main loop *****/

    while (quit == false)
    {
        /* Receive and decode frame from MUX */
        success = mux_read_frame_100ms(driver_fd, mux_frame);
        if (success == false)
        {
            continue; /* Behaviour to define: should we exit ? continue to next frame forever */
        }
        mux_check_frame_number(mux_frame);
        success = mux_decode_frame(mux_frame);
        if (success == false)
        {
            continue;
        }

        /* DEBUG */
        printf("====================");
        mux_print_raw(mux_frame);
        mux_print_decoded();
        printf("====================");
        /* END DEBUG*/

        /* TODO: other implementations */
    }

    /***** Closing application *****/

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        log_error("failed to close driver", NULL);
        return EXIT_FAILURE;
    }
    log_info("driver closed", NULL);

    return EXIT_SUCCESS;
}
