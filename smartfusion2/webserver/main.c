/*******************************************************************************
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 MSS MAC example web server using FreeRTOS and the lwIP TCP/IP stack.
 * Please refer to the README.TXT in this project's root folder to know more.
 *
 * SVN $Revision: 8059 $
 * SVN $Date: 2015-10-20 12:00:25 +0530 (Tue, 20 Oct 2015) $
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* lwIP includes. */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"

/* CMSIS includes */
#include "CMSIS/system_m2sxxx.h"

/* Driver includes */
#include "drivers/mss_timer/mss_timer.h"
#include "drivers/mss_ethernet_mac/mss_ethernet_mac.h"
#include "ethernet_status.h"

/*==============================================================================
 *
 */
/* Priorities used by the various different tasks. */
#define mainuIP_TASK_PRIORITY                   (tskIDLE_PRIORITY + 2)
#define mainUART_TASK_PRIORITY                  (tskIDLE_PRIORITY + 1)
#define mainLinkStatus_TASK_PRIORITY            (tskIDLE_PRIORITY + 1)

/* Web server task stack size. */
#define HTTPD_STACK_SIZE                        400
#define UART_TASK_STACK_SIZE                    200
#define LINK_STATUS_TASK_STACK_SIZE             200

#define ETHERNET_STATUS_QUEUE_LENGTH    1
#define DONT_BLOCK                      0

/*==============================================================================
 *
 */
 void ethernetif_tick(void);

/*
 * Setup the hardware platform.
 */
static void prvSetupHardware(void);

/*
 * Ethernet interface configuration function.
 */
static void prvEthernetConfigureInterface(void * param);

/*==============================================================================
 *
 */
/* The queue used by PTPd task to trnsmit status information to webserver task. */
xQueueHandle xPTPdOutQueue = NULL;

/*
 * The queue used by the EthLinkStatus task to communicate link speed/duplex
 * changes to the http_server task.
 */
xQueueHandle xEthStatusQueue = NULL;

/* lwIP MAC configuration. */
static struct netif s_EMAC_if;

/*==============================================================================
 *
 */
void prvUARTTask(void * pvParameters);
void prvLinkStatusTask(void * pvParameters);
void http_server_netconn_thread(void *arg);

/*==============================================================================
 *
 */
int main()
{

    /* Configure hardware platform. */
    prvSetupHardware();

    /*
     * Create the queue used by the EthLinkStatus task to communicate link
     * speed/duplex changes to the http_server task.
     */
    xEthStatusQueue = xQueueCreate(ETHERNET_STATUS_QUEUE_LENGTH, sizeof(ethernet_status_t));

    if(xEthStatusQueue != NULL)
    {
        /* Create the task handling user interractions through the UART. */
        xTaskCreate(prvUARTTask,
                    (signed char *) "UART",
                    UART_TASK_STACK_SIZE,
                    NULL,
                    mainUART_TASK_PRIORITY,
                    NULL);

        /* Create the web server task. */
        tcpip_init(prvEthernetConfigureInterface, NULL);
        xTaskCreate(http_server_netconn_thread,
                    (signed char *) "http_server",
                    HTTPD_STACK_SIZE,
                    NULL,
                    mainuIP_TASK_PRIORITY,
                    NULL );

        /* Create the task the Ethernet link status. */
        xTaskCreate(prvLinkStatusTask,
                    (signed char *) "EthLinkStatus",
                    LINK_STATUS_TASK_STACK_SIZE,
                    NULL,
                    mainLinkStatus_TASK_PRIORITY,
                    NULL);

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /*
     * If all is well, the scheduler will now be running, and the following line
     * will never be reached.  If the following line does execute, then there
     * was insufficient FreeRTOS heap memory available for the idle and/or timer
     * tasks to be created.  See the memory management section on the FreeRTOS
     * web site for more details.
     */
    for( ;; );
}

/*==============================================================================
 *
 */
static void prvSetupHardware( void )
{
    /*turn off the watchdog*/
    SYSREG->WDOG_CR = 0;

    /* Update global variables containing the system clock frequencies.*/
    SystemCoreClockUpdate();

    /* Enable cache. */
    SYSREG->CC_CR = 1u;

    /* Set interrupt priorities to a sensible default value. */
    NVIC_SetPriority(WdogWakeup_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(RTC_Wakeup_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C0_SMBAlert_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C0_SMBus_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C1_SMBAlert_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C1_SMBus_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(UART0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(UART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(EthernetMAC_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(DMA_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Timer1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Timer2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CAN_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(ENVM0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(ENVM1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(ComBlk_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(USB_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(USB_DMA_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(PLL_Lock_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(PLL_LockLost_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CommSwitchError_IR, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CacheError_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(DDR_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(HPDMA_Complete_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(HPDMA_Error_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(ECC_Error_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(MDDR_IOCalib_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FAB_PLL_Lock_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FAB_PLL_LockLost_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FIC64_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq8_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq9_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq10_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq11_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq12_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq13_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq14_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FabricIrq15_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO8_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO9_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO10_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO11_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO12_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO13_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO14_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO15_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO16_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO17_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO18_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO19_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO20_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO21_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO22_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO23_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO24_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO25_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO26_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO27_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO28_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO29_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO30_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO31_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
}

/*==============================================================================
 *
 */
void vApplicationMallocFailedHook( void )
{
    /*
     * Called if a call to pvPortMalloc() fails because there is insufficient
     * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
     * internally by FreeRTOS API functions that create tasks, queues, software
     * timers, and semaphores.  The size of the FreeRTOS heap is set by the
     * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
     */
    for( ;; );
}

/*==============================================================================
 *
 */
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
    (void)pcTaskName;
    (void)pxTask;

    /*
     * Run time stack overflow checking is performed if
     * configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected.
     */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

/*==============================================================================
 *
 */
void vApplicationIdleHook( void )
{
    volatile size_t xFreeStackSpace;

    /*
     * This function is called on each cycle of the idle task.  In this case it
     * does nothing useful, other than report the amount of FreeRTOS heap that
     * remains unallocated.
     */
    xFreeStackSpace = xPortGetFreeHeapSize();

    if( xFreeStackSpace > 100 )
    {
        /*
         * By now, the kernel has allocated everything it is going to, so if
         * there is a lot of heap remaining unallocated then the value of
         * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be reduced accordingly.
         */
    }
}

/*==============================================================================
 *
 */
void vMainConfigureTimerForRunTimeStats( void )
{
const unsigned long ulMax32BitValue = 0xffffffffUL;

    MSS_TIM64_init( MSS_TIMER_PERIODIC_MODE );
    MSS_TIM64_load_immediate( ulMax32BitValue, ulMax32BitValue );
    MSS_TIM64_start();
}

/*==============================================================================
 *
 */
unsigned long ulGetRunTimeCounterValue( void )
{
    unsigned long long ullCurrentValue = 0u;
    const unsigned long long ulMax64BitValue = 0xffffffffffffffffULL;
    unsigned long *pulHighWord, *pulLowWord;

    pulHighWord = (unsigned long *)&ullCurrentValue;
    pulLowWord = pulHighWord++;

    MSS_TIM64_get_current_value((uint32_t *)pulHighWord, (uint32_t *)pulLowWord);

    /* Convert the down count into an upcount. */
    ullCurrentValue = ulMax64BitValue - ullCurrentValue;

    /* Scale to a 32bit number of suitable frequency. */
    ullCurrentValue >>= 13;

    /* Just return 32 bits. */
    return (unsigned long)ullCurrentValue;
}

/*==============================================================================
 *
 */
static void prvEthernetConfigureInterface(void * param)
{
    struct ip_addr xIpAddr, xNetMast, xGateway;
    extern err_t ethernetif_init( struct netif *netif );

    /* Parameters are not used - suppress compiler error. */
    ( void ) param;

    /* Create and configure the EMAC interface. */
#ifdef NET_USE_DHCP
    IP4_ADDR( &xIpAddr, 0, 0, 0, 0 );
    IP4_ADDR( &xGateway, 192, 168, 1, 254 );
#else
    IP4_ADDR( &xIpAddr, 169, 254, 1, 23 );
    IP4_ADDR( &xGateway, 169, 254, 1, 23 );
#endif

    IP4_ADDR( &xNetMast, 255, 255, 255, 0 );

    netif_add( &s_EMAC_if, &xIpAddr, &xNetMast, &xGateway, NULL, ethernetif_init, tcpip_input );

    /* bring it up */

#ifdef NET_USE_DHCP
    dhcp_start(&s_EMAC_if);
#else
    netif_set_up(&s_EMAC_if);
#endif

    /* make it the default interface */
    netif_set_default(&s_EMAC_if);
}

/*==============================================================================
 *
 */
uint32_t get_ip_address(void)
{
    return (uint32_t)(s_EMAC_if.ip_addr.addr);
}

/*==============================================================================
 *
 */
void get_mac_address(uint8_t * mac_addr)
{
    uint32_t inc;

    for(inc = 0; inc < 6; ++inc)
    {
        mac_addr[inc] = s_EMAC_if.hwaddr[inc];
    }
}

/*==============================================================================
 *
 */
void prvLinkStatusTask(void * pvParameters)
{
    ethernet_status_t status;

    status.speed = MSS_MAC_10MBPS;
    status.duplex_mode = 0xFFU;

    for(;;)
    {
        volatile uint8_t linkup;
        uint8_t fullduplex;
        mss_mac_speed_t speed;

        /* Run through loop every 500 milliseconds. */
        vTaskDelay(500 / portTICK_RATE_MS);

        linkup = MSS_MAC_get_link_status(&speed,  &fullduplex);
        if((speed != status.speed) || (fullduplex != status.duplex_mode))
        {
            status.speed = speed;
            status.duplex_mode = fullduplex;
            xQueueSend(xEthStatusQueue, &status, DONT_BLOCK );
        }

        ethernetif_tick();
    }
}
