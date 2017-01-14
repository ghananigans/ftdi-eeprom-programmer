#ifndef __FTDI_H__
#define __FTDI_H__

#include <stdint.h>
#include <ftd2xx/bin/ftd2xx.h>

FT_STATUS
ftdi_list_devices (
    void
    );

FT_STATUS
ftdi_configure_vid_pid (
    uint16_t vid,
    uint16_t pid
    );

FT_STATUS
ftdi_open (
    int index,
    FT_HANDLE * ft_handle
    );

FT_STATUS
ftdi_read_eeprom (
    FT_HANDLE * ft_handle
    );


#endif
