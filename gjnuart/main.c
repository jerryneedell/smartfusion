/*******************************************************************************
 * (c) Copyright 2009-2015 Microsemi Corporation.  All rights reserved.
 *
 *  Simple SmartFusion microcontroller subsystem (MSS) GPIO example program.
 *  This sample program is targeted at the SmartFusion Advanced Development Kit.
 *  It blinks the development board's LEDs using a delay loop.
 *
 *  Please read the README.txt file in the root folder of this project to know more. 
 *
 *
 * SVN $Revision: 7838 $
 * SVN $Date: 2015-09-17 18:20:04 +0530 (Thu, 17 Sep 2015) $
 */
#include <stdio.h>
#include <stdlib.h>
#include "a2fxxxm3.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_watchdog/mss_watchdog.h"
#include "drivers/mss_uart/mss_uart.h"
/*
 * Delay loop down counter load value.
 */
#define DELAY_LOAD_VALUE     0x00100000
//#define ACTEL_STDIO_THRU_UART  1
/*-------------------------------------------------------------------------*//**
 * main() function.
 */
int main()
{
    volatile uint32_t *p_led = (volatile uint32_t *)0x40050000;
    //volatile int32_t delay_count = 0;
    //uint8_t message[12] = "Hello world";
    uint8_t key;
    uint8_t                               rx_size =0;

    /* Initialize and configure UART0. */
        MSS_UART_init
        (
            &g_mss_uart0,
          MSS_UART_57600_BAUD,
          MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
        );




    printf("blinky time\r\n");
    //MSS_UART_polled_tx
    //            ( &g_mss_uart0,
    //              message, sizeof(message)
    //            );


    /*
      * Disable watchdog.
      */
     MSS_WD_disable();


    /*
      * Initialize MSS GPIOs.
      */
     MSS_GPIO_init();
    /*
     * Configure MSS GPIOs.
     */
    //MSS_GPIO_config( MSS_GPIO_0 , MSS_GPIO_OUTPUT_MODE );
    //MSS_GPIO_config( MSS_GPIO_1 , MSS_GPIO_OUTPUT_MODE );
    //MSS_GPIO_config( MSS_GPIO_2 , MSS_GPIO_OUTPUT_MODE );
    //MSS_GPIO_config( MSS_GPIO_3 , MSS_GPIO_OUTPUT_MODE );
    
    *p_led = 0xf;
    /*
     * Infinite loop.
     */
    for(;;)
    {

        do
        {
            rx_size = MSS_UART_get_rx(&g_mss_uart0, &key, 1);
        }while(rx_size == 0);

        rx_size = 0;

            *p_led ^= 0xf;
            printf("we are printing - %c \r\n",key);
            if(key == '1')
            {
                printf("got a 1\r\n");
            }
            if(key == '2')
            {
                printf("got a 2\r\n");
            }

    }
    
    return 0;
}
