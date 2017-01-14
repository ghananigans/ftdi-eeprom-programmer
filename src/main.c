#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ftdi.h"

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
    FT_HANDLE ft_handle;

    fprintf(stdout, "FTDI Reprogrammer\n");

    if (!FT_SUCCESS(ftdi_configure_vid_pid(0x0403, 0x6001)))
    {
        fprintf(stderr, "Failed to configure VID and PID\n");
        return -1;
    }

    if (!FT_SUCCESS(ftdi_list_devices()))
    {
        fprintf(stderr, "Failed to list devices\n");
    }

    if (!FT_SUCCESS(ftdi_open(0, &ft_handle)))
    {
        fprintf(stderr, "Failed to open handle\n");
    }


    if (!FT_SUCCESS(ftdi_read_eeprom(&ft_handle)))
    {
        fprintf(stderr, "Failed to read eeprom\n");
    }
    return 0;
}
