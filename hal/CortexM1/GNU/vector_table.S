/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * Cortex-M1 vector table.
 *
 * SVN $Revision: 6670 $
 * SVN $Date: 2014-07-04 12:03:10 +0100 (Fri, 04 Jul 2014) $
 */
		.text
		.section .isr_vector
		
		.code 16
		.syntax unified
		
		.type HW_set_32bit_reg, function

        .global   NMI_Handler
        .global   HardFault_Handler
        .global   SVC_Handler
        .global   PendSV_Handler
        
        .global  SysTick_Handler
        .global  IRQ0_IRQHandler
        .global  IRQ1_IRQHandler
        .global  IRQ2_IRQHandler
        .global  IRQ3_IRQHandler
        .global  IRQ4_IRQHandler
        .global  IRQ5_IRQHandler
        .global  IRQ6_IRQHandler
        .global  IRQ7_IRQHandler
        
        .weak  SysTick_Handler
        .weak  IRQ0_IRQHandler
        .weak  IRQ1_IRQHandler
        .weak  IRQ2_IRQHandler
        .weak  IRQ3_IRQHandler
        .weak  IRQ4_IRQHandler
        .weak  IRQ5_IRQHandler
        .weak  IRQ6_IRQHandler
        .weak  IRQ7_IRQHandler
        
vector_table:
        .word    __main_stack_start
        .word    _start
        .word    NMI_Handler             /* NMI handler */
        .word    HardFault_Handler       /* Fault handler */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    0                       /* Reserved */
        .word    SVC_Handler             /* System service call with SVC instruction. */
        .word    0                       /* Reserved. */
        .word    0                       /* Reserved. */
        .word    PendSV_Handler          /* Pendable request for system service. */
        .word    SysTick_Handler         /* System tick timer. */
        .word    IRQ0_IRQHandler         /* External interrupt 0. */
        .word    IRQ1_IRQHandler        /* External interrupt 1. */
        .word    IRQ2_IRQHandler        /* External interrupt 2. */
        .word    IRQ3_IRQHandler        /* External interrupt 3. */
        .word    IRQ4_IRQHandler        /* External interrupt 4. */
        .word    IRQ5_IRQHandler        /* External interrupt 5. */
        .word    IRQ6_IRQHandler        /* External interrupt 6. */
        .word    IRQ7_IRQHandler        /* External interrupt 7. */

.end

