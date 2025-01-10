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
#include "bgf.h"

int main(void)
{
    bool quit = false;
    bool success = false;
    int32_t ret = 0;
    int32_t driver_fd = 0;

    /***** Starting application *****/

    driver_fd = drv_open();
    if (driver_fd == DRV_ERROR)
    {
        log_error("error while opening driver", NULL);
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
        success = mux_read_frame_100ms(driver_fd);
        if (success == false)
        {
            continue; /* Behaviour to define: should we exit ? continue to next frame forever */
        }
        mux_check_frame_number();
        success = mux_decode_frame_100ms();
        if (success == false)
        {
            continue;
        }

        /* TODO: other implementations */
        success = bgf_read_serial_message(driver_fd);
        log_warn("success of bgf read : %d", success);
        if (success == false)
        {
            log_error("Error bgf : in bgf_read_serial_message", NULL);
            continue;
        }
    }

    /***** Closing application *****/

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        log_error("error while closing driver", NULL);
        return EXIT_FAILURE;
    }
    log_info("driver closed", NULL);

    return EXIT_SUCCESS;
}
