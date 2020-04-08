/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 *
 * Cortex-M1 default exception handlers.
 *  
 * SVN $Revision: 924 $
 * SVN $Date: 2009-05-19 16:46:37 +0100 (Tue, 19 May 2009) $
 */

 /***************************************************************************//**
 * Non Maskable Interrupt.
 */
__attribute__((__interrupt__)) void NMI_Handler( void )
{
	while( 1 )
	{
		;
	}
}
 
/***************************************************************************//**
 * Hard Fault.
 */
__attribute__((__interrupt__)) void HardFault_Handler( void )
{
	while( 1 )
	{
		;
	}
}

/***************************************************************************//**
 * SVCall.
 */
__attribute__((__interrupt__)) void SVC_Handler( void )
{
	while( 1 )
	{
		;
	}
}

/***************************************************************************//**
 * PendSV.
 */
__attribute__((__interrupt__)) void PendSV_Handler( void )
{
	while( 1 )
	{
		;
	}
}
