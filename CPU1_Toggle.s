
.global CPU1_Toggle
.global CPU1_Cmd
.global CPU1_Rev

//------------------------------------------------------------------------------------
// void CPU1_Toggle (void)
CPU1_Toggle:

CPU1_OFF:  

		// update revision register to indicate test in running 
		ldr r0, =0x12345678
		ldr r1, =CPU1_Rev
		str r0, [r1]

		// Set GPIO00 Low
		//ldr r1, =#0xF001000C  // GP0DOUT
		//ldr r0, =#0x00000000  // bit0 - low
		//str r0, [r1]

		WFE
		
		// Set GPIO00 High
		//ldr r1, =#0xF001000C  // GP0DOUT 
		//ldr r0, =#0x00000001  // bit0 - high 
		//str r0, [r1] 
		
		ldr r0, CPU1_Cmd 
		cmp r0,#0
		beq CPU1_OFF
		
		ldr r1,  =0x00100000
		ldr r2,  =0x00200000
		ldr r3,  =0x33333333
		ldr r4,  =0x44444444
		ldr r5,  =0x55555555
		ldr r6,  =0x66666666
		ldr r7,  =0x77777777
		ldr r8,  =0x88888888
		ldr r9,  =0x99999999
		ldr r10, =0xaaaaaaaa
		ldr r11, =0xbbbbbbbb
		ldr r12, =0xcccccccc
		ldr r12, =0xcccccccc
		ldr lr,  =0x03000000
		ldr ip,  =0x00000001
			
CPU1_ON:

		ldrne   r3, [lr, r1, lsl #2]
		SMLAL   r8,r7,r6,r5 
		subs    ip, ip, #0
		ldr     r4, [lr, r2, lsl #2]
		SMLALD  r12,r11,r10,r9 
		
		ldrne   r3, [lr, r1, lsl #2]
		SMLAL   r8,r7,r6,r5 
		subs    ip, ip, #0
		ldr     r4, [lr, r2, lsl #2]
		SMLALD  r12,r11,r10,r9 
		
		ldrne   r3, [lr, r1, lsl #2]
		SMLAL   r8,r7,r6,r5 
		subs    ip, ip, #0
		ldr     r4, [lr, r2, lsl #2]
		SMLALD  r12,r11,r10,r9 
		
		ldrne   r3, [lr, r1, lsl #2]
		SMLAL   r8,r7,r6,r5 
		subs    ip, ip, #0
		ldr     r4, [lr, r2, lsl #2]
		SMLALD  r12,r11,r10,r9 
		
		ldrne   r3, [lr, r1, lsl #2]
		SMLAL   r8,r7,r6,r5 
		subs    ip, ip, #0
		ldr     r4, [lr, r2, lsl #2]
		SMLALD  r12,r11,r10,r9 
		
		 
		//UMAAL   r4,r3,r2,r1     // Unsigned Multiply Accumulate Accumulate Long //  64-bit(r12,r11) = [32-bit(r0) * 32-bit(r2)] +  32-bit(r11) + 32-bit(r12) // UMAAL<c> <RdLo>,<RdHi>,<Rn>,<Rm>  
		//SUBS    r10,r9,#1
		//SMLAL   r8,r7,r6,r5     // Signed Multiply Accumulate Long // 64 = 64 + 32 x 32
		//SMLALD  r12,r11,r10,r9  // Signed Multiply Accumulate Long Dual // 64 = 64 + 16 x 16 + 16 x 16
		
		SUBS   r0,r0,#1
		BNE    CPU1_ON
	
		ldr r1, =CPU1_Cmd
		str r0, [r1] 
		
		B     CPU1_OFF   
		
CPU1_Cmd:
	.word 0x00000000
CPU1_Rev:
	.word 0x00000000
//------------------------------------------------------------------------------------
