#ifndef __TRACERS_FPGA_H__
#define __TRACERS_FPGA_H__

#define DATECODE                        0x20210627
#define VERSION                         0x00

#define LED 				0
#define BUTTON 				0x20/4
#define SW5 				0x24/4
#define PPS_CLEAR			0x10000C/4

#define PPS_COUNT_LIMIT_FIFO_READ 	0x100020/4
#define PPS_PULSE_WIDTH 		0x100020/4
#define PPS_INTERRUPT_DELAY		0x100028/4

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
#define STATUS_PORT			8002
#define CMD_PORT			8003

#define CMD_MAX_BYTES			0x200
#define UART0_RX_BYTES			0x400


#endif /* __TRACERS_FPGA_H__ */
