OUTPUT_FORMAT ("elf32-littlearm","elf32-bigarm","elf32-littlearm")

/* Setting code entry point to the linker */
ENTRY(_startup)

/*SEARCH_DIR("..\arm-none-eabi\lib\armvx")*/


/* Memory Map*/
MEMORY
{
	/* sram (rwx) : ORIGIN = 0xFFFD0000, LENGTH = 0x00010000 */ /* 64KB  for  core 0 */
	 sram (rwx) : ORIGIN = 0xFFFE0000, LENGTH = 0x00010000   /* 64KB  for core 1 */
}

/* Section Definitions */
SECTIONS
{
	/* section_name start_address BLOCK(align) (NOLOAD) : AT ( load_addr )
	{
		file_name (section_name, section_name, section_name)
		file_name (section_name, section_name, section_name)
	} > mem_area
	*/

    startup :
    {
        *startup*(.text  .text.*)
        *startup*(.rodata  .rodata*)
    } > sram /* start up code */

    /* set the stack in SRAM assuming it has a faster accsess time then the DDR. */
    stack . (NOLOAD) :
    {
        . = ALIGN(4);
        __StackLimit = ABSOLUTE(.);  	    /* may use to monitor the bottom point of the stack */
        __StackSize = 0x1000; /* LENGTH (sram) - SIZEOF (startup); */
        ASSERT(__StackSize >= 0x1000, "Error: Stack size is less then the minimume required.");
        . = . + __StackSize;
        __StackTop = ABSOLUTE(.);         /* may use to set top point of the stack */
    } > sram

    text . :
    {
        *(.text  .text.*)            /* all .text sections (the code) */
        *(.rodata  .rodata*)          /* all .rodata sections (RO data, constants, strings)  */
        *(.glue_7t  .glue_7)         /* ??  ARMv7 core glue functions for thumb and ARM mode */
		*(.gnu.linkonce.r.*  .gnu.linkonce.t.*)
    } > sram /* code */

    ARM.extab  . :
    {
        *(.ARM.extab*)
        *(.gnu.linkonce.armextab.*)
    } > sram /* code */

    ARM.exidx  . :
    {
		__exidx_start = .;
		*(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
		__exidx_end = .;
    } > sram /* code */

    . = ALIGN(4);
    _etext = ABSOLUTE(.);
    _sidata = ABSOLUTE(.);

    /* .data section which is used for initialized data. values are stored at  _etext,  in the C code, relocated to ram_sdram */
    data  . : AT (_etext)   /* the RO data is write starting at _etext while the RW data will be loaded into _sdata*/
    {
        . = ALIGN(4);
        _sdata = ABSOLUTE(.);
        ASSERT(_sidata == _sdata, "Error: in RAM compilation, _sidata and _sdata should be equal.");
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = ABSOLUTE(.) ;
    } > sram /* data */


    /* .bss section which is used for uninitialized data */
    bss . (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = ABSOLUTE(.);
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = ABSOLUTE(.);
    } > sram /* data */

    . = ALIGN(4);
    end = ABSOLUTE(.) ;  		    /* end of assigned RAM, may use to set start point of the heap (used by libc or librdimon.a in rdimon-syscalls.o - sbrk()) */

}
