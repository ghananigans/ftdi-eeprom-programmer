#include <stdio.h>
#include <stdlib.h>
#include "ftdi.h"

#ifdef DEBUG
#define FTDI_DEBUG_PRINT(x) fprintf x
#else
#define FTDI_DEBUG_PRINT(x) /* DEBUG MODE DISABLED */
#endif

FT_STATUS
ftdi_list_devices (
    void
    )
{
    FT_STATUS ft_status;
    DWORD num_devices;
    FT_DEVICE_LIST_INFO_NODE * device_infos;

    ft_status = FT_CreateDeviceInfoList(&num_devices);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
        return ft_status;
    }

    fprintf(stdout, "Number of devices: %d\n", num_devices);

    if (num_devices > 0) {
        // allocate storage for list based on numDevs
        device_infos = (FT_DEVICE_LIST_INFO_NODE *) malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) * num_devices);

        // get the device information list
        ft_status = FT_GetDeviceInfoList(device_infos, &num_devices);
        if (!FT_SUCCESS(ft_status))
        {
            FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
            return ft_status;
        }

        for (int i = 0; i < num_devices; i++) {
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

FT_STATUS
ftdi_configure_vid_pid (
    uint16_t vid,
    uint16_t pid
    )
{
    FT_STATUS ft_status;

    ft_status = FT_SetVIDPID(0x0403, 0xA6D0);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
    }

    return ft_status;
}

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

FT_STATUS
ftdi_read_and_program_eeprom (
    FT_HANDLE * ft_handle
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

    ft_status = FT_EE_Read(*ft_handle, &ft_prog_data);
    if (!FT_SUCCESS(ft_status))
    {
        FTDI_DEBUG_PRINT((stderr, "[%s:%d] Failed with error code %d\n", __FUNCTION__, __LINE__, ft_status));
    }
    else
    {
        fprintf(stdout, "Manufacturer: %s\n", manufacturer);
        fprintf(stdout, "ManufacturerId: %s\n", manufacturer_id);
        fprintf(stdout, "Description: %s\n", description);
        fprintf(stdout, "Serial Number: %s\n", serial_number);
        fprintf(stdout, "VID: 0x%x\n", ft_prog_data.VendorId);
        fprintf(stdout, "PID: 0x%x\n", ft_prog_data.ProductId);
    }

    return ft_status;
}
