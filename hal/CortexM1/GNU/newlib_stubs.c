/*******************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 * 
 * Stubs for Newlib system calls.
 *  
 * SVN $Revision: 4173 $
 * SVN $Date: 2012-03-01 05:50:48 +0000 (Thu, 01 Mar 2012) $
 */
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>
#undef errno
extern int errno;

/*==============================================================================
 * Redirection of standard output to a CoreUARTapb instance.
 *------------------------------------------------------------------------------
 * A default implementation for the redirection of the output of printf() to a
 * UART is provided as the bottom of this file. This redirection is enabled by
 * adding the symbol/define ACTEL_STDIO_THRU_CORE_UART_APB to your project and
 * specifying the base address of the CoreUARTapbinstanceand baud rate using the
 * ACTEL_STDIO_UART_BASE_ADDR and ACTEL_STDIO_BAUD_VALUE defines.
 */
#ifdef ACTEL_STDIO_THRU_CORE_UART_APB
#include "core_uart_apb.h"

#ifndef ACTEL_STDIO_UART_BASE_ADDR
/*
 * The ACTEL_STDIO_UART_BASE_ADDR define should be set in your project's settings
 * to specify the value of the base address of the CoreUARTapb instance which will
 * transmit the standard output. e.g. add ACTEL_STDIO_UART_BASE_ADDR=0xC3000000 to
 * the list of defined symbols in the compiler's settings.
 */
#error ACTEL_STDIO_UART_BASE_ADDR not defined
#endif

#ifndef ACTEL_STDIO_BAUD_VALUE
/*
 * The ACTEL_STDIO_BAUD_VALUE define should be set in your project's settings to
 * specify the baud value used by the standard output CoreUARTapb instance for
 * generating the UART's baud rate.
 */
#error ACTEL_STDIO_BAUD_VALUE not defined
#endif

/*------------------------------------------------------------------------------
 * CoreUARTapb instance data for the CoreUARTapb instance used for standard
 * output.
 */
static UART_instance_t g_stdio_uart;

/*------------------------------------------------------------------------------
 * Global flag used to indicate if the UART driver needs to be initialized.
 */
static int g_stdio_uart_init_done = 0;

#endif	/* STDIO_THRU_CORE_UART_APB */

/*==============================================================================
 * Environment variables.
 * A pointer to a list of environment variables and their values. For a minimal
 * environment, this empty list is adequate:
 */
char *__env[1] = { 0 };
char **environ = __env;

/*==============================================================================
 * Close a file.
 */
int _close(int file)
{
    return -1;
}

/*==============================================================================
 * Transfer control to a new process.
 */
int _execve(char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

/*==============================================================================
 * Exit a program without cleaning up files.
 */
void _exit( int code )
{
	/* Should we force a system reset? */
	while( 1 )
	{
		;
	}
}

/*==============================================================================
 * Create a new process.
 */
int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

/*==============================================================================
 * Status of an open file.
 */
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/*==============================================================================
 * Process-ID
 */
int _getpid(void)
{
    return 1;
}

/*==============================================================================
 * Query whether output stream is a terminal.
 */
int _isatty(int file)
{
    return 1;
}

/*==============================================================================
 * Send a signal.
 */
int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

/*==============================================================================
 * Establish a new name for an existing file.
 */
int _link(char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

/*==============================================================================
 * Set position in a file.
 */
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/*==============================================================================
 * Open a file.
 */
int _open(const char *name, int flags, int mode)
{
    return -1;
}

/*==============================================================================
 * Read from a file.
 */
int _read(int file, char *ptr, int len)
{
    return 0;
}

/*==============================================================================
 * Increase program data space. As malloc and related functions depend on this,
 * it is useful to have a working implementation. The following suffices for a
 * standalone system; it exploits the symbol _end automatically defined by the
 * GNU linker. 
 */
caddr_t _sbrk(int incr)
{
    extern char _end;		/* Defined by the linker */
    static char *heap_end;
    char *prev_heap_end;
    char * stack_ptr;
    
    if (heap_end == 0)
    {
      heap_end = &_end;
    }
    
    prev_heap_end = heap_end;
    asm volatile ("MRS %0, msp" : "=r" (stack_ptr) );
    if (heap_end + incr > stack_ptr)
    {
      write (1, "Heap and stack collision\n", 25);
      abort ();
    }
  
    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

/*==============================================================================
 * Status of a file (by name).
 */
int _stat(char *file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/*==============================================================================
 * Timing information for current process.
 */
int _times(struct tms *buf)
{
    return -1;
}

/*==============================================================================
 * Remove a file's directory entry.
 */
int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}

/*==============================================================================
 * Wait for a child process.
 */
int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

/*==============================================================================
 * Write to a file. libc subroutines will use this system routine for output to
 * all files, including stdout—so if you need to generate any output, for
 * example to a serial port for debugging, you should make your minimal write
 * capable of doing this.
 */
int _write_r( void * reent, int file, char * ptr, int len )
{
#ifdef ACTEL_STDIO_THRU_CORE_UART_APB
	/*--------------------------------------------------------------------------
	 * Initialize the UART driver if it is the first time this function is
	 * called.
	 */
	if ( !g_stdio_uart_init_done )
	{
		UART_init( &g_stdio_uart, ACTEL_STDIO_UART_BASE_ADDR, ACTEL_STDIO_BAUD_VALUE, (DATA_8_BITS | NO_PARITY));
		g_stdio_uart_init_done = 1;
	}
	
	/*--------------------------------------------------------------------------
	 * Output text to the UART.
	 */
	UART_send( &g_stdio_uart, (uint8_t *)ptr, len );
	
	return len;
#else	/* ACTEL_STDIO_THRU_CORE_UART_APB */
	return 0;
#endif	/* ACTEL_STDIO_THRU_CORE_UART_APB */
}


