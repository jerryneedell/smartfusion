/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 * 
 * SVN $Revision: 190 $
 * SVN $Date: 2008-03-13 16:23:03 +0000 (Thu, 13 Mar 2008) $
 */
#ifndef __CPU_TYPES_H
#define __CPU_TYPES_H	1

#include <stdint.h>

/*------------------------------------------------------------------------------
 */
typedef unsigned int size_t;

/*------------------------------------------------------------------------------
 * addr_t: address type.
 * Used to specify the address of peripherals present in the processor's memory
 * map.
 */
typedef unsigned int addr_t;

/*------------------------------------------------------------------------------
 * psr_t: processor state register.
 * Used by HAL_disable_interrupts() and HAL_restore_interrupts() to store the
 * processor's state between disabling and restoring interrupts.
 */
typedef unsigned int psr_t;

#endif	/* __CPU_TYPES_H */
