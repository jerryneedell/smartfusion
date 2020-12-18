/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Producst Group.  All rights reserved.
 *
 *
 * SVN $Revision: 7579 $
 * SVN $Date: 2015-07-23 10:01:49 +0530 (Thu, 23 Jul 2015) $
 */
#include <string.h>
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "drivers/mss_uart/mss_uart.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"

#include "m2sxxx.h"
#include "mss_ethernet_mac.h"

#include "ethernet_status.h"

#include <assert.h>
#include "swfo_fpga.h"

#define FRAMESYNC 0xc830fafe

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif

extern volatile unsigned long *fpgabase;



ethernet_status_t g_ethernet_status;

int32_t cmd_sockfd = -1;
int32_t tlm_sockfd = -1;
int32_t hk_sockfd = -1;
int32_t pps_sockfd = -1;
volatile uint32_t g_tlm_packet_arrived = 0;
volatile uint32_t g_words_in_fifo;
volatile uint32_t g_pps_received = 0;
/*------------------------------------------------------------------------------
 * External functions.
 */

uint16_t generate_crc(uint8_t *packet, uint16_t length);
uint16_t crc_ccitt_update(uint16_t crc, uint8_t  x);
void generate_itf(uint32_t frame_sync, uint16_t apid, uint16_t sequence_number, uint8_t *data, uint16_t length, uint8_t *itf);

uint32_t get_ip_address(void);
void get_mac_address(uint8_t * mac_addr);
int32_t tcpClientOpen(uint32_t port);

void send_msg(const uint8_t * p_msg);
void send_uart0(const uint8_t * p_msg, size_t msg_size);
void ethernetif_tick(void);


void prvCMDServerTask( void * pvParameters)

{

	int lSocket;
	struct sockaddr_in sLocalAddr;

	lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (lSocket < 0) return;

	memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons(CMD_PORT);

	if (lwip_bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr)) < 0) {
	        lwip_close(lSocket);
	        return;
	}

	if ( lwip_listen(lSocket, 20) != 0 ){
	        lwip_close(lSocket);
	        return;
	}

	while (1) {
	        int clientfd;
	        struct sockaddr_in client_addr;
	        int addrlen=sizeof(client_addr);
	        char buffer[CMD_MAX_BYTES];
	        int nbytes;

	        clientfd = lwip_accept(lSocket, (struct sockaddr*)&client_addr, (socklen_t)&addrlen);
	        if (clientfd>0){
	            do{
	                nbytes=lwip_recv(clientfd, buffer, sizeof(buffer),0);
	                if (nbytes>0)
                        {
                          buffer[nbytes]=0;
                          send_uart0(buffer, nbytes);
                          // toggle LED
                          fpgabase[LED] ^= 0x10;
                        }
	            }  while (nbytes>0);

	             lwip_close(clientfd);
	          }
	    }
	    lwip_close(lSocket);
	}


void prvPPSTask( void * pvParameters)

{
  uint32_t pps_counter = 0;
  uint8_t pps_packet[4];
  if(hk_sockfd == -1)
    hk_sockfd=tcpClientOpen(HK_PORT);
  if(pps_sockfd == -1)
    pps_sockfd=tcpClientOpen(STATUS_PORT);
  /* Clear Pending PPS Interrupt*/
  NVIC_ClearPendingIRQ(FabricIrq0_IRQn);

  /* Enable Fabric Interrupt*/
  NVIC_EnableIRQ(FabricIrq0_IRQn);
  g_pps_received = 0;
  while(1)
  {
    if(g_pps_received)
    {
        uint32_t i;
        pps_counter++;
        // SWFO limits this to 16 bits - upper 16 bts are zeros
        pps_counter&=0xffff;
        for (i=0;i<4;i++)
            pps_packet[i]= (pps_counter >> 8*(3-i))&0xff;
        if(pps_sockfd == -1)
            pps_sockfd=tcpClientOpen(STATUS_PORT);
        if(pps_sockfd != -1)
        { 
            if(lwip_send(pps_sockfd, pps_packet, 4,0)==-1)
            {
              send_msg("PPS socket error - closing socket\n\r");
              lwip_close(pps_sockfd);
              pps_sockfd = -1;
            }
        }
        g_pps_received = 0;
    }
    ethernetif_tick();
  }
}




void FabricIrq0_IRQHandler(void)
{
    // reset the PPS signal
    fpgabase[PPS_CLEAR] = 0;
    // toggle LEDs
    fpgabase[LED] ^= 0x1;
    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
    // set flag for pps task
    g_pps_received = 1;
}



int32_t tcpClientOpen(uint32_t port)
{

        uint8_t ip_string[20];


	int32_t sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		send_msg((const uint8_t *)"socket creation failed...\r\n");
		return sockfd;
	}
	else
		send_msg((const uint8_t *)"Socket successfully created..\r\n");
	memset((uint8_t *)&servaddr, 0,sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
        /* Create and configure the EMAC interface. */
        uint8_t low_address;
        low_address = (fpgabase[SW5]&7)^7;  // get low 3 bits then XOR to invert bits
	sprintf(&ip_string,"192.168.250.13%d",low_address);
        servaddr.sin_addr.s_addr = inet_addr(ip_string);
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		send_msg((const uint8_t *)"connection with the server failed...\r\n");
        	// close the socket
	        lwip_close(sockfd);
                sockfd = -1;
		return sockfd;
	}
	else
		send_msg((const uint8_t *)"connected to the server..\r\n");

	return sockfd;
}

