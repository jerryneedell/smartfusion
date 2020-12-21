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

#define FRAMESYNC 0xc830fafe

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif

extern volatile unsigned long *fpgabase;



ethernet_status_t g_ethernet_status;

int32_t tlm_sockfd = -1;
int32_t hk_sockfd = -1;
int32_t pps_sockfd = -1;
volatile uint32_t g_tlm_packet_arrived = 0;
volatile uint32_t g_words_in_fifo;
/*------------------------------------------------------------------------------
 * External functions.
 */
void ethernetif_tick(void);
uint16_t generate_crc(uint8_t *packet, uint16_t length);
uint16_t crc_ccitt_update(uint16_t crc, uint8_t  x);
void generate_itf(uint32_t frame_sync, uint16_t apid, uint16_t sequence_number, uint8_t *data, uint16_t length, uint8_t *itf);

uint32_t get_ip_address(void);
void get_mac_address(uint8_t * mac_addr);
int32_t tcpClientOpen(uint32_t port);

void send_msg(const uint8_t * p_msg);
void send_uart0(const uint8_t * p_msg, size_t msg_size);


/*------------------------------------------------------------------------------
 *
 */
volatile uint32_t g_pps_received = 0;
/** The main function, never returns! */

void prvTCPServerTask( void * pvParameters)

{

        uint8_t port_string[12];
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
        send_msg((const uint8_t *)"CMD Server Lintening...\r\n");
        sprintf(&port_string,"PORT:%d\r\n",CMD_PORT);
        send_msg((const uint8_t *)port_string);

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
        for (i=0;i<4;i++)
            pps_packet[i]= (pps_counter >> 8*(3-i))&0xff;
        if(pps_sockfd == -1)
            pps_sockfd=tcpClientOpen(STATUS_PORT);
        if(pps_sockfd != -1)
           lwip_send(pps_sockfd, pps_packet, 4,0);
        g_pps_received = 0;
    }
    ethernetif_tick();
  }
}



void prvTLMTask( void * pvParameters)
{
  uint32_t tlm_packet_size = 0;
  uint8_t tlm_packet_buffer[4*TLM_MAX_WORDS];
  uint32_t words_sent;
  uint32_t words_to_send;
  uint32_t words_this_time;
  if(tlm_sockfd == -1)
    tlm_sockfd=tcpClientOpen(TLM_PORT);
  /* Clear Pending TLM Interrupt*/
  NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
  /* Enable Fabric Interrupt*/
  NVIC_EnableIRQ(FabricIrq1_IRQn);
  g_tlm_packet_arrived = 0;
  while(1)
  {
    // manually trigger ISR
    //if(fpgabase[TLM_INTERRUPT])
    //  FabricIrq1_IRQHandler();

    if(g_tlm_packet_arrived)
    {

      words_sent = 0;
      words_to_send = g_words_in_fifo;
      while(words_to_send)
      {
        words_this_time = (words_to_send > TLM_MAX_WORDS)?TLM_MAX_WORDS:words_to_send;
        words_to_send -= words_this_time; // save remaining words to be sent
        uint32_t i;
        for(i = 0; i < words_this_time; i++)
        {
           uint32_t j;
           uint32_t one_word;
           one_word = fpgabase[TLM_FIFO_READ];
           // put bytes intp buffer MSB first
           for (j=0;j<4;j++)
           {
              tlm_packet_buffer[4*i+j]= (one_word >> 8*(3-j))&0xff;
           }

        }

        if(tlm_sockfd == -1)
            tlm_sockfd=tcpClientOpen(TLM_PORT);
        if(tlm_sockfd != -1)
            lwip_send(tlm_sockfd, tlm_packet_buffer, 4*words_this_time,0);
      }

      g_tlm_packet_arrived = 0;
      NVIC_EnableIRQ(FabricIrq1_IRQn); // re-enable TLM interrupt
    }
  }
}

void FabricIrq0_IRQHandler(void)
{
    // reset the PPS signal
    fpgabase[PPS_CLEAR] = 0;
    // toggle LEDs
    fpgabase[LED] ^= 0x1;
    NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
    g_pps_received = 1;
}

void FabricIrq1_IRQHandler(void)
{
    NVIC_DisableIRQ(FabricIrq1_IRQn);  // disable TLM interrupt
    fpgabase[TLM_INTERRUPT_CLEAR] = 0; // clear the interrupt
    NVIC_ClearPendingIRQ(FabricIrq1_IRQn);
    g_words_in_fifo = fpgabase[TLM_FIFO_COUNTERS]&0x1fff;  // FiFO Read Counter
    // toggle LED
    fpgabase[LED] ^= 0x8;
    g_tlm_packet_arrived = 1;
}

void FabricIrq2_IRQHandler(void)
{
    fpgabase[LED] ^= 0x20;
    NVIC_ClearPendingIRQ(FabricIrq2_IRQn);
}



int32_t tcpClientOpen(uint32_t port)
{

        uint8_t ip_string[20];
        uint8_t port_string[12];


	int32_t sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		send_msg((const uint8_t *)"socket creation failed...\r\n");
                sprintf(&port_string,"PORT:%d\r\n",port);
                send_msg((const uint8_t *)port_string);
		return sockfd;
	}
	else
		send_msg((const uint8_t *)"Socket successfully created..\r\n");
                sprintf(&port_string,"PORT:%d\r\n",port);
                send_msg((const uint8_t *)port_string);
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
                sprintf(&port_string,"PORT:%d\r\n",port);
                send_msg((const uint8_t *)port_string);
		return sockfd;
	}
	else
		send_msg((const uint8_t *)"connected to the server..\r\n");
                sprintf(&port_string,"PORT:%d\r\n",port);
                send_msg((const uint8_t *)port_string);

	return sockfd;
}



uint16_t generate_crc(uint8_t *Data, uint16_t length)
{

    uint16_t crc;
    uint16_t index = 0;
    crc = 0xFFFF; //Initialization of crc to 0xFFFF for CCITT
	while (index < length){
		crc = crc_ccitt_update(crc,Data[index]);
		index++;
	}

    return crc;
}




uint16_t crc_ccitt_update(uint16_t crc, uint8_t  x)
{
  uint16_t crc_new = (uint8_t)(crc >> 8) | (crc << 8);
  crc_new ^= x;
  crc_new ^= (uint8_t)(crc_new & 0xff) >> 4;
  crc_new ^= crc_new << 12;
  crc_new ^= (crc_new & 0xff) << 5;
  crc = crc_new;
  return crc;
}

void generate_itf(uint32_t frame_sync, uint16_t apid, uint16_t sequence_number, uint8_t *data, uint16_t length, uint8_t *itf)
{
        uint32_t i;
        uint16_t crc;
        for (i=0;i<4;i++)
            itf[i]= (frame_sync >> 8*(3-i))&0xff;

        apid = (apid&0x1ff)|0x0800; //set sec header flag
        itf[4] = (apid>>8)&0xff;
        itf[5] = apid&0xff;
        sequence_number = (sequence_number&0x3fff)|0xc000; //set grouping flags
        itf[6] = (sequence_number>>8)&0xff;
        itf[7] = sequence_number&0xff;
        itf[8] = ((length-1)>>8)&0xff;
        itf[9] = (length-1)&0xff;

        for (i=0;i<length;i++)
            itf[i+10]= data[i];

        crc= generate_crc(itf,length+10);
        itf[length+10]= (crc>>8)&0xff;
        itf[length+11]= crc&0xff;

}
