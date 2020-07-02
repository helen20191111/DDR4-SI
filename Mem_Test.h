
#ifndef __MemTest_h__
#define __MemTest_h__

#include "Poleg.h"

extern volatile UINT32 CPU1_Cmd;
extern volatile UINT32 CPU1_Rev;

extern void CPU1_Toggle (void);


//#define DDR_LOG_DEBUG
#define L1_CACHE_SIZE_IN_BYTE (32*1024)
#define L1_CACHE_SIZE_IN_DWORD (L1_CACHE_SIZE_IN_BYTE/4)
#define L2_CACHE_SIZE_IN_BYTE (512*1024)
#define L2_CACHE_SIZE_IN_DWORD (L2_CACHE_SIZE_IN_BYTE/4)

// memory area for testing: 1MB...65MB
#define DDR_ADDR_BASE (1*1024*1024) // 1MB offset to avoid overwrite on interrupts vector area (and may be test code area)   // 23/12/2015: changed from 128K to 1M test start adders. Try to fix UBOOT restart failures when d-cache is ON. 
#define DDR_ADDR_MASK 0x3FFFFFF // 64MB block size for memory testing (- the 1MB base).
#define DDR_ADDR_SIZE (DDR_ADDR_MASK+1) 

#define TEST_BLOCK_SIZE_IN_DWORD (128) 

extern UINT16 MemoryCompare (const UINT16 *ptr1, const UINT16 *ptr2, UINT32 SizeInWord);
extern UINT16 MemStressTest (); 

extern const UINT16 DDR_MPR2_Expected [];


#endif	
