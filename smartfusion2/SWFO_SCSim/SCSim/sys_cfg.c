/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 *
 * SVN $Revision: 8059 $
 * SVN $Date: 2015-10-20 12:00:25 +0530 (Tue, 20 Oct 2015) $
 */
#include "m2sxxx.h"
#include "drivers/mss_ethernet_mac/mss_ethernet_mac.h"
#include "core_cm3.h"
#include "swfo_fpga.h"

#define PO_RESET_DETECT_MASK    0x00000001u
#define VALID_SPEED_CHOICE_KEY  0xB16B00B5u

#if defined(__GNUC__)
__attribute__ ((section (".noinit")))  static uint32_t g_ethernet_speed_choice;
__attribute__ ((section (".noinit")))  static uint32_t g_valid_choice_key;

#elif defined(__ICCARM__)
__no_init static uint32_t g_ethernet_speed_choice;
__no_init static uint32_t g_valid_choice_key;

#elif defined(__CC_ARM)
static uint32_t g_ethernet_speed_choice __attribute__((at(0x20000000)));  
static uint32_t g_valid_choice_key __attribute__((at(0x20000004)));
#endif


extern volatile unsigned long *fpgabase;


/*==============================================================================
 * 
 */
const uint8_t * sys_cfg_get_mac_address(void)
{
    static uint8_t mac_address[6];
    
    /* Create the MAC address. */
    uint8_t low_address;
    low_address = 128 + (fpgabase[SW5]&7)^7;  // get low 3 bis then XOR to invert bits

    mac_address[0] = 0xC0u;
    mac_address[1] = 0xB1u;
    mac_address[2] = 0x3Cu;
    mac_address[3] = 0x88u;
    mac_address[4] = 0x88u;
    mac_address[5] = low_address;
    
    return (const uint8_t *)mac_address;
}


/*==============================================================================
 * 
 */
uint32_t get_user_eth_speed_choice(void)
{
    uint32_t speed_choice;
    uint32_t power_on_reset;
    
    power_on_reset = SYSREG->RESET_SOURCE_CR & PO_RESET_DETECT_MASK;
    
    if(power_on_reset != 0u)
    {
        speed_choice = MSS_MAC_ANEG_ALL_SPEEDS;
        g_ethernet_speed_choice = MSS_MAC_ANEG_ALL_SPEEDS;
        g_valid_choice_key = VALID_SPEED_CHOICE_KEY;
        SYSREG->RESET_SOURCE_CR &= ~PO_RESET_DETECT_MASK;
    }
    else
    {
        if(VALID_SPEED_CHOICE_KEY == g_valid_choice_key)
        {
            speed_choice = g_ethernet_speed_choice;
        }
        else
        {
            speed_choice = MSS_MAC_ANEG_ALL_SPEEDS;
            g_ethernet_speed_choice = MSS_MAC_ANEG_ALL_SPEEDS;
            g_valid_choice_key = VALID_SPEED_CHOICE_KEY;
        }
    }
    
    return speed_choice;
}

