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
#include "m2sxxx.h"
#include "drivers/mss_uart/mss_uart.h"
/*
 * Delay loop down counter load value.
 */
#define DELAY_LOAD_VALUE     0x00100000
#define LED 0
#define COUNTER 0xA00000
#define PPS_CLEAR 0x10000C/4

volatile uint32_t *fpgabase = (volatile uint32_t *)0x30000000;

/*==============================================================================
 * Global variables.
 */
static volatile const uint8_t * g_tx_buffer;
static volatile size_t g_tx_size = 0;
static volatile uint32_t g_mac_tx_buffer_used = 1u;
/*------------------------------------------------------------------------------
  UART selection.
  Replace the line below with this one if you want to use UART1 instead of
  UART0:
  mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;
 */
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;


/*==============================================================================
 * Local functions.
 */
static void send_msg(const uint8_t * p_msg);
static void uart_tx_handler(mss_uart_instance_t * this_uart);


//#define ACTEL_STDIO_THRU_UART  1
/*-------------------------------------------------------------------------*//**
 * main() function.
 */
int main()
{
    uint8_t key;
    uint8_t                               rx_size =0;

    MSS_UART_init(gp_my_uart,
                  MSS_UART_57600_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_handler(gp_my_uart, uart_tx_handler);


    send_msg("blinky time\r\n");


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
                send_msg("got a 1 - start pps\r\n");
               /* Clear Pending PPS Interrupt*/
               NVIC_ClearPendingIRQ(FabricIrq0_IRQn);

               /* Enable Fabric Interrupt*/
               NVIC_EnableIRQ(FabricIrq0_IRQn);
            }
            else if(key == '2')
            {
                send_msg("got a 2 - stop pps\r\n");
                /* Disabling PPS Interrupt*/
                NVIC_DisableIRQ(FabricIrq0_IRQn);
                /* Clear Pending Fabric Interrupts*/
                NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
            }
            else
            {
                send_msg("key received - wtf \r\n");
            }
    }
    

    return 0;
}




void FabricIrq0_IRQHandler(void)
{
    // reset the PPS signal
    fpgabase[PPS_CLEAR] = 0;
    // toggle LEDs
    fpgabase[LED] ^= 0xff;
    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);

}

/*==============================================================================
 * 
 */
static void send_msg
(
    const uint8_t * p_msg
)
{
    size_t msg_size;
    size_t size_sent;
    
    while(g_tx_size > 0u)
    {
        /* Wait for previous message to complete tx. */
        ;
    }
    
    msg_size = 0u;
    while(p_msg[msg_size] != 0u)
    {
        ++msg_size;
    }
    
    g_tx_buffer = p_msg;
    g_tx_size = msg_size;
    
    size_sent = MSS_UART_fill_tx_fifo(gp_my_uart, p_msg, msg_size);
    g_tx_size = g_tx_size - size_sent;
    g_tx_buffer = &g_tx_buffer[size_sent];
    
    if(g_tx_size > 0u)
    {
        MSS_UART_enable_irq(gp_my_uart, MSS_UART_TBE_IRQ);
    }
}

/*==============================================================================
 * 
 */
static void uart_tx_handler(mss_uart_instance_t * this_uart)
{
    size_t size_in_fifo;
    
    if(g_tx_size > 0)
    {
        size_in_fifo = MSS_UART_fill_tx_fifo(this_uart,
                                             (const uint8_t *)g_tx_buffer,
                                             g_tx_size);
                                             
        if(size_in_fifo  ==  g_tx_size)
        {
            g_tx_size = 0;
            MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
        }
        else
        {
            g_tx_buffer = &g_tx_buffer[size_in_fifo];
            g_tx_size = g_tx_size - size_in_fifo;
        }
    }
    else
    {
        g_tx_size = 0;
        MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
    }
}
