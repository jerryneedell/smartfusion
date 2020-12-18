

#ifndef ETHERNET_STATUS_H
#define ETHERNET_STATUS_H

#include "mss_ethernet_mac.h"

typedef struct ethernet_status
{
    mss_mac_speed_t     speed;
    uint8_t         duplex_mode;
} ethernet_status_t;

#endif
