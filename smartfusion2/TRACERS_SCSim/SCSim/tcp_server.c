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
#include "tcp_server.h"
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


extern xQueueHandle xEthStatusQueue;

ethernet_status_t g_ethernet_status;

/*------------------------------------------------------------------------------
 * External functions.
 */

uint32_t get_ip_address(void);
void get_mac_address(uint8_t * mac_addr);
void tcpClientTest(uint8_t *packet, uint32_t packet_size, uint32_t port);


/*------------------------------------------------------------------------------
 *
 */
volatile uint32_t pps_received = 0;
/** The main function, never returns! */

void
tcp_server_thread(void *arg)

{

	int lSocket;
	struct sockaddr_in sLocalAddr;

	lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (lSocket < 0) return;

	memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons(23);

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
	                if (nbytes>0) lwip_send(clientfd, buffer, nbytes, 0);
	            }  while (nbytes>0);

	             lwip_close(clientfd);
	          }
	    }
	    lwip_close(lSocket);
	}


/** Initialize the TCP server (start its thread) */
//void
//tcp_server_init(void)
//{
//  sys_thread_new("tcp_server", tcp_server_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);/
//}


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
        tcpClientTest(pps_packet,4,30024);
        for (i=0;i<4;i++)
            pps_packet[i]= (fpgabase[BUTTON] >> 8*(3-i))&0xff;
        tcpClientTest(pps_packet,4,30023);
        for (i=0;i<4;i++)
            pps_packet[i]= (fpgabase[SW5] >> 8*(3-i))&0xff;
        tcpClientTest(pps_packet,4,30023);
        pps_received = 0;
    }
    /* Run through loop every 50 milliseconds. */
    vTaskDelay(50 / portTICK_RATE_MS);

  }


}

/** Initialize the PPS  (start its thread) */
//void
//pps_init(void)
//{
//  sys_thread_new("pps", prvPPSTask, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
//}

void FabricIrq0_IRQHandler(void)
{
    // reset the PPS signal
    fpgabase[PPS_CLEAR] = 0;
    // toggle LEDs
    fpgabase[LED] ^= 0xff;
    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
    pps_received = 1;
}