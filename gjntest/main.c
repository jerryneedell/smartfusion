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
#define MAJOR_FRAME 0x200C/4
#define COUNTER 0xA00000
#define  FABRIC_INT_CTRL_BASE_ADDR 0x40051000
#define  FABRIC_GPIO_BASE_ADDR 0x40050100
// enable Major Frame Interrupt
#define  FABRIC_INTR_ENBL 0x00000001
#define  FABRIC_INTR_CLR 0xFFFFFFFF
#define  FABRIC_INTR_ENBL_OFF_ADDR 0x00000020
#define  FABRIC_INTR_STAT_OFF_ADDR 0x0000002C
#define  FABRIC_INTR_CLR_OFF_ADDR 0x00000080


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

     /* Enabling Fabric Interrupt*/
    NVIC_EnableIRQ(Fabric_IRQn);


    /* Configuring the Interrupt control register in FPGA fabric
     * This will enable the Interrupt in the FPGA*/
    (*((uint32_t volatile *)(FABRIC_INT_CTRL_BASE_ADDR + FABRIC_INTR_ENBL_OFF_ADDR)) = FABRIC_INTR_ENBL);

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
            else if(key == '5')
            {
                printf("got a 5 - generate Major Frame IRQ irq\r\n");
                fpgabase[MAJOR_FRAME] = 0x10;
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

/* Fabric Interrupt Handler */
/* This Interrupt handler executes upon the occurrence of
   Fabric interrupt, which is from the FPGA Fabric.
   This function reads the status register from the FPGA fabric
   and checks for the Interrupt source. It prints source of the
   interrupt to the hyperterminal */
void Fabric_IRQHandler( void )
{
    uint32_t read_data;

    /* reading Interrupt Status register from Interrupt controller in FPGA fabric */
    read_data = (*((uint32_t volatile *)(FABRIC_INT_CTRL_BASE_ADDR + FABRIC_INTR_STAT_OFF_ADDR)));
    /* Identifying the source of the interrupt */
    if(read_data == 1)
    {
        printf("WFF93 Major Frame Interrupt Occurred\n\r");
        // clear the interrupt
        fpgabase[MAJOR_FRAME] = 0x1;
        read_data = 0;
    }
    else if(read_data == 2)
    {
        printf("WFF93 Minor Frame Interrupt Occurred \n\r");
        read_data = 0;
    }
    else if(read_data == 3)
    {
        printf("WFF93 FIF0 Empty Interrupt Occured\n\r");
        read_data = 0;
    }
    else if(read_data == 4)
    {
        printf("PLASMIC FIFO EMPTY Interrupt Occurred \n\r");
        read_data = 0;
    }
    else if(read_data == 5)
    {
        printf("PLASMIC FIFO Half Full Interrupt Occurred \n\r");
        read_data = 0;
    }
    else if(read_data == 6)
    {
        printf("IRQ 6 Occured - WTF?\n\r");
        read_data = 0;
    }
    else if(read_data == 7)
    {
        printf("IRQ 7 Occured - WTF?\n\r");
        read_data = 0;
    }
    else if(read_data == 8)
    {
        printf("IRQ 8 Occured - WTF?\n\r");
        read_data = 0;
    }

    /* Clearing the Interrupts in the FPGA */
    (*((uint32_t volatile *)(FABRIC_GPIO_BASE_ADDR + FABRIC_INTR_CLR_OFF_ADDR)) = FABRIC_INTR_CLR);

    NVIC_ClearPendingIRQ( Fabric_IRQn );

  }


