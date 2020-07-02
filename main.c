#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Common.h"
#include "CoreRegisters.h"
#include "TestMsgCore.h"
#include "Utility.h"
#include "Poleg.h"
#include "Mem_Test.h"
#include "main.h"
#include "MainMenu.h"

volatile UINT32 codestartup;

// --------------------------------------------------------------
// Interrupts
// --------------------------------------------------------------

typedef void (*INT_ROUTINE) (UINT32 Address);

// table pointer of the interrupt routine
extern INT_ROUTINE pUndefinedInstructionRoutine; 
extern INT_ROUTINE pSvcRoutine;
extern INT_ROUTINE pPrefetchAbortRoutine;
extern INT_ROUTINE pDataAbortRoutine;
extern INT_ROUTINE pIrqRoutine;
extern INT_ROUTINE pFiqRoutine;

int g_SkipDataAbort;

void IrqRoutine (UINT32 Address)
{
	LOG_ERROR (("'IRQ' event has been triggered at address 0x%08lX. \n",Address));
	//exit (-1);
}
//-------------------------------------------------------------
void FiqRoutine (UINT32 Address)
{
	LOG_ERROR (("'FIQ' event has been triggered at address 0x%08lX. \n",Address));
	//exit (-1);
}
//-------------------------------------------------------------
void DataAbortRoutine (UINT32 Address)
{
	if (g_SkipDataAbort==TRUE)
		return;
	LOG_ERROR (("'Data Abort' event has been triggered at address 0x%08lX. \n",Address));
	//exit (-1);
}
//-------------------------------------------------------------
void UndefinedInstructionRoutine (UINT32 Address)
{
	LOG_ERROR (("'Undefined Instruction' event has been triggered at address 0x%08lX in ARM state or 0x%08lX in Thumb state. \n",Address-4,Address-2));
	//exit (-1);
}
//-------------------------------------------------------------
void PrefetchAbortRoutine (UINT32 Address)
{
	LOG_ERROR (("'Pre-fetch Abort' event has been triggered at address 0x%08lX. \n",Address));
	//exit (-1);
}
//-------------------------------------------------------------
void SvcRoutine (UINT32 Address)
{
	LOG_ERROR (("'SVC' event has been triggered from address 0x%08lX. \n",Address));
	//exit (-1);
}
//-------------------------------------------------------------


//int g_test_variable = 100;

UINT32 PLL0_Freq_In_KHz, PLL1_Freq_In_KHz, PLL2_Freq_In_KHz;

//-------------------------------------------------------------
int main (void)
{
	UINT32 SCTLR_reg;
	int status;
	
	Uart_AutoDetect ();

	//---------------------------------
	// At start-up, all interrupts vectors are assign to error() function (this assuming code is loaded at 0x0000_0000 and core vectore table is at 0x0000_0000; or code is loaded at 0xFFFD_0000, core vectore table is at 0xFFFF_0000 and RAM2 first 256 bytes swap is enabled).
	pDataAbortRoutine = (INT_ROUTINE) DataAbortRoutine;
	pUndefinedInstructionRoutine = (INT_ROUTINE) UndefinedInstructionRoutine;
	pPrefetchAbortRoutine = (INT_ROUTINE) PrefetchAbortRoutine;
	pSvcRoutine =  (INT_ROUTINE) SvcRoutine;
	pFiqRoutine =  (INT_ROUTINE) FiqRoutine;
	pIrqRoutine =  (INT_ROUTINE) IrqRoutine;
	g_SkipDataAbort = FALSE;
	//---------------------------------

	LogMessage ("\n\n");
	LogHeader (" Build Date: %s,%s with GCC %u.%u.%u ",__DATE__,__TIME__,__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
	//LogMessage ("Core test random seed: 0x%08lX \n",GetRandSeed());
	//-------------------------------------------------------------
	// Disable Instruction cache
	//CP15_READ (SCTLR_reg, 0, c1, c0, 0);
	//SCTLR_reg &= ~((UINT32)1<<12);
	//CP15_WRITE (SCTLR_reg ,0, c1, c0, 0);
	//-------------------------------------------------------------
	// Invalidate Instruction cache.
	InvalidateCache ();
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// verify data initialize correctly (used when value is stored at ROM and copy to RAM). 
	/*
	g_test_variable++; // g_test_variable initialize to 100
	if (g_test_variable!=101)
	{
		LogWarning ("start-up code failed to initialize data variables ! \n");
		exit (-1);
	}
	*/
	//-------------------------------------------------------------
	CP15_READ (SCTLR_reg, 0, c1, c0, 0);
	codestartup = (UINT32)&_startup; // !! codestartup Must be volatile and _startup extern !! otherwise compiler remove some code
	if (codestartup == 0)
	{
		if (((SCTLR_reg>>13)&0x1) != 0) 	
		{
			LogWarning ("> Override vector table.\n");
			LogWarning ("  >> Set CPU vector table to address 0x0000_0000. \n");
			SCTLR_reg &= ~((UINT32)1<<13); // set vector table into address 0x0000_0000
			CP15_WRITE (SCTLR_reg, 0, c1, c0, 0);
		}
	}
	else if (codestartup == 0xFFFD0000)
	{
		if (((SCTLR_reg>>13)&0x1) == 0) 	
		{
			LogWarning ("> Override vector table.\n");
			LogWarning ("  >> Mapped first 256 bytes of address 0xFFFF_0000 to address 0xFFFD_0000. \n");
			SET_BIT_REG (FLOCKR1,18); // RAM2 first 256 bytes are mapped to address range FFFF_0000h to FFFF_00FFh.
			LogWarning ("  >> Set CPU vector table to address 0xFFFF_0000. \n");
			SCTLR_reg |= ((UINT32)1<<13); // set vector table into address 0xFFFF_0000
			CP15_WRITE (SCTLR_reg, 0, c1, c0, 0);
		}
	}
	else
	{
		LogWarning ("> Code start-up at address 0x%08lX. \n",codestartup);
		LogWarning ("  Code was not compiled to address 0x00000000 or 0xFFFD0000; interrupts will not work unless code vector table are copied ! \n");
	}
	//-------------------------------------------------------------
	LogMessage ("\n");
	LogHeader (" BMC Information ");
	//-------------------------------------------------------------
	UINT32 PDID_reg = READ_REG(PDID);
	switch (PDID_reg) 
	{
		case Poleg_ID_Z1:	LogError ("> Error: Found NPCM7mnx BMC chip version Z1.\n");	return (1); 
		case Poleg_ID_Z2:	LogPass ("> Found NPCM7mnx BMC chip version A1.\n");	break;
		default:
			LogError ("Error: Unknown NPCM7mnx BMC chip version. Found PDID=0x%08lX\n", PDID_reg); return (1);  
	}
	//-------------------------------------------------------------
	//--------------------------------------------------------------
	// read SCTLR register
	CP15_READ (SCTLR_reg, 0, c1, c0, 0);
	LogMessage ("> CPU CP15 SCTLR: 0x%08lX \n",SCTLR_reg);
	if (((SCTLR_reg>>0)&0x1)== 1)
		LogMessage ("   >> MMU is enable.\n");
	else
		LogMessage ("   >> MMU is disable.\n");
	if (((SCTLR_reg>>2)&0x1)== 1)
		LogMessage ("   >> Data caching is enable.\n");
	else
		LogMessage ("   >> Data caching is disable.\n");
	if (((SCTLR_reg>>11)&0x1)== 1)
		LogMessage ("   >> Program flow prediction is enable.\n");
	else
		LogMessage ("   >> Program flow prediction is disable.\n");
	if (((SCTLR_reg>>12)&0x1)== 1)
		LogMessage ("   >> Instruction caching is enable.\n");
	else
		LogMessage ("   >> Instruction caching is disable.\n");
	if (((SCTLR_reg>>13)&0x1)== 1)
		LogMessage ("   >> High exception vectors, Hivecs, base address 0xFFFF0000.\n");
	else
		LogMessage ("   >> Normal exception vectors, base address 0x00000000.\n");
	//-------------------------------------------------------------
	Check_Clocks ();
	//--------------------------------------------------------------

	status = MainMenu();

	return (status);
	
} // *End of main*
//------------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------
// 16/02/2015: Fixed PLL function; the function always returned "PLL in reset"
// 08/04/2015: Fixed PLL_FOUT calculation so it will not overflow DWORD.
UINT32 Check_BMC_PLL (DWORD PllCon, DWORD PllIndex)
{
	DWORD PLL_FBDV,PLL_OTDV1, PLL_OTDV2, PLL_INDV,PLL_FOUT;

	LogMessage ("  * PLLCON%u = 0x%08lX. ",PllIndex,PllCon);

	if ( ((PllCon>>12)&0x1) ==1 )
	{
		LogWarning (" (PLL is in Power-Down).\n");
		return (0);
	}

	PLL_FBDV = (PllCon >> 16) & 0xFFF; //  (bits 16..27, 12bit)
	PLL_OTDV1 = (PllCon >> 8) & 0x7; //  (bits 8..10, 3bit)
	PLL_OTDV2 = (PllCon >> 13) & 0x7; //  (bits 13..15, 3bit)
	PLL_INDV = (PllCon >> 0) & 0x3F; //  (bits 0..5, 6 bits)

	PLL_FOUT = 25000/*000*/ * (PLL_FBDV);
	PLL_FOUT = PLL_FOUT / ( (PLL_OTDV1) * (PLL_OTDV2) * (PLL_INDV) );
	if ((PllIndex==1) && (READ_REG(PDID)!=Poleg_ID_Z1))
		PLL_FOUT = PLL_FOUT /2;
	LogMessage (" (%lu MHz; ",PLL_FOUT/1000/*000*/);

	if ( ((PllCon>>31)&0x1) == 0 )
		LogWarning ("PLL is not locked");
	else
		LogPass ("locked");

	LogMessage (")\n");

	return (PLL_FOUT); // clock in KHz
}
//--------------------------------------------------------------------------
void Check_Clocks (void)
{
	UINT32 ClkSel,ClkDiv1,ClkDiv2,ClkDiv3;

	LogMessage ("> PLLs and Clocks: \n");

	PLL0_Freq_In_KHz = Check_BMC_PLL (READ_REG (PLLCON0),0);
	PLL1_Freq_In_KHz = Check_BMC_PLL (READ_REG (PLLCON1),1);
	PLL2_Freq_In_KHz = Check_BMC_PLL (READ_REG (PLLCON2),2);
	ClkSel = READ_REG (CLKSEL);
	LogMessage ("  * CLKSEL = 0x%08lX \n", ClkSel);
	//----------------------------------------
	LogMessage ("    - CPU Clock Source (CPUCKSEL) => ");
	switch ((ClkSel>>0)&0x03)
	{
	case 0:
		LogPass ("PLL 0. \n");
		break;
	case 1:
		LogPass ("PLL 1. \n");
		// 25-MAR-2017: move these code line to MC_Drv.c just before PLL1 configuration 
		//LogWarning (" -- Override setting and set CPU cock source to PLL0. \n");
		//Sleep (1000); // wait for all UART data to be send to console (in case CPU will stuck user will be able to see this note) 
		//WRITE_REG (CLKSEL,ClkSel&0xFFFFFFFC);
		break;
	case 2:
		LogWarning ("CLKREF clock (debug). \n");
		break;
	case 3:
		LogWarning ("Bypass clock from pin SYSBPCK (debug). \n");
		break;
	default :
		LOG_ERROR  (("Invalid source."));
		exit (-1);
	}
	//----------------------------------------
	LogMessage ("    - Memory Controller Clock Source (MCCKSEL) => ");
	switch ((ClkSel>>12)&0x03)
	{
	case 0:
		LogPass ("PLL 1. \n");
		break;
	case 2:
		LogWarning ("CLKREF Clock (debug). \n");
		break;
	case 3:
		LogWarning ("MCBPCK Clock (debug). \n");
		break;
	default :
		LOG_ERROR  (("Invalid source."));
		exit (-1);
	}
	//----------------------------------------

	//----------------------------------------
	ClkDiv1 = READ_REG (CLKDIV1);
	LogMessage ("  * CLKDIV1 = 0x%08lX \n",ClkDiv1);
	if ((ClkDiv1&0x1)==0)
		LogWarning ("    - CLK2(AXI16) = CPU clock. \n");
	else
		LogPass ("    - CLK2(AXI16) = CPU clock /2.  \n");
	switch ((ClkDiv1>>26)&0x03)
	{
	case 0:
		LogWarning ("    - CLK4 = CLK2) \n");
		break;
	case 1:
		LogPass ("    - CLK4 = CLK2/2) \n");
		break;
	case 2:
		LogWarning ("    - CLK4 = CLK2/3) \n");
		break;
	case 3:
		LogWarning ("    - CLK4 = CLK2/4) \n");
		break;
	}
	ClkDiv2 = READ_REG (CLKDIV2);
	LogMessage ("  * CLKDIV2 = 0x%08lX \n",ClkDiv2);
	ClkDiv3 = READ_REG (CLKDIV3);
	LogMessage ("  * CLKDIV3 = 0x%08lX \n",ClkDiv3);
	//----------------------------------------
} 



	


	


	
	
	







	


	
	



