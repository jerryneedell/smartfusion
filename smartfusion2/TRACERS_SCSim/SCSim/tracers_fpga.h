#ifndef __TRACERS_FPGA_H__
#define __TRACERS_FPGA_H__

#define DATECODE			0x20220418
#define VERSION				0x01

#define LED 				0
#define BUTTON 				0x20/4
#define SW5 				0x24/4
#define FPGA_DATECODE                   0x80/4
#define FPGA_BUILD                      0x84/4
#define PPS_CLEAR			0x10000C/4
#define TLM_FIFO_READ 			0x100000/4
#define TLM_FIFO_STATUS 		0x100010/4
#define TLM_FIFO_CLEAR 			0x100010/4
#define TLM_FIFO_COUNTERS 		0x100014/4

#define PPS_COUNT_LIMIT  	 	0x100020/4
#define PPS_PULSE_WIDTH 		0x100024/4
#define PPS_INTERRUPT_DELAY		0x100028/4

#define TLM_XMIT_WRITE 			0x100030/4
#define TLM_XMIT_STATUS 		0x100040/4
#define TLM_XMIT_CLEAR 			0x100040/4
#define TLM_XMIT_COUNTERS 		0x100044/4
#define TLM_XMIT_START	 		0x100048/4
#define TLM_INTERRUPT		 	0x10004C/4
#define TLM_INTERRUPT_CLEAR 		0x10004C/4

#define LOOPBACK_UART_WRITE		0x200000/4
#define LOOPBACK_UART_READ		0x200004/4
#define LOOPBACK_UART_CR1		0x200008/4
#define LOOPBACK_UART_CR2		0x20000C/4
#define LOOPBACK_UART_STATUS		0x200010/4
#define LOOPBACK_UART_CR3		0x200014/4

#define FIFO_EMPTY			0x1
#define FIFO_FULL			0x2
#define FIFO_UNDERFLOW			0x10
#define FIFO_OVERFLOW			0x20
#define FIFO_FAIL			0x100

#define HK_PORT				8000
#define TLM_PORT			8001
#define STATUS_PORT			8002
#define CMD_PORT			8003

#define TLM_MAX_WORDS			0x40
#define CMD_MAX_BYTES			0x200
#define UART0_RX_BYTES			0x400

#define PPS_PULSE_WIDTH_INIT 		24999999
//#define PPS_INTERRUPT_DELAY_INIT	9999999

#endif /* __TRACERS_FPGA_H__ */
