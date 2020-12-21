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

#define FRAMESYNC 0xc830fafe

extern volatile unsigned long *fpgabase;


uint32_t get_ip_address(void);
void get_mac_address(uint8_t * mac_addr);
static void  display_received_mac_addresses(void);
void read_mac_address(uint8_t * mac_addr, uint8_t *length);
void clear_mac_buf(void);
int32_t tcpClientOpen(uint32_t port);
void generate_itf(uint32_t frame_sync, uint16_t apid, uint16_t sequence_number, uint8_t *data, uint16_t length, uint8_t *itf);


/*==============================================================================
 * Local functions.
 */
void send_msg(const uint8_t * p_msg);
void send_uart0(const uint8_t * p_msg, size_t msg_size);
//static void uart0_tx_handler(mss_uart_instance_t * this_uart);
static void uart0_rx_handler(mss_uart_instance_t * this_uart);
//static void uart1_tx_handler(mss_uart_instance_t * this_uart);
static void display_instructions(void);
static void display_reset_msg(void);
static void display_link_status(void);
/*==============================================================================
 * Global variables.
 */
extern int32_t tlm_sockfd;
extern int32_t hk_sockfd;
extern int32_t pps_sockfd;
static volatile const uint8_t * g_tx_buffer;
static volatile size_t g_tx_size = 0;
static volatile const uint8_t * g_tx_uart0_buffer;
static volatile const uint8_t g_rx_uart0_buffer[UART0_RX_BYTES];
//uint8_t uart0_rx_buffer[100];
//size_t uart0_rx_size = 0;
static volatile size_t g_tx_uart0_size = 0;
static volatile size_t g_rx_uart0_size = 0;
static char ip_addr_msg[128];
static const uint8_t g_instructions_msg[] =
"---------TRACERS S/C Simulator Version 1.0---------------------------\r\n\
Press a key to select:\r\n\n\
  [P]: Enable PPS\r\n\
  [p]: Disable PPS\r\n\
  [T]: Enable Telemetry IRQ\r\n\
  [t]: Disable Telemetry IRQ\r\n\
  [L]: Enable Telemetry Loopback\r\n\
  [l]: Disable Telemetry Loopback\r\n\
  [X]: Generate TLM packet via Loopback\r\n\
  [m]: Received MAC addresses \r\n\
  [anything]: Display link status (MAC address and IP)\r\n\
";

static const uint8_t g_reset_msg[] =
"\r\nApplying changes and resetting system.\r\n";

static mss_uart_instance_t * const gp_comm_uart = &g_mss_uart0;
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;

/*==============================================================================
 * UART task.
 */
void prvUART0Task( void * pvParameters)
{

    /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_comm_uart,
                  MSS_UART_38400_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);


    for( ;; )
    {

        g_rx_uart0_size = MSS_UART_get_rx( gp_comm_uart, g_rx_uart0_buffer, sizeof(g_rx_uart0_buffer) );
        if(g_rx_uart0_size > 0)
        {

                if(hk_sockfd == -1)
                    hk_sockfd=tcpClientOpen(HK_PORT);
                if(hk_sockfd != -1)
                {
                    if(lwip_send(hk_sockfd,g_rx_uart0_buffer, g_rx_uart0_size ,0)==-1)
                    {
                      send_msg((const uint8_t *)"HK socket error - closing socket\n\r");
                      lwip_close(hk_sockfd);
                      hk_sockfd = -1;
                    }
                }
            g_rx_uart0_size=0;
            // toggle LED
            fpgabase[LED]^=0x2;
        }
    }
}
/*==============================================================================
 * UART task.
 */
void prvUART1Task( void * pvParameters)
{
    uint32_t seq_counter = 0;

    /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_my_uart,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);


    display_link_status();
    display_instructions();

    for( ;; )
    {
        size_t rx_size;
        uint8_t rx_buff[1];
        uint32_t npackets;

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
                case 'L':
                    fpgabase[4] |= 2; // enable TLM loopback
                    send_msg((const uint8_t *)"TLM Loopback Enabled\r\n");
                    break;
                case 'l':
                    fpgabase[4] &= 1; //  disable TLM loopback
                    send_msg((const uint8_t *)"TLM Loopback Disabled\r\n");
                    break;
                case 'x':
                case 'X':
                    for(npackets=0;npackets<10;npackets++)
                    {
                    seq_counter++;
                    uint8_t itf[152];
                    uint8_t tlm_packet[140];
                    uint32_t tlm_fifo_packet[38];
                    uint32_t i;
                    fpgabase[TLM_XMIT_CLEAR] = 0; // clear the TLM fifo
                    for (i=0;i<4;i++) // coarse time
                        tlm_packet[i]= ((seq_counter&0x7ffffff) >> 8*(3-i))&0xff;
                    for (i=0;i<4;i++)  // fine time
                        tlm_packet[4+i]= 0;
                    for (i=0;i<4;i++)
                        tlm_packet[8+i]= (fpgabase[BUTTON] >> 8*(3-i))&0xff;
                    for(i=0;i<128;i++)
                        tlm_packet[12+i]=i;
                    generate_itf(FRAMESYNC, 0x1aa, (uint16_t)seq_counter&0x3fff, tlm_packet, 140, itf);
                    for(i=0;i<38;i++)
                    {
                        tlm_fifo_packet[i] = (((uint32_t)itf[4*i])<<24) +
                                             (((uint32_t)itf[4*i+1])<<16) +
                                             (((uint32_t)itf[4*i+2])<<8) +
                                             (((uint32_t)itf[4*i+3]));
                    }
                    for(i=0;i<38;i++)
                    {
                        fpgabase[TLM_XMIT_WRITE] = tlm_fifo_packet[i];
                    }
                    fpgabase[TLM_XMIT_START] = 1;
                    sys_msleep(1);
                    }
                break;

                case 'P':
                    if(hk_sockfd == -1)
                       hk_sockfd=tcpClientOpen(HK_PORT);
                    if(pps_sockfd == -1)
                       pps_sockfd=tcpClientOpen(STATUS_PORT);
                    /* Clear Pending PPS Interrupt*/
                    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
                    /* Enable Fabric Interrupt*/
                    NVIC_EnableIRQ(FabricIrq0_IRQn);
                    send_msg((const uint8_t *)"PPS Enabled\r\n");
                break;

                case 'p':
                    /* Disabling PPS Interrupt*/
                    NVIC_DisableIRQ(FabricIrq0_IRQn);
                    /* Clear Pending Fabric Interrupts*/
                    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
                    lwip_close(hk_sockfd);
                    lwip_close(pps_sockfd);
                    pps_sockfd = -1;
                    hk_sockfd = -1;
                    send_msg((const uint8_t *)"PPS Disabled\r\n");
                break;

                case 'T':
                    if(tlm_sockfd == -1)
                       tlm_sockfd=tcpClientOpen(TLM_PORT);
                    /* Clear Pending TLM Interrupt*/
                    NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
                    /* Enable Fabric Interrupt*/
                    NVIC_EnableIRQ(FabricIrq1_IRQn);
                    send_msg((const uint8_t *)"TLM Enabled\r\n");
                break;

                case 't':
                    /* Disabling TLM Interrupt*/
                    NVIC_DisableIRQ(FabricIrq1_IRQn);
                    /* Clear Pending Fabric Interrupts*/
                    NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
                    lwip_close(tlm_sockfd);
                    tlm_sockfd = -1;
                    send_msg((const uint8_t *)"TLM Disabled\r\n");
                break;
                case 'Z':
                    /* Clear Pending IRQ2 Interrupt*/
                    NVIC_ClearPendingIRQ(FabricIrq2_IRQn);
                    /* Enable Fabric Interrupt*/
                    NVIC_EnableIRQ(FabricIrq2_IRQn);
                    send_msg((const uint8_t *)"IRQ2 Enabled\r\n");
                break;

                case 'z':
                    /* Disabling IRQ2 Interrupt*/
                    NVIC_DisableIRQ(FabricIrq2_IRQn);
                    /* Clear Pending Fabric Interrupts*/
                    NVIC_ClearPendingIRQ(FabricIrq2_IRQn);
                    send_msg((const uint8_t *)"IRQ2 Disbled\r\n");
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
        send_msg((const uint8_t*)"Use above IP address to access the CMD Server\r\n");
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


    while(!MSS_UART_tx_complete(gp_my_uart))
    {
        /* Wait for previous message to complete tx. */
        ;
    }


    msg_size = 0u;
    while(p_msg[msg_size] != 0u)
    {
        ++msg_size;
    }

    MSS_UART_irq_tx(gp_my_uart, p_msg, msg_size);

}
void send_uart0
(
    const uint8_t * p_msg,  size_t msg_size
)
{
    size_t size_sent;

    while(!MSS_UART_tx_complete(gp_comm_uart))
    {
        /* Wait for previous message to complete tx. */
        ;
    }

    MSS_UART_irq_tx(gp_comm_uart, p_msg, msg_size);
}

static void uart0_rx_handler(mss_uart_instance_t * this_uart)
{
    g_rx_uart0_size = MSS_UART_get_rx( this_uart, g_rx_uart0_buffer, sizeof(g_rx_uart0_buffer) );

}
static void display_reset_msg(void)
{
    MSS_UART_polled_tx(gp_my_uart, g_reset_msg, sizeof(g_reset_msg));
    while(0 == MSS_UART_tx_complete(gp_my_uart))
    {
        ;
    }
}

