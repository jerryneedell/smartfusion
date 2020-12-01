/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 *
 * SVN $Revision: 8059 $
 * SVN $Date: 2015-10-20 12:00:25 +0530 (Tue, 20 Oct 2015) $
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <stdio.h>
#include <string.h>
#include "tracers_fpga.h"

/* Driver includes */
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_ethernet_mac/mss_ethernet_mac.h"

volatile unsigned long *fpgabase;


uint32_t get_ip_address(void);
void set_user_eth_filter_choice(uint32_t filter_choice);
void set_user_eth_speed_choice(uint32_t speed_choice);
void get_mac_address(uint8_t * mac_addr);
static void  display_received_mac_addresses(void);
void read_mac_address(uint8_t * mac_addr, uint8_t *length);
void clear_mac_buf(void);
void tcpClientSend(uint8_t *packet, uint32_t packet_size, uint32_t port);

/*==============================================================================
 * Local functions.
 */
void send_msg(const uint8_t * p_msg);
void send_uart0(const uint8_t * p_msg);
static void uart0_tx_handler(mss_uart_instance_t * this_uart);
static void uart0_rx_handler(mss_uart_instance_t * this_uart);
static void uart1_tx_handler(mss_uart_instance_t * this_uart);
static void display_link_status(void);
static void display_instructions(void);
static void display_reset_msg(void);
/*==============================================================================
 * Global variables.
 */
static volatile const uint8_t * g_tx_buffer;
static volatile size_t g_tx_size = 0;
static volatile const uint8_t * g_tx_uart0_buffer;
static volatile size_t g_tx_uart0_size = 0;
static volatile const uint8_t g_rx_uart0_buffer[100];
static volatile size_t g_rx_uart0_size = 0;
static char ip_addr_msg[128];

/*Broadcast address*/
uint8_t address_filter_broad[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/*Unicast, multicast and broadcast address*/
uint8_t address_filter_hash [4][6] = {{0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
                  {0x43, 0x40, 0x40, 0x40, 0x40, 0x43},
                  {0xC0, 0xB1, 0x3C, 0x60, 0x60, 0x60},
                  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

static const uint8_t g_instructions_msg[] =
"----------------------------------------------------------------------\r\n\
Press a key to select:\r\n\n\
  [P]: Enable PPS\r\n\
  [p]: Disable PPS\r\n\
  [H]: Generate HK packetS\r\n\
  [T]: Generate Telemetry Packet\r\n\
  [s]: Display link status (MAC address and IP)\r\n\
  [m]: Received MAC addresses \r\n\
";

static const uint8_t g_reset_msg[] =
"\r\nApplying changes and resetting system.\r\n";

/*------------------------------------------------------------------------------
  UART selection.
  Replace the line below with this one if you want to use UART1 instead of
  UART0:
  mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;
 */
//static mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;
static mss_uart_instance_t * const gp_comm_uart = &g_mss_uart0;
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;

/*==============================================================================
 * UART task.
 */
void prvUART0Task( void * pvParameters)
{

    uint8_t  rx_buffer[100];
    uint32_t rx_size = 0;
    /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_comm_uart,
                  MSS_UART_38400_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_handler(gp_comm_uart, uart0_tx_handler);
    MSS_UART_set_rx_handler(gp_comm_uart, uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
    //MSS_UART_enable_irq(gp_comm_uart, MSS_UART_RBF_IRQ);

    for( ;; )
    {

        /* Run through loop every 500 milliseconds. */
        //vTaskDelay(500 / portTICK_RATE_MS);

        if(g_rx_uart0_size > 0)
        {
              uint32_t i;
              for( i= 0; i< g_rx_uart0_size;  i++)
              {
                 rx_buffer[rx_size + i ] = g_rx_uart0_buffer[i];
              }
              rx_size += g_rx_uart0_size;
              g_rx_uart0_size = 0;



//              if( rx_size > 10 )
//             {
//                 tcpClientSend(rx_buffer, rx_size, 8000);
//                 rx_size = 0;
//             }
                tcpClientSend(rx_buffer, rx_size, 8000);
                rx_size = 0;


            // toggle LED
            fpgabase[LED]^=0x2;
        }
        sys_msleep(5);
    }
}
/*==============================================================================
 * UART task.
 */
void prvUART1Task( void * pvParameters)
{

    /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_my_uart,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_handler(gp_my_uart, uart1_tx_handler);

    send_msg((const uint8_t*)"\r\n\r\n**********************************************************************\r\n");
    send_msg((const uint8_t*)"************** TRACERS SC SIMULATOR  *********************************\r\n");
    send_msg((const uint8_t*)"**********************************************************************\r\n\n");

    display_link_status();
    display_instructions();

    for( ;; )
    {
        size_t rx_size;
        uint8_t rx_buff[1];

        /* Run through loop every 500 milliseconds. */
        vTaskDelay(500 / portTICK_RATE_MS);

        /* Check for inpout from the user. */
        rx_size = MSS_UART_get_rx(gp_my_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                
                case 'm':
                case 'M':
                    display_received_mac_addresses();
                break;
                
                case 'h':
                case 'H':
                    tcpClientSend("Housekeeping", 12, 8000);
                break;

                case 'P':
                    /* Clear Pending PPS Interrupt*/
                    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);

                    /* Enable Fabric Interrupt*/
                    NVIC_EnableIRQ(FabricIrq0_IRQn);
                break;

                case 'p':
                    /* Disabling PPS Interrupt*/
                    NVIC_DisableIRQ(FabricIrq0_IRQn);
                    /* Clear Pending Fabric Interrupts*/
                    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
                break;

                default:
                    display_link_status();
                break;
            }
            sys_msleep(5);
            // toggle LED
            fpgabase[LED]^=0x4;
            display_instructions();
        }
    }
}
/*==============================================================================
 *
 */
static void  display_received_mac_addresses(void)
{
    static uint8_t mac_addr[60];
    static uint8_t length;
    static uint8_t mac_addr_msg[128];
    uint8_t a;

    read_mac_address(mac_addr, &length);

    if(length)
    {
        for(a = 0; a < length; a += 6)
        {
            snprintf((char *)mac_addr_msg, sizeof(mac_addr_msg),"\r\n  MAC address: %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                              mac_addr[0+a], mac_addr[1+a], mac_addr[2+a], mac_addr[3+a], mac_addr[4+a], mac_addr[5+a]);
             send_msg((const uint8_t*)mac_addr_msg);
        }
    }
}
/*==============================================================================
 *
 */
static void display_instructions(void)
{
    send_msg(g_instructions_msg);
    //send_uart0(g_instructions_msg);
}

/*==============================================================================
 *
 */
static void display_link_status(void)
{
    uint8_t link_up;
    uint8_t fullduplex;
    mss_mac_speed_t speed;

    link_up = MSS_MAC_get_link_status((mss_mac_speed_t *)&speed,
                                  (uint8_t *)&fullduplex);
    if(1u == link_up)
    {
        uint32_t ip_addr;

        uint8_t mac_addr[6];

        send_msg((const uint8_t*)"----------------------------------------------------------------------\r\n");
        send_msg((const uint8_t*)" Ethernet link up:");

        ip_addr = get_ip_address();
        get_mac_address(mac_addr);
        snprintf(ip_addr_msg, sizeof(ip_addr_msg),
                 "\r\n  MAC address: %02x:%02x:%02x:%02x:%02x:%02x\r\n  TCP/IP address: %d.%d.%d.%d \r\n",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
                  (int)(ip_addr & 0x000000FFu),
                  (int)((ip_addr >> 8u) & 0x000000FFu),
                  (int)((ip_addr >> 16u) & 0x000000FFu),
                  (int)((ip_addr >> 24u) & 0x000000FFu));
        send_msg((const uint8_t*)ip_addr_msg);

        switch(speed)
        {
            case MSS_MAC_10MBPS:
                send_msg((const uint8_t*)"  10Mbps ");
            break;

            case MSS_MAC_100MBPS:
                send_msg((const uint8_t*)"  100Mbps ");
            break;

            case MSS_MAC_1000MBPS:
                send_msg((const uint8_t*)"  1000Mbps ");
            break;

            default:
            break;
        }

        if(1u == fullduplex)
        {
            send_msg((const uint8_t*)"Full Duplex\r\n");
        }
        else
        {
            send_msg((const uint8_t*)"Half Duplex\r\n");
        }
        send_msg((const uint8_t*)"Use above IP address to access the WebServer using browser\r\n");
    }
    else
    {
        send_msg((const uint8_t*)"----------------------------------------------------------------------\r\n");
        send_msg((const uint8_t*)"\r\n Ethernet link down.\r\n");
    }
}

/*==============================================================================
 *
 */
void send_msg
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
void send_uart0
(
    const uint8_t * p_msg
)
{
    size_t msg_size;
    size_t size_sent;

    while(g_tx_uart0_size > 0u)
    {
        /* Wait for previous message to complete tx. */
        ;
    }

    msg_size = 0u;
    while(p_msg[msg_size] != 0u)
    {
        ++msg_size;
    }

    g_tx_uart0_buffer = p_msg;
    g_tx_uart0_size = msg_size;

    size_sent = MSS_UART_fill_tx_fifo(gp_comm_uart, p_msg, msg_size);
    g_tx_uart0_size = g_tx_uart0_size - size_sent;
    g_tx_uart0_buffer = &g_tx_uart0_buffer[size_sent];

    if(g_tx_uart0_size > 0u)
    {
        MSS_UART_enable_irq(gp_comm_uart, MSS_UART_TBE_IRQ);
    }
}

/*==============================================================================
 *
 */
static void uart0_tx_handler(mss_uart_instance_t * this_uart)
{
    size_t size_in_fifo;

    if(g_tx_uart0_size > 0)
    {
        size_in_fifo = MSS_UART_fill_tx_fifo(this_uart,
                                             (const uint8_t *)g_tx_uart0_buffer,
                                             g_tx_uart0_size);

        if(size_in_fifo  ==  g_tx_uart0_size)
        {
            g_tx_uart0_size = 0;
            MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
        }
        else
        {
            g_tx_uart0_buffer = &g_tx_uart0_buffer[size_in_fifo];
            g_tx_uart0_size = g_tx_uart0_size - size_in_fifo;
        }
    }
    else
    {
        g_tx_uart0_size = 0;
        MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
    }
}
static void uart0_rx_handler(mss_uart_instance_t * this_uart)
{
    g_rx_uart0_size = MSS_UART_get_rx( this_uart, g_rx_uart0_buffer, sizeof(g_rx_uart0_buffer) );

}
static void uart1_tx_handler(mss_uart_instance_t * this_uart)
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

static void display_reset_msg(void)
{
    MSS_UART_polled_tx(gp_my_uart, g_reset_msg, sizeof(g_reset_msg));
    while(0 == MSS_UART_tx_complete(gp_my_uart))
    {
        ;
    }
}

