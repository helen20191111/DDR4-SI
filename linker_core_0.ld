OUTPUT_FORMAT ("elf32-littlearm","elf32-bigarm","elf32-littlearm")

/* Setting code entry point to the linker */
ENTRY(_startup)


/* Memory Map*/
MEMORY
{
	sram (rwx) 	   : ORIGIN = 0xFFFD0000, LENGTH = 0x0001C000  /* 112K for code and global data */
	sram_TLB (rwx) : ORIGIN = 0xFFFEC000, LENGTH = 0x00004000  /* 16KB TLBs */
	sram_RAM3 (rwx): ORIGIN = 0xC0008000, LENGTH = 0x00001000  /* 4KB for stack */    
}

/* Section Definitions */
SECTIONS
{
	/* my_section_name start_address BLOCK(align) (NOLOAD) : AT ( load_addr )
	{
		file_name (section_name, section_name, section_name)
		file_name (section_name, section_name, section_name)
	} > mem_area
	
	file_name:  '*'  means any file.
	section_name: '*'  means any section.
	start_address: 
	"." is the last address used;  
	if start_address is "." then mem_area used only for linker to verify "." is in mem_area range. 
	if start_address is not used, "." is set with mem_area.last.
	*/

    text_startup :
    {
        *startup*(.text,  .text.*)
        *startup*(.rodata,  .rodata*)
    } > sram /* place start up code here, typically at ROM area. */
       
    text_all_others :
    {
        *(.text  .text.*)	/* all .text sections (the code) */
    } > sram /* place all other code here, typically at ROM area. */
    
    rodata :
    {
        *(.rodata  .rodata*)         /* all .rodata sections (RO data, constants, strings)  */
        *(.glue_7t  .glue_7)         /*  ARMv7 core glue functions for thumb and ARM mode */
		*(.gnu.linkonce.r.*  .gnu.linkonce.t.*)
    } > sram /* Read only data, typically at ROM area. */
    
    ARM.extab :
    {
        *(.ARM.extab*)
        *(.gnu.linkonce.armextab.*)
    } > sram /* ROM area */

    ARM.exidx :
    {
		__exidx_start = .;
		*(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
		__exidx_end = .;
    } > sram /* ROM area */
     
    . = ALIGN(4);
    
    _etext = ABSOLUTE(.);  
    _sidata = ABSOLUTE(.);

    /* .data section which is used for initialized data (global variable). The init values are stored at ROM (_etext), at 'strat.s' it is copied to RAM */
    data . : AT (_etext)   /* the RO data is placed at _etext (the load_addr) while the RW data will be copied into _sdata*/
    {
        . = ALIGN(4);
        _sdata = ABSOLUTE(.);
        ASSERT(_sidata == _sdata, "Error: in RAM compilation, _sidata and _sdata should be equal.");
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = ABSOLUTE(.) ;
    } > sram /* RAM area */


    /* .bss section which is used for uninitialized data (global variable); no pre-load is needed, just place holder in RAM */
    bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = ABSOLUTE(.);
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = ABSOLUTE(.);
    } > sram  /* RAM area */


    stack (NOLOAD) :
    {
        . = ALIGN(4);
        __StackLimit = ABSOLUTE(.);  	    /* may use to monitor the bottom point of the stack; no pre-load is needed, just place holder in RAM */
        __StackSize = 0x1000; /* LENGTH (sram) - SIZEOF (startup); */
        ASSERT(__StackSize >= 0x1000, "Error: Stack size is less then the minimume required.");
        . = . + __StackSize;
        __StackTop = ABSOLUTE(.);         /* may use to set top point of the stack */
    } > sram_RAM3 /* ram */
    
 
    . = ALIGN(4);
    end = ABSOLUTE(.) ;  		    /* end of assigned RAM, may use to set start point of the heap (used by libc or librdimon.a in rdimon-syscalls.o - sbrk()) */

}
