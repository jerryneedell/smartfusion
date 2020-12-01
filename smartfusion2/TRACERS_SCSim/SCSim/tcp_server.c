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
#include "tracers_fpga.h"


#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif

volatile unsigned long *fpgabase;



ethernet_status_t g_ethernet_status;

/*------------------------------------------------------------------------------
 * External functions.
 */

uint32_t get_ip_address(void);
void get_mac_address(uint8_t * mac_addr);
void tcpClientSend(uint8_t *packet, uint32_t packet_size, uint32_t port);

void send_msg(const uint8_t * p_msg);
void send_uart0(const uint8_t * p_msg);

/*------------------------------------------------------------------------------
 *
 */
volatile uint32_t pps_received = 0;
/** The main function, never returns! */

void prvTCPServerTask( void * pvParameters)

{

	int lSocket;
	struct sockaddr_in sLocalAddr;

	lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (lSocket < 0) return;

	memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons(8003);

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
	        char buffer[1024];
	        int nbytes;

	        clientfd = lwip_accept(lSocket, (struct sockaddr*)&client_addr, (socklen_t)&addrlen);
	        if (clientfd>0){
	            do{
	                nbytes=lwip_recv(clientfd, buffer, sizeof(buffer),0);
	                if (nbytes>0)
                        {
                          buffer[nbytes]=0;
                          send_uart0(buffer);
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
  pps_received = 0;
  while(1)
  {
    if(pps_received)
    {
        uint32_t i;
        pps_counter++;
        for (i=0;i<4;i++)
            pps_packet[i]= (pps_counter >> 8*(3-i))&0xff;
        tcpClientSend(pps_packet,4,8002);
        for (i=0;i<4;i++)
            pps_packet[i]= (fpgabase[BUTTON] >> 8*(3-i))&0xff;
        tcpClientSend(pps_packet,4,8001);
        for (i=0;i<4;i++)
            pps_packet[i]= (fpgabase[SW5] >> 8*(3-i))&0xff;
        tcpClientSend(pps_packet,4,8000);
        pps_received = 0;
    }
    /* Run through loop every 50 milliseconds. */
    vTaskDelay(50 / portTICK_RATE_MS);

  }


}


void FabricIrq0_IRQHandler(void)
{
    // reset the PPS signal
    fpgabase[PPS_CLEAR] = 0;
    // toggle LEDs
    fpgabase[LED] ^= 0x1;
    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
    pps_received = 1;
}


void tcpClientSend(uint8_t *packet, uint32_t packet_size, uint32_t port)
{

        uint8_t ip_string[20];


	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	uint8_t buff[80];
	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		send_msg((const uint8_t *)"socket creation failed...\r\n");
		return;
	}
//	else
//		send_msg((const uint8_t *)"Socket successfully created..\r\n");
	memset((uint8_t *)&servaddr, 0,sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
        /* Create and configure the EMAC interface. */
        uint8_t low_address;
        low_address = (fpgabase[SW5]&7)^7;  // get low 3 bits then XOR to invert bits
	sprintf(&ip_string,"192.168.250.13%d",low_address);
//        send_msg(ip_string);
        servaddr.sin_addr.s_addr = inet_addr(ip_string);
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		send_msg((const uint8_t *)"connection with the server failed...\r\n");
        	// close the socket
	        lwip_close(sockfd);
		return;
	}
//	else
//		send_msg((const uint8_t *)"connected to the server..\r\n");

	lwip_send(sockfd, packet, packet_size,0);




	// close the socket
	lwip_close(sockfd);
}

