/*------------------------------------------------------------------------
 * Copyright (c) 2014 by Nuvoton Technology Israel
 * All rights reserved.
 * Written by Lior.Albaz.
 *------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include "Common.h"
#include "CoreRegisters.h"
#include "TestMsgCore.h"
#include "Utility.h"

void Sleep (DWORD Cycles)
{
    volatile DWORD delay;

    while (Cycles!=0)
    {
         for  (delay=0;delay<0x10;delay++);
         Cycles--;
    }
    return;

} // *End of Delay*



//-------------------------------------------------
// Accessed CPU within an MPCore core
UINT32 Get_MPCore_ID (void)
{
	UINT32 CPU_ID;
	CP15_READ (CPU_ID, 0, c0, c0, 5);
	//CPU_ID &= 0x03;
	return (CPU_ID);
}
//-------------------------------------------------

//-------------------------------------------------
// reads Main ID register - return the device ID code that contains information about the processor.
UINT32 Get_CPU_ID_CODE (void)
{
	UINT32 CPU_ID_CODE;
	CP15_READ (CPU_ID_CODE, 0, c0, c0, 0);
	return (CPU_ID_CODE);
}
//-------------------------------------------------

extern void Error (int status)
{
	// bl and r0 registers can identify the calling code
	__asm__ ("BKPT 0x00FF");
	while(1);
}
//----------------------------------------------------------
extern void InvalidateCache (void)
{
	UINT32 reg32;
	// Invalidate Instruction cache.
	reg32 = 0;
	CP15_WRITE (reg32,0,c7,c5,0); // ICIALLU: Invalidate all instruction caches to PoU. Also flushes branch target cache.
	reg32 = 0;
	CP15_WRITE (reg32,0,c7,c5,6); // BPIALL: Invalidate entire branch predictor array.
}



//---------------------------------------------------------------------------
//                              Memory Dump Display
//---------------------------------------------------------------------------
extern void MemoryDumpD (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines)
{	
	UINT16 line;
	UINT32 *pData32 = (UINT32*)Address;
	UINT32 LineBuff [4];
	for (line=0;line<NumOfLines;line++)
	{
		LineBuff [0] = *pData32++;
		LineBuff [1] = *pData32++;
		LineBuff [2] = *pData32++;
		LineBuff [3] = *pData32++;
		LogMessage ("0x%08lX: 0x%08lX 0x%08lX 0x%08lX 0x%08lX \n",DisplayAddress,LineBuff [0],LineBuff [1],LineBuff [2],LineBuff [3]);
		DisplayAddress+=16;
	}	
}
//-------------------------------------------------------------
extern void MemoryDumpW (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines)
{	
	UINT16 line;
	UINT16 *pData16 = (UINT16*)Address;
	UINT16 Data16; 
	UINT8 index;

	for (line=0;line<NumOfLines;line++)
	{
		LogMessage ("0x%08lX: ",DisplayAddress); DisplayAddress+=16;
		for (index=0;index<8;index++)
		{
			Data16 = *pData16++;
			LogMessage ("0x%04X  ",Data16); 
		}
		LogMessage ("\n");
	}	
}
//-------------------------------------------------------------
extern void MemoryDumpW_Compare (UINT32 Address_Reference, UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines)
{	
	UINT16 line;
	UINT16 *pData16 = (UINT16*)Address;
	UINT16 *pData16_Reference = (UINT16*)Address_Reference;
	UINT16 Data16,Data16_Reference; 
	UINT8 index;

	for (line=0;line<NumOfLines;line++)
	{
		LogMessage ("0x%08lX: ",DisplayAddress); DisplayAddress+=16;

		for (index=0;index<8;index++)
		{
			Data16 = *pData16++;
			Data16_Reference = *pData16_Reference++;
			if (Data16!=Data16_Reference)
				LogError ("0x%04X  ",Data16); 
			else
				LogPass ("0x%04X  ",Data16);
		}
		LogMessage ("\n");
	}	
}
//-------------------------------------------------------------
extern void MemoryDumpB (UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines)
{	
	UINT16 line;
	UINT8 *pData8 = (UINT8*)Address;
	UINT8 index;
	UINT8 Data8;

	for (line=0;line<NumOfLines;line++)
	{
		LogMessage ("0x%08lX: ",DisplayAddress); DisplayAddress+=16;
		for (index=0;index<16;index++)
		{
			Data8 = *pData8++;
			LogMessage ("0x%02X  ",Data8); 
		}
		LogMessage ("\n");
	}	
}
//-------------------------------------------------------------
extern void MemoryDumpB_Compare (UINT32 Address_Reference, UINT32 Address, UINT32 DisplayAddress, UINT16 NumOfLines)
{	
	UINT16 line;
	UINT8 *pData8 = (UINT8*)Address;
	UINT8 *pData8_Reference = (UINT8*)Address_Reference;
	UINT8 index;
	UINT8 Data8,Data8_Reference; 
	
	for (line=0;line<NumOfLines;line++)
	{
		LogMessage ("0x%08lX: ",DisplayAddress); DisplayAddress+=16;
		for (index=0;index<16;index++)
		{
			Data8 = *pData8++;
			Data8_Reference = *pData8_Reference++;
			if (Data8!=Data8_Reference)
				LogError ("0x%02X  ",Data8); 
			else
				LogPass ("0x%02X  ",Data8);
		}
		LogMessage ("\n");
	}	
}
//-------------------------------------------------------------


 
//--------------------------------------------
// Same as memcmp but use 8bit burst only. 
// return:
// (-1) indicates that the contents of both memory blocks are equal
// other value for error offset.
//--------------------------------------------
extern int MemCmp (const void *ptr1, const void *ptr2, UINT32 SizeInByte)
{
	UINT32 Offset;

	Offset = 0;
	while (SizeInByte!=0)
	{
		if ( *((UINT8*)ptr1) != *((UINT8*)ptr2) )
			return (Offset);	

		ptr1 = (UINT8*)ptr1 + 1;
		ptr2 = (UINT8*)ptr2 + 1;
		Offset++;
		SizeInByte--;	
	}
	return (-1); // pass
}
//--------------------------------------------
// Same as memcpy but use 8bit burst only. 
//--------------------------------------------
extern void MemCpy (void *dst, const void *src, UINT32 SizeInByte)
{
	UINT8 *pDst = (UINT8 *)dst;
	UINT8 const *pSrc = (UINT8 const *)src;

	while (SizeInByte--)
	{
		*pDst++ = *pSrc++;
	}
}
/*
void * memcpy (void * dst, void const * src, size_t len)
{
	char * pDst = (char *) dst;
	char const * pSrc = (char const *) src;

	while (len--)
	{
		*pDst++ = *pSrc++;
	}

	return (dst);
}
*/
//-----------------------------------------------------------------
// Same as memcpy but use 8bit burst only. 
//-----------------------------------------------------------------
extern void MemSet (void *dst, int val, UINT32 SizeInByte)
{
	UINT8 *pDst = (UINT8 *)dst;

	while (SizeInByte--)
	{
		*pDst++ = val;
	}
}

//------------------------------------------------------------
// rand values are from 0 to 32768
static unsigned long int next = 1;

extern unsigned long int GetRandSeed (void)
{
	return (next);
}
extern int rand (void)
{
	next = next * 1103515245 + 12345;
	return ((unsigned int)(next>>16)) & 0x7FFF ;
}
extern unsigned char rand_8bit (void)
{
	next = next * 1103515245 + 12345;
	return (unsigned char)(next);
}
extern unsigned long int rand_32bit (void)
{
	next = next * 1103515245 + 12345;
	return (next);
}
extern void SetRandSeed (unsigned long int seed)
{
	next = seed;
}

extern void srand (unsigned int seed)
{
	next = seed;
}
