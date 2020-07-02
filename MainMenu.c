#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Common.h"
#include "CoreRegisters.h"
#include "TestMsgCore.h"
#include "Utility.h"
#include "Poleg.h"
#include "MainMenu.h"
#include "Mem_Test.h"
//#include "MC_Drv.h"
#include "MC_Cadence_Define_Register_CTL_C.h"	// MC Field Definition
#include "DDR4_FHY_Registers.h"
#include "main.h"


// Note:
// VdIVW: JEDEC defined 136mV pk-pk mask voltage that is 68mV on each direction.  
// TdIVW: JEDEC defined 0.2UI timing window that is 240psec@800MHz that is 120psec for each direction. 
// Since we don't test margin with a scope but use memory tests on each point we can relax JEDEC values. 
#define VrefDQ_Margin 50 // testing for 50mV margin on each direction. (each vref step is ~7.5mV)
#define TimeDQ_Margin 3  // 3 for 90psec margin on each direction. (each delay step is ~30psec)

#define Dummy_Delay_Short (64*1024)   // dummy access to memory in order for values to update (not sure if this is needed at all)
#define Dummy_Delay_Long  (1024*1024) // dummy access to memory in order for values to update (not sure if this is needed at all)
 
//------------------------------------------------------------------------------
BOOL TestInputMargin (void);
BOOL TestOutputMargin (void);

BOOL Sweep_Trim_InputDQ_Offset  (UINT32 DqMask[], UINT8 ReqMargin, char *Title);
BOOL Sweep_Trim_InputDQS_Offset (UINT32 DqMask[], UINT8 ReqMargin, char *Title);
BOOL Sweep_Trim_OutputDQ_Offset (UINT32 DqMask[], UINT8 ReqMargin, char *Title);
//------------------------------------------------------------------------------
//void JumpToUboot (void);
int MPR_Page2_Readout (UINT8 *MPR);
int WriteModeReg (UINT8 Index, UINT32 Data);
BOOL DisplayInfo (void);
int RelocateTlbs (int Mode);
static UINT32 GetNumOfLanes (void);
//------------------------------------------------------------------------------
UINT16 VrefDQ_Convert_Step_to_mV (UINT8 Range, UINT8 Step);
UINT8 VrefDQ_Convert_mV_to_Step (UINT8 Range, UINT16 VrefDQ_mV);
int VrefDQ_Convert_mV_to_Step_Range (UINT16 VrefDQ_mV, UINT8 *Range/*init with default*/, UINT8 *Step);
void VrefDQ_Set_DRAM (UINT8 Range, UINT8 Step);
void VrefDQ_Get_DRAM (UINT8 *Range, UINT8 *Step);
void VrefDQ_Set_PHY (UINT8 ilane, UINT8 Range, UINT8 Step);
void VrefDQ_Get_PHY (UINT8 ilane, UINT8 *Range, UINT8 *Step);
//------------------------------------------------------------------------------
int  DQ_to_INT (UINT8 DQ_Trim);
UINT8 INT_to_DQ(int DQ_Trim);
//------------------------------------------------------------------------------


char g_KeyPress;
int g_SkipDataAbort;
UINT32 g_TlbsOriginAddr;
int g_IsMMUenable;

//------------------------------------------------------------------------------
const char g_Text_AbortedByUser [] = {"\n --- aborted by user.\n"};
const char g_Text_InvalidFormat [] = {"--- Invalid format or value, retry.\n"};
//---------------------------------------------------------------------------------------------------------------------
extern int MainMenu (void)
{
	int status = 0;
	LogMessage ("\n");

	if ( (READ_BIT_REG(INTCR2,19)==0) || (READ_BIT_REG(IPSRST1,12)==1) )
	{
		LogError ("\n ***** Error: Memory Controller was not initialized ****** \n");
		return (1);
	}

	RelocateTlbs(0); //  If MMU is enabled, relocate TLBs to SRAM	

	status |= DisplayInfo();

	LogWarning ("\nNOTE:\n");
	LogWarning ("* DDR4 memory is not valid while sweeping, in case host use BMC graphics, it may stuck. \n");
	LogWarning ("* Press 'ESC' to abort the sweep. \n");

	// configure GPIO00; it been used in CPU1_Toggle code. 
	//SET_BIT_REG (GPnOE(0), 0);   // enable GPIO00 OE 
	//CLEAR_BIT_REG (GPnOTYP(0), 0); // set GPIO00 Push-Pull
	/* test GPIO
	while (1)
	{
		WRITE_REG (HW_DWORD(0xF001000C), 1);
		WRITE_REG (HW_DWORD(0xF001000C), 0);

	}
	*/

	WRITE_REG (SCRPAD,(UINT32)(&CPU1_Toggle));
	CPU1_Cmd = 0; 
	CPU1_Rev = 0;
	__asm__ ("SEV"); // causes an event to be signaled to all cores within a multiprocessor system	
	Sleep (1000);
	if (CPU1_Rev == 0)
	{
		LogError ("\n ***** Error: CPU1 may already running. Issue hardware reset before running this test !!! ***** \n");
		return (1);
	}
	//LogMessage ("> CPU1 run toggle test (jump to address 0x%08lx, Rev 0x%08lx). \n", READ_REG(SCRPAD), CPU1_Rev);


	UINT32 TmpDsclCnt = 0;
	TmpDsclCnt = READ_REG(DSCL_CNT);
	if (((TmpDsclCnt>>24)&0x01)==0x01)
	{
		LogWarning ("> Disable DSCL before sweeping.\n");
		WRITE_REG (DSCL_CNT,TmpDsclCnt & (~((UINT32)1<<24)));
	}

	status |= TestInputMargin();
	status |= TestOutputMargin();

	if (status == 0)
	{
		LogPass (" ****************************\n");
		LogPass (" *** Test Completed: PASS ***\n");
		LogPass (" ****************************\n");
	}
	else
	{
		LogError (" ****************************\n");
		LogError (" *** Test Completed: FAIL ***\n");
		LogError (" ****************************\n");
	}
	

	// restore DSCL to default 
	WRITE_REG (DSCL_CNT,TmpDsclCnt); 

	// JumpToUboot ();
	
	RelocateTlbs(1); // if MMU is enabled, return TLBs to DDR

	// 15/09/2015: add RAM-V and vector table return to normal
	UINT32 SCTLR_reg;
	CLEAR_BIT_REG (FLOCKR1,18); // disable RAM2 first 256 bytes mapping (TBD: I assume boot-block always disable it so UBOOT expected this to be disabled)
	CP15_READ (SCTLR_reg, 0, c1, c0, 0);
	SCTLR_reg &= ~((UINT32)1<<13); // set vector table into address 0x0000_0000 (TBD: I assume boot-block always set vector table to 0x0000_0000 so UBOOT expected this)
	CP15_WRITE (SCTLR_reg, 0, c1, c0, 0);
				
	return (status);
}
//---------------------------------------------------------------------------------------------------------------------
BOOL TestInputMargin (void)
{
	UINT8 ilane;
	UINT8 PHY_VrefDQ_Step_Qrigin[3], PHY_VrefDQ_Range_Origin[3];
	UINT16 VrefDQ_mV_Qrigin[3];
	UINT8 PHY_VrefDQ_Step, PHY_VrefDQ_Range;
	UINT16 VrefDQ_mV;
	UINT8 NumOfLane = GetNumOfLanes();
	UINT32 DqMask[3] = {0,0,0};
	BOOL status = 0;

	LogMessage ("\n");
	//--------------------------------------------------------------------------
	// Get VrefDQ default value
	LogHeader (" PHY VrefDQ: Default SetPoint");
	for (ilane=0; ilane<NumOfLane; ilane++)
	{
		VrefDQ_Get_PHY (ilane, &PHY_VrefDQ_Range_Origin[ilane], &PHY_VrefDQ_Step_Qrigin[ilane]);
		VrefDQ_mV_Qrigin[ilane] = VrefDQ_Convert_Step_to_mV(PHY_VrefDQ_Range_Origin[ilane], PHY_VrefDQ_Step_Qrigin[ilane]);
		LogMessage ("> Lane%u: %u mv (range: %u, step %u)\n", ilane, VrefDQ_mV_Qrigin[ilane], PHY_VrefDQ_Range_Origin[ilane]+1, PHY_VrefDQ_Step_Qrigin[ilane]);
	}
	//--------------------------------------------------------------------------
	// Sweep input parameters
	status |= Sweep_Trim_InputDQS_Offset (DqMask, 8, "@VrefDQ:default");
	//--------------------------------------------------------------------------
	LogHeader (" Set PHY VrefDQ: SetPoint+%umV ", VrefDQ_Margin);
	for (ilane=0; ilane<NumOfLane; ilane++)
	{
		PHY_VrefDQ_Step = 0;
		PHY_VrefDQ_Range = PHY_VrefDQ_Range_Origin[ilane];
		VrefDQ_mV = VrefDQ_mV_Qrigin[ilane] + VrefDQ_Margin;
		VrefDQ_Convert_mV_to_Step_Range (VrefDQ_mV, &PHY_VrefDQ_Range, &PHY_VrefDQ_Step); // look for best range/step that meet mV. Try first with current range. 
		VrefDQ_mV = VrefDQ_Convert_Step_to_mV(PHY_VrefDQ_Range, PHY_VrefDQ_Step);
		LogMessage ("> Lane%u: %u mv (range: %u, step %u)\n", ilane, VrefDQ_mV, PHY_VrefDQ_Range+1, PHY_VrefDQ_Step);
		VrefDQ_Set_PHY (ilane, PHY_VrefDQ_Range, PHY_VrefDQ_Step); // Set new VrefDQ values 
	}
	//--------------------------------------------------------------------------
	// Sweep input parameters
	status |= Sweep_Trim_InputDQ_Offset (DqMask, TimeDQ_Margin, "@VrefDQ:+dV");
	//--------------------------------------------------------------------------
	LogHeader (" Set PHY VrefDQ: SetPoint-%umV", VrefDQ_Margin);
	for (ilane=0; ilane<NumOfLane; ilane++)
	{
		PHY_VrefDQ_Step = 0;
		PHY_VrefDQ_Range = PHY_VrefDQ_Range_Origin[ilane];
		VrefDQ_mV = VrefDQ_mV_Qrigin[ilane] - VrefDQ_Margin; 
		VrefDQ_Convert_mV_to_Step_Range (VrefDQ_mV, &PHY_VrefDQ_Range, &PHY_VrefDQ_Step); // look for best range/step that meet mV. Try first with current range. 
		VrefDQ_mV = VrefDQ_Convert_Step_to_mV(PHY_VrefDQ_Range, PHY_VrefDQ_Step);
		LogMessage ("> Lane%u: %u mv (range: %u, step %u)\n", ilane, VrefDQ_mV, PHY_VrefDQ_Range+1, PHY_VrefDQ_Step);
		VrefDQ_Set_PHY (ilane, PHY_VrefDQ_Range, PHY_VrefDQ_Step); // Set new VrefDQ values 
	}
	//--------------------------------------------------------------------------
	// Sweep input parameters
	status |= Sweep_Trim_InputDQ_Offset (DqMask, TimeDQ_Margin, "@VrefDQ:-dV");
	//--------------------------------------------------------------------------
	// restore trim values 
	for (ilane=0; ilane<NumOfLane; ilane++)
		VrefDQ_Set_PHY (ilane, PHY_VrefDQ_Range_Origin[ilane], PHY_VrefDQ_Step_Qrigin[ilane]); // Set new VrefDQ values 
	//--------------------------------------------------------------------------
	
	LogMessage ("\n");

	return (status);
}	

//---------------------------------------------------------------------------------------------------------------------
BOOL TestOutputMargin (void)
{
	UINT8 DRAM_VrefDQ_Step_Qrigin, DRAM_VrefDQ_Range_Origin;
	UINT16 VrefDQ_mV_Qrigin;
	UINT8 DRAM_VrefDQ_Step, DRAM_VrefDQ_Range;
	UINT16 VrefDQ_mV;
	UINT32 DqMask[3] = {0,0,0};
	BOOL status = 0;
	

	//--------------------------------------------------------------------------
	// Get VrefDQ default value
	VrefDQ_Get_DRAM (&DRAM_VrefDQ_Range_Origin, &DRAM_VrefDQ_Step_Qrigin);
	VrefDQ_mV_Qrigin = VrefDQ_Convert_Step_to_mV(DRAM_VrefDQ_Range_Origin, DRAM_VrefDQ_Step_Qrigin);
	LogHeader (" DRAM VrefDQ: Default SetPoint");
	LogMessage ("> %u mv (range: %u, step %u)\n\n", VrefDQ_mV_Qrigin, DRAM_VrefDQ_Range_Origin+1, DRAM_VrefDQ_Step_Qrigin);
	//-------------------------------------------------------------------------
	// Sweep output parameters
	//status |= Sweep_Trim_OutputDQ_Offset (DqMask, 4, "@VrefDQ:default");
	//-------------------------------------------------------------------------
	DRAM_VrefDQ_Step = 0;
	DRAM_VrefDQ_Range = DRAM_VrefDQ_Range_Origin;
	VrefDQ_mV = VrefDQ_mV_Qrigin + VrefDQ_Margin; 
	VrefDQ_Convert_mV_to_Step_Range (VrefDQ_mV, &DRAM_VrefDQ_Range, &DRAM_VrefDQ_Step);
	VrefDQ_mV = VrefDQ_Convert_Step_to_mV(DRAM_VrefDQ_Range, DRAM_VrefDQ_Step);
	LogHeader (" Set DRAM VrefDQ: SetPoint+%umV", VrefDQ_Margin);
	LogMessage ("> %u mv (range: %u, step %u)\n", VrefDQ_mV, DRAM_VrefDQ_Range+1, DRAM_VrefDQ_Step);
	VrefDQ_Set_DRAM (DRAM_VrefDQ_Range, DRAM_VrefDQ_Step); // Set new VrefDQ values 
	//--------------------------------------------------------------------------
	// Sweep output parameters
	status |= Sweep_Trim_OutputDQ_Offset (DqMask, TimeDQ_Margin, "@VrefDQ:+dV");
	//---------------------------------------------------------------------------
	DRAM_VrefDQ_Step = 0;
	DRAM_VrefDQ_Range = DRAM_VrefDQ_Range_Origin;
	VrefDQ_mV = VrefDQ_mV_Qrigin - VrefDQ_Margin; 
	VrefDQ_Convert_mV_to_Step_Range (VrefDQ_mV, &DRAM_VrefDQ_Range, &DRAM_VrefDQ_Step);
	VrefDQ_mV = VrefDQ_Convert_Step_to_mV(DRAM_VrefDQ_Range, DRAM_VrefDQ_Step);
	LogHeader (" Set DRAM VrefDQ: SetPoint-%umV", VrefDQ_Margin);
	LogMessage ("> %u mv (range: %u, step %u)\n", VrefDQ_mV, DRAM_VrefDQ_Range+1, DRAM_VrefDQ_Step);
	VrefDQ_Set_DRAM (DRAM_VrefDQ_Range, DRAM_VrefDQ_Step); // Set new VrefDQ values 
	//--------------------------------------------------------------------------
	// Sweep output parameters
	status |= Sweep_Trim_OutputDQ_Offset (DqMask, TimeDQ_Margin, "@VrefDQ:-dV");
	//--------------------------------------------------------------------------
	// restore trim values 
	VrefDQ_Set_DRAM (DRAM_VrefDQ_Range_Origin, DRAM_VrefDQ_Step_Qrigin); 
	//--------------------------------------------------------------------------
	LogMessage ("\n");

	return (status);
}

//---------------------------------------------------------------------
// If MMU is enabled, relocate TLBs: 
// Mode = 0: to top 16K of the internal
// Mode = 1: back to previous location stored at g_TlbsOriginAddr (global variable)
int RelocateTlbs (int Mode)
{
	UINT32 RelocateToAddr;
	UINT32 SCTLR, TTBR0, TTBR1, TTBCR; 
	UINT16 status; 

	CP15_READ (SCTLR, 0, c1, c0, 0);
	if (((SCTLR>>0)&0x1)!= 1)
	{
		g_IsMMUenable = FALSE;
		return (0); // MMU is disabled, no need to relocate.
	}

	g_IsMMUenable = TRUE;

	CP15_READ (TTBR0, 0, c2, c0, 0);  
	CP15_READ (TTBR1, 0, c2, c0, 1);
	CP15_READ (TTBCR, 0, c2, c0, 2);
	LogMessage ("\n> Found MMU is enable. TLBs must be relocated. \n");

	if ((TTBCR&0x07)!= 0)
	{
		LOG_ERROR ((" TTBR0.N is not 0; don't know how to relocate TLBs. "));
		return (-1);
	}

	if (Mode==0)
		RelocateToAddr = 0xFFFF0000-(16*1024); // Relocate TLBs to top 16K of the internl SRAM.
	else
		RelocateToAddr = g_TlbsOriginAddr; // Relocate TBLs back to previous location stored at g_TlbsOriginAddr

	g_TlbsOriginAddr = TTBR0&0xFFFFC000;
	LogMessage ("  >> Current address: 0x%08lX; Size: 16KB. \n",g_TlbsOriginAddr);
	TTBR0 &= ~0xFFFFC000;
	TTBR0 |= RelocateToAddr & 0xFFFFC000;
	LogMessage ("  >> Relocate address: 0x%08lX. \n",TTBR0&0xFFFFC000);

	// Copy TLBs from current location to new location
	MemCpy ((void*)(TTBR0&0xFFFFC000),(void*)(g_TlbsOriginAddr), 16*1024);
	status = MemoryCompare ((UINT16*)(TTBR0&0xFFFFC000),(UINT16*)(g_TlbsOriginAddr),16*1024/2);
	if (status != 0)
	{
		LOG_ERROR ((" Failed to copy TLBs. "));
		return (-1);
	}
	CP15_WRITE (TTBR0, 0, c2, c0, 0); 
	LogPass ("  >> Done. \n");
	return (0);
}
//---------------------------------------------------------------------------------------------------------------------
static UINT32 GetNumOfLanes (void)
{ // check in MC registers if ECC is enabled, if not 2 lanes are used
	UINT32 TmpReg32 = IOR32(MC_BASE_ADDR + (93*4)); // DENALI_CTL_93_DATA
	if (((TmpReg32>>24)&0x01)==0)
		return (2); // ECC is not used.

	return (3); // ECC is used.
}
//--------------------------------------------------
const UINT8 CL[16] = {9,10,11,12,13,14,15,16,18,20,22,24,0,0,0,0};
const UINT8 CWL[8] = {9,10,11,12,14,16,18,0};
BOOL DisplayInfo (void)
{
	UINT32 TmpReg32;
	UINT32 ilane,ibit;
	UINT8 val;
	int status;
	UINT8 NumOfLanes;
	
	LogMessage("\n");
	LogHeader (" DDR4 PHY Information ");

	//----------------------------------------------------------
	LogMessage ("> Clock frequency: \n");
	Check_BMC_PLL (READ_REG (PLLCON1), 1);

	//----------------------------------------------------------
	LogMessage ("> PHY Revision: 0x%08lX \n",READ_REG (PHY_REV_CNTRL_REG));

	//----------------------------------------------------------
	TmpReg32 = IOR32(MC_BASE_ADDR + (93*4)); // DENALI_CTL_93_DATA
	if (((TmpReg32>>24)&0x01)==0)
	{
		LogWarning ("> ECC is OFF. \n");
		NumOfLanes = 2;
	}
	else
	{
		LogPass ("> ECC is ON. \n");
		NumOfLanes = 3;

		UINT32 int_status = IOR32(MC_BASE_ADDR + (116*4)); // DENALI_CTL_116_DATA  read all status

		if (((int_status>>5)&0x3)!=0)
		{
			UINT32 ECC_U_ADDR = IOR32(MC_BASE_ADDR + (95*4)); // DENALI_CTL_95_DATA  Address of uncorrectable ECC event.
			UINT32 ECC_U_DATA = IOR32(MC_BASE_ADDR + (97*4)); // DENALI_CTL_97_DATA  Data of uncorrectable ECC event.
			LogError("  >> Uncorrectable ECC errors: Addr:0x%08lX, Data:0x%08lX.\n",ECC_U_ADDR,ECC_U_DATA);
			IOW32(MC_BASE_ADDR + (117*4),0x3<<5); // DENALI_CTL_117_DATA  clear status
		}

		if (((int_status>>3)&0x3)!=0)
		{
			UINT32 ECC_C_ADDR = IOR32(MC_BASE_ADDR + (98*4)); // DENALI_CTL_98_DATA  Address of correctable ECC event.
			UINT32 ECC_C_DATA = IOR32(MC_BASE_ADDR + (100*4)); // DENALI_CTL_100_DATA  Data of correctable ECC event.
			LogError("  >> Correctable ECC errors:   Addr:0x%08lX, Data:0x%08lX.\n",ECC_C_ADDR,ECC_C_DATA);
			IOW32(MC_BASE_ADDR + (117*4),0x3<<3); // DENALI_CTL_117_DATA  clear status
		}
	}

	//---------------------------------------------------------------
	TmpReg32 = READ_REG(SCL_START) /*cuml_scl_rslt*/ & READ_REG (WRLVL_CTRL) /*write-leveling status*/ & (~(READ_REG(DYNAMIC_WRITE_BIT_LVL)>>20)) /*bit_lvl_wr_failure_status*/ & (READ_REG(IP_DQ_DQS_BITWISE_TRIM)>>8) /*bit_lvl_done_status*/ & (~(READ_REG(DYNAMIC_BIT_LVL)>>14)) /*bit_lvl_failure_status*/ ;
	LogMessage ("> Self-Configuring Logic (SCL), Write-Leveling and Read/Write Bit-Leveling status: \n");
	for (ilane=0;ilane<NumOfLanes;ilane++)
	{
		if ((TmpReg32&(1<<ilane))==0)
			LogError ("  >> lane%u: Failed.\n",ilane);
		else
			LogPass  ("  >> lane%u: Passed.\n",ilane);
	}
	//---------------------------------------------------------------
	TmpReg32 = READ_REG(DSCL_CNT);
	LogMessage ("> DSCL_CNT (0x67) = 0x%08lX \n", TmpReg32);
	if (((TmpReg32>>24)&0x01)==0x00)
		LogWarning("  >> Dynamic SCL is OFF !\n");
	else
	{
		LogPass ("  >> Dynamic SCL is ON. \n");
		LogMessage ("  >> dscl_exp_cnt = %lu x 256 PHY clock cycles. \n",TmpReg32&0x00FFFFFF);
		if (((TmpReg32>>25)&0x01)==1)
			LogMessage ("  >> dscl_save_restore_needed = 1. \n");
		else
		{
			UINT32 Reg32 = READ_REG(PHY_SCL_START_ADDR);
			UINT32 Addr = ((Reg32&0x3FF0000) >> 16)*4 +  (Reg32&0x7FFF)*0x4000 +   ((Reg32&0x60000000) >> 29)*0x1000 ;
			LogWarning ("  >> dscl_save_restore_needed = 0. (PHY_SCL_START_ADDR = 0x%08lX; Actual Addr = 0x%08lX). \n",Reg32,Addr);
		}

		if ( (READ_REG(DYNAMIC_BIT_LVL)&0x01) == 0 ) 
			LogMessage ("  >> Dynamic SCL without Bit-Levelling. \n");
		else 
		{
			LogMessage ("  >> Dynamic SCL with Bit-Levelling. \n");
			if ( (READ_REG(DYNAMIC_WRITE_BIT_LVL)&0x01) == 0 ) 
				LogMessage ("  >> Dynamic SCL without Write Bit-Levelling. \n");
			else 
				LogMessage ("  >> Dynamic SCL with Write Bit-Levelling. \n");
		}
	}

	//--------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // just for case
	TmpReg32 = READ_REG(PHY_DLL_ADRCTRL);
	LogMessage ("> PHY_DLL_ADRCTRL (0x4A) = 0x%08lX \n", TmpReg32);
	LogMessageColor(COLOR_YELLOW, "  >> Number of delay elements corresponds to one clock cycle (dll_mas_dly): %lu. \n", TmpReg32>>24);
	LogMessageColor(COLOR_YELLOW, "  >> Number of delay elements corresponds to 1/4 clock cycle (dll_slv_dly_wire): %lu.\n", (TmpReg32>>16)&0x3F);
	if ((TmpReg32&0x100)==0) 
	{
		LogMessage ("  >> Delay of control signals with respect to output data signals (dlls_trim_adrctl): ");
		if ((TmpReg32&0x200)!=0) 
			LogMessage ("+"); // set to increment; limited by 1/4 clock (i.e., from dlls_trim_clk)
		else
			LogMessage ("-"); // decrement; limited by 1/4 clock
		LogMessage ("%lu.\n", (TmpReg32>>0)&0x3F); 
	}
	//---------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // just for case
	TmpReg32 = READ_REG(PHY_DLL_RECALIB);
	LogMessage ("> PHY_DLL_RECALIB (0x49) = 0x%08lX \n", TmpReg32);
	LogMessage ("  >> Delay of row/column address signals with respect to data output signals (dlls_trim_adrctrl_ma): ");
	if ((TmpReg32&((UINT32)1<<27))!=0) 
		LogMessage ("+"); // set to increment; limited by 1/4 clock (i.e., from dlls_trim_clk)
	else
		LogMessage ("-"); // decrement; limited by 1/4 clock
	LogMessage ("%lu.\n", (TmpReg32>>0)&0x3F);

	//---------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // no a must, just in case
	TmpReg32 = READ_REG(PHY_DLL_TRIM_CLK);
	LogMessage ("> PHY_DLL_TRIM_CLK (0x69) = 0x%08lX \n", TmpReg32);
	LogMessage ("  >> Delay of clock signal with respect to data output signals (dlls_trim_clk): ");
	if ((TmpReg32&0x40)!=0) 
		LogMessage ("+"); // set to increment;
	else
		LogMessage ("-"); // decrement. (limited by the value of 1/4 clock)
	LogMessage ("%lu.\n", (TmpReg32>>0)&0x3F);

	//---------------------------------------------------------------
	/*
	TmpReg32 = READ_REG(PHY_DLL_RISE_FALL);
	LogMessage ("> PHY_DLL_RISE_FALL (0x63) = 0x%08lX. \n", TmpReg32);
	LogMessage ("  >> rise_cycle_cnt (half-cycle period detected by the DLL): = %lu \n", (TmpReg32>>4)&0xFF);
	*/
	//---------------------------------------------------------------
	
	//---------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // just in case
	TmpReg32 = READ_REG(SCL_LATENCY);
	LogMessage ("> SCL_LATENCY (0x43) = 0x%08lX. \n", TmpReg32);
	LogMessage ("  >> capture_clk_dly: = %lu \n", (TmpReg32>>0)&0xF);
	LogMessageColor(COLOR_YELLOW,"  >> main_clk_dly: = %lu \n", (TmpReg32>>4)&0xF);
	//---------------------------------------------------------------

	LogMessage ("> Data Output Delay: \n");
	for (ilane=0; ilane<NumOfLanes; ilane++)
	{
		LogMessage ("  >> Lane%lu: ", ilane );
		for (ibit = 0 ; ibit < 8 ; ibit++) 
		{
			WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
			val = (UINT8) (READ_REG (OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F) ;
			LogMessageColor(COLOR_YELLOW,"DQ%lu:0x%02X, ",ibit,val);
		}

		ibit = 8;
		WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
		val = (UINT8) (READ_REG (OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F) ;
		LogMessageColor(COLOR_YELLOW,"DM:0x%02X, ",val);

		ibit = 9;
		WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
		val = (UINT8) (READ_REG (OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F) ;
		LogMessage ("Base:0x%02X, ",val);

		WRITE_REG (PHY_LANE_SEL,(ilane*6));
		val = (UINT8) (READ_REG (PHY_DLL_TRIM_1) & 0x3F) ;
		if (((READ_REG (PHY_DLL_INCR_TRIM_1)>>ilane) & 0x01)==0x01)
			LogMessage ("trim_1:+%u, ",val); // output dqs timing with respect to output dq/dm signals
		else
			LogMessage ("trim_1:-%u, ",val);

		WRITE_REG (PHY_LANE_SEL,(ilane*5));
		val = (UINT8) ((READ_REG (UNQ_ANALOG_DLL_3) >> 0) & 0x1F);
		LogMessage ("phase1:%u/64 cycle, ",val);  // DQS delayed on the write side

		WRITE_REG (PHY_LANE_SEL,(ilane*6));
		val = (UINT8) (READ_REG (PHY_DLL_TRIM_2) & 0x3f) ;
		LogMessageColor(COLOR_YELLOW,"trim_2:%u, ",val); //  adjust output dq/dqs/dm timing with respect to DRAM clock; This value is set by write-levelling 
	
		WRITE_REG (PHY_LANE_SEL,(ilane*8));
		val = (UINT8)((READ_REG (UNQ_ANALOG_DLL_2) >> 24) & 0xFF); // master delay line setting of this lane
		LogMessageColor(COLOR_YELLOW,"mas_dly:%u, ",val);
	
		LogMessage ("\n");
	}	
	//----------------------------------------------------------------------
	LogMessage ("> Data Input Delay: \n");
	for (ilane=0; ilane<NumOfLanes; ilane++)
	{
		LogMessage ("  >> Lane%lu: ", ilane );
		for (ibit = 0 ; ibit < 8 ; ibit++) 
		{
			WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
			val = (UINT8) (READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F) ;
			LogMessageColor(COLOR_YELLOW,"DQ%lu:0x%02X, ",ibit,val);
		}

		ibit = 8;
		WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
		val = (UINT8) (READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F) ;
		LogMessage ("         Base:0x%02X, ",val);

		WRITE_REG (PHY_LANE_SEL,(ilane*6));
		val = (UINT8) (READ_REG (PHY_DLL_TRIM_3) & 0x3F) ;
		if (((READ_REG (PHY_DLL_INCR_TRIM_3)>>ilane) & 0x01)==0x01)
			LogMessage ("trim_3:+%u, ",val);
		else
			LogMessage ("trim_3:-%u, ",val); // input dqs timing with respect to input dq

		WRITE_REG (PHY_LANE_SEL,(ilane*5));
		val = (UINT8) ((READ_REG (UNQ_ANALOG_DLL_3) >> 8) & 0x1F);
		LogMessage ("phase2:%u/64 cycle, ",val);

		WRITE_REG (PHY_LANE_SEL,(ilane*8));
		val = (UINT8) (READ_REG (SCL_DCAPCLK_DLY) & 0xFF) ;
		LogMessageColor(COLOR_YELLOW,"data_capture_clk:0x%02X, ",val); // data_capture_clk edge used to transfer data from the input DQS clock domain to the PHY core clock domain // Automatically programmed by SCL

		WRITE_REG (PHY_LANE_SEL,(ilane*3));
		val = (UINT8) (READ_REG (SCL_MAIN_CLK_DELTA) & 0x7); // SCL latency // Automatically programmed by SCL
		LogMessageColor(COLOR_YELLOW,"main_clk_delta_dly:0x%02X, ",val);

		WRITE_REG (PHY_LANE_SEL,(ilane*8));
		val = (UINT8) ((READ_REG(SCL_START)>>20)&0xFF); // number of delay elements required to delay the clock signal to align with the read DQS falling edge
		LogMessageColor(COLOR_YELLOW,"cycle_cnt:0x%02X, ",val);

		LogMessage ("\n");
	}
	//---------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // just for case
	TmpReg32 = READ_REG (UNIQUIFY_IO_1);
	LogMessage ("> UNIQUIFY_IO_1 (0x5C) = 0x%08lX: \n",TmpReg32);
	if ( (TmpReg32&(1<<0/*start_io_calib*/ | 1<<15/*override_cal_p*/ | 1<<23/*override_cal_n*/)) == (1<<0) )
		LogMessage ("  >> ZQ auto calibration is ON. Period: %u x 256 PHY clocks. \n", (READ_REG (PHY_DLL_RECALIB)>>8)&0x3ffff);
	else
		LogWarning ("  >> ZQ auto calibration is OFF or overrided. \n");
	
	WRITE_REG (PHY_LANE_SEL,0); // just for case
	TmpReg32 = READ_REG (UNIQUIFY_IO_2);
	LogMessage ("> UNIQUIFY_IO_2 (0x5D) = 0x%08lX: \n",TmpReg32);
	LogMessage ("  >> ZQ Values:  nfet_cal:0x%lX, pfet_cal:0x%lX \n",(TmpReg32>>24)&0xf, (TmpReg32>>16)&0xf);
	//---------------------------------------------------------------
	WRITE_REG (PHY_LANE_SEL,0); // just for case
	TmpReg32 = READ_REG (PHY_PAD_CTRL);
	LogMessage ("> PHY_PAD_CTRL (0x48) = 0x%08lX: \n",TmpReg32);
	LogMessage ("  >> DQ/DQS input dynamic termination (ODT): ");
	switch (TmpReg32&0x7)
	{
		case 0: LogMessage ("No \n"); break;
		case 1: LogMessage ("240R \n"); break;
		case 2: LogMessage ("120R \n"); break;
		case 3: LogMessage ("80R \n"); break;
		case 4: LogMessage ("60R \n"); break;
		case 5: LogMessage ("48R \n"); break;
		default: LogError ("Reserved \n"); 
	}
	LogMessage ("  >> DQ/DM/DQS output drive strength: ");
	if ( (TmpReg32&((UINT32)1<<4)) == 0)
		LogMessage ("34R. \n");
	else
		LogMessage ("48R. \n");
	LogMessage ("  >> Address and control output drive strength: ");
	if ( (TmpReg32&((UINT32)1<<16)) == 0)
		LogMessage ("34R. \n");
	else
		LogMessage ("48R. \n");
	LogMessage ("  >> Clock Output drive strength: ");
	if ( (TmpReg32&((UINT32)1<<20)) == 0)
		LogMessage ("34R. \n");
	else
		LogMessage ("48R. \n");
	LogMessage ("  >> preamble_dly = ");
	switch((TmpReg32>>29)&0x3)
	{
	case 0: LogMessage ("2 cycles. \n"); break;
	case 1: LogMessage ("1.5 cycles. \n"); break;
	case 2: LogMessage ("1 cycle. \n"); break;
	default:
		LogError ("Invalid. \n");
	}
	/*
	LogMessage ("  >> DLL type used to center the dqs signal on the dq bits on reads: ");
	if ( (TmpReg32&((UINT32)1<<9)) == 0)
		LogMessage ("Analog. \n");
	else
		LogMessage ("Digital. \n");	
	*/
	//-----------------------------------------------------------------
	LogMessage ("> PHY VREF: \n");
	for (ilane=0; ilane<NumOfLanes; ilane++)
	{
		UINT8 Step, Range;
		WRITE_REG (PHY_LANE_SEL,((UINT32)ilane*7));	
		UINT32 Reg_VREF_TRAINING = READ_REG (VREF_TRAINING);
		Step = (UINT8)((Reg_VREF_TRAINING >> 4) & 0x3F);
		Range = (UINT8)((Reg_VREF_TRAINING >> 10) & 0x1);
		LogMessage ("  >> Lane%lu: ",ilane);
		LogMessageColor(COLOR_YELLOW,"Range:%u; Step:0x%02X; (%u mV)\n",Range+1, Step, VrefDQ_Convert_Step_to_mV(Range, Step));
	}
	//---------------------------------------------------------------
	LogMessage("\n");
	LogHeader (" DRAM Device Information ");

	UINT16 MPR0_Readout [8*4];
	UINT16 *pMPR0_Readout = MPR0_Readout;
	//-------------
	LogMessage ("  >> Readout MPR Page 0: \n");
	MPR_Readout_RawData (0, MPR0_Readout);
	UINT8 x_row, y_row;
	for (y_row = 0; y_row<4; y_row++)
	{
		LogMessage("      %u: ",y_row);
		for (x_row = 0; x_row<8; x_row++)
		{
			LogMessage("0x%04X ",*pMPR0_Readout);
			pMPR0_Readout++;
		}
		LogMessage ("\n");
	}
	if (MemoryCompare(DDR_MPR2_Expected,MPR0_Readout,32)!=0)
	{
		LogError ((" Error: Values are not as expected. \n"));
		return (1);
	}
	//--------------
	UINT8 MPR2[4];
	status = MPR_Page2_Readout (MPR2);
	LogMessage ("  >> Readout MPR Page 2 => 0: 0x%02X, 1: 0x%02X, 2: 0x%02X, 3: 0x%02X. ",MPR2[0],MPR2[1],MPR2[2],MPR2[3]);
	if (status == (-1))
	{
		LogError (" (Error: values may be invalid) \n");
		return (1);
	}
	else
		//LogPass (" (values are valid) \n");
		LogPass ("\n");
	//--------------------------------
	LogMessage ("  >> VREF DQ: ");
	LogMessageColor(COLOR_YELLOW,"Range:%u; Step:0x%02X; (%u mV)\n",(MPR2[1]>>7) + 1, (MPR2[1]>>1)&0x3F, VrefDQ_Convert_Step_to_mV(MPR2[1]>>7,(MPR2[1]>>1)&0x3F));
	LogMessage ("  >> Output drive strength: ");
	switch (MPR2[3]&0x03)
	{
		case 0:  LogMessage ("34R. \n"); break;
		case 1:  LogMessage ("48R. \n"); break;
		default: LogError ("Reserved \n"); break;
	}
	LogMessage ("  >> RTT_NOM: ");
	switch (MPR2[3]>>5)
	{
		case 0: LogMessage ("Off \n"); break;
		case 1: LogMessage ("60R \n"); break;
		case 2: LogMessage ("120R \n"); break;
		case 3: LogMessage ("40R \n"); break;
		case 4: LogMessage ("240R \n"); break;
		case 5: LogMessage ("48R \n"); break;
		case 6: LogMessage ("80R \n"); break;
		case 7: LogMessage ("34R \n"); break;
		default: LogError ("Reserved \n"); 
	}
	LogMessage ("  >> RTT_PARK: ");
	switch ((MPR2[3]>>2) & 0x7)
	{
		case 0: LogMessage ("Off \n"); break;
		case 1: LogMessage ("60R \n"); break; // RZQ/4
		case 2: LogMessage ("120R \n"); break; // RZQ/2
		case 3: LogMessage ("40R \n"); break; // RZQ/6
		case 4: LogMessage ("240R \n"); break; // RZQ/1
		case 5: LogMessage ("48R \n"); break; // RZQ/5
		case 6: LogMessage ("80R \n"); break; // RZQ/3
		case 7: LogMessage ("34R \n"); break; // RZQ/7
		default: LogError ("Reserved \n"); 
	}
	LogMessage ("  >> RTT_WR: ");
	switch ((MPR2[0]>>0) & 0x3)
	{
		case 0: LogMessage ("Off \n"); break;
		case 1: LogMessage ("120R \n"); break; // RZQ/2
		case 2: LogMessage ("240R \n"); break; // RZQ/1
		default: LogError ("Reserved \n"); 
	}
	LogMessage ("  >> CAS Latency (CL): 0x%x (%u) \n",MPR2[2]>>4, CL[MPR2[2]>>4]);  // 09/03/2016: added more info
	LogMessage ("  >> CAS Write Latency (CWL): 0x%x (%u) \n", MPR2[2]&0x07, CWL[MPR2[2]&0x07]); // 09/03/2016: fixed a bug in the shift and added more info

	return (0);
}
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------
/*
void JumpToUboot (void)
{
	UINT32 UBOOT_ExecAddr; 
	JUMP_CODE pUboot;
	UINT16 status;
	LogMessage ("> Verify memory is stable before restart UBOOT ... ");
	status = MemStressTest ();
	if (status!=0)
	{
		LogError ("Failed, abort. \n");
		return;
	}
	LogPass ("Pass. \n");
	// look for UBOOT tag at SPI0 Flash (at first 1MB)
	LogMessage ("> Looking for UBOOT header .... ");
	UINT32 *pUbootHeader = (UINT32*)0x80000000;
	while (1)
	{
		if (*pUbootHeader == 0x4f4f4255)
		{
			if (*(pUbootHeader+1) == (0x4b4c4254))
				break;
		}
		pUbootHeader += (0x1000/4) ;

		if (pUbootHeader == (UINT32*)0x80100000)
		{
			LogError ("Failed, abort. \n");
			return;
		}
	}
	// Parsing the header
	UINT32 RelocateAddress = *(pUbootHeader + (0x140/4) );
	UINT32 SizeInByte = *(pUbootHeader + (0x144/4) );
	UINT32 Version = *(pUbootHeader + (0x148/4) );
	// Note: disregard FIU0 and FUI3 header settings.
	LogPass (" Found at address 0x%08lX; Version:0x%08lX, Relocate Address:0x%08lX, Size:0x%08lX. \n",(UINT32)pUbootHeader, Version, RelocateAddress, SizeInByte);
	LogWarning ("  Press any key to continue or ESC to abort ...  ");
	while (KbHit()==FALSE);
	if (GetChar()==ESC_KEY)
	{
		LogWarning (g_Text_AbortedByUser);
		return;
	}
	LogMessage ("\n");

	LogMessage ("> If MMU is enabled, return TLBs to DDR; Disable Data-Cache and MMU; Return vector table to address 0x0000_0000.\n");
	RelocateTlbs(1); // if MMU is enabled, return TLBs to DDR

	CLEAR_BIT_REG (FLOCKR1,18); // disable RAM2 first 256 bytes mapping (TBD: I assume boot-block always disable it so UBOOT expected this to be disabled) // 15/09/2015: add RAM-V return to normal

	UINT32 SCTLR_reg;
	CP15_READ (SCTLR_reg, 0, c1, c0, 0);
	SCTLR_reg &= ~ ( ((UINT32)1<<0) | ((UINT32)1<<2) ); // disable MMU (bit 0) and Data caching (bit 2)
	SCTLR_reg &= ~((UINT32)1<<13); // set vector table into address 0x0000_0000 (TBD: I assume boot-block always set vector table to 0x0000_0000 so UBOOT expected this)
	CP15_WRITE (SCTLR_reg, 0, c1, c0, 0);

	// check if relocated UBOOT code is needed
	if (RelocateAddress!=0)
	{	// relocated
		LogMessage ("> Relocate UBOOT .... ");

		MemCpy ((UINT32*)RelocateAddress, (UINT32*)pUbootHeader, SizeInByte+0x200);  // + 200 for the header 
		if (MemCmp ((UINT16*)RelocateAddress, (UINT16*)pUbootHeader, SizeInByte+0x200)!=-1) // + 200 for the header  
		{
			LogError ("Failed. abort. \n");
			return;
		}
		LogPass ("Done. \n");
		UBOOT_ExecAddr = RelocateAddress + 0x200;
	}
	else // exec UBOOT in place
		UBOOT_ExecAddr = (UINT32)(pUbootHeader+(0x200/4)); 

	InvalidateCache ();

	LogWarning ("> Jump to address 0x%08lX .... \n",UBOOT_ExecAddr);

	pUboot = (JUMP_CODE)UBOOT_ExecAddr;
	pUboot();
	while (1);
}
*/
//------------------------------------------------------
// check for abort key
int CheckForAbort (void)
{
	if (g_KeyPress==ESC_KEY)
		return (-1);
	else
	{
		if (KbHit()==TRUE)
		{
			g_KeyPress = GetChar();
			if (g_KeyPress==ESC_KEY)
			{
				LogWarning ("\n\n %s \n",g_Text_AbortedByUser);
				return (-1);
			}
		}
	}
	return (0);
}
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//DQ_Trim: -63...0...63 = 0x3F...0x01,(0x00)0x40,0x41...0x7F
int  DQ_to_INT (UINT8 DQ_Trim)
{
	if ((DQ_Trim==0x00) || (DQ_Trim==0x40))
		return (0);
	else if (DQ_Trim>0x40) 
		return ((int)(DQ_Trim&0x3F));
	else
		return ((int)(-1)*(int)(DQ_Trim&0x3F));		
}

UINT8 INT_to_DQ (int DQ_Trim)
{
	if (DQ_Trim==0)
		return (0x40);
	else if (DQ_Trim>0) 
		return (0x40 | (UINT8)(DQ_Trim));
	else
		return ((UINT8)(DQ_Trim*(-1)));		
}

UINT8 Inc_DQ_Trim (UINT8 DQ_Trim)
{
	if (DQ_Trim==0x01)
		DQ_Trim = 0x40;
	else if (DQ_Trim>=0x40)
		DQ_Trim = (DQ_Trim+1) & 0x7F;  // limit when reach to 0x7F 
	else
		DQ_Trim--;

	return (DQ_Trim&0x7F);
}
UINT8 Dec_DQ_Trim (UINT8 DQ_Trim)
{
	if (DQ_Trim==0x40)
		DQ_Trim = 0x01;
	else if (DQ_Trim>0x40)
		DQ_Trim--;
	else 
		DQ_Trim = (DQ_Trim+1) & 0x3F;  // limit when reach to 0x3F 
	
	return (DQ_Trim);
}
// This is delta sweep. This test sweep input DQn trim value by relative to present DQ trim settings. It scan up to +/- 63 steps from preset DQn value or until one of the data line reach max/min trim value.
BOOL Sweep_Trim_InputDQ_Offset (UINT32 DqMask[], UINT8 ReqMargin, char *Title)
{
	UINT32 ilane,ibit;
	int OriginTrimDQS[3], OriginTrimDQ[3][8];
	int TmpTrimDQ[3][8];
	int SweepDirection;
	UINT8 MarginP=0, MarginM=0;
	UINT16 LastResoults;
	UINT8 IsReachLimit;
	UINT8 DqTrim, DqsTrim;
	int DqTrim_int, DqsTrim_int; 
	int DelayLine_int;
	int Offset;

	g_KeyPress = 0;

	LogMessage ("\n");
	LogHeader (" Sweep Input DQ Delay Relative to DQS (%s)", Title);
	//LogMessage ("> Sweep Mask: Lane0:0x%02lX, Lane1:0x%02lX, Lane2:0x%02lX. \n", DqMask[0], DqMask[1], DqMask[2]);
	//--------------------------------------------------------------------------
	// Each input delay line include 64 steps.
	// DQS Range: 0...63
	// DQn Range: DQn(-63...+63) + DQS_IP (base delay, 0..63) 
	// DQn: -63 = 0x7F;  0=0x00=0x40; +64 = 0x3F
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Get default trim values
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		for (ibit=0; ibit<8; ibit++) 
		{
			WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
			DqTrim = (UINT8) (READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F);
			OriginTrimDQ [ilane][ibit] = DQ_to_INT (DqTrim);
			//LogMessage ("DQn[%lu][%lu]=0x%02x(%d).\n", ilane, ibit, DqTrim, OriginTrimDQ[ilane][ibit]);
		}
		ibit = 8;
		WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
		DqsTrim = (UINT8)(READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F);
		OriginTrimDQS[ilane] = (int)(DqsTrim);
		//LogMessage ("DQS[%lu]=0x%02x(%d).\n", ilane, DqsTrim, OriginTrimDQS[ilane]);
	}
	//--------------------------------------------------------------------------
	// Sweep .... 
	//--------------------------------------------------------------------------
	MemCpy (TmpTrimDQ, OriginTrimDQ, sizeof(TmpTrimDQ)); // copy origin values to tmp array  
	IsReachLimit = FALSE;
	SweepDirection = 1;  // starting with up. 
	Offset = 0;
	while (1)
	{
		//-----------------------------------------------
		// update trim values
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			for (ibit=0; ibit<8; ibit++) 
			{ 
				if ( (DqMask[ilane]) & ((UINT32)1<<ibit) ) // DqMask: 1-skip, 0-test
					continue;		
				DqTrim_int = TmpTrimDQ[ilane][ibit];
				DqTrim = INT_to_DQ (DqTrim_int);
				WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
				WRITE_REG (IP_DQ_DQS_BITWISE_TRIM, 0x80 | DqTrim);
				//LogMessage ("DQn[%lu][%lu]<=0x%02x(%d).\n", ilane, ibit, DqTrim, DqTrim_int);
			}
		}
		if (Offset==0)
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
		else
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Short); // dummy access to memory in order for values to update 
		//-----------------------------------------------
		// test the memory
		//-----------------------------------------------
		LastResoults = MemStressTest ();
		LogMessage ("> Offset %3d: 0x%04X \n",Offset, LastResoults); 

		//-----------------------------------------------
		// Check Resoult limits 
		//-----------------------------------------------
		if (LastResoults!=0x0000)
		{
			LogMessage ("---------- \n");
			IsReachLimit = TRUE;
		}

		//-----------------------------------------------
		// Calculate next step and check trim and delay-line limits
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			DqsTrim_int = OriginTrimDQS[ilane];
			for (ibit=0; ibit<8; ibit++) 
			{
				if (DqMask[ilane]&((UINT32)1<<ibit)) // DqMask: 1-skip, 0-test
					continue;

				DqTrim_int = TmpTrimDQ[ilane][ibit];
				DqTrim_int += SweepDirection;
				TmpTrimDQ[ilane][ibit] = DqTrim_int;
				if ( (IsReachLimit!=TRUE) && ((DqTrim_int>63) || (DqTrim_int<-63)) )
				{

					LogMessage ("--- trim limit --- (DqTrim_int:%d, ilane:%u, ibit:%u) \n", DqTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}

				DelayLine_int = DqTrim_int + DqsTrim_int;
				if ( (IsReachLimit!=TRUE) && ((DelayLine_int>63) || (DelayLine_int<0)) )
				{
					LogMessage ("--- delay-line limit --- (DelayLine:%d, DqTrim:%d, DqsTrim:%d, ilane:%u, ibit:%u) \n", DelayLine_int, DqTrim_int, DqsTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}
			}
		}
		//-----------------------------------------------
		if (IsReachLimit==TRUE)
		{
			if (SweepDirection>0)
			{ // change sweep direction to negative (0, -1...-63)
				MemCpy (TmpTrimDQ, OriginTrimDQ, sizeof(TmpTrimDQ)); // copy origin values to tmp  
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginP = (UINT8)(Offset-SweepDirection); 
				else
					MarginP = (UINT8)(Offset); 
				Offset = 0;
				SweepDirection *= -1;
				IsReachLimit = FALSE;
			}
			else
			{
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginM = (UINT8)(abs(Offset-SweepDirection)); 
				else
					MarginM = (UINT8)(abs(Offset)); 
				break; // done
			}
		}
		else
		{
			Offset += SweepDirection;
		}
		//-----------------------------------------------
		if (CheckForAbort()!=0)
			break;
	}
	//--------------------------------------------------------------------------
	// restore origin trim values 
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		for (ibit=0; ibit<8; ibit++) 
		{
			if (DqMask[ilane]&((UINT32)1<<ibit)) // DqMask: 1-skip, 0-test
				continue;
			DqTrim_int = OriginTrimDQ[ilane][ibit];
			DqTrim = INT_to_DQ (DqTrim_int);
			WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
			WRITE_REG (IP_DQ_DQS_BITWISE_TRIM, 0x80 | DqTrim);
		}
	}
	MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
	//--------------------------------------------------------------------------
	// Restouls
	LogMessage ("(%s) InputDQ Margin: Min required: +/-%u;  Measured: -%u/+%u. ", Title, ReqMargin, MarginM, MarginP);
	if ((MarginM >= ReqMargin) &&  (MarginP >= ReqMargin))
	{
		LogPass (" ==> PASS \n\n");
		return (0);
	}
	else
	{
		LogError (" ==> FAIL \n\n");
		return (1);
	}
}
///---------------------------------------------------------------------------------------------------------------------
BOOL Sweep_Trim_InputDQS_Offset (UINT32 DqMask[], UINT8 ReqMargin, char *Title)
{
	UINT32 ilane,ibit;
	int OriginTrimDQS[3], OriginTrimDQ[3][8];
	int TmpTrimDQS[3];
	int SweepDirection;
	UINT8 MarginP=0, MarginM=0;
	UINT16 LastResoults;
	UINT8 IsReachLimit;
	UINT8 DqTrim, DqsTrim;
	int DqTrim_int, DqsTrim_int; 
	int DelayLine_int;
	int Offset;

	g_KeyPress = 0;

	LogMessage ("\n");
	LogHeader (" Sweep Round-Trip Delay (%s) ", Title); // this register name is DQS in spec
	//LogMessage ("> Sweep Mask: Lane0:0x%02lX, Lane1:0x%02lX, Lane2:0x%02lX. \n", DqMask[0], DqMask[1], DqMask[2]);
	//--------------------------------------------------------------------------
	// Each input delay line include 64 steps.
	// DQS Range: 0...63
	// DQn Range: DQn(-63...+63) + DQS_IP (base delay, 0..63) 
	// DQn: -63 = 0x7F;  0=0x00=0x40; +64 = 0x3F
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Get default trim values
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		for (ibit=0; ibit<8; ibit++) 
		{
			WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
			DqTrim = (UINT8) (READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F);
			OriginTrimDQ [ilane][ibit] = DQ_to_INT (DqTrim);
			//LogMessage ("DQn[%lu][%lu]=0x%02x(%d).\n", ilane, ibit, DqTrim, OriginTrimDQ[ilane][ibit]);
		}
		ibit = 8;
		WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
		DqsTrim = (UINT8)(READ_REG (IP_DQ_DQS_BITWISE_TRIM) & 0x7F);
		OriginTrimDQS[ilane] = (int)(DqsTrim);
		//LogMessage ("DQS[%lu]=0x%02x(%d).\n", ilane, DqsTrim, OriginTrimDQS[ilane]);
	}
	//--------------------------------------------------------------------------
	// Sweep .... 
	//--------------------------------------------------------------------------
	MemCpy (TmpTrimDQS, OriginTrimDQS, sizeof(TmpTrimDQS)); // copy origin values to tmp array  
	IsReachLimit = FALSE;
	SweepDirection = 1;  // starting with up. 
	Offset = 0;
	while (1)
	{
		//-----------------------------------------------
		// update trim values
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			if (DqMask[ilane]) // DqMask: 1-skip, 0-test
				continue;		

			DqsTrim_int = TmpTrimDQS[ilane];
			DqsTrim = (UINT8)DqsTrim_int;
			ibit = 8;
			WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
			WRITE_REG (IP_DQ_DQS_BITWISE_TRIM, 0x80 | DqsTrim);
			//LogMessage ("DQS[%lu]<=0x%02x(%d).\n", ilane, DqsTrim, DqsTrim_int);	
		}
		if (Offset==0)
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
		else
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Short); // dummy access to memory in order for values to update 
		//-----------------------------------------------
		// Test the memory
		//-----------------------------------------------
		LastResoults = MemStressTest ();
		LogMessage ("> Offset %3d: 0x%04X \n",Offset, LastResoults); 

		//-----------------------------------------------
		// Check result limits 
		//-----------------------------------------------
		if (LastResoults!=0x0000)
		{
			LogMessage ("---------- \n");
			IsReachLimit = TRUE;
		}
		
		//-----------------------------------------------
		// Calculate next step and check trim and delay-line limits
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			DqsTrim_int = TmpTrimDQS[ilane];
			DqsTrim_int += SweepDirection;
			TmpTrimDQS[ilane] = DqsTrim_int;
			for (ibit=0; ibit<8; ibit++) 
			{
				if (DqMask[ilane]&((UINT32)1<<ibit)) // DqMask: 1-skip, 0-test
					continue;

				if ( (IsReachLimit!=TRUE) && ((DqsTrim_int>63) || (DqsTrim_int<0)) )
				{
					LogMessage ("--- trim limit --- (DqsTrim:%d, ilane:%u, ibit:%u) \n", DqsTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}

				DqTrim_int = OriginTrimDQ[ilane][ibit];
				DelayLine_int = DqTrim_int + DqsTrim_int;
				if ( (IsReachLimit!=TRUE) && ((DelayLine_int>63) || (DelayLine_int<0)) )
				{
					LogMessage ("--- delay-line limit --- (DelayLine:%d, DqTrim:%d, DqsTrim:%d, ilane:%u, ibit:%u) \n", DelayLine_int, DqTrim_int, DqsTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}
			}
		}
		//-----------------------------------------------
		if (IsReachLimit==TRUE)
		{
			if (SweepDirection>0)
			{ // change sweep direction to negative (0, -1...-63)
				MemCpy (TmpTrimDQS, OriginTrimDQS, sizeof(TmpTrimDQS)); // copy origin values to tmp array 
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginP = (UINT8)(Offset-SweepDirection); 
				else
					MarginP = (UINT8)(Offset); 
				Offset = 0;
				SweepDirection *= -1;
				IsReachLimit = FALSE;
			}
			else
			{
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginM = (UINT8)(abs(Offset-SweepDirection)); 
				else
					MarginM = (UINT8)(abs(Offset)); 
				break; // done
			}
		}
		else
		{
			Offset += SweepDirection;
		}
		//-----------------------------------------------
		if (CheckForAbort()!=0)
			break;
	}
	//--------------------------------------------------------------------------
	// restore origin trim values 
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		if (DqMask[ilane]) // DqMask: 1-skip, 0-test
			continue;
		DqsTrim_int = OriginTrimDQS[ilane];
		DqsTrim = (UINT8)DqsTrim_int;
		ibit = 8;
		WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
		WRITE_REG (IP_DQ_DQS_BITWISE_TRIM, 0x80 | DqsTrim);
	}
	MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
	//--------------------------------------------------------------------------
	// Restouls
	LogMessage ("(%s) Round-Trip Margin:  Min required: +/-%u;  Measured: -%u/+%u. ", Title, ReqMargin, MarginM, MarginP);
	if ((MarginM >= ReqMargin) &&  (MarginP >= ReqMargin))
	{
		LogPass (" ==> PASS \n\n");
		return (0);
	}
	else
	{
		LogError (" ==> FAIL \n\n");
		return (1);
	}
}
///---------------------------------------------------------------------------------------------------------------------
BOOL Sweep_Trim_OutputDQ_Offset (UINT32 DqMask[], UINT8 ReqMargin, char *Title)
{
	UINT32 ilane,ibit;
	int OriginTrimDQS[3], OriginTrimDQ[3][9];
	int TmpTrimDQ[3][9];
	int SweepDirection;
	UINT8 MarginP=0, MarginM=0;
	UINT16 LastResoults;
	UINT8 IsReachLimit;
	UINT8 DqTrim, DqsTrim;
	int DqTrim_int, DqsTrim_int; 
	int DelayLine_int;
	int Offset;

	g_KeyPress = 0;

	LogMessage ("\n");
	LogHeader (" Sweep Output DQ Delay Relative to DQS (%s)", Title);
	//LogMessage ("> Sweep Mask: Lane0:0x%02lX, Lane1:0x%02lX, Lane2:0x%02lX. \n", DqMask[0], DqMask[1], DqMask[2]);
	//--------------------------------------------------------------------------
	// Each input delay line include 64 steps.
	// DQS Range: 0...63
	// DQn Range: DQn(-63...+63) + DQS_OP (base delay, 0..63) 
	// DQn: -63 = 0x7F;  0=0x00=0x40; +64 = 0x3F
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Get default trim values
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		for (ibit=0; ibit<9; ibit++) 
		{
			WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
			DqTrim = (UINT8) (READ_REG (OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F);
			OriginTrimDQ [ilane][ibit] = DQ_to_INT (DqTrim);
			//LogMessage ("DQn[%lu][%lu]=0x%02x(%d).\n", ilane, ibit, DqTrim, OriginTrimDQ[ilane][ibit]);
		}
		ibit = 9;
		WRITE_REG (PHY_LANE_SEL, (ilane*7)+((UINT32)ibit<<8));
		DqsTrim = (UINT8)(READ_REG (OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F);
		OriginTrimDQS[ilane] = (int)(DqsTrim);
		//LogMessage ("DQS[%lu]=0x%02x(%d).\n", ilane, DqsTrim, OriginTrimDQS[ilane]);
	}
	//--------------------------------------------------------------------------
	// Sweep .... 
	//--------------------------------------------------------------------------
	MemCpy (TmpTrimDQ, OriginTrimDQ, sizeof(TmpTrimDQ)); // copy origin values to tmp array  
	IsReachLimit = FALSE;
	SweepDirection = 1;  // starting with up. 
	Offset = 0;
	while (1)
	{
		//-----------------------------------------------
		// update trim values
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			for (ibit=0; ibit<9; ibit++) 
			{ 
				if ( (DqMask[ilane]) & ((UINT32)1<<ibit) ) // DqMask: 1-skip, 0-test
					continue;		
				DqTrim_int = TmpTrimDQ[ilane][ibit];
				DqTrim = INT_to_DQ (DqTrim_int);
				WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
				WRITE_REG (OP_DQ_DM_DQS_BITWISE_TRIM, 0x80 | DqTrim);
				//LogMessage ("DQn[%lu][%lu]<=0x%02x(%d).\n", ilane, ibit, DqTrim, DqTrim_int);
			}
		}
		if (Offset==0)
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
		else
			MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Short); // dummy access to memory in order for values to update 
		//-----------------------------------------------
		// Test the memory
		//-----------------------------------------------
		LastResoults = MemStressTest ();
		LogMessage ("> Offset %3d: 0x%04X \n",Offset, LastResoults); 

		//-----------------------------------------------
		// Check resoult limits 
		//-----------------------------------------------
		if (LastResoults!=0x0000)
		{
			LogMessage ("---------- \n");
			IsReachLimit = TRUE;
		}
		
		//-----------------------------------------------
		// Calculate next step and check trim and delay-line limits
		//-----------------------------------------------
		for (ilane=0; ilane<3; ilane++)
		{
			DqsTrim_int = OriginTrimDQS[ilane];
			for (ibit=0; ibit<9; ibit++) 
			{
				if (DqMask[ilane]&((UINT32)1<<ibit)) // DqMask: 1-skip, 0-test
					continue;
				DqTrim_int = TmpTrimDQ[ilane][ibit];
				DqTrim_int += SweepDirection;
				TmpTrimDQ[ilane][ibit] = DqTrim_int;
				if ( (IsReachLimit!=TRUE) && ((DqTrim_int>63) || (DqTrim_int<-63)) )
				{

					LogMessage ("--- trim limit --- (DqTrim_int:%d, ilane:%u, ibit:%u) \n", DqTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}

				DelayLine_int = DqTrim_int + DqsTrim_int;
				if ( (IsReachLimit!=TRUE) && ((DelayLine_int>63) || (DelayLine_int<0)) )
				{
					LogMessage ("--- delay-line limit --- (DelayLine:%d, DqTrim:%d, DqsTrim:%d, ilane:%u, ibit:%u) \n", DelayLine_int, DqTrim_int, DqsTrim_int, ilane, ibit);
					IsReachLimit = TRUE;
				}
			}
		}
		//-----------------------------------------------
		if (IsReachLimit==TRUE)
		{
			if (SweepDirection>0)
			{ // change sweep direction to negative (0, -1...-63)
				MemCpy (TmpTrimDQ, OriginTrimDQ, sizeof(TmpTrimDQ)); // copy origin values to tmp  
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginP = (UINT8)(Offset-SweepDirection); 
				else
					MarginP = (UINT8)(Offset); 
				Offset = 0;
				SweepDirection *= -1;
				IsReachLimit = FALSE;
			}
			else
			{
				if ((LastResoults!=0x0000) && (Offset!=0))
					MarginM = (UINT8)(abs(Offset-SweepDirection)); 
				else
					MarginM = (UINT8)(abs(Offset)); 
				break; // done
			}
		}
		else
		{
			Offset += SweepDirection;
		}
		//-----------------------------------------------
		if (CheckForAbort()!=0)
			break;
	}
	//--------------------------------------------------------------------------
	// restore origin trim values 
	//--------------------------------------------------------------------------
	for (ilane=0; ilane<3; ilane++)
	{
		for (ibit=0; ibit<9; ibit++) 
		{
			if (DqMask[ilane]&((UINT32)1<<ibit)) // DqMask: 1-skip, 0-test
				continue;
			DqTrim_int = OriginTrimDQ[ilane][ibit];
			DqTrim = INT_to_DQ (DqTrim_int);
			WRITE_REG (PHY_LANE_SEL,(ilane*7)+((UINT32)ibit<<8));
			WRITE_REG (OP_DQ_DM_DQS_BITWISE_TRIM, 0x80 | DqTrim);
		}
	}
	MemCmp((void*)DDR_ADDR_BASE, (void*)DDR_ADDR_BASE+(1024*1024), Dummy_Delay_Long); // dummy access to memory in order for values to update 
	//--------------------------------------------------------------------------
	// Restouls
	LogMessage ("(%s) OutputDQ Margin: Min required: +/-%u;  Measured: -%u/+%u. ", Title, ReqMargin, MarginM, MarginP);
	if ((MarginM >= ReqMargin) &&  (MarginP >= ReqMargin))
	{
		LogPass (" ==> PASS \n\n");
		return (0);
	}
	else
	{
		LogError (" ==> FAIL \n\n");
		return (1);
	}
}


//---------------------------------------------------------------------------------------------------------------------
//		DDR - MPR registers
//---------------------------------------------------------------------------------------------------------------------
/*
 Note: 
 * MPR Page-0 registers can readout using serial, parallel, stagger readout mode while Page-1 to Page-3 can readout using serial mode only.
 * Data readout using parallel and stagger depend on BMC to DDR connection (i.e, data can be swap inside a byte lane) therefore data readout may not be the same on all boards type.
 * Readout is done by set configure DDR4 device via MC to the right mode and then read real memory addresses. In case of data-cache enable the read-out may not be represent the actual data (typically on the second read).
 * Write it same way as read but it's allow only to page-0, all others pages are RO. Look like only parallel mode is supported for write therefore if we don't know DDR to BMC lane data connection we can't (or it's not so simple) to write back the reset values of 0x55, 0x33, 0x0F, 0x00 to page-0.
 
 Example of EVB MPR Page-0 readout: 

	> Readout MPR_P0 in serail mode: (bit data is output on all 8 bit lane; here we have two lanes with the same readout data)
		0x00000000: 0x0000  0xFFFF  0x0000  0xFFFF  0x0000  0xFFFF  0x0000  0xFFFF  (MPR_P0_R0: 0x55; 0x55)
		0x00001000: 0x0000  0x0000  0xFFFF  0xFFFF  0x0000  0x0000  0xFFFF  0xFFFF  (MPR_P0_R1: 0x33; 0x33)
		0x00002000: 0x0000  0x0000  0x0000  0x0000  0xFFFF  0xFFFF  0xFFFF  0xFFFF  (MPR_P0_R2: 0x0F; 0x0F)
		0x00003000: 0x0000  0x0000  0x0000  0x0000  0x0000  0x0000  0x0000  0x0000  (MPR_P0_R3: 0x00; 0x00)

	> Readout MPR_P0 in parallel mode: (the value 0x55, 0x33, 0x0F, 0x00 on each lane but with different DDR to BMC data connection)
		0x00000000: 0xF0E4  0xF0E4  0xF0E4  0xF0E4  0xF0E4  0xF0E4  0xF0E4  0xF0E4
		0x00001000: 0x592E  0x592E  0x592E  0x592E  0x592E  0x592E  0x592E  0x592E
		0x00002000: 0x659C  0x659C  0x659C  0x659C  0x659C  0x659C  0x659C  0x659C
		0x00003000: 0x0000  0x0000  0x0000  0x0000  0x0000  0x0000  0x0000  0x0000

	> Readout MPR_P0 in stagger mode:
		0x00000000: 0x0000  0x0611  0xA0C0  0xA6D1  0x090A  0x0F1B  0xA9CA  0xAFDB
		0x00001000: 0x0000  0x5024  0x0611  0x5635  0xA0C0  0xF0E4  0xA6D1  0xF6F5
		0x00002000: 0x0000  0x090A  0x5024  0x592E  0x0611  0x0F1B  0x5635  0x5F3F
		0x00003000: 0x0000  0xA0C0  0x090A  0xA9CA  0x5024  0xF0E4  0x592E  0xF9EE
*/
//----------------------------------------------------------------------
// Readout: array of UINT16 * 8 * 4.
//----------------------------------------------------------------------
void MPR_Readout_RawData (UINT8 Page, UINT16 *RawData)
{
	UINT32 MC_MR3;
	UINT32 MPR_Page_Offset; 
	int status;

	MemSet (RawData,0x00,2*8*4);

	MC_MR3 = IOR32(MC_BASE_ADDR + (86*4)) & 0x1FFFF ; // ENALI_CTL_86.MR3_DATA_F0_0
	MC_MR3 = MC_MR3 & ~(UINT32)0x7 & ~((UINT32)3<<11); // remove MPR operation (bit 2), MPR selection (bits 1:0) and MPR Read Format (bits 12:11) 

	status = WriteModeReg (3, MC_MR3 | (UINT32)0<<11 /*Readout serial mode*/ | (UINT32)1<<2 /*Enable MPR*/ | (UINT32)Page<<0 /*Page number*/);	
	if (status!=0) // 05/10/2015
		return;
	Sleep (10); // some delay to the DDR device to enter MPR mode

	// if ECC is enable, disable it.
	UINT32 ECC_Mode = REG_READ_FIELD(MC_BASE_ADDR + ((UINT32)(ECC_EN_ADDR) * 4), ECC_EN_WIDTH, ECC_EN_OFFSET);
	if (ECC_Mode!=0)
		REG_SET_FIELD(MC_BASE_ADDR + ((UINT32)(ECC_EN_ADDR) * 4), ECC_EN_WIDTH, ECC_EN_OFFSET, 0); 

	// Note: There is indirect mothod to read from MPR using MC read_mpr bit and data is placed at MC mprr_data_x registers.
	// decided to use direct read mothod (memory read). To bypass cache issue (CPU read from cache and not from DDR) use DMA to read from memory. 
	for (MPR_Page_Offset=0;MPR_Page_Offset<4; MPR_Page_Offset++)
	{

		GDMA_CTL (0,0) = 0;
		GDMA_SRCB(0,0) = (UINT32)(MPR_Page_Offset<<12);  // BA[1:0] set register index inside a page of MPR. CPU adress to DDR address:  [ ....  ;BA[1:0] ; Column[9:0]; byte/word]
		GDMA_DSTB(0,0) = (UINT32)RawData; 
		GDMA_TCNT(0,0) = 8 /*8*UINT16*/;
		RawData += 8;

		GDMA_CTL (0,0) = 1<<16/*software req*/ | 1<<12/*16 bits transfer*/ | 1<<0/*Enable*/ | 0<<7/*inc/dec Source*/ | 0<<6/*inc/dec Destination*/; // Software-mode, incremented
		while ((GDMA_CTL (0,0) & (1<<18))==0);
	}

	// re-enable ECC if was enabled.
	REG_SET_FIELD(MC_BASE_ADDR + ((UINT32)(ECC_EN_ADDR) * 4), ECC_EN_WIDTH, ECC_EN_OFFSET, ECC_Mode);

	// Disable MPR
	WriteModeReg (3, MC_MR3);
}
//----------------------------------------------------------------------
// MPR should be array of 4.
//----------------------------------------------------------------------
int MPR_Page2_Readout (UINT8 *MPR)
{
	UINT16 MPR2_Readout [8*4];
	UINT16 *pMPR2_Readout = MPR2_Readout;
	
	MPR_Readout_RawData (2,MPR2_Readout); // read page-2 raw data (at serial mode)
	UINT32 MPR_Page_Offset; 

	for (MPR_Page_Offset=0; MPR_Page_Offset<4; MPR_Page_Offset++)
	{
		UINT8 tmp_data = 0;
		UINT32 bit_index=0x80; 
		while (bit_index!=0)
		{
			if (*pMPR2_Readout == 0xFFFF)
				tmp_data |= bit_index;
			else if (*pMPR2_Readout != 0x0000)
				return (-1); // in serial mode all data bus must be the same value. If not, there may be a signal integrity issue.

			bit_index = bit_index >> 1;
			pMPR2_Readout++;
		}
		*MPR = tmp_data;
		MPR++;
	}
	return (0);
}
//----------------------------------------------------------------------
void VrefDQ_Get_DRAM (UINT8 *Range, UINT8 *Step)
{
	// Get value from DDR
	UINT8 MPR2[4];
	if (MPR_Page2_Readout (MPR2)==(-1))
		MPR2[1] = 0;
	*Step = (MPR2[1]>>1) & 0x3F;
	*Range = MPR2[1]>>7;
}
//----------------------------------------------------------------------
int WriteModeReg (UINT8 Index, UINT32 Data)
{
	// DENALI_CTL_70:
	// 23=1 => bits 7:0 define the memory mode register number (data is placed at MRSINGLE_DATA_0, we have only one CS)
	// 17=1 => write to all mode registers (DDR4), bits 7:0 are ignored (data are placed at mr0_data_fN_0 to mr6_data_fN_0, we have only one CS)
	// 16=1 => write to all mode registers (DDR3), bits 7:0 are ignored (data are placed at mr0_data_fN_0 to mr3_data_fN_0, we have only one CS)
	// 24=0 => 15:8 CS number (valid only when bit 24 is clear)
	// 24=1 => write to all CSs, 15:8 are ignored.
	// 25: trigger

	//LogMessage ("\nWrite to MR%u data 0x%08lX\n", Index, Data);

	// clear all status bits
	IOW32(MC_BASE_ADDR + (117*4), -1); // DENALI_CTL_117
	//LogMessage ("DENALI_CTL_116 = 0x%08lX \n",IOR32(MC_BASE_ADDR + (116*4)));


/* debug

	// Note: Using the method of writing to a specific mode registers does not work (i.e., bit 23=1, bits 7:0 specify mode register number). Maybe I didn't understand bit 7:0 functionality
	// this method does work !!!
	// As an alternative, write to all mode registers with the assumption that all mode registers in MC are up-to-date.

	switch (Index)
	{
	case 0:
		IOW32(MC_BASE_ADDR + (MR0_DATA_F0_0_ADDR*4), Data<<8); // DENALI_CTL_79.MR0_DATA_F0_0
		break;

	case 1:  
		IOW32(MC_BASE_ADDR + (MR1_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_80.MR1_DATA_F0_0
		break;

	case 2:  
		IOW32(MC_BASE_ADDR + (MR2_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_81.MR2_DATA_F0_0
		break;

	case 3:  
		IOW32(MC_BASE_ADDR + (MR3_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_86.MR3_DATA_F0_0
		break;

	case 4:  
		IOW32(MC_BASE_ADDR + (MR4_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_88.MR4_DATA_F0_0
		break;

	case 5:  
		IOW32(MC_BASE_ADDR + (MR5_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_90.MR5_DATA_F0_0
		break;

	case 6:  
		IOW32(MC_BASE_ADDR + (MR6_DATA_F0_0_ADDR*4), Data); // DENALI_CTL_92.MR6_DATA_F0_0
		break;

	default:
		return (-1);
	}

	UINT32 WRITE_MODEREG = (UINT32)1<<25 | (UINT32)1<<17 ; // write to CS 0, to all DDR4 MR registers  
*/
	
	IOW32(MC_BASE_ADDR + (85*4), Data); // DENALI_CTL_85.MRSINGLE_DATA_0
	UINT32 WRITE_MODEREG = (UINT32)1<<25 | (UINT32)1<<23 | Index; // write to CS 0, to MR specify in bits 7:0 

	IOW32(MC_BASE_ADDR + (70*4), WRITE_MODEREG); // DENALI_CTL_70

	//LogMessage ("DENALI_CTL_116 = 0x%08lX \n",IOR32(MC_BASE_ADDR + (116*4)));

	UINT32 TimeOut=10000;
	while (1)
	{
		if ( ((IOR32(MC_BASE_ADDR + (116*4))>>25) & 0x01) == 0x01)  // DENALI_CTL_116.25
		{
			if  ( (IOR32(MC_BASE_ADDR + (71*4)) & 0xFF) == 0x01 ) // DENALI_CTL_71.MRW_STATUS
				return (-1);
			else
				return (0);
		}

		if (TimeOut==0)
		{
			LogError ("WriteModeReg::TimeOut\n");
			LogMessage ("DENALI_CTL_116 = 0x%08lX \n",IOR32(MC_BASE_ADDR + (116*4)));
			LogMessage ("DENALI_CTL_71 = 0x%08lX \n",IOR32(MC_BASE_ADDR + (71*4)));
			return (-1);
		}

		TimeOut--;
	}
}
//-------------------------------------------------------------------------------
UINT16 VrefDQ_Convert_Step_to_mV (UINT8 Range, UINT8 Step)
{
	UINT16 VrefDQ_Percent;
	UINT16 VrefDQ_mV;

	if (Range==0) // range 1
		VrefDQ_Percent = (UINT16)6000 + ((UINT16)Step * (UINT16)65);
	else // range 2
		VrefDQ_Percent = (UINT16)4500 + ((UINT16)Step * (UINT16)65);

	VrefDQ_mV = (UINT16)( (((UINT32)1200 * (UINT32)VrefDQ_Percent) + 5000) / (UINT32)10000); 
	return (VrefDQ_mV);
}
//-------------------------------------------------------------------------------
UINT8 VrefDQ_Convert_to_Step (UINT8 Range, UINT16 VrefDQ_mV)
{
	UINT8 Step;
	UINT16 VrefDQ_Percent;

	VrefDQ_Percent = (UINT16)( (((UINT32)VrefDQ_mV * (UINT32)10000) + 600)  / (UINT32)1200); 

	if (Range==0) // range 1
		if ((VrefDQ_Percent >= 6000)&& (VrefDQ_Percent <= 9250))
			Step = (UINT8)(((VrefDQ_Percent - (UINT16)6000) + 32) / (UINT16)65);
		else
			Step = (UINT8)(-1); // out-of-range
	else // range 2
		if ((VrefDQ_Percent >= 4500) && (VrefDQ_Percent <= 7750))
			Step = (UINT8)(((VrefDQ_Percent - (UINT16)4500) + 32) / (UINT16)65);
		else 
			Step = (UINT8)(-1); // out-of-range

	return (Step);
}
//-------------------------------------------------------------------------------
int VrefDQ_Convert_mV_to_Step_Range (UINT16 VrefDQ_mV, UINT8 *Range, UINT8 *Step)
{
	*Step = VrefDQ_Convert_to_Step(*Range, VrefDQ_mV);
	if (*Step == (UINT8)(-1))
	{
		// swap range and try again 
		if (*Range == 0) 
			*Range = 1;
		else
			*Range = 0;

		*Step = VrefDQ_Convert_to_Step(*Range, VrefDQ_mV);	
		if (*Step == (UINT8)(-1))
		{
			LOG_ERROR (("Failed to set DRAM VrefDQ. Out-Of-Range on both ranges. VrefDQ_mV=%u",VrefDQ_mV));
			while (1);
			return (-1);
		}
	}
	return (0);
}
//-------------------------------------------------------------------------------
void VrefDQ_Set_DRAM (UINT8 Range, UINT8 Step)
{
	// We need to find DDR.MR6 origin value (note that in this register only the tCCD_L filed is needed (CAS_n to CAS_n command	delay for same bank group), typically it should be 5 for 800MHz).
	// Since DDR.MR6 is write only we can try read MC register but can never be sure if this was the last value written.
	UINT32 MR6_origin = IOR32(MC_BASE_ADDR + (MR6_DATA_F0_0_ADDR*4)) & 0x1FFFF ; // DENALI_CTL_92
	UINT32 MR6_mask = MR6_origin & ~(UINT32)0x7F; // remove VrefDQ Training Range (bit 6) and VrefDQ Training Value (bits 5:0)
	WriteModeReg (6, MR6_mask | (UINT32)Step |  (UINT32)Range<<6 | (UINT32)1<<7 /*place in VrefDQ training mode*/ );
	Sleep (10);
	WriteModeReg (6, MR6_mask | (UINT32)Step |  (UINT32)Range<<6 );
};
//-------------------------------------------------------------------------------
void VrefDQ_Get_PHY (UINT8 ilane, UINT8 *Range, UINT8 *Step)
{
	WRITE_REG (PHY_LANE_SEL,((UINT32)ilane*7));	
	UINT32 Reg_VREF_TRAINING = READ_REG (VREF_TRAINING);
	*Step = (UINT8)((Reg_VREF_TRAINING >> 4) & 0x3F);
	*Range = (UINT8)((Reg_VREF_TRAINING >> 10) & 0x1);
}
//-------------------------------------------------------------------------------
void VrefDQ_Set_PHY (UINT8 ilane, UINT8 Range, UINT8 Step)
{
	/*
		vref_value is 7 bit (bits 4 to 10 of VREF_TRAINING register) that build from 6 bits step (bits 4 to 9) and 1 bit range (bit 10). 
		
		PHY issue:
		range (bit 10 of VREF_TRAINING register) is not connected and always write 0. this bit should select range 1 and 2. therefore range-1 is alway selected. 
		read is OK. 
		
		The bypass is via two write cycles:
		1. Normal 7 bit write (4 to 10) but only vref_value bits 0 to 5 will be update while bit 6 keep 0.
		2. Shifted 7 bit write (4 to 10) this means vref_value bits 1 to 6 will be update while bit 0 of the next lane will keep 0.  
		3. repeat 1-2 for lane 1 and 2.  

		internally shift register look like this: <6,5,4,3,2,1,0><6,5,4,3,2,1,0><6,5,4,3,2,1,0>; 7 bit for each lane. 

	*/

	UINT32 vref_value = (UINT32)Range<<6 | (UINT32)Step;

	// update vref_value; this step use to write vref_value.0
	WRITE_REG (PHY_LANE_SEL,((UINT32)ilane*7));	
	WRITE_REG (VREF_TRAINING, vref_value<<4 | 0x04/*vref_output_enable*/);  

	// update vref_value ; this step use to write vref_value.6..1 
	vref_value = vref_value >> 1;
	WRITE_REG (PHY_LANE_SEL,((UINT32)ilane*7)+1/*shift by 1*/);	
	WRITE_REG (VREF_TRAINING, vref_value<<4 | 0x04/*vref_output_enable*/);   

}
//-------------------------------------------------------------------------------
void VrefDQ_PHY_Test (void)
{ 
	UINT8 ilane;
	UINT8 Range, Step;
	UINT16 VrefDQ_mV;
	ilane = 0;
	while (1)
	{
		for (Range=0; Range<2; Range++)
			for (Step=0; Step<0x33; Step++)
				for (ilane=0; ilane<3; ilane++)
				{
					VrefDQ_Set_PHY (ilane, Range, Step);
					Sleep (10);
					// get
					VrefDQ_Get_PHY (ilane, &Range, &Step);
					VrefDQ_mV = VrefDQ_Convert_Step_to_mV(Range, Step);
					LogHeader (" PHY VrefDQ: lane: %u, %u mv (range: %u, step %u)", ilane, VrefDQ_mV, Range+1, Step);
			}
	}
};

