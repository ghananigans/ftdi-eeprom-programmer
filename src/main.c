#include <stdio.h>
#include <stdlib.h>
#include <ftd2xx/bin/ftd2xx.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) fprintf x
#else
#define DEBUG_PRINT(x) /* DEBUG MODE NOT ENABLED */
#endif

int
main (
    int argc,
    char ** argv
    )
{
    FT_STATUS ft_status;
    FT_HANDLE ft_handle;
    DWORD num_devices;

    fprintf(stdout, "FTDI Reprogrammer\n");

    ft_status = FT_SetVIDPID(0x0403, 0xA6D0);
    if (!FT_SUCCESS(ft_status))
    {
        fprintf(stdout, "[%s:%d] Failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    ft_status = FT_CreateDeviceInfoList(&num_devices);
    if (!FT_SUCCESS(ft_status))
    {
        fprintf(stdout, "[%s:%d] Failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(stdout, "Number of devices: %d\n", num_devices);


    return 0;
}
