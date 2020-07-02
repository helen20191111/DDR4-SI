
#ifndef __DDR4_FHY_REG_h__
#define __DDR4_FHY_REG_h__

#include "Poleg.h"

#define SCL_START					HW_DWORD((MCPHY_BASE_ADDR+(0x40<<2))) 
#define SCL_DATA_0					HW_DWORD((MCPHY_BASE_ADDR+(0x41<<2))) 
#define SCL_DATA_1					HW_DWORD((MCPHY_BASE_ADDR+(0x42<<2))) 
#define SCL_LATENCY					HW_DWORD((MCPHY_BASE_ADDR+(0x43<<2))) 
#define SCL_CONFIG_1				HW_DWORD((MCPHY_BASE_ADDR+(0x46<<2))) 
#define SCL_CONFIG_2				HW_DWORD((MCPHY_BASE_ADDR+(0x47<<2))) 
#define PHY_PAD_CTRL				HW_DWORD((MCPHY_BASE_ADDR+(0x48<<2))) 
#define PHY_DLL_RECALIB				HW_DWORD((MCPHY_BASE_ADDR+(0x49<<2))) 
#define PHY_DLL_ADRCTRL				HW_DWORD((MCPHY_BASE_ADDR+(0x4A<<2)))
#define PHY_LANE_SEL				HW_DWORD((MCPHY_BASE_ADDR+(0x4B<<2))) // Trim delay and Inc/Dec setting for each input DQ(0...7); The MSB is the Incr (1)/Decr (0) control and the other bits are the trim
#define PHY_DLL_TRIM_1				HW_DWORD((MCPHY_BASE_ADDR+(0x4C<<2))) 
#define PHY_DLL_TRIM_2				HW_DWORD((MCPHY_BASE_ADDR+(0x4D<<2))) 
#define PHY_DLL_TRIM_3				HW_DWORD((MCPHY_BASE_ADDR+(0x4E<<2)))
#define SCL_DCAPCLK_DLY				HW_DWORD((MCPHY_BASE_ADDR+(0x4F<<2))) 
#define SCL_MAIN_CLK_DELTA			HW_DWORD((MCPHY_BASE_ADDR+(0x50<<2))) 
#define WRLVL_CTRL					HW_DWORD((MCPHY_BASE_ADDR+(0x52<<2)))
#define WRLVL_AUTOINC_TRIM			HW_DWORD((MCPHY_BASE_ADDR+(0x53<<2)))
#define WRLVL_DYN_ODT				HW_DWORD((MCPHY_BASE_ADDR+(0x54<<2)))
#define WRLVL_ON_OFF				HW_DWORD((MCPHY_BASE_ADDR+(0x55<<2)))
#define UNQ_ANALOG_DLL_1			HW_DWORD((MCPHY_BASE_ADDR+(0x57<<2)))
#define UNQ_ANALOG_DLL_2			HW_DWORD((MCPHY_BASE_ADDR+(0x58<<2))) 
#define PHY_DLL_INCR_TRIM_1			HW_DWORD((MCPHY_BASE_ADDR+(0x59<<2))) 
#define PHY_DLL_INCR_TRIM_3			HW_DWORD((MCPHY_BASE_ADDR+(0x5A<<2))) 
#define SCL_CONFIG_3				HW_DWORD((MCPHY_BASE_ADDR+(0x5B<<2)))
#define UNIQUIFY_IO_1				HW_DWORD((MCPHY_BASE_ADDR+(0x5C<<2)))
#define UNIQUIFY_IO_2				HW_DWORD((MCPHY_BASE_ADDR+(0x5D<<2)))
#define PHY_SCL_START_ADDR			HW_DWORD((MCPHY_BASE_ADDR+(0x62<<2))) 
#define PHY_DLL_RISE_FALL			HW_DWORD((MCPHY_BASE_ADDR+(0x63<<2))) 
#define UNQ_ANALOG_DLL_3			HW_DWORD((MCPHY_BASE_ADDR+(0x64<<2))) 
#define IP_DQ_DQS_BITWISE_TRIM		HW_DWORD((MCPHY_BASE_ADDR+(0x65<<2))) // Trim delay and Inc/Dec setting for each input DQ(0...7)/DQS (actually it's base delay); The MSB is the Incr (1)/Decr (0) control and the other bits are the trim
#define DSCL_CNT					HW_DWORD((MCPHY_BASE_ADDR+(0x67<<2))) 
#define OP_DQ_DM_DQS_BITWISE_TRIM	HW_DWORD((MCPHY_BASE_ADDR+(0x68<<2))) // Trim delay and Inc/Dec setting for each output DQ(0...7)/DM/DQS (actually it's base delay); The MSB is the Incr (1)/Decr (0) and the other bits are the trim
#define PHY_DLL_TRIM_CLK			HW_DWORD((MCPHY_BASE_ADDR+(0x69<<2)))
#define DYNAMIC_BIT_LVL				HW_DWORD((MCPHY_BASE_ADDR+(0x6B<<2)))
#define PHY_REV_CNTRL_REG			HW_DWORD((MCPHY_BASE_ADDR+(0x6C<<2))) 
#define SCL_WINDOW_TRIM				HW_DWORD((MCPHY_BASE_ADDR+(0x6D<<2))) 
#define DISABLE_GATING_FOR_SCL      HW_DWORD((MCPHY_BASE_ADDR+(0x6E<<2)))
#define SCL_CONFIG_4				HW_DWORD((MCPHY_BASE_ADDR+(0x6F<<2)))
#define DYNAMIC_WRITE_BIT_LVL		HW_DWORD((MCPHY_BASE_ADDR+(0x70<<2))) 
#define DDR4_CONFIG_1               HW_DWORD((MCPHY_BASE_ADDR+(0x71<<2)))
#define VREF_TRAINING				HW_DWORD((MCPHY_BASE_ADDR+(0x72<<2))) 
#define VREF_TRAINING_WINDOWS		HW_DWORD((MCPHY_BASE_ADDR+(0x75<<2)))


// PHY output driver for DQ/DQS/DM signals	
#define PHY_DRV_DQ_34R	(UINT32)0<<4
#define PHY_DRV_DQ_48R	(UINT32)1<<4

// PHY output driver for Address/Control signals
#define PHY_DRV_CTL_34R	(UINT32)0<<16
#define PHY_DRV_CTL_48R	(UINT32)1<<16

// PHY output driver for clock signals
#define PHY_DRV_CLK_34R	(UINT32)0<<20
#define PHY_DRV_CLK_48R	(UINT32)1<<20

// PHY input ODT for DQ/DQS signals
#define PHY_ODT_DQ_48R	(UINT32)0x05
#define PHY_ODT_DQ_60R	(UINT32)0x04
#define PHY_ODT_DQ_80R	(UINT32)0x03 
#define PHY_ODT_DQ_120R	(UINT32)0x02
#define PHY_ODT_DQ_240R (UINT32)0x01
#define PHY_ODT_DQ_NO	(UINT32)0x00


#endif	
