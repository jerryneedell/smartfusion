/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 * 
 * Cortex-M1 boot code.
 * 
 * SVN $Revision: 190 $
 * SVN $Date: 2008-03-13 16:23:03 +0000 (Thu, 13 Mar 2008) $
 */

/*------------------------------------------------------------------------------
 * main() function prototype as is called from this file.
 */
int main(void);

/*------------------------------------------------------------------------------
 * Symbols from the linker script used to locate the text, data and bss sections.
 */
extern const unsigned int __stack;

extern unsigned int __text_load;
extern unsigned int __text_start;
extern unsigned int _etext;

extern unsigned int __data_load;
extern unsigned int __data_start;
extern unsigned int _edata;

extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

/*------------------------------------------------------------------------------
 * _start() function called invoked on power up and warm reset.
 */
void _start( void)
{
	/*
	 * Copy text section if required (copy executable from LMA to VMA).
	 */
	{
		unsigned int * text_lma = &__text_load;
		unsigned int * end_text_vma = &_etext;
		unsigned int * text_vma = &__text_start;
		
		if ( text_vma != text_lma)
		{
			while ( text_vma <= end_text_vma)
			{
				*text_vma++ = *text_lma++;
			}
		}
	}

	/*
	 * Copy data section if required (initialised variables).
	 */
	{
		unsigned int * data_lma = &__data_load;
		unsigned int * end_data_vma = &_edata;
		unsigned int * data_vma = &__data_start;
		
		if ( data_vma != data_lma )
		{
			while ( data_vma <= end_data_vma )
			{
				*data_vma++ = *data_lma++;
			}
		}
	}
	
	/*
	 * Zero out the bss section (set non-initialised variables to 0). 
	 */
	{
		unsigned int * bss = &__bss_start__;
		unsigned int * bss_end = &__bss_end__;
		
		if ( bss_end > bss)
		{
			while ( bss <= bss_end )
			{
				*bss++ = 0;
			}
		}
	}
	
	/*
	 * Call main.
	 */
	main();
	
	/*
	 * We should never return from _start().
	 */
	while( 1 )
	{
		;
	}
}
 
