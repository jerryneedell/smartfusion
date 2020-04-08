/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 * 
 * Cortex-M1 Nested Vectored Interrupt Controller (NVIC) driver implementation.
 * 
 * SVN $Revision: 231 $
 * SVN $Date: 2008-04-03 15:11:02 +0100 (Thu, 03 Apr 2008) $
 */
#include "cpu_types.h"
#include "hal.h"
#include "cortex_nvic.h"
#include "cortex_nvic_regs.h"

/***************************************************************************//**
 * Base address of the Nested Vectored Interrupt Controller.
 * This address is constant regardless of the FPGA design. 
 */
#define NVIC_BASE_ADDR	0xE000E000UL

/***************************************************************************//**
 * Defines used with write-clear registers.
 */
#define DISABLE_INTERRUPTS  0xFFFFFFFFUL
#define CLEAR_INTERRUPTS  0xFFFFFFFFUL

/***************************************************************************//**
 * NVIC_init()
 * See "cortex_nvic.h" for details of how to use this function.
 */
void NVIC_init( void )
{
    /* Disable external interrupts. */
	HAL_set_32bit_reg(NVIC_BASE_ADDR, CLRENA, DISABLE_INTERRUPTS );

    /* Clear pending external interrupts. */
    HAL_set_32bit_reg(NVIC_BASE_ADDR, CLRPEND, CLEAR_INTERRUPTS );
}

/***************************************************************************//**
 * NVIC_set_priority()
 * See "cortex_nvic.h" for details of how to use this function.
 */
void NVIC_set_priority
(
	uint32_t interrupt_nb,
	uint32_t priority_level
)
{
}

/***************************************************************************//**
 * NVIC_enable_interrupt()
 * See "cortex_nvic.h" for details of how to use this function.
 */
void NVIC_enable_interrupt( uint32_t interrupt_nb )
{
    uint32_t int_mask = 1 << interrupt_nb;
	HAL_set_32bit_reg( NVIC_BASE_ADDR, SETENA, int_mask );
}

/***************************************************************************//**
 * NVIC_disable_interrupt()
 * See "cortex_nvic.h" for details of how to use this function.
 */
void NVIC_disable_interrupt( uint32_t interrupt_nb )
{
    uint32_t int_mask = 1 << interrupt_nb;
	HAL_set_32bit_reg(NVIC_BASE_ADDR, CLRENA, int_mask );
}

/***************************************************************************//**
 * NVIC_clear_interrupt()
 * See "cortex_nvic.h" for details of how to use this function.
 */
void NVIC_clear_interrupt( uint32_t interrupt_nb )
{
    uint32_t int_mask = 1 << interrupt_nb;
	HAL_set_32bit_reg(NVIC_BASE_ADDR, CLRPEND, int_mask );
}
