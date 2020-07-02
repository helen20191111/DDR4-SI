/*------------------------------------------------------------------------
 * Copyright (c) 2014 by Nuvoton Technology Israel
 * All rights reserved.
 * Written by Lior.Albaz.
 *------------------------------------------------------------------------*/


#ifndef __Utility_h__
#define __Utility_h__

typedef void (*JUMP_CODE) (void); 

extern void	Sleep (UINT32 Cycles);
extern void Error (int status);

extern unsigned long int GetRandSeed (void);

/*
 Co-Processor Access
 --------------------
 
 Assembly code: 
 MRC p15, Op1,<Rd>, CRn, CRm, Op2; Read
 MCR p15, Op1,<Rd>, CRn, CRm, Op2; Write

 Coprocessor 15: CRn -> Op1 -> CRm -> Op2 -> register
 
 CRn: Register number within the system control co-processor. (e.g. c0, c2)
 CRm: Operational register number within CRn.  (e.g. c0, c3)
 Op1: Opcode_1 value for the register. 
 Op2: Opcode_2 value for the register.

*/
#define CP15_READ(Rd,Op1,CRn,CRm,Op2)  asm("MRC p15,"#Op1",%0,"#CRn","#CRm","#Op2"":"=r"(Rd))
#define CP15_WRITE(Wr,Op1,CRn,CRm,Op2) asm("MCR p15,"#Op1",%0,"#CRn","#CRm","#Op2""::"r"(Wr))
/* usage example:
	unsigned long int Write_Data = 10;
	unsigned long int Read_Data;
	CP15_READ (Read_Data, 0, c0, c0, 0);
	CP15_WRITE (Write_Data,1,c2,c3,4);
*/

extern UINT32 Get_CPU_ID_CODE (void);
extern UINT32 Get_MPCore_ID (void);



/*
  unsigned long tx=6, ty=9;
  register unsigned long _r6 asm ("r6");
  register unsigned long _r5 asm ("r5");
  _r6 = tx;
  _r5 = ty;

  ty= tx*2 + ty*2;

 asm ("mov r5, %0" : :"r"(ty));
 asm ("mov r6, %0" : :"r"(tx));
 asm ("add r6, r5");

  asm ("add %0,%2"  : "=r"(y) : "0"(y), "r"(x)) ;


 asm ("add %0,%2"  : "=r"(y) : "0"(y), "r"(x)) ;  // y = add (y,x)
 */
 //   int x,y;
 //asm volatile ("add %0,%2"  : "=r"(y)  : "0"(y), "r"(x) : "r8", "r9", "r10") ;  // y = add (y,x)


//  Memory Dump Display
extern void MemoryDumpB (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines);
extern void MemoryDumpW (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines);
extern void MemoryDumpD (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines);
extern void MemoryDumpW_Compare (UINT32 Address_Reference, UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines);



//--------------------------------------------
// Same as memcmp but use 8bit burst only. 
// return:
// (-1) indicates that the contents of both memory blocks are equal
// other value for error offset.
//--------------------------------------------
extern int MemCmp (const void *ptr1, const void *ptr2, UINT32 SizeInByte);

//--------------------------------------------
// Same as memcmy but use 8bit burst only. 
//--------------------------------------------
extern void MemCpy (void *dst, const void *src, UINT32 SizeInByte);

//--------------------------------------------
// Same as memset but use 8bit burst only. 
//--------------------------------------------
extern void MemSet (void *dst, int val, UINT32 SizeInByte);

extern void InvalidateCache (void);


extern int					rand		(void);
extern unsigned char		rand_8bit	(void);
extern unsigned long int	rand_32bit	(void);

extern void srand (unsigned int seed);
extern void SetRandSeed (unsigned long int seed);
extern unsigned long int GetRandSeed (void);

#endif
