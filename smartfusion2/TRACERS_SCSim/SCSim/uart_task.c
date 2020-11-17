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


/* Driver includes */
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_ethernet_mac/mss_ethernet_mac.h"


uint32_t get_ip_address(void);
void set_user_eth_filter_choice(uint32_t filter_choice);
void set_user_eth_speed_choice(uint32_t speed_choice);
void get_mac_address(uint8_t * mac_addr);
static void  display_received_mac_addresses(void);
void read_mac_address(uint8_t * mac_addr, uint8_t *length);
void clear_mac_buf(void);

/*==============================================================================
 * Local functions.
 */
static void send_msg(const uint8_t * p_msg);
static void uart0_tx_handler(mss_uart_instance_t * this_uart);
static void uart1_tx_handler(mss_uart_instance_t * this_uart);
static void display_link_status(void);
static void display_instructions(void);
static void display_reset_msg(void);
void tcpClientTest(uint8_t *packet, uint32_t packet_size, uint32_t port);
/*==============================================================================
 * Global variables.
 */
static volatile const uint8_t * g_tx_buffer;
static volatile size_t g_tx_size = 0;
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
 Options to choose PHY interface parameters:\r\n\n\
  [1]: Enable auto-negotiation to all speeds\r\n\
  [2]: Select 1000Mpbs, Full-duplex \r\n\
  [3]: Select 1000Mpbs Half-duplex \r\n\
  [4]: Select 100Mpbs Full-duplex \r\n\
  [5]: Select 100Mpbs Half-duplex \r\n\
  [6]: Select 10Mpbs Full-duplex \r\n\
  [7]: Select 10Mpbs Half-duplex \r\n\
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

    /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_comm_uart,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_handler(gp_comm_uart, uart0_tx_handler);

    for( ;; )
    {
        size_t rx_size;
        uint8_t rx_buff[1];

        /* Run through loop every 500 milliseconds. */
        vTaskDelay(500 / portTICK_RATE_MS);

        /* Check for inpout from the user. */
        rx_size = MSS_UART_get_rx(gp_comm_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case 't':
                case 'T':
                    tcpClientTest("Hello, world!", 12, 30023);
                break;

                default:
                break;
            }
            sys_msleep(5);
        }
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
    send_msg((const uint8_t*)"**************SmartFusion2 MSS MAC WebServer Example *****************\r\n");
    send_msg((const uint8_t*)"**********************************************************************\r\n\n");

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
                case '1':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_ALL_SPEEDS);
                break;

                case '2':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_1000M_FD);
                break;

                case '3':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_1000M_HD);
                break;

                case '4':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_100M_FD);
                break;

                case '5':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_100M_HD);
                break;

                case '6':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_10M_FD);
                break;

                case '7':
                    display_reset_msg();
                    set_user_eth_speed_choice(MSS_MAC_ANEG_10M_HD);
                break;   
                
                case 'm':
                case 'M':
                    display_received_mac_addresses();
                break;
                
                case 't':
                case 'T':
                    tcpClientTest("Hello, world!", 12, 30023);
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
static void uart0_tx_handler(mss_uart_instance_t * this_uart)
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


void tcpClientTest(uint8_t *packet, uint32_t packet_size, uint32_t port)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	uint8_t buff[80];
	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		send_msg((const uint8_t *)"socket creation failed...\r\n");
		return;
	}
	else
		send_msg((const uint8_t *)"Socket successfully created..\r\n");
	memset((uint8_t *)&servaddr, 0,sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("10.42.0.1");
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		send_msg((const uint8_t *)"connection with the server failed...\r\n");
        	// close the socket
	        lwip_close(sockfd);
		return;
	}
	else
		send_msg((const uint8_t *)"connected to the server..\r\n");

	lwip_send(sockfd, packet, packet_size,0);




	// close the socket
	lwip_close(sockfd);
}

