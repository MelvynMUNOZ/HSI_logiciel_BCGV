#include <stdio.h>
#include "drv_api.h"

int main(void)
{
    int32_t ret;
    int32_t driver_fd;

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

    while (1)
    {
    }

    ret = drv_close(driver_fd);
    if (ret == DRV_ERROR)
    {
        printf("ERROR: failed to close driver\n");
        return EXIT_FAILURE;
    }
    else
    {
        printf("INFO: driver closed");
    }

    return EXIT_SUCCESS;
}