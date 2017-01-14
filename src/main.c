#include <stdio.h>
#include <stdint.h>

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
    DWORD new_vid;
    DWORD new_pid;
    DWORD current_vid;
    DWORD current_pid;
    char * new_description;
    int ftdi_open_index;
    DWORD num_devices;

    fprintf(stdout, "FTDI Reprogrammer\n");

    //
    // TODO: Get user input for this.
    //
    current_vid = 0x0403; // Current VID of device
    current_pid = 0x6002; // Current PID of device
    new_vid = 0x0403;     // New VID of device
    new_pid = 0x6001;     // New PID of device
    new_description = (char *) "USB <-> Serial Converter\0"; // New USB String description

    if (!FT_SUCCESS(ftdi_configure_vid_pid(current_vid, current_pid)))
    {
        fprintf(stderr, "Failed to configure VID and PID\n");
        return -1;
    }

    if (!FT_SUCCESS(ftdi_list_devices(&num_devices)))
    {
        fprintf(stderr, "Failed to list devices\n");
        return -1;
    }

    if (num_devices < 1)
    {
        fprintf(stdout, "No devices to program\n");
        return 0;
    }

    //
    // TODO: Get user input for this.
    //
    ftdi_open_index = 0;

    if (!FT_SUCCESS(ftdi_open(ftdi_open_index, &ft_handle)))
    {
        fprintf(stderr, "Failed to open handle\n");
        return -1;
    }

    fprintf(stdout, "Press any key to program eeprom of device #%d...", ftdi_open_index);
    getchar();

    fprintf(stdout, "Before Programming:\n");
    if (!FT_SUCCESS(ftdi_program_eeprom(&ft_handle, new_vid, new_pid, new_description)))
    {
        fprintf(stderr, "Failed to read eeprom\n");
        return -1;
    }

    fprintf(stdout, "\nAfter Proramming:\n");
    if (!FT_SUCCESS(ftdi_read_eeprom(&ft_handle)))
    {
        fprintf(stderr, "Failed to read eeprom\n");
        return -1;
    }

    return 0;
}
