/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 MSS Ethernet MAC example for transmit and receive packet application.
 * Please refer to the README.TXT in this project's root folder to know more.
 *
 * SVN $Revision: 8059 $
 * SVN $Date: 2015-10-20 12:00:25 +0530 (Tue, 20 Oct 2015) $
*/

/*-------------------------------------------------------------------------*/
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_ethernet_mac/mss_ethernet_mac.h"
#include <stdio.h>

#define PACKET_SIZE                   1518u

/* Buffers for Tx and Rx */
#if defined(__GNUC__)

__attribute__ ((aligned (4))) static uint8_t g_mac_tx_buffer[PACKET_SIZE]  = \
  {0x10,0x10,0x10,0x10,0x10,0x10, 0xc0,0xb1,0x3c,0x88,0x88,0x82};
__attribute__ ((aligned (4))) static uint8_t g_mac_rx_buffer[PACKET_SIZE] ;

#elif defined(__CC_ARM)
    __align(4) static uint8_t g_mac_tx_buffer[PACKET_SIZE] = \
      {0x10,0x10,0x10,0x10,0x10,0x10, 0xc0,0xb1,0x3c,0x88,0x88,0x82};
    __align(4) static uint8_t g_mac_rx_buffer[PACKET_SIZE];
#elif defined(__ICCARM__)
#pragma data_alignment = 4
    static uint8_t g_mac_tx_buffer[PACKET_SIZE] = \
      {0x10,0x10,0x10,0x10,0x10,0x10, 0xc0,0xb1,0x3c,0x88,0x88,0x82};
    static uint8_t g_mac_rx_buffer[PACKET_SIZE];
#endif

static uint32_t g_ethernet_speed_choice = MSS_MAC_ANEG_ALL_SPEEDS;


const static uint8_t mac_address[6] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};

/* MSS Ethernet MAC configuration record */
static  mss_mac_cfg_t mac_config;


volatile uint32_t g_pckt_rcvd_len = 0;

static const uint8_t g_instructions_msg[] =
"\r\n----------------------------------------------------------------------\r\n\
 Press a key to select:\r\n\
 Options to choose PHY interface parameters:\r\n\n\
  [1]: Enable auto-negotiation to all speeds\r\n\
  [2]: Select 1000Mpbs Full-duplex \r\n\
  [3]: Select 1000Mpbs Half-duplex \r\n\
  [4]: Select 100Mpbs Full-duplex \r\n\
  [5]: Select 100Mpbs Half-duplex \r\n\
  [6]: Select 10Mpbs Full-duplex \r\n\
  [7]: Select 10Mpbs Half-duplex \r\n\
";

static const uint8_t g_reset_msg[] =
"\r\nApplying changes and re-configuring SmartFusion2 MSS Ethernet MAC.\r\n";
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
void mac_rx_callback (uint8_t * p_rx_packet, uint32_t pckt_length, void * caller_info);
static void packet_tx_complete_handler(void * caller_info);



static void display_instructions(void)
{
    send_msg (g_instructions_msg);
}

/*==============================================================================
 *
 */
void get_user_eth_config_choice(uint32_t* speed)
{
    *speed = g_ethernet_speed_choice;

}

/*==============================================================================
 *
 */
void set_user_eth_speed_choice(uint32_t speed_choice)
{
    g_ethernet_speed_choice = speed_choice;
}


static void display_reset_msg(void)
{
    MSS_UART_polled_tx(gp_my_uart, g_reset_msg, sizeof(g_reset_msg));
    while(0 == MSS_UART_tx_complete(gp_my_uart))
    {
        ;
    }  
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
        send_msg((const uint8_t*)"\r\n\n----------------------------------------------------------------------");
        send_msg((const uint8_t*)"\r\n              Ethernet link up:");

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
    }
    else
    {
        send_msg((const uint8_t*)"----------------------------------------------------------------------\r\n");
        send_msg((const uint8_t*)"\r\n Ethernet link down.\r\n");
    }
}

void initialize_mss_mac(void)
{

    MSS_MAC_init(&mac_config);

    MSS_MAC_set_tx_callback(packet_tx_complete_handler);

    MSS_MAC_set_rx_callback(mac_rx_callback);

    MSS_MAC_receive_pkt(g_mac_rx_buffer, 0);

    NVIC_EnableIRQ(EthernetMAC_IRQn);

    display_link_status();
}
/**
 * main() function.
 */
int main()
{
    size_t rx_size=0;
    uint8_t rx_buff[1];
    uint8_t count=40;
    uint32_t u_speed=0;
    char rx_pckt_msg[128];
    
    /* Turn off the watchdog */

    SYSREG->WDOG_CR = 0;
    
    NVIC_SetPriority(EthernetMAC_IRQn, 5u);
    NVIC_SetPriority(UART0_IRQn, 4u);
/*---------------------------------------------------------------
    Initialization and Setup
----------------------------------------------------------------*/
     /*--------------------------------------------------------------------------
     * Initialize and configure UART.
     */
    MSS_UART_init(gp_my_uart,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_handler(gp_my_uart, uart_tx_handler);
    
    send_msg((const uint8_t*)"\r\n-----------------------------------------------------------");
    send_msg((const uint8_t*)"\r\n    SmartFusion2 MSS Ethernet MAC Transmit and Receive ");
    send_msg((const uint8_t*)"\r\n              Packet Application ");
    send_msg((const uint8_t*)"\r\n-----------------------------------------------------------");

    MSS_MAC_cfg_struct_def_init(&mac_config);

    mac_config.mac_addr[0] = mac_address[0];
    mac_config.mac_addr[1] = mac_address[1];
    mac_config.mac_addr[2] = mac_address[2];
    mac_config.mac_addr[3] = mac_address[3];
    mac_config.mac_addr[4] = mac_address[4];
    mac_config.mac_addr[5] = mac_address[5];
    mac_config.speed_duplex_select = MSS_MAC_ANEG_1000M_FD;
    mac_config.phy_addr = 0x00;;

    initialize_mss_mac();

/*---------------------------------------------------------------
* Infinite loop.
----------------------------------------------------------------*/
    for(;;)
    {
        /* Check for inpout from the user. */
        rx_size = MSS_UART_get_rx(&g_mss_uart0, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
            default:
                display_instructions();
                *rx_buff=0;
                rx_size=0;

                do{
                        rx_size = MSS_UART_get_rx(&g_mss_uart0, rx_buff, sizeof(rx_buff));
                }while(0 == rx_size);
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


                    default:
                    break;
                }
            break;
            }
            rx_size=0;
            get_user_eth_config_choice(&u_speed);
            mac_config.speed_duplex_select = u_speed;

            initialize_mss_mac();

        }
        if(g_pckt_rcvd_len != 0)
        {
            count++;
            snprintf(rx_pckt_msg, sizeof(rx_pckt_msg),"\n\rRX pkt size = %d", (int)g_pckt_rcvd_len);
            send_msg((const uint8_t*)rx_pckt_msg);
                    
            MSS_MAC_send_pkt(g_mac_tx_buffer, 64u, (void *)&g_mac_tx_buffer_used);

            g_pckt_rcvd_len = 0;
            MSS_MAC_receive_pkt(g_mac_rx_buffer, 0);
        
            if(count >= 40)
            {
                count = 0;
                display_link_status();
                send_msg((const uint8_t*)"\r\n\n             Press any key to change link configuration\r\n\n");
            }
        }
    }
}

static void packet_tx_complete_handler(void * caller_info)
{
    /*
     * caller_info points to g_mac_tx_buffer_used. Signal that content of
     * g_mac_tx_buffer has been sent by the MAC by resetting
     * g_mac_tx_buffer_used.
     */
    *((uint32_t *)caller_info) = 0;
    send_msg((const uint8_t*)"  TX success");
}

void mac_rx_callback
(
    uint8_t * p_rx_packet,
    uint32_t pckt_length,
    void * caller_info
)
{
    g_pckt_rcvd_len = pckt_length;
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
