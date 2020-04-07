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
#include "drivers/mss_watchdog/mss_watchdog.h"
#include "drivers/mss_timer/mss_timer.h"
#include "drivers/mss_uart/mss_uart.h"
/*
 * Delay loop down counter load value.
 */
#define DELAY_LOAD_VALUE     0x00100000
#define LED 0
#define COUNTER 0xA00000
volatile uint32_t *fpgabase = (volatile uint32_t *)0x40050000;

//#define ACTEL_STDIO_THRU_UART  1
/*-------------------------------------------------------------------------*//**
 * main() function.
 */
int main()
{
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

    /*
      * Disable watchdog.
      */
     MSS_WD_disable();
    // turn on the LEDS
    fpgabase[LED] = 0xff;
    MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
    MSS_TIM1_load_background(COUNTER);
    // start timer 1
    MSS_TIM1_start();
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

            if(key == '1')
            {
                printf("got a 1 - start timer\r\n");
                // start timer 1
                MSS_TIM1_start();
            }
            else if(key == '2')
            {
                printf("got a 2 - stop timer\r\n");
                // stop timer 1
                MSS_TIM1_stop();
            }
            else if(key == '3')
            {
                printf("got a 3 - enable irq\r\n");
                // enable timer 1 irq
                MSS_TIM1_enable_irq();
            }
            else if(key == '4')
            {
                printf("got a 4 - disable irq\r\n");
                // disable timer 1 irq
                MSS_TIM1_disable_irq();
            }
            else
            {
                printf("key received - %c \r\n",key);
            }
    }
    

    return 0;
}



// Timer1 ISR
void Timer1_IRQHandler(void)
{
    // pulse counters
    fpgabase[0x4080/4] = 0x1fffff;
    // toggle LEDs
    fpgabase[0] ^= 0xff;
    MSS_TIM1_clear_irq();
}

