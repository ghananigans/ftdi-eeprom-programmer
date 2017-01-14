#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ftdi.h"

#ifdef DEBUG
#define FTDI_DEBUG_PRINT(x) fprintf x
#else
#define FTDI_DEBUG_PRINT(x) /* DEBUG MODE DISABLED */
#endif

static
FT_STATUS
ftdi_read_and_program_eeprom (
    FT_HANDLE * ft_handle,
    uint8_t read_only,
    uint16_t new_vid,
    uint16_t new_pid,
    char * new_description
    );

//
// List all devices with matching VID and PID as set earlier
//
FT_STATUS
ftdi_list_devices (
    DWORD * num_devices
    )
{
    FT_STATUS ft_status;
    FT_DEVICE_LIST_INFO_NODE * device_infos;

    assert(num_devices);

    ft_status = FT_CreateDeviceInfoList(num_devices);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
        return ft_status;
    }

    fprintf(stdout, "Number of devices: %d\n", *num_devices);

    if (*num_devices > 0) {
        // allocate storage for list based on numDevs
        device_infos = (FT_DEVICE_LIST_INFO_NODE *) malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) * (*num_devices));

        // get the device information list
        ft_status = FT_GetDeviceInfoList(device_infos, num_devices);
        if (!FT_SUCCESS(ft_status))
        {
            FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
            return ft_status;
        }

        for (int i = 0; i < *num_devices; i++) {
            printf("Dev %d:\n",i);
            printf(" Flags=0x%x\n", device_infos[i].Flags);
            printf(" Type=0x%x\n", device_infos[i].Type);
            printf(" ID=0x%x\n", device_infos[i].ID);
            printf(" LocId=0x%x\n", device_infos[i].LocId);
            printf(" SerialNumber=%s\n", device_infos[i].SerialNumber);
            printf(" Description=%s\n", device_infos[i].Description);
            printf(" ftHandle=%p\n", device_infos[i].ftHandle);
        }

        free(device_infos);
    }

    return ft_status;
}

//
// Set VID and PID so the D2XX driver can find matching
// devices (if programming eeprom, use current vid and pid)
//
FT_STATUS
ftdi_configure_vid_pid (
    uint16_t vid,
    uint16_t pid
    )
{
    FT_STATUS ft_status;

    ft_status = FT_SetVIDPID(vid, pid);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
    }

    return ft_status;
}

//
// Get a handle to an device
//
FT_STATUS
ftdi_open (
    int index,
    FT_HANDLE * ft_handle
    )
{
    FT_STATUS ft_status;

    ft_status = FT_Open(index, ft_handle);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
    }

    return ft_status;
}

//
// Read the eeprom
//
FT_STATUS
ftdi_read_eeprom (
    FT_HANDLE * ft_handle
    )
{
    return ftdi_read_and_program_eeprom(ft_handle, 1, 0, 0, 0);
}

//
// Read and Program the eeprom
//
FT_STATUS
ftdi_program_eeprom (
    FT_HANDLE * ft_handle,
    uint16_t new_vid,
    uint16_t new_pid,
    char * new_description
    )
{
    return ftdi_read_and_program_eeprom(ft_handle, 0, new_vid, new_pid, new_description);
}

//
// This function will read and/or program the eeprom
//
static
FT_STATUS
ftdi_read_and_program_eeprom (
    FT_HANDLE * ft_handle,
    uint8_t read_only,
    uint16_t new_vid,
    uint16_t new_pid,
    char * new_description
    )
{
    FT_STATUS ft_status;
    char manufacturer[64];
    char manufacturer_id[64];
    char description[64];
    char serial_number[64];

    FT_PROGRAM_DATA ft_prog_data;
    ft_prog_data.Signature1 = 0x0;
    ft_prog_data.Signature2 = 0xffffffff;
    ft_prog_data.Version = FT_DEVICE_232R;
    ft_prog_data.Manufacturer = manufacturer;
    ft_prog_data.ManufacturerId = manufacturer_id;
    ft_prog_data.Description = description;
    ft_prog_data.SerialNumber = serial_number;

    //
    // Do a read of the eeprom.
    // If programming, we need this so when programming,
    // fields other than the vid, pid and string description are
    // untouched (Read before Write) since this function can't write to
    // JUST the pid, vid and string description
    //
    ft_status = FT_EE_Read(*ft_handle, &ft_prog_data);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
        return ft_status;
    }

    fprintf(stdout, "Manufacturer: %s\n", manufacturer);
    fprintf(stdout, "ManufacturerId: %s\n", manufacturer_id);
    fprintf(stdout, "Description: %s\n", description);
    fprintf(stdout, "Serial Number: %s\n", serial_number);
    fprintf(stdout, "VID: 0x%x\n", ft_prog_data.VendorId);
    fprintf(stdout, "PID: 0x%x\n", ft_prog_data.ProductId);

    if (read_only)
    {
        // Read the eeprom only.
        return ft_status;
    }

    // New Vendor Id and Product ID
    if (new_vid)
    {
        ft_prog_data.VendorId = new_vid;
    }

    if (new_pid)
    {
        ft_prog_data.ProductId = new_pid;
    }

    if (new_description)
    {
        ft_prog_data.Description = new_description;
    }

    //
    // Program the prom
    //
    ft_status = FT_EE_Program(*ft_handle, &ft_prog_data);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
    }


    return ft_status;
}
