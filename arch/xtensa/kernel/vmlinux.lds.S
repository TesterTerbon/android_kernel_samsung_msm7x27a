/*
 * arch/xtensa/kernel/vmlinux.lds.S
 *
 * Xtensa linker script
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2008 Tensilica Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Chris Zankel <chris@zankel.net>
 * Marc Gauthier <marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 */

#include <asm-generic/vmlinux.lds.h>
#include <asm/page.h>
#include <asm/thread_info.h>

<<<<<<< HEAD
=======
#include <asm/vectors.h>
>>>>>>> refs/remotes/origin/master
#include <variant/core.h>
#include <platform/hardware.h>
OUTPUT_ARCH(xtensa)
ENTRY(_start)

#ifdef __XTENSA_EB__
jiffies = jiffies_64 + 4;
#else
jiffies = jiffies_64;
#endif

#ifndef KERNELOFFSET
<<<<<<< HEAD
#define KERNELOFFSET 0xd0001000
=======
#define KERNELOFFSET 0xd0003000
>>>>>>> refs/remotes/origin/master
#endif

/* Note: In the following macros, it would be nice to specify only the
   vector name and section kind and construct "sym" and "section" using
   CPP concatenation, but that does not work reliably.  Concatenating a
   string with "." produces an invalid token.  CPP will not print a
   warning because it thinks this is an assembly file, but it leaves
   them as multiple tokens and there may or may not be whitespace
   between them.  */

/* Macro for a relocation entry */

#define RELOCATE_ENTRY(sym, section)		\
	LONG(sym ## _start);			\
	LONG(sym ## _end);			\
	LONG(LOADADDR(section))

/* Macro to define a section for a vector.
 *
 * Use of the MIN function catches the types of errors illustrated in
 * the following example:
 *
 * Assume the section .DoubleExceptionVector.literal is completely
 * full.  Then a programmer adds code to .DoubleExceptionVector.text
 * that produces another literal.  The final literal position will
 * overlay onto the first word of the adjacent code section
 * .DoubleExceptionVector.text.  (In practice, the literals will
 * overwrite the code, and the first few instructions will be
 * garbage.)
 */

#define SECTION_VECTOR(sym, section, addr, max_prevsec_size, prevsec)       \
  section addr : AT((MIN(LOADADDR(prevsec) + max_prevsec_size,		    \
		         LOADADDR(prevsec) + SIZEOF(prevsec)) + 3) & ~ 3)   \
  {									    \
    . = ALIGN(4);							    \
    sym ## _start = ABSOLUTE(.);		 			    \
    *(section)								    \
    sym ## _end = ABSOLUTE(.);						    \
  }

/*
 *  Mapping of input sections to output sections when linking.
 */

SECTIONS
{
  . = KERNELOFFSET;
  /* .text section */

  _text = .;
  _stext = .;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  _ftext = .;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

  .text :
  {
    /* The HEAD_TEXT section must be the first section! */
    HEAD_TEXT
    TEXT_TEXT
    VMLINUX_SYMBOL(__sched_text_start) = .;
    *(.sched.literal .sched.text)
    VMLINUX_SYMBOL(__sched_text_end) = .;
    VMLINUX_SYMBOL(__lock_text_start) = .;
    *(.spinlock.literal .spinlock.text)
    VMLINUX_SYMBOL(__lock_text_end) = .;

  }
  _etext = .;
  PROVIDE (etext = .);

  . = ALIGN(16);

  RODATA

  /*  Relocation table */

  .fixup   : { *(.fixup) }

  EXCEPTION_TABLE(16)
  /* Data section */

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  _fdata = .;
=======
  _sdata = .;
>>>>>>> refs/remotes/origin/cm-10.0
=======
  _sdata = .;
>>>>>>> refs/remotes/origin/master
=======
  _sdata = .;
>>>>>>> refs/remotes/origin/cm-11.0
  RW_DATA_SECTION(XCHAL_ICACHE_LINESIZE, PAGE_SIZE, THREAD_SIZE)
  _edata = .;

  /* Initialization code and data: */

  . = ALIGN(PAGE_SIZE);
  __init_begin = .;
  INIT_TEXT_SECTION(PAGE_SIZE)

  .init.data :
  {
    INIT_DATA
    . = ALIGN(0x4);
    __tagtable_begin = .;
    *(.taglist)
    __tagtable_end = .;

    . = ALIGN(16);
    __boot_reloc_table_start = ABSOLUTE(.);

    RELOCATE_ENTRY(_WindowVectors_text,
		   .WindowVectors.text);
<<<<<<< HEAD
=======
#if XCHAL_EXCM_LEVEL >= 2
    RELOCATE_ENTRY(_Level2InterruptVector_text,
		   .Level2InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 3
    RELOCATE_ENTRY(_Level3InterruptVector_text,
		   .Level3InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 4
    RELOCATE_ENTRY(_Level4InterruptVector_text,
		   .Level4InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 5
    RELOCATE_ENTRY(_Level5InterruptVector_text,
		   .Level5InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 6
    RELOCATE_ENTRY(_Level6InterruptVector_text,
		   .Level6InterruptVector.text);
#endif
>>>>>>> refs/remotes/origin/master
    RELOCATE_ENTRY(_KernelExceptionVector_text,
		   .KernelExceptionVector.text);
    RELOCATE_ENTRY(_UserExceptionVector_text,
		   .UserExceptionVector.text);
    RELOCATE_ENTRY(_DoubleExceptionVector_literal,
		   .DoubleExceptionVector.literal);
    RELOCATE_ENTRY(_DoubleExceptionVector_text,
		   .DoubleExceptionVector.text);
    RELOCATE_ENTRY(_DebugInterruptVector_text,
		   .DebugInterruptVector.text);
  
    __boot_reloc_table_end = ABSOLUTE(.) ;

    INIT_SETUP(XCHAL_ICACHE_LINESIZE)
    INIT_CALLS
    CON_INITCALL
    SECURITY_INITCALL
    INIT_RAM_FS
  }

  PERCPU_SECTION(XCHAL_ICACHE_LINESIZE)

  /* We need this dummy segment here */

  . = ALIGN(4);
  .dummy : { LONG(0) }

  /* The vectors are relocated to the real position at startup time */

  SECTION_VECTOR (_WindowVectors_text,
		  .WindowVectors.text,
<<<<<<< HEAD
		  XCHAL_WINDOW_VECTORS_VADDR, 4,
		  .dummy)
  SECTION_VECTOR (_DebugInterruptVector_literal,
		  .DebugInterruptVector.literal,
		  XCHAL_DEBUG_VECTOR_VADDR - 4,
=======
		  WINDOW_VECTORS_VADDR, 4,
		  .dummy)
  SECTION_VECTOR (_DebugInterruptVector_literal,
		  .DebugInterruptVector.literal,
		  DEBUG_VECTOR_VADDR - 4,
>>>>>>> refs/remotes/origin/master
		  SIZEOF(.WindowVectors.text),
		  .WindowVectors.text)
  SECTION_VECTOR (_DebugInterruptVector_text,
		  .DebugInterruptVector.text,
<<<<<<< HEAD
		  XCHAL_DEBUG_VECTOR_VADDR,
		  4,
		  .DebugInterruptVector.literal)
  SECTION_VECTOR (_KernelExceptionVector_literal,
		  .KernelExceptionVector.literal,
		  XCHAL_KERNEL_VECTOR_VADDR - 4,
		  SIZEOF(.DebugInterruptVector.text),
		  .DebugInterruptVector.text)
  SECTION_VECTOR (_KernelExceptionVector_text,
		  .KernelExceptionVector.text,
		  XCHAL_KERNEL_VECTOR_VADDR,
=======
		  DEBUG_VECTOR_VADDR,
		  4,
		  .DebugInterruptVector.literal)
#undef LAST
#define LAST	.DebugInterruptVector.text
#if XCHAL_EXCM_LEVEL >= 2
  SECTION_VECTOR (_Level2InterruptVector_text,
		  .Level2InterruptVector.text,
		  INTLEVEL2_VECTOR_VADDR,
		  SIZEOF(LAST), LAST)
# undef LAST
# define LAST	.Level2InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 3
  SECTION_VECTOR (_Level3InterruptVector_text,
		  .Level3InterruptVector.text,
		  INTLEVEL3_VECTOR_VADDR,
		  SIZEOF(LAST), LAST)
# undef LAST
# define LAST	.Level3InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 4
  SECTION_VECTOR (_Level4InterruptVector_text,
		  .Level4InterruptVector.text,
		  INTLEVEL4_VECTOR_VADDR,
		  SIZEOF(LAST), LAST)
# undef LAST
# define LAST	.Level4InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 5
  SECTION_VECTOR (_Level5InterruptVector_text,
		  .Level5InterruptVector.text,
		  INTLEVEL5_VECTOR_VADDR,
		  SIZEOF(LAST), LAST)
# undef LAST
# define LAST	.Level5InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 6
  SECTION_VECTOR (_Level6InterruptVector_text,
		  .Level6InterruptVector.text,
		  INTLEVEL6_VECTOR_VADDR,
		  SIZEOF(LAST), LAST)
# undef LAST
# define LAST	.Level6InterruptVector.text
#endif
  SECTION_VECTOR (_KernelExceptionVector_literal,
		  .KernelExceptionVector.literal,
		  KERNEL_VECTOR_VADDR - 4,
		  SIZEOF(LAST), LAST)
#undef LAST
  SECTION_VECTOR (_KernelExceptionVector_text,
		  .KernelExceptionVector.text,
		  KERNEL_VECTOR_VADDR,
>>>>>>> refs/remotes/origin/master
		  4,
		  .KernelExceptionVector.literal)
  SECTION_VECTOR (_UserExceptionVector_literal,
		  .UserExceptionVector.literal,
<<<<<<< HEAD
		  XCHAL_USER_VECTOR_VADDR - 4,
=======
		  USER_VECTOR_VADDR - 4,
>>>>>>> refs/remotes/origin/master
		  SIZEOF(.KernelExceptionVector.text),
		  .KernelExceptionVector.text)
  SECTION_VECTOR (_UserExceptionVector_text,
		  .UserExceptionVector.text,
<<<<<<< HEAD
		  XCHAL_USER_VECTOR_VADDR,
=======
		  USER_VECTOR_VADDR,
>>>>>>> refs/remotes/origin/master
		  4,
		  .UserExceptionVector.literal)
  SECTION_VECTOR (_DoubleExceptionVector_literal,
		  .DoubleExceptionVector.literal,
<<<<<<< HEAD
		  XCHAL_DOUBLEEXC_VECTOR_VADDR - 16,
=======
		  DOUBLEEXC_VECTOR_VADDR - 16,
>>>>>>> refs/remotes/origin/master
		  SIZEOF(.UserExceptionVector.text),
		  .UserExceptionVector.text)
  SECTION_VECTOR (_DoubleExceptionVector_text,
		  .DoubleExceptionVector.text,
<<<<<<< HEAD
		  XCHAL_DOUBLEEXC_VECTOR_VADDR,
=======
		  DOUBLEEXC_VECTOR_VADDR,
>>>>>>> refs/remotes/origin/master
		  32,
		  .DoubleExceptionVector.literal)

  . = (LOADADDR( .DoubleExceptionVector.text ) + SIZEOF( .DoubleExceptionVector.text ) + 3) & ~ 3;
  . = ALIGN(PAGE_SIZE);

  __init_end = .;

  BSS_SECTION(0, 8192, 0)

  _end = .;

  /* only used by the boot loader  */

  . = ALIGN(0x10);
  .bootstrap : { *(.bootstrap.literal .bootstrap.text .bootstrap.data) }

<<<<<<< HEAD
  . = ALIGN(0x1000);
  __initrd_start = .;
  .initrd : { *(.initrd) }
  __initrd_end = .;

  .ResetVector.text XCHAL_RESET_VECTOR_VADDR :
=======
  .ResetVector.text RESET_VECTOR_VADDR :
>>>>>>> refs/remotes/origin/master
  {
    *(.ResetVector.text)
  }

<<<<<<< HEAD
=======

  /*
   * This is a remapped copy of the Secondary Reset Vector Code.
   * It keeps gdb in sync with the PC after switching
   * to the temporary mapping used while setting up
   * the V2 MMU mappings for Linux.
   *
   * Only debug information about this section is put in the kernel image.
   */
  .SecondaryResetVector.remapped_text 0x46000000 (INFO):
  {
	*(.SecondaryResetVector.remapped_text)
  }


>>>>>>> refs/remotes/origin/master
  .xt.lit : { *(.xt.lit) }
  .xt.prop : { *(.xt.prop) }

  .debug  0 :  { *(.debug) }
  .line  0 :  { *(.line) }
  .debug_srcinfo  0 :  { *(.debug_srcinfo) }
  .debug_sfnames  0 :  { *(.debug_sfnames) }
  .debug_aranges  0 :  { *(.debug_aranges) }
  .debug_pubnames  0 :  { *(.debug_pubnames) }
  .debug_info  0 :  { *(.debug_info) }
  .debug_abbrev  0 :  { *(.debug_abbrev) }
  .debug_line  0 :  { *(.debug_line) }
  .debug_frame  0 :  { *(.debug_frame) }
  .debug_str  0 :  { *(.debug_str) }
  .debug_loc  0 :  { *(.debug_loc) }
  .debug_macinfo  0 :  { *(.debug_macinfo) }
  .debug_weaknames  0 :  { *(.debug_weaknames) }
  .debug_funcnames  0 :  { *(.debug_funcnames) }
  .debug_typenames  0 :  { *(.debug_typenames) }
  .debug_varnames  0 :  { *(.debug_varnames) }

  .xt.insn 0 :
  {
    *(.xt.insn)
    *(.gnu.linkonce.x*)
  }

  .xt.lit 0 :
  {
    *(.xt.lit)
    *(.gnu.linkonce.p*)
  }

  /* Sections to be discarded */
  DISCARDS
  /DISCARD/ : { *(.exit.literal) }
}
