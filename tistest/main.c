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
#define INT_2HZ 4
#define INTR 0x20/4
#define INTR_CLR 0x20/4
#define  FABRIC_INT_CTRL_BASE_ADDR 0x40051000
// enable Major Frame and Minor Frame Interrupt
#define  FABRIC_INTR_ENBL 0x00000004
#define  FABRIC_INTR_CLR 0xFFFFFFFF
#define  FABRIC_INTR_ENBL_OFF_ADDR 0x00000020
#define  FABRIC_INTR_ENBL_CLR_OFF_ADDR 0x00000024
#define  FABRIC_INTR_STAT_OFF_ADDR 0x0000002C


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


    // turn on the LEDS
    fpgabase[LED] = 0xff;
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
                printf("got a 1 - start interrupt\r\n");
                NVIC_ClearPendingIRQ( Fabric_IRQn );
                NVIC_EnableIRQ(Fabric_IRQn);
                fpgabase[INTR_CLR] = 0x4;

                /* Enabling Fabric Interrupt*/


               /* Configuring the Interrupt control register in FPGA fabric
               * This will enable the Interrupt in the FPGA*/
                (*((uint32_t volatile *)(FABRIC_INT_CTRL_BASE_ADDR + FABRIC_INTR_ENBL_OFF_ADDR)) = FABRIC_INTR_ENBL);

            }
            else if(key == '2')
            {
                printf("got a 2 - stop interrupt\r\n");

                /* Enabling Fabric Interrupt*/


               /* Configuring the Interrupt control register in FPGA fabric
               * This will enable the Interrupt in the FPGA*/
                (*((uint32_t volatile *)(FABRIC_INT_CTRL_BASE_ADDR + FABRIC_INTR_ENBL_CLR_OFF_ADDR)) = FABRIC_INTR_ENBL);
                NVIC_DisableIRQ(Fabric_IRQn);
                NVIC_ClearPendingIRQ( Fabric_IRQn );
            }
            else
            {
                printf("key received - %c \r\n",key);
            }
    }
    

    return 0;
}



/* Fabric Interrupt Handler */
/* This Interrupt handler executes upon the occurrence of
   Fabric interrupt, which is from the FPGA Fabric.
   This function reads the status register from the FPGA fabric
   and checks for the Interrupt source. It prints source of the
   interrupt to the hyperterminal */
void Fabric_IRQHandler( void )
{
    uint32_t read_data;

    fpgabase[LED] ^= 0xa;
    fpgabase[INTR_CLR] = 0x4;
    /* reading Interrupt Status register from Interrupt controller in FPGA fabric */
    read_data = (*((uint32_t volatile *)(FABRIC_INT_CTRL_BASE_ADDR + FABRIC_INTR_STAT_OFF_ADDR)));
    /* Identifying the source of the interrupt */
    if(read_data & 4)
    {
        printf("WFF93 Major Frame Interrupt Occurred\n\r");
        // clear the interrupt
        read_data &= 0xfffffffB;
        // toggle LEDs
    }
    else 
    {
        printf(" WTF?\n\r");
    }

    read_data = 0;

    NVIC_ClearPendingIRQ( Fabric_IRQn );

  }


