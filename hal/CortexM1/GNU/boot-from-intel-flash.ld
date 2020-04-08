/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * Cortex-M1 example linker script: Boot from Intel 28F640 (2x16) Flash memory
 *
 * SVN $Revision: 7510 $
 * SVN $Date: 2015-07-01 09:41:12 +0100 (Wed, 01 Jul 2015) $
 */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
	      "elf32-littlearm")
GROUP(-lc -lgcc -lm)
OUTPUT_ARCH(arm)
ENTRY(_start)
SEARCH_DIR(.)
__DYNAMIC  =  0;

/*******************************************************************************
 * Start of board customization.
 *******************************************************************************/
MEMORY
{
  /*
   * WARNING: The words "SOFTCONSOLE", "FLASH", and "USE", the colon ":", and
   *          the name of the type of flash memory are all in a specific order.
   *          Please do not modify that comment line, in order to ensure
   *          debugging of your application will use the flash memory correctly.
   */

   /* SOFTCONSOLE FLASH USE: intel-28f640-2x16 */
  rom (rx)  : ORIGIN = 0x00000000, LENGTH = 1M
  
  /* Normal SRAM */
  ram (rwx) : ORIGIN = 0x10000000, LENGTH = 1M
}

RAM_START_ADDRESS = 0x10000000;	/* Must be the same value MEMORY region ram ORIGIN above. */
RAM_SIZE = 1M;					/* Must be the same value MEMORY region ram LENGTH above. */
MAIN_STACK_SIZE = 256k;			/* Cortex main stack size. */
PROCESS_STACK_SIZE	= 16k;		/* Cortex process stack size (only available with OS extensions).*/

/*******************************************************************************
 * End of board customization.
 *******************************************************************************/
 
PROVIDE (__main_stack_start = RAM_START_ADDRESS + RAM_SIZE);
PROVIDE (__process_stack_start = __main_stack_start - MAIN_STACK_SIZE);

SECTIONS
{
  .reset :
  {
    KEEP (*(.isr_vector))
    KEEP (*sys_boot.o(.text))
    . = ALIGN(0x4);
  } >rom
  
  .text :
  {
    CREATE_OBJECT_SYMBOLS
    __text_load = LOADADDR(.text);
    __text_start = .;
    
    *(.text .text.* .gnu.linkonce.t.*)
    *(.plt)
    *(.gnu.warning)
    *(.glue_7t) *(.glue_7) *(.vfp11_veneer)

    . = ALIGN(0x4);
    /* These are for running static constructors and destructors under ELF.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))

    *(.rodata .rodata.* .gnu.linkonce.r.*)

    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.gcc_except_table) 
    *(.eh_frame_hdr)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ram AT>rom
  /* .ARM.exidx is sorted, so has to go in its own output section.  */
   __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >ram AT>rom
  __exidx_end = .;
  _etext = .;

  .data :
  {
    __data_load = LOADADDR(.data);
    __data_start = .;
    KEEP(*(.jcr))
    *(.got.plt) *(.got)
    *(.shdata)
    *(.data .data.* .gnu.linkonce.d.*)
    . = ALIGN (4);
  	_edata = .;
  } >ram AT>rom

  .bss :
  {
    __bss_start__ = . ;
    *(.shbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN (8);
    __bss_end__ = .;
    _end = .;
    __end = _end;
    PROVIDE(end = .);
  } >ram AT>rom

  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }

  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack)  }
}
