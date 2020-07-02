
.global _startup
.global _exit

.global reset_handler
.global undefined_instruction_handler
.global svc_handler
.global prefetch_abort_handler
.global data_abort_handler
.global irq_handler
.global fiq_handler

.global pUndefinedInstructionRoutine
.global pSvcRoutine
.global pPrefetchAbortRoutine
.global pDataAbortRoutine
.global pIrqRoutine
.global pFiqRoutine


// ARM9 and Cortex A9 interrupts vector table. Vector table is placed at physical address 0x00000000 or at physical address 0xFFFF0000, depends on core settings. 
// In order for the interrupts to work, this code section must be placed at interrupts vector table physical address or copy it to later on (from '_startup' to 'reset_handler').

.section .text.startup

_startup:
/*0x00*/    b reset_handler                 /* Note: start with SVC/SWI processor mode */
/*0x04*/    b undefined_instruction_handler /* Undef processor mode */
/*0x08*/    b svc_handler                   /* SVC/SWI processor mode */
/*0x0C*/    b prefetch_abort_handler        /* Abort processor mode */
/*0x10*/    b data_abort_handler            /* Abort processor mode */
/*0x14*/    b error							/* reserved */
/*0x18*/    b irq_handler                   /* IRQ processor mode */
/*0x1C*/    b fiq_handler                   /* FIQ processor mode */

// the 'error' routine can be replaced with any C code routine ==> void IntRoutine (UINT32 Addr)
/*0-0x20*/	 									.word 0x00000000
/*1-0x24*/    pUndefinedInstructionRoutine:		.word error
/*2-0x28*/    pSvcRoutine:						.word error
/*3-0x2C*/    pPrefetchAbortRoutine:			.word error
/*4-0x30*/    pDataAbortRoutine:				.word error
/*5-0x34*/										.word 0x00000000
/*6-0x38*/    pIrqRoutine:						.word error   
/*7-0x3C*/    pFiqRoutine:						.word error

TestMsg:
/*0-0x40*/    .word  0x12345678       /* Magic Word */
              .word  0x10000001       /* TestMsg revision  */
              .word  _startup         /* code loading address */
              .word  _startup         /* code entry point address */
              .word  0xFFFFFFFF       /* TestMsg header addr */
              
/*.skip 0x40*/
              
error:
    // BKPT 0xEEEE
	mov r0, #-1
	b _exit
	//b reset_handler
    //b   .
    // MOVS pc, lr // typically there is nowhere to return to !!

undefined_instruction_handler:
    push {r0-r12,lr}
    mov  r0, lr			// r0 is the returning address while the undefined instruction address is at -4 in ARM state and -2 in Thumb state.
    
    // ldr r1, =pUndefinedInstructionRoutine   // set r1 with pUndefinedInstructionRoutine value  r1 = (UINT32)(&Addr)
    ldr r1, pUndefinedInstructionRoutine       // set r1 with the value point at pUndefinedInstructionRoutine address  r1 = Addr
    blx  r1
    pop {r0-r12,lr}
    b reset_handler
    // MOVS pc,lr  // typically there is nowhere to return to !!

prefetch_abort_handler:
    push {r0-r12,lr}
    sub  r0, lr, #4		// r0 is the address of the aborting prefetch 
    ldr r1, pPrefetchAbortRoutine
    blx r1
    pop {r0-r12,lr}
    b reset_handler
    // SUBS pc, lr , #4	// retuns to the aborting address; typically there is nowhere to return to !!

data_abort_handler:
    push {r0-r12,lr}
    sub  r0, lr, #8		// r0 is the address of the Load or Store instruction that generated the Data Abort.
    ldr r1, pDataAbortRoutine
    blx r1
    pop {r0-r12,lr}
    SUBS pc, lr, #4		// retuns to the next command (assuming ARM state) after the aborting instruction;

svc_handler: /*SWI*/
    push {r0-r12,lr}
    mov  r0, lr			// r0 is the returning address while the calling address (previous instruction) is -4 in ARM state and -2 in Thumb state.
    ldr r1, pSvcRoutine
    blx r1
    pop {r0-r12,lr}
    MOVS pc,lr  // The S means restore CPSR from SPSR

irq_handler:
	push {r0-r12,lr}
	sub  r0, lr, #4		// r0 is the address of the instruction that was not executed because the IRQ took priority
    ldr r1, pIrqRoutine
    blx r1
    pop {r0-r12,lr}
    SUBS pc, lr , #4

fiq_handler: // TBD: there is no need to push all registers.....
    push {r0-r12,lr}
    sub  r0, lr, #4		// r0 is the address of the instruction that was not executed because the FIQ took priority
    ldr r1, pFiqRoutine
    blx r1
    pop {r0-r12,lr}
    SUBS pc, lr , #4
    
reset_handler:

	// This code use it's own stack and not the caller stack (i.e, caller can be UBOOT or FUP).
	// the code push caller registers into caller stack (there is no need to push r0 since it is the return value)
	// If caller stack is not initialize or invalid address this code may stuck (e.g. when using JTAG to load a code. In this case init the stack via JTAG to some RAM address before run the code)
	push {r1-r12,lr}
	
	// store caller sp register into top of the new stack
	ldr r0, =__StackTop // top of the stack
	sub r0, r0 ,#4
	str sp, [r0], #-4

    /* Init the stacks pointers */
    MRS r2, CPSR        // MRS: Move the contents of special register CPSR to a general-purpose register
    BIC r2, r2, #0x1f   // r2 = r2 & (~0x1F) ==> clear bits 0..4;
    ORR r2, r2, #0x1C0  // r2 = r2 | 0x1C0 ==> disable FIQ, IRQ and Abort

    ORR r3, r2, #0x11 // enter FIQ mode
    MSR CPSR_c, r3
    mov sp, r0
    sub r0, r0, #0x100  // will use 0x100 bytes for FIQ

    ORR r3, r2, #0x12 // enter IRQ mode
    MSR CPSR_c, r3
    mov sp, r0
    sub r0, r0, #0x100  // will use 0x100 bytes for IRQ

    ORR r3, r2, #0x17 // enter Abort mode
    MSR CPSR_c, r3
    mov sp, r0
    sub r0, r0, #0x100  // will use 0x100 bytes for Abort

    ORR r3, r2, #0x1B // enter Undef mode
    MSR CPSR_c, r3
    mov sp, r0
    sub r0, r0, #0x100  // will use 0x100 bytes for Undef
    
    ORR r3, r2, #0x13 // enter SVC mode
    MSR CPSR_c, r3
    mov sp, r0
    /*
    sub r0, r0, #0x100  // will use 0x100 bytes for SVC

    BIC r3, r3, #0x1C0 // r2 = r2 & (~0x1C0) ==> enbale FIQ, IRQ and Abort
    MSR CPSR_c, r3

    ORR r3, r2, #0x10  // enter system/user mode (and stay in user mode). In system/user you can't enable or disable core interrupts.
    MSR CPSR_c, r3
    mov sp, r0         // will use up to  __StackLimit
    */
    
    /* Copy the data segment initializers from 'ROM' to 'RAM' */
    ldr     r0, =_sidata  /* Start address for the initialization values of the .data section */
    ldr     r1, =_sdata   /* Start address for the .data section */
    ldr     r2, =_edata   /* End address for the .data section */
 data_loop:
    cmp     r1, r2
    itt     lt
    ldrlt   r3, [r0], #4
    strlt   r3, [r1], #4
    blt     data_loop

     /* Zero fill the stack  */
    ldr     r0, =__StackLimit   /* Start address for the stack  */
    ldr     r1, =__StackTop     /* End address for the stack */
    sub		r1, r1 ,#8
    mov     r2, #0
 stack_loop:
    cmp     r0, r1
    it      lt
    strlt   r2, [r0], #4
    blt     stack_loop

    /* Zero fill the bss segment  */
    ldr     r0, =__bss_start__      /* Start address for the .bss section */
    ldr     r1, =__bss_end__        /* End address for the .bss section   */
    mov     r2, #0
 bss_loop:
    cmp     r0, r1
    it      lt
    strlt   r2, [r0], #4
    blt     bss_loop


	/* jump to main  */
    bl main
    /* return from main, r0 is update, all other are invalid */
 
_exit:

    // return caller sp and registers (except r0)
    ldr r1, =__StackTop // top of the stack
    sub r1, r1 ,#4
	ldr sp, [r1]
    pop {r1-r12,pc}
	
    /* endless loop in case of a problem */
    b   .
 
