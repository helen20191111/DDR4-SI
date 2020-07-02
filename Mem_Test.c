//Optimize_Fast /*do not delte this note*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Common.h"
#include "CoreRegisters.h"
#include "TestMsgCore.h"
#include "Utility.h"
#include "Poleg.h"
#include "Mem_Test.h"
#include "MainMenu.h"
#include "main.h"


//------------------------------------------------
// Memory Test
//------------------------------------------------
/*
8.1.2. Supported AXI transfers
The Cortex-A9 master ports generate only a subset of all possible AXI transactions.

For cacheable transactions:
* WRAP4 64-bit for read transfers (line-fills)
* INCR4 64-bit for write transfers (evictions)

For non-cacheable transactions:
* INCR N (N:1- 9) 64-bit read transfers
* INCR 1 for 64-bit write transfers
* INCR N (N:1-16) 32-bit read transfers
* INCR N (N:1-2) for 32-bit write transfers
* INCR 1 for 8-bit and 16-bit read/write transfers
* INCR 1 for 8-bit, 16-bit, 32-bit, 64-bit exclusive read/write transfers
* INCR 1 for 8-bit and 32-bit read/write (locked) for swap

The following points apply to AXI transactions:
* WRAP bursts are only read transfers, 64-bit, 4 transfers
* INCR 1 can be any size for read or write
* INCR burst (more than one transfer) are only 32-bit or 64-bit
* No transaction is marked as FIXED
* Write transfers with all byte strobes low can occur.

*/

/* UBOOT Info 
   UBOOT runs from SPI or 0x8000 but it copy itself to top of the memory so the lower 64M are free to run memory tests.
*/

// -------   Golden Numbers   ----------
// 23/12/2015: changed Golden_Numbers buffer size to aline 0xFF for simplicity
const UINT32 Golden_Numbers [256] = 
{
	// Keep 0         Keep 1
	0x007F00FF,		~0x007F00FF,  // lane-1 fixed to 0, lane-0 MSB bit 7 toggle while others are const 1
	0x00BF00FF,		~0x00BF00FF,  
	0x00DF00FF,		~0x00DF00FF, 
	0x00EF00FF,		~0x00EF00FF,
	0x00F700FF,		~0x00F700FF, 
	0x00FB00FF,		~0x00FB00FF, 
	0x00FD00FF,		~0x00FD00FF, 
	0x00FE00FF,		~0x00FE00FF,  // lane-1 fixed to 0, lane-0 MSB bit 0 toggle while others are const 1
	0x00FF007F,		~0x00FF007F,  // lane-1 fixed to 0, lane-0 LSB bit 7 toggle while others are const 1	
	0x00FF00BF,		~0x00FF00BF, 
	0x00FF00DF,		~0x00FF00DF,
	0x00FF00EF,		~0x00FF00EF, 
	0x00FF00F7,		~0x00FF00F7,
	0x00FF00FB,		~0x00FF00FB,
	0x00FF00FD,		~0x00FF00FD,  
	0x00FF00FE,		~0x00FF00FE,  // lane-1 fixed to 0, lane-0 LSB bit 0 toggle while others are const 1
	0x00FF00FF,     ~0x00FF00FF,  // lane-1 fixed to 0, lane-0 fixed to 1

	0x7F0000FF,		~0x7F0000FF,  // lane-1 bit 7 const 0 while others toggle, lane-0 toggle
	0xBF0000FF,		~0xBF0000FF, 
	0xDF0000FF,		~0xDF0000FF, 
	0xEF0000FF,		~0xEF0000FF,
	0xF70000FF,		~0xF70000FF, 
	0xFB0000FF,		~0xFB0000FF, 
	0xFD0000FF,		~0xFD0000FF, 
	0xFE0000FF,		~0xFE0000FF,  
	0xFF00007F,		~0xFF00007F,  // lane-0 bit 7 const 0 while others toggle, lane-1 toggle	
	0xFF0000BF,		~0xFF0000BF, 
	0xFF0000DF,		~0xFF0000DF,
	0xFF0000EF,		~0xFF0000EF, 
	0xFF0000F7,		~0xFF0000F7,
	0xFF0000FB,		~0xFF0000FB,
	0xFF0000FD,		~0xFF0000FD,  
	0xFF0000FE,		~0xFF0000FE,  // lane-0 bit 0 const 0 while others toggle, lane-1 toggle
	0xFF0000FF,     ~0xFF0000FF,  // lane-0 toggle, lane-1 toggle (invert)

	0x007FFF00,		~0x007FFF00, 	
	0x00BFFF00,		~0x00BFFF00, 
	0x00DFFF00,		~0x00DFFF00,  
	0x00EFFF00,		~0x00EFFF00, 
	0x00F7FF00,		~0x00F7FF00, 
	0x00FBFF00,		~0x00FBFF00, 
	0x00FDFF00,		~0x00FDFF00, 
	0x00FEFF00,		~0x00FEFF00,  
	0x00FF7F00,		~0x00FF7F00, 	
	0x00FFBF00,		~0x00FFBF00,
	0x00FFDF00,		~0x00FFDF00,
	0x00FFEF00,		~0x00FFEF00, 
	0x00FFF700,		~0x00FFF700,
	0x00FFFB00,		~0x00FFFB00,
	0x00FFFD00,		~0x00FFFD00, 
	0x00FFFE00,		~0x00FFFE00, 
	0x00FFFF00,     ~0x00FFFF00, // lane-0 toggle, lane-1 toggle (invert)

	0x7F00FF00,		~0x7F00FF00, 	// New numbers 28-10-2010
	0xBF00FF00,		~0xBF00FF00, 
	0xDF00FF00,		~0xDF00FF00,  
	0xEF00FF00,		~0xEF00FF00, 
	0xF700FF00,		~0xF700FF00, 
	0xFB00FF00,		~0xFB00FF00, 
	0xFD00FF00,		~0xFD00FF00, 
	0xFE00FF00,		~0xFE00FF00,  
	0xFF007F00,		~0xFF007F00, 	
	0xFF00BF00,		~0xFF00BF00,
	0xFF00DF00,		~0xFF00DF00,
	0xFF00EF00,		~0xFF00EF00, 
	0xFF00F700,		~0xFF00F700,
	0xFF00FB00,		~0xFF00FB00,
	0xFF00FD00,		~0xFF00FD00, 
	0xFF00FE00,		~0xFF00FE00, 
	0xFF00FF00,		~0xFF00FF00,  // lane-1 fixed to 1, lane-0 fixed to 0

	0x7FFF0000,		~0x7FFF0000,  // bit 15 const (0 or 1), all others bits are toggle  // New numbers 30-04-2015
	0xBFFF0000,		~0xBFFF0000, 
	0xDFFF0000,		~0xDFFF0000,  
	0xEFFF0000,		~0xEFFF0000, 
	0xF7FF0000,		~0xF7FF0000, 
	0xFBFF0000,		~0xFBFF0000, 
	0xFDFF0000,		~0xFDFF0000, 
	0xFEFF0000,		~0xFEFF0000,  
	0xFF7F0000,		~0xFF7F0000, 	
	0xFFBF0000,		~0xFFBF0000,
	0xFFDF0000,		~0xFFDF0000,
	0xFFEF0000,		~0xFFEF0000, 
	0xFFF70000,		~0xFFF70000,
	0xFFFB0000,		~0xFFFB0000,
	0xFFFD0000,		~0xFFFD0000, 
	0xFFFE0000,		~0xFFFE0000,  // bit 0 const (0 or 1), all others bits are toggle
	0xFFFF0000,		~0xFFFF0000,  // all bits are toggle

	0xAA55AA55,     ~0xAA55AA55,
	0xFFFFFFFF,		~0xFFFFFFFF,   // New numbers 28-10-2010

	//---------------------------
	// padded to aline with 256
	//---------------------------

	0x007F00FF,		~0x007F00FF,  // lane-1 fixed to 0, lane-0 MSB bit 7 toggle while others are const 1
	0x00BF00FF,		~0x00BF00FF,  
	0x00DF00FF,		~0x00DF00FF, 
	0x00EF00FF,		~0x00EF00FF,
	0x00F700FF,		~0x00F700FF, 
	0x00FB00FF,		~0x00FB00FF, 
	0x00FD00FF,		~0x00FD00FF, 
	0x00FE00FF,		~0x00FE00FF,  // lane-1 fixed to 0, lane-0 MSB bit 0 toggle while others are const 1
	0x00FF007F,		~0x00FF007F,  // lane-1 fixed to 0, lane-0 LSB bit 7 toggle while others are const 1	
	0x00FF00BF,		~0x00FF00BF, 
	0x00FF00DF,		~0x00FF00DF,
	0x00FF00EF,		~0x00FF00EF, 
	0x00FF00F7,		~0x00FF00F7,
	0x00FF00FB,		~0x00FF00FB,
	0x00FF00FD,		~0x00FF00FD,  
	0x00FF00FE,		~0x00FF00FE,  // lane-1 fixed to 0, lane-0 LSB bit 0 toggle while others are const 1
	0x00FF00FF,     ~0x00FF00FF,  // lane-1 fixed to 0, lane-0 fixed to 1

	0x7F0000FF,		~0x7F0000FF,  // lane-1 bit 7 const 0 while others toggle, lane-0 toggle
	0xBF0000FF,		~0xBF0000FF, 
	0xDF0000FF,		~0xDF0000FF, 
	0xEF0000FF,		~0xEF0000FF,
	0xF70000FF,		~0xF70000FF, 
	0xFB0000FF,		~0xFB0000FF, 
	0xFD0000FF,		~0xFD0000FF, 
	0xFE0000FF,		~0xFE0000FF,  
	0xFF00007F,		~0xFF00007F,  // lane-0 bit 7 const 0 while others toggle, lane-1 toggle	
	0xFF0000BF,		~0xFF0000BF, 
	0xFF0000DF,		~0xFF0000DF,
	0xFF0000EF,		~0xFF0000EF, 
	0xFF0000F7,		~0xFF0000F7,
	0xFF0000FB,		~0xFF0000FB,
	0xFF0000FD,		~0xFF0000FD,  
	0xFF0000FE,		~0xFF0000FE,  // lane-0 bit 0 const 0 while others toggle, lane-1 toggle
	0xFF0000FF,     ~0xFF0000FF,  // lane-0 toggle, lane-1 toggle (invert)

	0x007FFF00,		~0x007FFF00, 	
	0x00BFFF00,		~0x00BFFF00, 
	0x00DFFF00,		~0x00DFFF00,  
	0x00EFFF00,		~0x00EFFF00, 
	0x00F7FF00,		~0x00F7FF00, 
	0x00FBFF00,		~0x00FBFF00, 
	0x00FDFF00,		~0x00FDFF00, 

}; 
#define Num_Golden_Numbers (sizeof(Golden_Numbers)/sizeof(UINT32)) 
// --------------------------------------------------------------
const UINT16 DDR_MPR2_Expected [] = 
{
	0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
	0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF,
	0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

UINT32 g_DataWrite1Buff [TEST_BLOCK_SIZE_IN_DWORD] __attribute__ ((aligned (16)));
//UINT32 g_DataWrite2Buff [TEST_BLOCK_SIZE_IN_DWORD] __attribute__ ((aligned (16)));
UINT32 g_DataReadBuff   [TEST_BLOCK_SIZE_IN_DWORD] __attribute__ ((aligned (16)));
UINT32 g_AddrBuff       [TEST_BLOCK_SIZE_IN_DWORD] __attribute__ ((aligned (16)));

//-------------------------------------------------------------------------------------------------
// This memory test use address/data pairs. 
static UINT16 Run_MemTest_Type5_dword (UINT32 NumOfLoops, BYTE IsSilent)
{
	UINT32 l_NumOfLoops = 0; 
	UINT16 result_16=0,temp_result_16;
	UINT32 index;
	UINT32 temp_result_32;
	UINT32 Temp_addr;

	while (1)
	{
		if (l_NumOfLoops>=NumOfLoops)
			break;

		// stop the test in scan mode (Silent mode) when all bits are with errors 
		if ((IsSilent==TRUE) && (result_16==0xFFFF))
			break; 

		/*
		// check for abort from time to time 
		if ( (l_NumOfLoops&0xFF) == 0 )
		{
			if (CheckForAbort()!=0)
				return (0xFFFF);
		}
		*/

		//---------------------------------------------------------
		// fill a buffer data/address pairs 
		if ((l_NumOfLoops&7) == 0)
		{
			// option 1: sequence address (good for DM)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = ~g_DataWrite1Buff[index];
				g_AddrBuff[index] = DDR_ADDR_BASE + 4*index;
			}
		}
		else if ((l_NumOfLoops&7) == 1)
		{
			// option 2: random address (good when d-cache is off)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = ~g_DataWrite1Buff[index];
				// get random address and verify it does not exist in the buffer. 
				UINT32 index2 = (-1);
				while (index2 == (-1))
				{
					Temp_addr = ((rand_32bit()&DDR_ADDR_MASK) + DDR_ADDR_BASE) & 0xFFFFFFFC/*aline to DWORD*/;
					// make sure this new address is not in the list
					for (index2=0; index2<index; index2++)
					{
						if (g_AddrBuff[index2]==Temp_addr)
						{
							index2 = (-1);
							break;
						}
					}
				}
				g_AddrBuff[index] = Temp_addr;  // add this random address to the list
			}
		}
		else
		{
			// option 3: jump in L2 cache size (good when d-cache is on but not for DM testing since it read/write to a line address)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = ~g_DataWrite1Buff[index];
				g_AddrBuff[index] = DDR_ADDR_BASE + (L2_CACHE_SIZE_IN_BYTE * index);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		CPU1_Cmd = l_NumOfLoops&0xFF;
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
		// write (~ 100 usec)
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			HW_DWORD(g_AddrBuff[index]) = g_DataWrite1Buff[index];

		CPU1_Cmd = l_NumOfLoops&0xFF;
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
		// read & write anti-pattern (~ 100 usec)
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			 g_DataReadBuff[index] = HW_DWORD(g_AddrBuff[index]);
			 HW_DWORD(g_AddrBuff[index]) = ~g_DataReadBuff[index];
		}

		// verify pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			UINT32 l_expected = g_DataWrite1Buff[index];
			UINT32 l_actual = g_DataReadBuff[index];
			if (l_actual!=l_expected)
			{
				temp_result_32 = l_actual ^ l_expected;
				temp_result_16 = (UINT16)(temp_result_32) | (UINT16)((temp_result_32>>16));
				result_16 |= temp_result_16;

				if (IsSilent==FALSE) 
				{
					LogError ("\n Error: Loop:%lu/%lu; Index:%lu; Error mask 0x%04X.",l_NumOfLoops,NumOfLoops, index, temp_result_16);
					LogError (" Address:0x%08lX; Found:0x%08lX; Expected:0x%08lX; ",g_AddrBuff[index], l_actual, l_expected);
					l_actual = HW_DWORD(g_AddrBuff[index]);
					if (l_actual!=l_expected)
						LogError (" Retry read:0x%08lX; \n",l_actual);
					else
						LogPass  (" Retry read:0x%08lX; \n",l_actual);
				}
			}
		}

		
		CPU1_Cmd = l_NumOfLoops&0xFF;
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	

		// read anti-pattern (~ 100 usec)
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			g_DataReadBuff[index] = HW_DWORD(g_AddrBuff[index]);

		// verify anti-pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			UINT32 l_expected = ~g_DataWrite1Buff[index];
			UINT32 l_actual = g_DataReadBuff[index];
			if (l_actual!=l_expected)
			{
				temp_result_32 = l_actual ^ l_expected;
				temp_result_16 = (UINT16)(temp_result_32) | (UINT16)((temp_result_32>>16));
				result_16 |= temp_result_16;

				if (IsSilent==FALSE) 
				{
					LogError ("\n Error: Loop:%lu/%lu; Index:%lu; Error mask 0x%04X.",l_NumOfLoops,NumOfLoops, index, temp_result_16);
					LogError (" Address:0x%08lX; Found:0x%08lX; Expected:0x%08lX; ",g_AddrBuff[index], l_actual, l_expected);
					l_actual = HW_DWORD(g_AddrBuff[index]);
					if (l_actual!=l_expected)
						LogError (" Retry read:0x%08lX; \n",l_actual);
					else
						LogPass  (" Retry read:0x%08lX; \n",l_actual);
				}
			}
		}
	
		l_NumOfLoops++;
	}
	return (result_16);
}
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// This memory test use address/data pairs. 
static UINT16 Run_MemTest_Type5_byte (UINT32 NumOfLoops, BYTE IsSilent)
{
	UINT32 l_NumOfLoops = 0; 
	UINT16 result_16=0,temp_result_16;
	UINT32 index;
	UINT32 temp_result_32;
	UINT32 Temp_addr;

	while (1)
	{
		if (l_NumOfLoops>=NumOfLoops)
			break;

		// stop the test in scan mode (Silent mode) when all bits are with errors 
		if ((IsSilent==TRUE) && ((result_16==0xFFFF)))
			break; 

		/*
		// check for abort from time to time 
		if ( (l_NumOfLoops&0xFF) == 0 )
		{
			if (CheckForAbort()!=0)
				return (0xFFFF);
		}
		*/

		//---------------------------------------------------------
		// fill a buffer data/address pairs 
		if ((l_NumOfLoops&3) == 0)
		{
			// option 1: sequence address (good for DM)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = (UINT8) Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = (UINT8) ~g_DataWrite1Buff[index];
				g_AddrBuff[index] = DDR_ADDR_BASE + index;
			}
		}
		else if ((l_NumOfLoops&3) == 1)
		{
			// option 2: random address (good when d-cache is off)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = (UINT8) Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = (UINT8) ~g_DataWrite1Buff[index];
				// get random address and verify it does not exist in the buffer. 
				UINT32 index2 = (-1);
				while (index2 == (-1))
				{
					Temp_addr = ((rand_32bit()&DDR_ADDR_MASK) + DDR_ADDR_BASE) | 1/*aline to BYTE*/;
					// make sure this new address is not in the list
					for (index2=0; index2<index; index2++)
					{
						if (g_AddrBuff[index2]==Temp_addr)
						{
							index2 = (-1);
							break;
						}
					}
				}
				g_AddrBuff[index] = Temp_addr;  // add this random address to the list
			}
		}
		else
		{
			// option 3: jump in L2 cache size (good when d-cache is on but not for DM testing since it read/write to a line address)
			for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			{
				g_DataWrite1Buff[index] = (UINT8) Golden_Numbers[rand_8bit()]; 
				//g_DataWrite2Buff[index] = (UINT8) ~g_DataWrite1Buff[index];
				g_AddrBuff[index] = DDR_ADDR_BASE + (L2_CACHE_SIZE_IN_BYTE * index) + 1;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		CPU1_Cmd = 100; 
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
		// write
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			HW_BYTE(g_AddrBuff[index]) = g_DataWrite1Buff[index];

		CPU1_Cmd = 100; 
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
		// read & write anti-pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			g_DataReadBuff[index] = HW_BYTE(g_AddrBuff[index]);
			HW_BYTE(g_AddrBuff[index]) = (UINT8)(~g_DataReadBuff[index]);
		}

		// verify pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			UINT32 l_expected = (UINT8)(g_DataWrite1Buff[index]);
			UINT32 l_actual = (UINT8)g_DataReadBuff[index];
			if (l_actual!=l_expected)
			{
				temp_result_32 = l_actual ^ l_expected;
				temp_result_16 = (UINT16)(temp_result_32) | (UINT16)((temp_result_32>>16));
				temp_result_16 |= temp_result_16<<8; // copy the LSB status into MSB since we display 16 bit but test 8 bit data
				result_16 |= temp_result_16;

				if (IsSilent==FALSE) 
				{
					LogError ("\n Error: Loop:%lu/%lu; Index:%lu; Error mask 0x%04X.",l_NumOfLoops,NumOfLoops, index, temp_result_16);
					LogError (" Address:0x%08lX; Found:0x%08lX; Expected:0x%08lX; ",g_AddrBuff[index], l_actual, l_expected);
					l_actual = HW_DWORD(g_AddrBuff[index]);
					if (l_actual!=l_expected)
						LogError (" Retry read:0x%08lX; \n",l_actual);
					else
						LogPass  (" Retry read:0x%08lX; \n",l_actual);
				}
			}
		}

	
		CPU1_Cmd = 100; 
		__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
		// read anti-pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
			g_DataReadBuff[index] = HW_BYTE(g_AddrBuff[index]);

		// verify anti-pattern
		for (index=0; index<TEST_BLOCK_SIZE_IN_DWORD; index++)
		{
			UINT32 l_expected = (UINT8)(~g_DataWrite1Buff[index]);
			UINT32 l_actual = g_DataReadBuff[index];
			if (l_actual!=l_expected)
			{
				temp_result_32 = l_actual ^ l_expected;
				temp_result_16 = (UINT16)(temp_result_32) | (UINT16)((temp_result_32>>16));
				temp_result_16 |= temp_result_16<<8; // copy the LSB status into MSB since we display 16 bit but test 8 bit data
				result_16 |= temp_result_16;

				if (IsSilent==FALSE) 
				{
					LogError ("\n Error: Loop:%lu/%lu; Index:%lu; Error mask 0x%04X.",l_NumOfLoops,NumOfLoops, index, temp_result_16);
					LogError (" Address:0x%08lX; Found:0x%08lX; Expected:0x%08lX; ",g_AddrBuff[index], l_actual, l_expected);
					l_actual = HW_DWORD(g_AddrBuff[index]);
					if (l_actual!=l_expected)
						LogError (" Retry read:0x%08lX; \n",l_actual);
					else
						LogPass  (" Retry read:0x%08lX; \n",l_actual);
				}
			}
		}

		l_NumOfLoops++;
	}
	return (result_16);
}
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
extern UINT16 MemoryCompare (const UINT16 *ptr1, const UINT16 *ptr2, UINT32 SizeInWord)
{
	UINT16 result;
	result = 0;
	while ( (SizeInWord!=0) && (result!=0xFFFF) )
	{
		SizeInWord--;
		result |= *ptr1 ^ *ptr2;
		ptr1++;
		ptr2++;
	}
	return (result); 
}
//-------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
extern UINT16 MemStressTest (void)
{
	UINT16 status;
	
	g_SkipDataAbort = TRUE;
	srand(0);

	status = 0;

	//------------------------------------------------------
	status |= Run_MemTest_Type5_dword (3000, TRUE);
	if (status==0xFFFF)
	{
		g_SkipDataAbort = FALSE;
		return (status);
	}
	//------------------------------------------------------

	//------------------------------------------------------
	status |= Run_MemTest_Type5_byte (300, TRUE);
	if (status==0xFFFF)
	{
		g_SkipDataAbort = FALSE;
		return (status);
	}
	//------------------------------------------------------

	g_SkipDataAbort = FALSE;
	return (status);
}
//---------------------------------------------------------------------------------------------------------------------------

