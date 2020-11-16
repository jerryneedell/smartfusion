/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/* Add the required include Files
 */
#include <string.h>
#include "mss_ethernet_mac.h"
#include "../../CMSIS/m2sxxx.h"
#include "sys_cfg.h"

/* FreeRTOS headers */
#include "FreeRTOSConfig.h"

/* Header Files required by LWIP */
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"

#include "netif/etharp.h"

/*
 * PHY addresses of most common SmartFusion2 boards.
 */
//#define M88E1340_PHY_ADDR                   0x01
// gjn changed 11/11/2020
#define M88E1340_PHY_ADDR                   0x00
#define M88E1111_PHY_ADDR                   0x07
#define MAX24288_PHY_ADDR                   0x00
#define KSZ8051MNL_PHY_ADDR                 0x00

/* Define those to better describe your network interface. */
#define IFNAME0 'E0'

#define BUFFER_USED     1u
#define BUFFER_EMPTY    0u
#define RELEASE_BUFFER  BUFFER_EMPTY

/* Buffers for Tx and Rx */
#if defined(__GNUC__)
    static uint8_t g_mac_tx_buffer[MSS_MAC_MAX_TX_BUF_SIZE] __attribute__ ((aligned (4)));
    static uint8_t g_mac_rx_buffer[MSS_MAC_MAX_RX_BUF_SIZE] __attribute__ ((aligned (4)));

#elif defined(__arm__)
    __align(4) static uint8_t g_mac_tx_buffer[MSS_MAC_MAX_TX_BUF_SIZE];
    __align(4) static uint8_t g_mac_rx_buffer[MSS_MAC_MAX_RX_BUF_SIZE];

#elif defined(__ICCARM__)
    #pragma data_alignment = 4
    static uint8_t g_mac_tx_buffer[MSS_MAC_MAX_TX_BUF_SIZE];
    static uint8_t g_mac_rx_buffer[MSS_MAC_MAX_RX_BUF_SIZE];
#endif

static volatile uint32_t g_mac_tx_buffer_used = RELEASE_BUFFER;
static volatile uint32_t g_mac_rx_buffer_data_valid;

struct netif * g_p_mac_netif = 0;

static uint8_t g_rx_paused = 0;

static uint8_t r_mac_addr[60] = {0};
static uint8_t r_mac_length = 0;
static uint8_t prev_packetset[6] = {0};

/* MAC configuration record */
mss_mac_cfg_t g_mac_config;

void mac_rx_callback
(
    uint8_t * p_rx_packet,
    uint32_t pckt_length,
    void * caller_info
);

static void ethernetif_input
(
    struct netif *netif,
    uint8_t * p_rx_packet,
    uint32_t pckt_length
);

static void packet_tx_complete_handler(void * caller_info);

static void low_level_init(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);

static struct pbuf * low_level_input
(
    struct netif *netif,
    uint8_t * p_rx_packet,
    uint32_t pckt_length
);


/**=============================================================================
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    g_rx_paused = 0;
    
    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = 0;
    netif->name[0] = 'E';
    netif->name[1] = '0';

    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

/**=============================================================================
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
    const uint8_t * own_hw_adr;
      
    /*turn off the watchdog*/
    SYSREG->WDOG_CR = 0;
    
    /* We only have one network Interface */
    /* Initialize the Network interface */
    netif->num = 1;
    own_hw_adr = sys_cfg_get_mac_address();
    netif->hwaddr[0] = own_hw_adr[0];
    netif->hwaddr[1] = own_hw_adr[1];
    netif->hwaddr[2] = own_hw_adr[2];
    netif->hwaddr[3] = own_hw_adr[3];
    netif->hwaddr[4] = own_hw_adr[4];
    netif->hwaddr[5] = own_hw_adr[5];
    
    netif->hwaddr_len = 6; /* Defined in LWIP ETHARP_HWADDR_LEN; */

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
    /* RB : Added IGMP flag */
    netif->flags |= NETIF_FLAG_IGMP;
		
    /* Keep track of netif pointer for use by interrupt service routine. */
    g_p_mac_netif = netif;
    
    /*--------------------- Initialize packet containers ---------------------*/
    g_mac_tx_buffer_used = RELEASE_BUFFER;
    g_mac_rx_buffer_data_valid = RELEASE_BUFFER;
    
    /*-------------------------- Initialize the MAC --------------------------*/
    /*
     * The interrupt can cause a context switch, so ensure its priority is
     * between configKERNEL_INTERRUPT_PRIORITY and
     * configMAX_SYSCALL_INTERRUPT_PRIORITY.
     */
    NVIC_SetPriority(EthernetMAC_IRQn,
                     configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    
    /*
     * Get the default configuration for the Ethernet MAC and change settings
     * to match the system/application. The default values typically changed
     * are:
     *  - phy_addr:
     *      Specifies the MII management interface address of the external PHY.
     *  - mac_addr:
     *      Specifies the MAC address of the device. This number should be
     *      unique on the network the device is connected to.
     *  - speed_duplex_select:
     *      Specifies the allowed speed and duplex mode for setting up a link.
     *      This can be used to specify the set of allowed speeds and duplex
     *      modes used during auto-negotiation or force the link speed to a
     *      specific speed and duplex mode.
     */

    MSS_MAC_cfg_struct_def_init(&g_mac_config);
    //g_mac_config.interface = TBI;
    g_mac_config.phy_addr = M88E1340_PHY_ADDR;

    g_mac_config.speed_duplex_select = MSS_MAC_ANEG_ALL_SPEEDS;
    g_mac_config.mac_addr[0] = own_hw_adr[0];
    g_mac_config.mac_addr[1] = own_hw_adr[1];
    g_mac_config.mac_addr[2] = own_hw_adr[2];
    g_mac_config.mac_addr[3] = own_hw_adr[3];
    g_mac_config.mac_addr[4] = own_hw_adr[4];
    g_mac_config.mac_addr[5] = own_hw_adr[5];

    /*
     * Initialize MAC with specified configuration. The Ethernet MAC is
     * functional after this function returns but still requires transmit and
     * receive buffers to be allocated for communications to take place.
     */
    MSS_MAC_init(&g_mac_config);

    /*
     * Register MAC interrupt handler listener functions. These functions will
     * be called  by the MAC driver when a packet ahs been sent or received.
     * These callback functions are intended to help managing transmit and
     * receive buffers by indicating when a transmit buffer can be released or
     * a receive buffer has been filled with an rx packet.
     */
    MSS_MAC_set_tx_callback(packet_tx_complete_handler);
    MSS_MAC_set_rx_callback(mac_rx_callback);

    /* 
     * Allocate a receive buffer. This function will need to be called each time
     * a packet is received to hand back the receive buffer to the MAC driver
     */
    MSS_MAC_receive_pkt(g_mac_rx_buffer, 0);
}

/**=============================================================================
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    uint16_t pckt_length = 0u;
    uint32_t pbuf_chain_end = 0u;
    
    uint8_t tx_status;

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /*--------------------------------------------------------------------------
     * Wait for packet buffer to become free.
     */
    while(BUFFER_USED == g_mac_tx_buffer_used)
    {
        /*
         * Ensure the Ethernet MAC interrupt is enabled. We need the interrupt
         * to be enabled in order to get packet_tx_complete_handler() called to
         * update g_mac_tx_buffer_used. The interrupt could hve been disabled as
         * a result of a receive packet storm.
         */
        NVIC_EnableIRQ(EthernetMAC_IRQn);
    }
    g_mac_tx_buffer_used = BUFFER_USED;
    
    /*--------------------------------------------------------------------------
     * Copy pbuf chain into single buffer.
     */
    q = p;
    do {
        memcpy(&g_mac_tx_buffer[pckt_length], q->payload, q->len);
        pckt_length += q->len;
        if(q->len == q->tot_len)
        {
            pbuf_chain_end = 1u;
        }
        else
        {
            q = q->next;
        }
    } while(0u == pbuf_chain_end);
    
    /*--------------------------------------------------------------------------
     * Initiate packet transmit. Keep retrying until there is room in the MAC Tx
     * ring.
     */
    do {
        tx_status = MSS_MAC_send_pkt(g_mac_tx_buffer,
                                     pckt_length,
                                     (void *)&g_mac_tx_buffer_used);
    } while(MSS_MAC_SUCCESS != tx_status);
    
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    return ERR_OK;
}

/**=============================================================================
 *
 */
static void packet_tx_complete_handler(void * caller_info)
{
    /*
     * caller_info points to g_mac_tx_buffer_used. Signal that content of
     * g_mac_tx_buffer has been sent by the MAC by resetting
     * g_mac_tx_buffer_used.
     */
    *((uint32_t *)caller_info) = RELEASE_BUFFER;
}

/**=============================================================================
    Bottom-half of receive packet handler
*/
void mac_rx_callback
(
    uint8_t * p_rx_packet,
    uint32_t pckt_length,
    void * caller_info
)
{
    if(BUFFER_EMPTY == g_mac_rx_buffer_data_valid)
    {
        g_mac_rx_buffer_data_valid = BUFFER_USED;
        
        if(g_p_mac_netif != 0)
        {
            ethernetif_input(g_p_mac_netif, p_rx_packet, pckt_length);
        }
    }
}

/**=============================================================================
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
ethernetif_input
(
    struct netif *netif,
    uint8_t * p_rx_packet,
    uint32_t pckt_length
)
{
    struct eth_hdr *ethhdr;
    struct pbuf *p;
    err_t input_error;

#ifndef PACKET_MACADDR // received mac address

    if(p_rx_packet[0] != prev_packetset[0] && p_rx_packet[1] != prev_packetset[1] && p_rx_packet[2] != prev_packetset[2] && \
                      p_rx_packet[3] != prev_packetset[3] && p_rx_packet[4] != prev_packetset[4] && p_rx_packet[5] != prev_packetset[5])
    {
        memcpy((r_mac_addr + r_mac_length), p_rx_packet, 6);
        r_mac_length += 6;
        if(r_mac_length == 60)
        {
            r_mac_length = 0;
        }
        prev_packetset[0] = p_rx_packet[0];
        prev_packetset[1] = p_rx_packet[1];
        prev_packetset[2] = p_rx_packet[2];
        prev_packetset[3] = p_rx_packet[3];
        prev_packetset[4] = p_rx_packet[4];
        prev_packetset[5] = p_rx_packet[5];

     }
#endif

    /* move received packet into a new pbuf */
    p = low_level_input(netif, p_rx_packet, pckt_length);
    /* no packet could be read, silently ignore this */
    if (p == NULL) return;
    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type)) {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        input_error = netif->input(p, netif);
        if (input_error!=ERR_OK)
        { /* LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n")); */
            pbuf_free(p);
            p = NULL;
        }
    break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}

/**=============================================================================
 * 
 */
void ethernetif_tick(void)
{
    if(g_rx_paused != 0)
    {
        /* Un-pause the ethernet MAC receive. */
        NVIC_EnableIRQ(EthernetMAC_IRQn);
        g_rx_paused = 0;
    }
}

/**=============================================================================
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input
(
    struct netif *netif,
    uint8_t * p_rx_packet,
    uint32_t pckt_length
)
{
    struct pbuf *p, *q;
    u16_t len;

    p = NULL;
    
    /* Obtain the size of the packet and put it into the "len"
       variable. */
    len = pckt_length;

    if(len > 0)
    {

#if ETH_PAD_SIZE
        len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

        /* We allocate a pbuf chain of pbufs from the pool. */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

        if (p != NULL)
        {
            uint16_t length = 0;

#if ETH_PAD_SIZE
            pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
            /* We iterate over the pbuf chain until we have read the entire
             * packet into the pbuf. */
            for(q = p; q != NULL; q = q->next)
            {
                /* Read enough bytes to fill this pbuf in the chain. The
                 * available data in the pbuf is given by the q->len
                 * variable. */
                /* read data into(q->payload, q->len); */
                memcpy(q->payload, &g_mac_rx_buffer[length], q->len);
                length += q->len;
            }
            /* Assign next packet for reception */
            MSS_MAC_receive_pkt(g_mac_rx_buffer, 0);

            g_mac_rx_buffer_data_valid = RELEASE_BUFFER;
#if ETH_PAD_SIZE
            pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
        }
        else
        {
            /* Assign next packet for reception */
            MSS_MAC_receive_pkt(g_mac_rx_buffer, 0);
            g_mac_rx_buffer_data_valid = RELEASE_BUFFER;
        }
    }
    
    return p;
}

/**************************************************************************//**
 *
 */
void read_mac_address(uint8_t * mac_addr, uint8_t *length)
{
    memcpy(mac_addr, r_mac_addr, r_mac_length);
    *length = r_mac_length;
}
/**************************************************************************//**
 *
 */
void clear_mac_buf(void)
{
    r_mac_length = 0;
    memset(r_mac_addr, 0, 60);
    memset(prev_packetset, 0, 6);
}
