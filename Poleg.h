/*----- Global definitions -----*/

#ifndef __Poleg_h__
#define __Poleg_h__


// Version: 0.1   (10/12/2013 by DoronZ - First version)
// Version: 0.2	  (11/12/2013 by OhadR - Added VCD registers)
// Version: 0.3	  (22/12/2013 by DoronZ - Added GPIO registers)
// Version: 0.4	  (25/12/2013 by DoronZ - Updated GCR module registers)
// Version: 0.5	  (31/12/2013 by DoronZ - Added interrupts list)
// Version: 0.6   (19/01/2014 by OhadR - Updated VCD Module [new BSD registers])
// Version: 0.7   (19/01/2014 by OhadR - Added GFXI registers)
// Version: 0.8	  (29/01/2014 by DoronZ - Added registers for module SIOX1/2)
// Version: 0.9	  (05/02/2014 by OhadR - Added ECE registers, Updated AIC base address)
// Version: 1.0	  (19/02/2014 by DoronZ - Added core UARTs 0-3 registers)
// Version: 1.1   (27/02/2014 by OhadR - Added co-processor section: TWD, ICU registers)
// Version: 1.2	  (12/03/2014 By DoronZ - Added PWM, MFT and PSPI registers)
// Version: 1.3	  (18/03/2014 By DoronZ - Added Timer)
// Version: 1.4	  (18/03/2014 By Hila - Added reference to Poleg_SVB.h)
// Version: 1.5	  (30/04/2014 By Amir - Added CP Peripheral Access Windows)
// Version: 1.6	  (30/04/2014 By DoronZ - Added PECI module registers)
// Version: 1.7   (04/05/2014 By OhadR - Added more CP control registers)
// Version: 1.8   (04/05/2014 By DoronZ - Added BIOS POST CODE FIFO and SWC registers)
// Version: 1.9   (07/05/2014 By DoronZ - Added HSIOX and HGPIO registers)
// Version: 2.0	  (20/05/2014 By Amir - Added CP System Access Windows)
// Version: 2.1	  (21/05/2014 By DoronZ - Added another register to the SWC module)
// Version: 2.2   (26/05/2014 By Amir - Added RNG registers )
// Version: 2.3   (27/05/2014 By Amir - Added ESPI slave interface module core registers )
// Version: 2.4   (27/05/2014 By OhadR - Added fuse array registers )
// Version: 2.5   (01/06/2014 By Amir - Added VDM-RX registers )
// Version: 2.6   (02/06/2014 By DoronZ - Fixed location of register XEVCFG(m,n) of SIOX module )
// Version: 2.7   (09/06/2014 By DoronZ - Changed all the registers with (N) variable - to work with new poleg template)
// Version: 2.8	  (11/06/2014 By OhadR - Added MC PHY and MC base addresses
// Version: 2.9	  (15/06/2014 By DoronZ - Corrected interrupts numbers (+32 to everything)
// Version: 3.0	  (23/06/2014 By DoronZ - Added SHA1-2 registers
// Version: 3.1   (23/06/2014 By Amir	- Added AES registers 
// Version: 3.2   (25/06/2014 By Amir	- Added DES registers 
// Version: 3.3   (10/07/2014 By DoronZ - Fixed the clock controle module registers and added some new ones
// Version: 3.4	  (14/10/2014 By Ohad - Added (CLKDIV3) to clock controller module
// Version: 3.5	  (16/10/2014 By Amir - Updated FIU_TEST register name to FIU_CFG
// Version: 3.6	  (17/10/2014 By DoronZ - Added ADC module registers
// version: 3.7	  (14/10/2014 By Hila	- Changed GPIO register names
// Version: 3.8	  (20/7/2014  By Amir - modified MACRO definition of FIU registers 
// Version: 3.9	  (06/08/2014 By DoronZ - Fixed register name in clocks module and added a few macros
// Version: 4.0	  (11/08/2014 By DoronZ - Added a few more PKA registers
// Version: 4.1	  (12/08/2014 By DoronZ - Added SD/MMC and VDMA modules registers
// Version: 4.2	  (01/09/2014 By DoronZ - Added macro for CLKEN
// Version: 4.3	  (01/09/2014 By OhadR  - Added Chip ID for Z2 and A1)
// Version: 4.4	  (07/09/2014 By OhadR  - Added macros for fues arrays)
// Version: 4.5   (18/09/2014 By OhadR  - added OTP interrupt to interrupts)
// Version: 4.6	  (24/12/2014 By Amir 	- added SPI_INT_X to interrupt vector
// Version: 4.7	  (15/01/2015 By DoronZ	- added MDIO addr/data registers to the GMAC module
// Version: 4.8	  (20/01/2015 By OhadR	- added macros for MFSEL registers
// Version: 4.9   (20/01/2015 By OhadR	- added macros for SMBus fair init values registers)
// Version: 5.0   (28/01/2015 By OhadR	- added PCIe integration registers (in GCR)
// Version: 5.1   (08/02/2015 By OhadR	- Fixed macro in CLK registers (CLKEN, IPSRST)
// Version: 5.2   (09/02/2015 By OhadR	- Added PCI Registers of GFX
// Version: 5.3   (22/02/2015 By DoronZ	- Added all eSPI registers
// Version: 5.4   (03/03/2015 By OhadR  - Added CPBPNTR register - same as OBSCR2 ) 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//					CORE SIDE REGISTERS 															//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define INT_START_NUM		32
/*--------------- Interrupts -------------*/
#define ADC_INT				(0 + INT_START_NUM)	// ADC module
#define COPR_INT 			(1 + INT_START_NUM)			// CoProcessor subsystem
#define UART_INT_0			(2 + INT_START_NUM)			// UART 0 module
#define UART_INT_1			(3 + INT_START_NUM)			// UART 1 module
#define UART_INT_2			(4 + INT_START_NUM)			// UART 2 module
#define UART_INT_3			(5 + INT_START_NUM)			// UART 3 module
#define PECI_INT			(6 + INT_START_NUM)		// PECI module
#define VSYNC_INT			(7 + INT_START_NUM)			// Graphics module via system manager module
#define PCIMBX_INT			(8 + INT_START_NUM)			// PCI mailbox module
#define KCS_HIB_INT			(9 + INT_START_NUM)			// KCS/HIB (from host interface) module
#define LPC_MBX_INT			(10 + INT_START_NUM)			// LPC or eSPI mailbox
#define SHM_INT 			(11 + INT_START_NUM)			// SHM module
#define BT_INT				(13 + INT_START_NUM)			// Block transfer
#define GMAC1_INT			(14 + INT_START_NUM)			// GMAC1 module
#define EMC1Rx_INT			(15 + INT_START_NUM)			// EMC1 Rx module
#define EMC1Tx_INT			(16 + INT_START_NUM)			// EMC1 Tx module
#define GMAC2_INT			(17 + INT_START_NUM)			// GMAC2 module
#define eSPI_INT			(18 + INT_START_NUM)			// eSPI module
#define SIOX_INT_1			(19 + INT_START_NUM)			// SIOX serial GPIO expander module 1
#define SIOX_INT_2			(20 + INT_START_NUM)			// SIOX serial GPIO expander module 2
#define VCD_INT 			(22 + INT_START_NUM)			// VCD module
#define DVC_INT				(23 + INT_START_NUM)			// DVC module
#define ECE_INT				(24 + INT_START_NUM)			// ECE module
#define MC_INT				(25 + INT_START_NUM)			// Memory controller module
#define MMC_INT				(26 + INT_START_NUM)			// MMC module (SDHC2)
#define SDRDR_INT			(27 + INT_START_NUM)			// SDHC1 - SD
#define PSPI2_INT 			(28 + INT_START_NUM)			// Slow peripheral SPI 2
#define VDMA_INT			(29 + INT_START_NUM)			// VDMA module
#define MCTP_INT			(30 + INT_START_NUM)			// VDMX module
#define PSPI1_INT			(31 + INT_START_NUM)			// Slow peripheral SPI 1
#define TIMER_INT_0			(32 + INT_START_NUM)			// Timer module 0 Timer 0
#define TIMER_INT_1			(33 + INT_START_NUM)			// Timer module 0 Timer 1
#define TIMER_INT_2			(34 + INT_START_NUM)			// Timer module 0 Timer 2
#define TIMER_INT_3			(35 + INT_START_NUM)			// Timer module 0 Timer 3
#define TIMER_INT_4			(36 + INT_START_NUM)			// Timer module 0 Timer 4
#define TIMER_INT_5			(37 + INT_START_NUM)			// Timer module 1 Timer 0
#define TIMER_INT_6			(38 + INT_START_NUM)			// Timer module 1 Timer 1
#define TIMER_INT_7			(39 + INT_START_NUM)			// Timer module 1 Timer 2
#define TIMER_INT_8			(40 + INT_START_NUM)			// Timer module 1 Timer 3
#define TIMER_INT_9			(41 + INT_START_NUM)			// Timer module 1 Timer 4
#define TIMER_INT_10		(42 + INT_START_NUM)			// Timer module 2 Timer 0
#define TIMER_INT_11		(43 + INT_START_NUM)			// Timer module 2 Timer 1
#define TIMER_INT_12		(44 + INT_START_NUM)			// Timer module 2 Timer 2
#define TIMER_INT_13		(45 + INT_START_NUM)			// Timer module 2 Timer 3
#define TIMER_INT_14		(46 + INT_START_NUM)			// Timer module 2 Timer 4
#define WDF_INT_0			(47 + INT_START_NUM)			// Timer module 0 watchdog
#define WDF_INT_1			(48 + INT_START_NUM)			// Timer module 1 watchdog
#define WDF_INT_2			(49 + INT_START_NUM)			// Timer module 2 watchdog
#define USB_DEV_INT_0		(51 + INT_START_NUM)			// USB device 0
#define USB_DEV_INT_1		(52 + INT_START_NUM)	 		// USB device 1
#define USB_DEV_INT_2		(53 + INT_START_NUM)			// USB device 2
#define USB_DEV_INT_3		(54 + INT_START_NUM)			// USB device 3
#define USB_DEV_INT_4		(55 + INT_START_NUM)			// USB device 4
#define USB_DEV_INT_5		(56 + INT_START_NUM)			// USB device 5
#define USB_DEV_INT_6		(57 + INT_START_NUM)			// USB device 6
#define USB_DEV_INT_7		(58 + INT_START_NUM)			// USB device 7
#define USB_DEV_INT_8		(59 + INT_START_NUM)			// USB device 8
#define USB_DEV_INT_9		(60 + INT_START_NUM)			// USB device 9
#define USB_HST_INT_0		(61 + INT_START_NUM)			// USB host 0
#define SMB_INT_0			(64 + INT_START_NUM)			// SMBus and I2C module 0
#define SMB_INT_1			(65 + INT_START_NUM)			// SMBus and I2C module 1
#define SMB_INT_2			(66 + INT_START_NUM)			// SMBus and I2C module 2
#define SMB_INT_3			(67 + INT_START_NUM)			// SMBus and I2C module 3
#define SMB_INT_4			(68 + INT_START_NUM)			// SMBus and I2C module 4
#define SMB_INT_5			(69 + INT_START_NUM)			// SMBus and I2C module 5
#define SMB_INT_6			(70 + INT_START_NUM)			// SMBus and I2C module 6
#define SMB_INT_7			(71 + INT_START_NUM)			// SMBus and I2C module 7
#define SMB_INT_8			(72 + INT_START_NUM)			// SMBus and I2C module 8
#define SMB_INT_9			(73 + INT_START_NUM)			// SMBus and I2C module 9
#define SMB_INT_10			(74 + INT_START_NUM)			// SMBus and I2C module 10
#define SMB_INT_11			(75 + INT_START_NUM)			// SMBus and I2C module 11
#define AES_INT				(80 + INT_START_NUM)			// AES module
#define DES_INT				(81 + INT_START_NUM)			// 3DES module
#define SHA_INT				(82 + INT_START_NUM)			// SHA module
#define SECACC_INT			(83 + INT_START_NUM)			// ECC and RSA accelerator module
#define SPI_INT_0			(85 + INT_START_NUM)			// FIU module 0
#define SPI_INT_X			(86 + INT_START_NUM)			// FIU module X
#define SPI_INT_3			(87 + INT_START_NUM)			// FIU module 3
#define GDMA_INT_0			(88 + INT_START_NUM)			// GDMA module 0
#define GDMA_INT_1			(89 + INT_START_NUM)			// GDMA module 1
#define GDMA_INT_2			(90 + INT_START_NUM)			// GDMA module 2
#define GDMA_INT_3			(91 + INT_START_NUM)			// GDMA module 3
#define OTP_INT_1			(92 + INT_START_NUM)			// KeyArray interrupt
#define PWM_INT_0			(93 + INT_START_NUM)			// PWM module 0 outputting PWM0-3
#define PWM_INT_1			(94 + INT_START_NUM)			// PWM module 1 outputting PWM4-7
#define OTP_INT_2			(95 + INT_START_NUM)			// FuseArray interrupt
#define MFT_INT_0			(96 + INT_START_NUM)			// MFT module 0
#define MFT_INT_1			(97 + INT_START_NUM)			// MFT module 1
#define MFT_INT_2			(98 + INT_START_NUM)			// MFT module 2
#define MFT_INT_3			(99 + INT_START_NUM)			// MFT module 3
#define MFT_INT_4			(100 + INT_START_NUM)			// MFT module 4
#define MFT_INT_5			(101 + INT_START_NUM)			// MFT module 5
#define MFT_INT_6			(102 + INT_START_NUM)			// MFT module 6
#define MFT_INT_7			(103 + INT_START_NUM)			// MFT module 7
#define EMC2Rx_INT			(114 + INT_START_NUM)			// EMC2 Rx module
#define EMC2Tx_INT			(115 + INT_START_NUM)			// EMC2 Tx module
#define GPIO_INT_0			(116 + INT_START_NUM)			// GPIO module outputting GPIO0-31
#define GPIO_INT_1			(117 + INT_START_NUM)			// GPIO module outputting GPIO32-63
#define GPIO_INT_2			(118 + INT_START_NUM)			// GPIO module outputting GPIO64-95
#define GPIO_INT_3			(119 + INT_START_NUM)			// GPIO module outputting GPIO96-127
#define GPIO_INT_4			(120 + INT_START_NUM)			// GPIO module outputting GPIO128-159
#define GPIO_INT_5			(121 + INT_START_NUM)			// GPIO module outputting GPIO160-191
#define GPIO_INT_6			(122 + INT_START_NUM)			// GPIO module outputting GPIO192-223
#define GPIO_INT_7			(123 + INT_START_NUM)			// GPIO module outputting GPIO224-255
#define PCIe_RC_INT			(127 + INT_START_NUM)			// PCIe root complex (combined A-D and MSI)

/*----------------------------------------*/

#define POLEG_ID			0x00A92750
#define Poleg_ID			0x00A92750
#define PolegIDTest			0x00926650
#define Poleg_ID_Z1			0x00A92750
#define Poleg_ID_Z2			0x04A92750 // Z2 and A1 (data sheet 0.70)
//#define Poleg_ID_A1			0x10A92750


/*----- System Global Control -----*/
#define GCR_BA				0xF0800000
#define PDID				HW_DWORD((GCR_BA+0x000))		// Product Identifier
#define PWRON				HW_DWORD((GCR_BA+0x004))		// Power-On Setting
#define ARBCON				HW_DWORD((GCR_BA+0x008))		// Arbitration Control
#define MFSEL1				HW_DWORD((GCR_BA+0x00C))		// Multiple Function Pin Select 1
#define MFSEL2				HW_DWORD((GCR_BA+0x010))		// Multiple Function Pin Select 2
#define MISCPE				HW_DWORD((GCR_BA+0x014))		// Miscellaneous Pin Pull-up/Down Enable
#define GPIOP0PE			HW_DWORD((GCR_BA+0x018))		// GPIO Port 0 Pin Pull-Up/Down Enable
#define GPIOP1PE			HW_DWORD((GCR_BA+0x01C))		// GPIO Port 1 Pin Pull-Up/Down Enable
#define SPSWC				HW_DWORD((GCR_BA+0x038))		// Serial Port Switch Control
#define INTCR				HW_DWORD((GCR_BA+0x03C))		// Integration Control
#define INTSR				HW_DWORD((GCR_BA+0x040))		// Integration Status
#define OBSCR1				HW_DWORD((GCR_BA+0x044))		// Observability Control
#define OBSDR1				HW_DWORD((GCR_BA+0x048))		// Observability Data
#define HIFCR				HW_DWORD((GCR_BA+0x050))		// Host Interface Control
#define SD1IRV1				HW_DWORD((GCR_BA+0x054))		// SDHC interface Reset Value 1
#define SD1IRV2				HW_DWORD((GCR_BA+0x058))		// SDHC interface Reset Value 2
#define SD1IRV3				HW_DWORD((GCR_BA+0x05C))		// SDHC interface Reset Value 3
#define INTCR2				HW_DWORD((GCR_BA+0x060))		// Integration control Register 2
#define MFSEL3				HW_DWORD((GCR_BA+0x064))		// Multiple function Pin Select 3
#define SRCNT				HW_DWORD((GCR_BA+0x068))		// Slew Rate Control
#define RESSR				HW_DWORD((GCR_BA+0x06C))		// Reset Status
#define RLOCKR1 			HW_DWORD((GCR_BA+0x070))		// Register Lock 1
#define FLOCKR1 			HW_DWORD((GCR_BA+0x074))		// Function Lock 1
#define DSCNT				HW_DWORD((GCR_BA+0x078))		// Drive Strength Control
#define MDLR				HW_DWORD((GCR_BA+0x07C))		// Module Disable Lock
#define DACLVLR 			HW_DWORD((GCR_BA+0x098))		// DAC Level Control
#define INTCR3				HW_DWORD((GCR_BA+0x09C))		// Integration Control 3
#define EMMCCTCNT			HW_DWORD((GCR_BA+0x0A0))		// EMMC BOOT Control
#define EMMCCTST			HW_DWORD((GCR_BA+0x0A4))		// EMMC BOOT Status
#define EMMCBTT				HW_DWORD((GCR_BA+0x0A8))		// EMMC BOOT Time
#define VSINTR				HW_DWORD((GCR_BA+0x0AC))		// VSYNC Interrupt Control
#define MFSEL4				HW_DWORD((GCR_BA+0x0B0))		// Multiple function Pin Select 4
#define SD2IRV1				HW_DWORD((GCR_BA+0x0B4))		// SD2 Interface Reset Value 1
#define SD2IRV2				HW_DWORD((GCR_BA+0x0B8))		// SD2 Interface Reset Value 2
#define SD2IRV3				HW_DWORD((GCR_BA+0x0BC))		// SD2 Interface Reset Value 3
#define OBSCR2				HW_DWORD((GCR_BA+0x0C4))		// Observability Control 2
#define CPBPNTR				HW_DWORD((GCR_BA+0x0C4))		// Coprocessor Boot Pointer Register
#define OBSDR2				HW_DWORD((GCR_BA+0x0C8))		// Observability Data 2
#define CPCTL				HW_DWORD((GCR_BA+0x0D0))		// Coprocessor control
#define CP2BST				HW_DWORD((GCR_BA+0x0D4))		// Coprocessor to BMC Core Status
#define B2CPNT				HW_DWORD((GCR_BA+0x0D8))		// BMC Core to Coprocessor Notification
#define CPPCTL				HW_DWORD((GCR_BA+0x0DC))		// Coprocessor Protection Control
#define I2CSEGSEL			HW_DWORD((GCR_BA+0x0E0))		// I2C Segment Pin Select
#define I2CSEGCTL			HW_DWORD((GCR_BA+0x0E4))		// I2C Segment Control
#define VSRCR				HW_DWORD((GCR_BA+0x0E8))		// Voltage Supply Report Register 
#define MBISTCTL			HW_DWORD((GCR_BA+0x100))		// Engineering MBIST Control
#define MBIST1R 			HW_DWORD((GCR_BA+0x104))		// Engineering MBIST 1
#define MBIST2R 			HW_DWORD((GCR_BA+0x108))		// Engineering MBIST 2
#define MBIST3R 			HW_DWORD((GCR_BA+0x10C))		// Engineering MBIST 3
#define ETSR				HW_DWORD((GCR_BA+0x110))		// Engineering Test Strap
#define DFT1R				HW_DWORD((GCR_BA+0x114))		// Engineering DFT 1
#define DFT2R				HW_DWORD((GCR_BA+0x118))		// Engineering DFT 2
#define EDFFSR				HW_DWORD((GCR_BA+0x120))		// Engineering Debug Flip-Flop Scan
#define INTCRPCE2			HW_DWORD((GCR_BA+0x12C))		// Engineering Integration Control for PCIe 2
#define INTCRPCE0			HW_DWORD((GCR_BA+0x130))		// Engineering Integration Control for PCIe 0
#define INTCRPCE1			HW_DWORD((GCR_BA+0x134))		// Engineering Integration Control for PCIe 1
#define DACTEST				HW_DWORD((GCR_BA+0x138))		// Engineering DAC Test
#define SCRPAD				HW_DWORD((GCR_BA+0x13C))		// Scratchpad Register
#define USBTEST				HW_DWORD((GCR_BA+0x140))		// Engineering USB Test Control
#define SD1IRV4				HW_DWORD((GCR_BA+0x200))		// SDHC interface Reset Value 4
#define SD1IRV5				HW_DWORD((GCR_BA+0x204))		// SDHC interface Reset Value 5
#define SD1IRV6				HW_DWORD((GCR_BA+0x208))		// SDHC interface Reset Value 6
#define SD1IRV7				HW_DWORD((GCR_BA+0x20C))		// SDHC interface Reset Value 7
#define SD1IRV8				HW_DWORD((GCR_BA+0x210))		// SDHC interface Reset Value 8
#define SD1IRV9				HW_DWORD((GCR_BA+0x214))		// SDHC interface Reset Value 9
#define SD1IRV10			HW_DWORD((GCR_BA+0x218))		// SDHC interface Reset Value 10
#define SD1IRV11			HW_DWORD((GCR_BA+0x21C))		// SDHC interface Reset Value 11
#define SD2IRV4				HW_DWORD((GCR_BA+0x220))		// SDHCn Interface Reset Value 4
#define SD2IRV5				HW_DWORD((GCR_BA+0x224))		// SDHCn Interface Reset Value 5
#define SD2IRV6				HW_DWORD((GCR_BA+0x228))		// SDHCn Interface Reset Value 6
#define SD2IRV7				HW_DWORD((GCR_BA+0x22C))		// SDHCn Interface Reset Value 7
#define SD2IRV8				HW_DWORD((GCR_BA+0x230))		// SDHCn Interface Reset Value 8
#define SD2IRV9				HW_DWORD((GCR_BA+0x234))		// SDHCn Interface Reset Value 9
#define SD2IRV10			HW_DWORD((GCR_BA+0x238))		// SDHCn Interface Reset Value 10
#define SD2IRV11			HW_DWORD((GCR_BA+0x23C))		// SDHCn Interface Reset Value 11

#define MFSEL(n)			HW_DWORD(GCR_BA + (n == 1 ? 0x00C : (n == 2 ? 0x010 : (n == 3 ? 0x064 : (n == 4 ? 0x0B0 : 0x00C)))))

/*----- Clock Controller -----*/
#define CLK_BA				0xF0801000	
#define CLKEN1				HW_DWORD(CLK_BA+0x00)			// Clock Enable 1
#define CLKSEL				HW_DWORD(CLK_BA+0x04)			// Clock select
#define CLKDIV1				HW_DWORD(CLK_BA+0x08)			// Clock Divider Control Register 1
#define PLLCON0 			HW_DWORD(CLK_BA+0x0C)			// PLL control 0
#define PLLCON1 			HW_DWORD(CLK_BA+0x10)			// PLL control 1
#define SWRSTR				HW_DWORD(CLK_BA+0x14)			// Power management control
#define IRQWAKECON			HW_DWORD(CLK_BA+0x18)			// IRQ Wake-up control
#define IRQWAKEFLAG			HW_DWORD(CLK_BA+0x1C)			// IRQ Wake-up flag
#define IPSRST1 			HW_DWORD(CLK_BA+0x20)			// IP software reset flag 1
#define IPSRST2 			HW_DWORD(CLK_BA+0x24)			// IP software reset flag 2
#define CLKEN2				HW_DWORD(CLK_BA+0x28)			// Clock Enable 2
#define CLKDIV2				HW_DWORD(CLK_BA+0x2C)			// Clock Divider Control 2
#define CLKEN3				HW_DWORD(CLK_BA+0x30)			// Clock Enable 3
#define IPSRST3 			HW_DWORD(CLK_BA+0x34)			// IP software reset flag 3
#define WD0RCR	 			HW_DWORD(CLK_BA+0x38)			// Watchdog 0 Reset Control
#define WD1RCR	 			HW_DWORD(CLK_BA+0x3C)			// Watchdog 1 Reset Control
#define WD2RCR	 			HW_DWORD(CLK_BA+0x40)			// Watchdog 2 Reset Control
#define SWRSTC1	 			HW_DWORD(CLK_BA+0x44)			// Software Reset Control 1
#define SWRSTC2	 			HW_DWORD(CLK_BA+0x48)			// Software Reset Control 2
#define SWRSTC3	 			HW_DWORD(CLK_BA+0x4C)			// Software Reset Control 3
#define SWRSTC4	 			HW_DWORD(CLK_BA+0x50)			// Software Reset Control 4
#define PLLCON2				HW_DWORD(CLK_BA+0x54)			// PLL control 2
#define CLKDIV3				HW_DWORD(CLK_BA+0x58)			// Clock divider 3
#define CORSTC	 			HW_DWORD(CLK_BA+0x5C)			// CORST Control
#define AHBCLKI				HW_DWORD(CLK_BA+0x64)			// AHB Clock Frequency Information
#define SECCNT				HW_DWORD(CLK_BA+0x68)			// Seconds Counter
#define CNTR25M				HW_DWORD(CLK_BA+0x6C)			// Mili Seconds Counter
#define WDnRCR(n)	 		HW_DWORD(CLK_BA + 0x38 + (##n*4))
#define SWRSTCn(n)	 		HW_DWORD(CLK_BA + 0x44 + (##n*4))
#define IPSRSTn(n)	 		HW_DWORD(CLK_BA + (n == 1 ? 0x20 : (n == 2 ? 0x24 : (n == 3 ? 0x                                                 34 : 0x20))))
#define CLKENn(n)			HW_DWORD(CLK_BA + (n == 1 ? 0x00 : (n == 2 ? 0x28 : (n == 3 ? 0x30 : 0x30))))

/*------------- Memory Controller ----------------*/
#define MCPHY_BASE_ADDR           	0xF05F0000
#define MC_BASE_ADDR 				0xF0824000

/*----- Ethernet MAC Controller (EMC) -----*/
#define EMC1_BA 			0xF0825000
#define EMC2_BA				0xF0826000
#define	EMC_M_BASE(n)		((n)*0x1000)
#define CAMCMR(n)			HW_DWORD((EMC1_BA+0x0000) + EMC_M_BASE(n-1))			// CAM Command
#define CAMEN(n)			HW_DWORD((EMC1_BA+0x0004) + EMC_M_BASE(n-1))			// CAM Enable
#define CAM0M(n)			HW_DWORD((EMC1_BA+0x0008) + EMC_M_BASE(n-1))			// CAM0 Most Significant Word
#define CAM0L(n)			HW_DWORD((EMC1_BA+0x000C) + EMC_M_BASE(n-1))			// CAM0 Least Significant Word
#define CAM1M(n)			HW_DWORD((EMC1_BA+0x0010) + EMC_M_BASE(n-1))			// CAM1 Most Significant Word
#define CAM1L(n)			HW_DWORD((EMC1_BA+0x0014) + EMC_M_BASE(n-1))			// CAM1 Least Significant Word
#define CAM2M(n)			HW_DWORD((EMC1_BA+0x0018) + EMC_M_BASE(n-1))			// CAM2 Most Significant Word
#define CAM2L(n)			HW_DWORD((EMC1_BA+0x001C) + EMC_M_BASE(n-1))			// CAM2 Least Significant Word
#define CAM3M(n)			HW_DWORD((EMC1_BA+0x0020) + EMC_M_BASE(n-1))			// CAM3 Most Significant Word
#define CAM3L(n)			HW_DWORD((EMC1_BA+0x0024) + EMC_M_BASE(n-1))			// CAM3 Least Significant Word
#define CAM4M(n)			HW_DWORD((EMC1_BA+0x0028) + EMC_M_BASE(n-1))			// CAM4 Most Significant Word
#define CAM4L(n)			HW_DWORD((EMC1_BA+0x002C) + EMC_M_BASE(n-1))			// CAM4 Least Significant Word
#define CAM5M(n)			HW_DWORD((EMC1_BA+0x0030) + EMC_M_BASE(n-1))			// CAM5 Most Significant Word
#define CAM5L(n)			HW_DWORD((EMC1_BA+0x0034) + EMC_M_BASE(n-1))			// CAM5 Least Significant Word
#define CAM6M(n)			HW_DWORD((EMC1_BA+0x0038) + EMC_M_BASE(n-1))			// CAM6 Most Significant Word
#define CAM6L(n)			HW_DWORD((EMC1_BA+0x003C) + EMC_M_BASE(n-1))			// CAM6 Least Significant Word
#define CAM7M(n)			HW_DWORD((EMC1_BA+0x0040) + EMC_M_BASE(n-1))			// CAM7 Most Significant Word
#define CAM7L(n)			HW_DWORD((EMC1_BA+0x0044) + EMC_M_BASE(n-1)) 			// CAM7 Least Significant Word
#define CAM8M(n)			HW_DWORD((EMC1_BA+0x0048) + EMC_M_BASE(n-1))			// CAM8 Most Significant Word
#define CAM8L(n)			HW_DWORD((EMC1_BA+0x004C) + EMC_M_BASE(n-1))			// CAM8 Least Significant Word
#define CAM9M(n)			HW_DWORD((EMC1_BA+0x0050) + EMC_M_BASE(n-1))			// CAM9 Most Significant Word
#define CAM9L(n)			HW_DWORD((EMC1_BA+0x0054) + EMC_M_BASE(n-1))			// CAM9 Least Significant Word
#define CAM10M(n)			HW_DWORD((EMC1_BA+0x0058) + EMC_M_BASE(n-1))			// CAM10 Most Significant Word
#define CAM10L(n)			HW_DWORD((EMC1_BA+0x005C) + EMC_M_BASE(n-1))			// CAM10 Least Significant Word
#define CAM11M(n)			HW_DWORD((EMC1_BA+0x0060) + EMC_M_BASE(n-1))			// CAM11 Most Significant Word
#define CAM11L(n)			HW_DWORD((EMC1_BA+0x0064) + EMC_M_BASE(n-1))			// CAM11 Least Significant Word
#define CAM12M(n)			HW_DWORD((EMC1_BA+0x0068) + EMC_M_BASE(n-1))			// CAM12 Most Significant Word
#define CAM12L(n)			HW_DWORD((EMC1_BA+0x006C) + EMC_M_BASE(n-1))			// CAM12 Least Significant Word
#define CAM13M(n)			HW_DWORD((EMC1_BA+0x0070) + EMC_M_BASE(n-1))			// CAM13 Most Significant Word
#define CAM13L(n)			HW_DWORD((EMC1_BA+0x0074) + EMC_M_BASE(n-1))			// CAM13 Least Significant Word
#define CAM14M(n)			HW_DWORD((EMC1_BA+0x0078) + EMC_M_BASE(n-1))			// CAM14 Most Significant Word
#define CAM14L(n)			HW_DWORD((EMC1_BA+0x007C) + EMC_M_BASE(n-1))			// CAM14 Least Significant Word
#define CAM15M(n)			HW_DWORD((EMC1_BA+0x0080) + EMC_M_BASE(n-1))			// CAM15 Most Significant Word
#define CAM15L(n)			HW_DWORD((EMC1_BA+0x0084) + EMC_M_BASE(n-1))			// CAM15 Least Significant Word
#define TXDLSA(n)			HW_DWORD((EMC1_BA+0x0088) + EMC_M_BASE(n-1))			// Transmit Desc. Link List Start
#define RXDLSA(n)			HW_DWORD((EMC1_BA+0x008C) + EMC_M_BASE(n-1))			// Receive Desc. Link List Start
#define MCMDR(n)			HW_DWORD((EMC1_BA+0x0090) + EMC_M_BASE(n-1))			// MAC Command
#define MIID(n)				HW_DWORD((EMC1_BA+0x0094) + EMC_M_BASE(n-1))			// MII MAnagement Data
#define MIIDA(n)			HW_DWORD((EMC1_BA+0x0098) + EMC_M_BASE(n-1))			// MII Management Control & Address
#define FFTCR(n)			HW_DWORD((EMC1_BA+0x009C) + EMC_M_BASE(n-1))			// FIFO Threshold Control
#define TSDR(n) 			HW_DWORD((EMC1_BA+0x00A0) + EMC_M_BASE(n-1))			// Transmit Start Demand
#define RSDR(n) 			HW_DWORD((EMC1_BA+0x00A4) + EMC_M_BASE(n-1))			// Receive Start Deman
#define DMARFC(n)			HW_DWORD((EMC1_BA+0x00A8) + EMC_M_BASE(n-1))			// Maximum Receive Frame Control
#define MIEN(n) 			HW_DWORD((EMC1_BA+0x00AC) + EMC_M_BASE(n-1))			// MAC Interrupt Enable
#define MISTA(n)			HW_DWORD((EMC1_BA+0x00B0) + EMC_M_BASE(n-1))			// MAC Interrupt Status
#define MGSTA(n)			HW_DWORD((EMC1_BA+0x00B4) + EMC_M_BASE(n-1))			// MAC General Status
#define MPCNT(n)			HW_DWORD((EMC1_BA+0x00B8) + EMC_M_BASE(n-1))			// Missed Packet Count
#define MRPC(n) 			HW_DWORD((EMC1_BA+0x00BC) + EMC_M_BASE(n-1))			// MAC Receive Pause Count
#define MRPCC(n)			HW_DWORD((EMC1_BA+0x00C0) + EMC_M_BASE(n-1))			// MAC Receive Pause Current Count
#define MREPC(n)			HW_DWORD((EMC1_BA+0x00C4) + EMC_M_BASE(n-1))			// MAC Remote Pause Count
#define DMARFS(n)			HW_DWORD((EMC1_BA+0x00C8) + EMC_M_BASE(n-1))			// DMA Receive Frame Status
#define CTXDSA(n)			HW_DWORD((EMC1_BA+0x00CC) + EMC_M_BASE(n-1))			// Current Transmit Desc. Start Address
#define CTXBSA(n)			HW_DWORD((EMC1_BA+0x00D0) + EMC_M_BASE(n-1))			// Current Transmit Buffer Start Address
#define CRXDSA(n)			HW_DWORD((EMC1_BA+0x00D4) + EMC_M_BASE(n-1))			// Current Receive Desc. Start Address
#define CRXBSA(n)			HW_DWORD((EMC1_BA+0x00D8) + EMC_M_BASE(n-1))			// Current Receive Buffer Start Address
#define RXFSM(n)			HW_DWORD((EMC1_BA+0x0200) + EMC_M_BASE(n-1))			// Receive Finite State Machine
#define TXFSM(n)			HW_DWORD((EMC1_BA+0x0204) + EMC_M_BASE(n-1))			// Transmit Finite State Machine
#define FSM0(n) 			HW_DWORD((EMC1_BA+0x0208) + EMC_M_BASE(n-1))			// Finite State Machine 0
#define FSM1(n) 			HW_DWORD((EMC1_BA+0x020C) + EMC_M_BASE(n-1))			// Finite State Machine 1
#define DCR(n)				HW_DWORD((EMC1_BA+0x0210) + EMC_M_BASE(n-1))			// Debug Configuration
#define DMMIR(n)			HW_DWORD((EMC1_BA+0x0214) + EMC_M_BASE(n-1))			// Debug Mode MAC Information
#define BISTR(n)			HW_DWORD((EMC1_BA+0x0300) + EMC_M_BASE(n-1))			// BIST Mode

#define CAMM(m,n)			HW_DWORD(CAM0M(##n) + (##m*8))							// CAMx Most Significant Word
#define CAML(m,n)			HW_DWORD(CAM0L(##n) + (##m*8))							// CALx Least Significant Word


/*-----Gigabyte MAC Controller (GMAC) -----*/
#define GMAC1_BA 			0xF0802000
#define GMAC2_BA			0xF0804000
#define	GMAC_M_BASE(n)		((n)*0x2000)
#define GMAC_CFG(n)			HW_DWORD((GMAC1_BA+0x0000) + GMAC_M_BASE(n-1))		// Operation Mode Configuration
#define GMAC_FRM_FLT(n)		HW_DWORD((GMAC1_BA+0x0004) + GMAC_M_BASE(n-1))		// Frame filter controls
#define GMAC_MDIO_ADDR(n)   HW_DWORD((GMAC1_BA+0x0010) + GMAC_M_BASE(n-1))		// MDIO Address
#define GMAC_MDIO_DATA(n)   HW_DWORD((GMAC1_BA+0x0014) + GMAC_M_BASE(n-1))		// MDIO Data
#define GMAC_RGMII_STS(n)	HW_DWORD((GMAC1_BA+0x00D8) + GMAC_M_BASE(n-1))		// Frame filter controls
#define REGISTER0(n)		HW_DWORD((GMAC1_BA+0x1000) + GMAC_M_BASE(n-1))		// Bus mode
#define REGISTER1(n)		HW_DWORD((GMAC1_BA+0x1004) + GMAC_M_BASE(n-1))		// Transmit Poll Demand
#define REGISTER2(n)		HW_DWORD((GMAC1_BA+0x1008) + GMAC_M_BASE(n-1))		// Receive Poll Demand
#define REGISTER3(n)		HW_DWORD((GMAC1_BA+0x100C) + GMAC_M_BASE(n-1))		// Receive Descriptor List Address
#define REGISTER4(n)		HW_DWORD((GMAC1_BA+0x1010) + GMAC_M_BASE(n-1))		// Transmit Descriptor List Address
#define REGISTER5(n)		HW_DWORD((GMAC1_BA+0x1014) + GMAC_M_BASE(n-1))		// Status
#define REGISTER6(n)		HW_DWORD((GMAC1_BA+0x1018) + GMAC_M_BASE(n-1))		// Operation Mode
#define REGISTER7(n)		HW_DWORD((GMAC1_BA+0x101C) + GMAC_M_BASE(n-1))		// Interrupt Enable
#define REGISTER8(n)		HW_DWORD((GMAC1_BA+0x1020) + GMAC_M_BASE(n-1))		// Missed Frame And Buffer Overflow Counter
#define REGISTER18(n)		HW_DWORD((GMAC1_BA+0x1048) + GMAC_M_BASE(n-1))		// Current Memory Transmit Descriptor
#define REGISTER19(n)		HW_DWORD((GMAC1_BA+0x104C) + GMAC_M_BASE(n-1))		// Current Memory Receive Descriptor
#define REGISTER20(n)		HW_DWORD((GMAC1_BA+0x1050) + GMAC_M_BASE(n-1))		// Current Memory Transmit Buffer Address
#define REGISTER21(n)		HW_DWORD((GMAC1_BA+0x1054) + GMAC_M_BASE(n-1))		// Current Memory Receive Buffer Address


/*----- Universal Asynchronous Receiver Transmitter (UART) -----*/
#define UART0_BA			0xF0001000
#define UART1_BA			0xF0002000
#define UART2_BA			0xF0003000
#define UART3_BA			0xF0004000
#define	UART_M_BASE(n)		((n)*0x1000)
#define UART_RBR(n) 		HW_BYTE((UART0_BA+0x00) + UART_M_BASE(n))		// Receive Buffer
#define UART_RXD(n) 		HW_BYTE((UART0_BA+0x00) + UART_M_BASE(n))		// Receive Buffer (same as above)
#define UART_THR(n) 		HW_BYTE((UART0_BA+0x00) + UART_M_BASE(n))		// Transmit Holding
#define UART_TXD(n) 		HW_BYTE((UART0_BA+0x00) + UART_M_BASE(n))		// Transmit Buffer (same as above)
#define UART_IER(n) 		HW_BYTE((UART0_BA+0x04) + UART_M_BASE(n))		// Interrupt Enable
#define UART_DLL(n) 		HW_BYTE((UART0_BA+0x00) + UART_M_BASE(n))		// Divisor Latch (Low Byte)
#define UART_DLM(n) 		HW_BYTE((UART0_BA+0x04) + UART_M_BASE(n))		// Divisor Latch (High Byte)
#define UART_IIR(n) 		HW_BYTE((UART0_BA+0x08) + UART_M_BASE(n))		// Interrupt Identification
#define UART_FCR(n) 		HW_BYTE((UART0_BA+0x08) + UART_M_BASE(n))		// FIFO Control
#define UART_LCR(n) 		HW_BYTE((UART0_BA+0x0C) + UART_M_BASE(n))		// Line Control
#define UART_MCR(n) 		HW_BYTE((UART0_BA+0x10) + UART_M_BASE(n))		// Modem Control - except UART0
#define UART_LSR(n) 		HW_BYTE((UART0_BA+0x14) + UART_M_BASE(n))		// Line Status
#define UART_MSR(n) 		HW_BYTE((UART0_BA+0x18) + UART_M_BASE(n))		// Modem Status - except UART0
#define UART_TOR(n) 		HW_BYTE((UART0_BA+0x1C) + UART_M_BASE(n))		// Time-Out

#define UART_RBR_ADDR(n) 		((UART0_BA+0x00) + UART_M_BASE(n))		// Receive Buffer
#define UART_RXD_ADDR(n) 		((UART0_BA+0x00) + UART_M_BASE(n))		// Receive Buffer (same as above)
#define UART_THR_ADDR(n) 		((UART0_BA+0x00) + UART_M_BASE(n))		// Transmit Holding
#define UART_TXD_ADDR(n) 		((UART0_BA+0x00) + UART_M_BASE(n))		// Transmit Buffer (same as above)
#define UART_IER_ADDR(n) 		((UART0_BA+0x04) + UART_M_BASE(n))		// Interrupt Enable
#define UART_DLL_ADDR(n) 		((UART0_BA+0x00) + UART_M_BASE(n))		// Divisor Latch (Low Byte)
#define UART_DLM_ADDR(n) 		((UART0_BA+0x04) + UART_M_BASE(n))		// Divisor Latch (High Byte)
#define UART_IIR_ADDR(n) 		((UART0_BA+0x08) + UART_M_BASE(n))		// Interrupt Identification
#define UART_FCR_ADDR(n) 		((UART0_BA+0x08) + UART_M_BASE(n))		// FIFO Control
#define UART_LCR_ADDR(n) 		((UART0_BA+0x0C) + UART_M_BASE(n))		// Line Control
#define UART_MCR_ADDR(n) 		((UART0_BA+0x10) + UART_M_BASE(n))		// Modem Control - except UART0
#define UART_LSR_ADDR(n) 		((UART0_BA+0x14) + UART_M_BASE(n))		// Line Status
#define UART_MSR_ADDR(n) 		((UART0_BA+0x18) + UART_M_BASE(n))		// Modem Status - except UART0
#define UART_TOR_ADDR(n) 		((UART0_BA+0x1C) + UART_M_BASE(n))		// Time-Out


/*----- PECI -----*/
#define PECI_BA 				0xF0100000
#define	PECI_N_BASE(n)			((n)*4)
#define PECI_CTL_STS			HW_BYTE((PECI_BA+0x000))			// PECI Control Status
#define PECI_RD_LENGTH			HW_BYTE((PECI_BA+0x004))			// PECI Transaction Control
#define PECI_ADDR				HW_BYTE((PECI_BA+0x008))			// PECI Address
#define PECI_CMD				HW_BYTE((PECI_BA+0x00C))			// PECI Command
#define PECI_CTL2				HW_BYTE((PECI_BA+0x010)) 			// PECI Control 2
#define PECI_WR_LENGTH			HW_BYTE((PECI_BA+0x01C))			// PECI Write Length
#define PECI_ENG1				HW_BYTE((PECI_BA+0x024))			// PECI Engineering 1
#define PECI_ENG2				HW_BYTE((PECI_BA+0x028))			// PECI Engineering 2
#define PECI_ENG3				HW_BYTE((PECI_BA+0x02C))			// PECI Engineering 3
#define PECI_DAT_INOUT(n)		HW_BYTE((PECI_BA+0x100) + PECI_N_BASE(n))	// PECI Data INOUT n


/*----- USB 2.0 -----*/

#define USB_DEV_BA					0xF0830000
#define USBD_BA(n)					HW_DWORD(USB_DEV_BA + (0x1000*(##n)))
#define USB2_ID(n) 					HW_DWORD(USBD_BA(##n) + 0x0000)	// Identification 
#define USB2_HWGENERAL(n)			HW_DWORD(USBD_BA(##n) + 0x0004)	// General Hardware Parameters
#define USB2_HWDEVICE(n)			HW_DWORD(USBD_BA(##n) + 0x000C)	// Device Hardware Parameters
#define USB2_HWTXBUF(n)				HW_DWORD(USBD_BA(##n) + 0x0010)	// TX Buffer Hardware Parameters
#define USB2_HWRXBUF(n)				HW_DWORD(USBD_BA(##n) + 0x0014)	// RX Buffer Hardware Parameters
#define USB2_GPTIMER0LD(n) 			HW_DWORD(USBD_BA(##n) + 0x0080)	// General Purpose Timer #0 Load
#define USB2_GPTIMER0CTRL(n)		HW_DWORD(USBD_BA(##n) + 0x0084)	// General Purpose Timer #0 Control
#define USB2_GPTIMER1LD(n) 			HW_DWORD(USBD_BA(##n) + 0x0088)	// General Purpose Timer #1 Load
#define USB2_GPTIMER1CTRL(n)		HW_DWORD(USBD_BA(##n) + 0x008C)	// General Purpose Timer #1 Control
#define USB2_SBSCFG(n)				HW_DWORD(USBD_BA(##n) + 0x0090)	// System Bus configuration
#define USB2_CAPLENGTH(n)			HW_BYTE(USBD_BA(##n) + 0x0100)	// Capability Register Length
#define USB2_DCIVERSION(n) 			HW_WORD(USBD_BA(##n) + 0x0120)	// Device Interface Version Number
#define USB2_DCCPARAMS(n)			HW_DWORD(USBD_BA(##n) + 0x0124)	// Device Control Capability Parameters
#define USB2_USBCMD(n)				HW_DWORD(USBD_BA(##n) + 0x0140)	// USB Command
#define USB2_USBSTS(n)				HW_DWORD(USBD_BA(##n) + 0x0144)	// USB Status
#define USB2_USBINTR(n)				HW_DWORD(USBD_BA(##n) + 0x0148)	// USB Interrupt Enable
#define USB2_FRINDEX(n)				HW_DWORD(USBD_BA(##n) + 0x014C)	// USB Frame Index
#define USB2_DeviceAddr(n) 			HW_DWORD(USBD_BA(##n) + 0x0154)	// USB Device Address
#define USB2_EndpointlistAddr(n)	HW_DWORD(USBD_BA(##n) + 0x0158)	// Address at Endpoint list in memory
#define USB2_BURSTSIZE(n)			HW_DWORD(USBD_BA(##n) + 0x0160)	// Programmable Burst Size
#define USB2_ENDPTNAK(n)			HW_DWORD(USBD_BA(##n) + 0x0178)	// Endpoint NAK
#define USB2_ENDPTNAKEN(n) 			HW_DWORD(USBD_BA(##n) + 0x017C)	// Endpoint NAK Enable
#define USB2_PORTSC1(n)				HW_DWORD(USBD_BA(##n) + 0x0184)	// Port Status/Control 1
#define USB2_USBMODE(n)				HW_DWORD(USBD_BA(##n) + 0x01A8)	// USB Device Mode
#define USB2_ENPDTSETUPSTAT(n)		HW_DWORD(USBD_BA(##n) + 0x01AC)	// Endpoint Setup Status
#define USB2_ENDPTPRIME(n) 			HW_DWORD(USBD_BA(##n) + 0x01B0)	// Endpoint Initialization
#define USB2_ENDPTFLUSH(n) 			HW_DWORD(USBD_BA(##n) + 0x01B4)	// Endpoint De-Initialize
#define USB2_ENDPTSTAT(n)			HW_DWORD(USBD_BA(##n) + 0x01B8)	// Endpoint Status
#define USB2_ENDPTCOMPLETE(n)		HW_DWORD(USBD_BA(##n) + 0x01BC)	// Endpoint Complete
#define USB2_ENDPTCTRL0(n) 			HW_DWORD(USBD_BA(##n) + 0x01C0)	// Endpoint Control 0
#define USB2_ENDPTCTRL1(n) 			HW_DWORD(USBD_BA(##n) + 0x01C4)	// Endpoint Control 1
#define USB2_ENDPTCTRL2(n) 			HW_DWORD(USBD_BA(##n) + 0x01C8)	// Endpoint Control 2


#define USBH_BA						0xF0806000
#define INSNREG00					HW_BYTE(USBH_BA+0x90)
#define INSNREG01					HW_BYTE(USBH_BA+0x94)
#define INSNREG02					HW_BYTE(USBH_BA+0x98)
#define INSNREG03					HW_BYTE(USBH_BA+0x8C)
/*----- FIU -----*/
#define FIU0_BA				0xFB000000
#define FIU3_BA				0xC0000000
#define FIU4_BA				0xFB001000
#define FIUX_BA				FIU4_BA
#define FIU_DRD_CFG(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0000) // Direct Read Configuration
#define FIU_DWR_CFG(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0004) // Direct Write Configuration
#define FIU_UMA_CFG(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0008) // UMA Configuration
#define FIU_UMA_CTS(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x000C) // UMA Control and Status
#define FIU_UMA_CMD(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0010) // UMA Command
#define FIU_UMA_ADDR(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0014) // UMA Address
#define FIU_PRT_CFG(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0018) // Protection Configuration
#define FIU_STPL_CFG(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x001C) // Status Polling Configuration
#define FIU_UMA_DW0(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0020) // UMA Write Data Bytes 0-3
#define FIU_UMA_DW1(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0024) // UMA Write Data Bytes 4-7
#define FIU_UMA_DW2(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0028) // UMA Write Data Bytes 8-11
#define FIU_UMA_DW3(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x002C) // UMA Write Data Bytes 12-15
#define FIU_UMA_DR0(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0030) // UMA Read Data Bytes 0-3
#define FIU_UMA_DR1(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0034) // UMA Read Data Bytes 4-7
#define FIU_UMA_DR2(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0038) // UMA Read Data Bytes 8-11
#define FIU_UMA_DR3(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x003C) // UMA Read Data Bytes 12-15
#define FIU_PRT_CMD0(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0040) // UMA Protection Command 0
#define FIU_PRT_CMD1(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0044) // UMA Protection Command 1
#define FIU_PRT_CMD2(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0048) // UMA Protection Command 2
#define FIU_PRT_CMD3(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x004C) // UMA Protection Command 3
#define FIU_PRT_CMD4(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0050) // UMA Protection Command 4
#define FIU_PRT_CMD5(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0054) // UMA Protection Command 5
#define FIU_PRT_CMD6(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0058) // UMA Protection Command 6
#define FIU_PRT_CMD7(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x005C) // UMA Protection Command 7
#define FIU_PRT_CMD8(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0060) // UMA Protection Command 8
#define FIU_PRT_CMD9(n)		HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0064) // UMA Protection Command 9
#define FIU_CFG(n)			HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x0078) 	  // Test
#define FIU_VER(n)			HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + 0x007C) 	  // Version
#define FIU_PRT_CMD(n, m)   HW_DWORD((n == 0 ? FIU0_BA : (n == 3 ? FIU3_BA : (n == 4 ? FIUX_BA : FIU0_BA))) + m*4 + 0x40) // UMA Protection Commands

//#define FIU_PRT_CMD(n, m) 	HW_DWORD(((##n==0) ? FIU0_BA : (##n==3) ? FIU3_BA : (##n==4) ? FIUX_BA : FIU0_BA) + FIU_PRT_CMD0(n) + (##m * 4))	// UMA Protection Commands



/*----- General Purpose I/O (GPIO) -----*/
#define GPIO0_BA			0xF0010000
#define GPIO1_BA			0xF0011000
#define GPIO2_BA			0xF0012000
#define GPIO3_BA			0xF0013000
#define GPIO4_BA			0xF0014000
#define GPIO5_BA			0xF0015000
#define GPIO6_BA			0xF0016000
#define GPIO7_BA			0xF0017000
#define	GPIO_M_BASE(n)		((n)*0x1000)
#define GPTLOCK1(n)			HW_DWORD((GPIO0_BA+0x0000) + GPIO_M_BASE(n))	// GPIO Temporary Lock 1
#define GPnDIN(n)			HW_DWORD((GPIO0_BA+0x0004) + GPIO_M_BASE(n))	// GPIO Data In
#define GPnPOL(n)			HW_DWORD((GPIO0_BA+0x0008) + GPIO_M_BASE(n))	// GPIO Polarity
#define GPnDOUT(n)			HW_DWORD((GPIO0_BA+0x000C) + GPIO_M_BASE(n))	// GPIO Data Out
#define GPnOE(n)			HW_DWORD((GPIO0_BA+0x0010) + GPIO_M_BASE(n))	// GPIO Output Enable
#define GPnOTYP(n)			HW_DWORD((GPIO0_BA+0x0014) + GPIO_M_BASE(n))	// GPIO Output Type
#define GPnMP(n)			HW_DWORD((GPIO0_BA+0x0018) + GPIO_M_BASE(n))	// GPIO on Main Power
#define GPnPU(n)			HW_DWORD((GPIO0_BA+0x001C) + GPIO_M_BASE(n))	// GPIO Pull-Up Control
#define GPnPD(n)			HW_DWORD((GPIO0_BA+0x0020) + GPIO_M_BASE(n))	// GPIO Pull-Down Control
#define GPnDBNC(n)			HW_DWORD((GPIO0_BA+0x0024) + GPIO_M_BASE(n))	// GPIO Debounce Enable
#define GPnEVTYP(n)			HW_DWORD((GPIO0_BA+0x0028) + GPIO_M_BASE(n))	// GPIO Event Type
#define GPnEVBE(n)			HW_DWORD((GPIO0_BA+0x002C) + GPIO_M_BASE(n))	// GPIO Event for Both Edges
#define GPnOBL0(n)			HW_DWORD((GPIO0_BA+0x0030) + GPIO_M_BASE(n))	// GPIO Output Blink Control 0
#define GPnOBL1(n)			HW_DWORD((GPIO0_BA+0x0034) + GPIO_M_BASE(n))	// GPIO Output Blink Control 1
#define GPnOBL2(n)			HW_DWORD((GPIO0_BA+0x0038) + GPIO_M_BASE(n))	// GPIO Output Blink Control 2
#define GPnOBL3(n)			HW_DWORD((GPIO0_BA+0x003C) + GPIO_M_BASE(n))	// GPIO Output Blink Control 3
#define GPnOBLx(n)			HW_DWORD(((GPIO0_BA+0x0030) + GPIO_M_BASE(n)) + (4 * (n / 8)) )	// GPIO Output Blink Control 3
#define GPnEVEN(n)			HW_DWORD((GPIO0_BA+0x0040) + GPIO_M_BASE(n))	// GPIO Event Enable
#define GPnEVENS(n)			HW_DWORD((GPIO0_BA+0x0044) + GPIO_M_BASE(n))	// GPIO Event Set
#define GPnEVENC(n)			HW_DWORD((GPIO0_BA+0x0048) + GPIO_M_BASE(n))	// GPIO Event Clear
#define GPnEVST(n)			HW_DWORD((GPIO0_BA+0x004C) + GPIO_M_BASE(n))	// GPIO Event Status
#define GPnSPLCK(n)			HW_DWORD((GPIO0_BA+0x0050) + GPIO_M_BASE(n))	// GPIO Standby Power Lock
#define GPnMPLCK(n)			HW_DWORD((GPIO0_BA+0x0054) + GPIO_M_BASE(n))	// GPIO Main Power Lock
#define GPnIEM(n)			HW_DWORD((GPIO0_BA+0x0058) + GPIO_M_BASE(n))	// GPIO Input Enable Mask
#define GPnOSRC(n)			HW_DWORD((GPIO0_BA+0x005C) + GPIO_M_BASE(n))	// GPIO Output Slew-Rate Control
#define GPnODSC(n)			HW_DWORD((GPIO0_BA+0x0060) + GPIO_M_BASE(n))	// GPIO Output Drive Strength Control
#define GPnVER(n)			HW_DWORD((GPIO0_BA+0x0064) + GPIO_M_BASE(n))	// GPIO Version
#define GPnDOS(n)			HW_DWORD((GPIO0_BA+0x0068) + GPIO_M_BASE(n))	// GPIO Data Out Register Set
#define GPnDOC(n)			HW_DWORD((GPIO0_BA+0x006C) + GPIO_M_BASE(n))	// GPIO Data Out Register C;ear
#define GPnOES(n)			HW_DWORD((GPIO0_BA+0x0070) + GPIO_M_BASE(n))	// GPIO Output Enable Register Set
#define GPnOEC(n)			HW_DWORD((GPIO0_BA+0x0074) + GPIO_M_BASE(n))	// GPIO Output Enable Register ;
#define GPnTLOCK2(n)			HW_DWORD((GPIO0_BA+0x0000) + GPIO_M_BASE(n))	// GPIO Temporary Lock 2
#define GPxCFG0(module, port)	   HW_DWORD(((GPIO0_BA+((##port==0) ? 0x14 : \
														(##port==1) ? 0x24 : \
														(##port==2) ? 0x3C : \
														(##port==3) ? 0x50 : \
														(##port==4) ? 0x64 : \
														(##port==5) ? 0x78 : 0x90)) + GPIO_M_BASE(module))	 

#define GPxCFG1(module, port)	   HW_DWORD(((GPIO0_BA+((##port==0) ? 0x18 : \
														(##port==1) ? 0x28 : \
														(##port==2) ? 0x40 : \
														(##port==3) ? 0x54 : \
														(##port==4) ? 0x68 : \
														(##port==5) ? 0x7C : 0x94)) + GPIO_M_BASE(module))

#define GPxCFG2(module, port)	   HW_DWORD(((GPIO0_BA+((##port==1) ? 0x2C : \
														(##port==2) ? 0x44 : \
														(##port==3) ? 0x58 : \
														(##port==4) ? 0x6C : \
														(##port==5) ? 0x80 : 0x98)) + GPIO_M_BASE(module))

#define GPxDOUT(module, port)	   HW_DWORD(((GPIO0_BA+((##port==0) ? 0x1C : \
														(##port==1) ? 0x34 : \
														(##port==2) ? 0x48 : \
														(##port==3) ? 0x5C : \
														(##port==4) ? 0x70 : \
														(##port==5) ? 0x84 : 0x9C)) + GPIO_M_BASE(module))

#define GPxDIN(module, port)	   HW_DWORD(((GPIO0_BA+((##port==0) ? 0x20 : \
														(##port==1) ? 0x38 : \
														(##port==2) ? 0x4C : \
														(##port==3) ? 0x60 : \
														(##port==4) ? 0x74 : \
														(##port==5) ? 0x88 : \
														(##port==6) ? 0x8C : 0xA0)) + GPIO_M_BASE(module))


/*----- SMBus  -----*/
#define SMB0_BA				0xF0080000
#define SMB1_BA 			0xF0081000
#define SMB2_BA 			0xF0082000
#define SMB3_BA 			0xF0083000
#define SMB4_BA 			0xF0084000
#define SMB5_BA 			0xF0085000
#define SMB6_BA 			0xF0086000
#define SMB7_BA 			0xF0087000
#define SMB8_BA 			0xF0088000
#define SMB9_BA 			0xF0089000
#define SMB10_BA 			0xF008A000
#define SMB11_BA 			0xF008B000
#define	SMB_M_BASE(n)		((n)*0x1000)
#define SMBnSDA(n)			HW_BYTE((SMB0_BA+0x00) + SMB_M_BASE(n))		// SMB serial data
#define SMBnST(n)			HW_BYTE((SMB0_BA+0x02) + SMB_M_BASE(n))		// SMB status
#define SMBnCST(n)			HW_BYTE((SMB0_BA+0x04) + SMB_M_BASE(n))		// SMB control status
#define SMBnCTL1(n) 		HW_BYTE((SMB0_BA+0x06) + SMB_M_BASE(n))		// SMB control 1
#define SMBnADDR1(n)		HW_BYTE((SMB0_BA+0x08) + SMB_M_BASE(n))		// SMB own address 1
#define SMBnCTL2(n) 		HW_BYTE((SMB0_BA+0x0A) + SMB_M_BASE(n))		// SMB control 2
#define SMBnADDR2(n)		HW_BYTE((SMB0_BA+0x0C) + SMB_M_BASE(n))		// SMB own address 2
#define SMBnCTL3(n) 		HW_BYTE((SMB0_BA+0x0E) + SMB_M_BASE(n))		// SMB control 3

//BANK 0 Select

#define SMBnADDR3(n)		HW_BYTE((SMB0_BA+0x10) + SMB_M_BASE(n))		// SMB own address 3
#define SMBnADDR7(n)		HW_BYTE((SMB0_BA+0x11) + SMB_M_BASE(n))		// SMB own address 7
#define SMBnADDR4(n)		HW_BYTE((SMB0_BA+0x12) + SMB_M_BASE(n))		// SMB own address 4
#define SMBnADDR8(n)		HW_BYTE((SMB0_BA+0x13) + SMB_M_BASE(n))		// SMB own address 7
#define SMBnADDR5(n)		HW_BYTE((SMB0_BA+0x14) + SMB_M_BASE(n))		// SMB own address 5
#define SMBnADDR9(n)		HW_BYTE((SMB0_BA+0x15) + SMB_M_BASE(n))		// SMB own address 9
#define SMBnADDR6(n)		HW_BYTE((SMB0_BA+0x16) + SMB_M_BASE(n))		// SMB own address 6
#define SMBnADDR10(n)		HW_BYTE((SMB0_BA+0x17) + SMB_M_BASE(n))		// SMB own address 10
#define SMBnCST2(n) 		HW_BYTE((SMB0_BA+0x18) + SMB_M_BASE(n))		// SMB control status 2
#define SMBnCST3(n) 		HW_BYTE((SMB0_BA+0x19) + SMB_M_BASE(n))		// SMB control status 3
#define SMBnCTL4(n) 		HW_BYTE((SMB0_BA+0x1A) + SMB_M_BASE(n))		// SMB control 4
#define SMBnCTL5(n) 		HW_BYTE((SMB0_BA+0x1B) + SMB_M_BASE(n))		// SMB control 5
#define SMBnSCLLT(n) 		HW_BYTE((SMB0_BA+0x1C) + SMB_M_BASE(n))		// SMB Low time register
#define SMBnFIF_CTL(n) 		HW_BYTE((SMB0_BA+0x1D) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnSCLHT(n) 		HW_BYTE((SMB0_BA+0x1E) + SMB_M_BASE(n))		// SMB High time register
#define SMBnVER(n) 			HW_BYTE((SMB0_BA+0x1F) + SMB_M_BASE(n))		// SMB Version


#define SMBnSDA_ADDR(n)			((SMB0_BA+0x00) + SMB_M_BASE(n))		// SMB serial data
#define SMBnST_ADDR(n)			((SMB0_BA+0x02) + SMB_M_BASE(n))		// SMB status
#define SMBnCST_ADDR(n)			((SMB0_BA+0x04) + SMB_M_BASE(n))		// SMB control status
#define SMBnCTL1_ADDR(n) 		((SMB0_BA+0x06) + SMB_M_BASE(n))		// SMB control 1
#define SMBnADDR1_ADDR(n)		((SMB0_BA+0x08) + SMB_M_BASE(n))		// SMB own address 1
#define SMBnCTL2_ADDR(n) 		((SMB0_BA+0x0A) + SMB_M_BASE(n))		// SMB control 2
#define SMBnADDR2_ADDR(n)		((SMB0_BA+0x0C) + SMB_M_BASE(n))		// SMB own address 2
#define SMBnCTL3_ADDR(n) 		((SMB0_BA+0x0E) + SMB_M_BASE(n))		// SMB control 3

//BANK 0
#define SMBnADDR3_ADDR(n)		((SMB0_BA+0x10) + SMB_M_BASE(n))		// SMB own address 3
#define SMBnADDR7_ADDR(n)		((SMB0_BA+0x11) + SMB_M_BASE(n))		// SMB own address 7
#define SMBnADDR4_ADDR(n)		((SMB0_BA+0x12) + SMB_M_BASE(n))		// SMB own address 4
#define SMBnADDR8_ADDR(n)		((SMB0_BA+0x13) + SMB_M_BASE(n))		// SMB own address 8
#define SMBnADDR5_ADDR(n)		((SMB0_BA+0x14) + SMB_M_BASE(n))		// SMB own address 5
#define SMBnADDR9_ADDR(n)		((SMB0_BA+0x15) + SMB_M_BASE(n))		// SMB own address 5
#define SMBnADDR6_ADDR(n)		((SMB0_BA+0x16) + SMB_M_BASE(n))		// SMB own address 6
#define SMBnADDR10_ADDR(n)		((SMB0_BA+0x17) + SMB_M_BASE(n))		// SMB own address 10
#define SMBnCST2_ADDR(n) 		((SMB0_BA+0x18) + SMB_M_BASE(n))		// SMB control status 2
#define SMBnCST3_ADDR(n) 		((SMB0_BA+0x19) + SMB_M_BASE(n))		// SMB control status 3
#define SMBnCTL4_ADDR(n) 		((SMB0_BA+0x1A) + SMB_M_BASE(n))		// SMB control 4
#define SMBnCTL5_ADDR(n) 		((SMB0_BA+0x1B) + SMB_M_BASE(n))		// SMB control 5
#define SMBnSCLLT_ADDR(n) 		((SMB0_BA+0x1C) + SMB_M_BASE(n))		// SMB Low time register
#define SMBnFIF_CTL_ADDR(n) 	((SMB0_BA+0x1D) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnSCLHT_ADDR(n) 		((SMB0_BA+0x1E) + SMB_M_BASE(n))		// SMB High time register
#define SMBnVER_ADDR(n) 		((SMB0_BA+0x1F) + SMB_M_BASE(n))		// SMB Version

//BANK 1
#define SMBnFIF_CTS_ADDR(n) 	((SMB0_BA+0x10) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBn_FAIR_PER(n) 		((SMB0_BA+0x11) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnTXF_CTL_ADDR(n) 	((SMB0_BA+0x12) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnT_OUT_ADDR(n) 		((SMB0_BA+0x14) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnPEC_ADDR(n) 		((SMB0_BA+0x16) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnCST2_ADDR(n) 		((SMB0_BA+0x18) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnCST3_ADDR(n) 		((SMB0_BA+0x19) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnTXF_STS_ADDR(n) 	((SMB0_BA+0x1A) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnRXF_STS_ADDR(n) 	((SMB0_BA+0x1C) + SMB_M_BASE(n))		// SMB FIFO control 
#define SMBnRXF_CTL_ADDR(n) 	((SMB0_BA+0x1E) + SMB_M_BASE(n))		// SMB FIFO control 


/****** RNG ******/
#define RNG_BA			0xF000B000
#define RNGCS 			HW_BYTE((RNG_BA+0x00))
#define RNGD 			HW_BYTE((RNG_BA+0x04))
#define RNGTST 			HW_BYTE((RNG_BA+0x08))


/****** VDMA ******/
#define VDMA_BA			0xF0822000
#define VDMA_CTL 		HW_DWORD(VDMA_BA+0x0000)
#define VDMA_SRCB 		HW_DWORD(VDMA_BA+0x0004)
#define VDMA_DSTB 		HW_DWORD(VDMA_BA+0x0008)
#define VDMA_TCNT 		HW_DWORD(VDMA_BA+0x000C)
#define VDMA_CSRC 		HW_DWORD(VDMA_BA+0x0010)
#define VDMA_CDST 		HW_DWORD(VDMA_BA+0x0014)
#define VDMA_CTCNT 		HW_DWORD(VDMA_BA+0x0018)
#define VDMA_ECTL 		HW_DWORD(VDMA_BA+0x0040)
#define VDMA_ESRCSZ		HW_DWORD(VDMA_BA+0x0044)
#define VDMA_ERDPNT		HW_DWORD(VDMA_BA+0x0048)
#define VDMA_EST0AD		HW_DWORD(VDMA_BA+0x0050)
#define VDMA_EST0MK		HW_DWORD(VDMA_BA+0x0054)
#define VDMA_EST0DT		HW_DWORD(VDMA_BA+0x0058)
#define VDMA_EST1AD		HW_DWORD(VDMA_BA+0x0060)
#define VDMA_EST1MK		HW_DWORD(VDMA_BA+0x0064)
#define VDMA_EST1DT		HW_DWORD(VDMA_BA+0x0068)


/******  GDMA Registers *****/

#define GDMA0_BA			0xF0850000
#define GDMA1_BA			0xF0851000
#define GDMA2_BA			0xF0852000
#define GDMA_BA(m)			(GDMA0_BA + (m)*0x1000)
#define GDMA_CHANNEL_OFFSET(c)	(0x20 * c)
//#define	GDMA_M_BASE(n)		((n)*0x1000)
//#define GDMA_SRCB0(n)		HW_DWORD((GDMA0_BA+0x4) + GDMA_M_BASE(n))		//Channel 0 Source Base Address Register
//#define GDMA_SRCB1(n)		HW_DWORD((GDMA0_BA+0x24) + GDMA_M_BASE(n))	//Channel 1 Source Base Address Register

#define GDMA_CTL(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x0)
#define GDMA_SRCB(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x4)
#define GDMA_DSTB(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x8)
#define GDMA_TCNT(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0xc)
#define GDMA_CSRC(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x10)
#define GDMA_CDST(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x14)
#define GDMA_CTCNT(m,c)		HW_DWORD(GDMA_BA(m) + GDMA_CHANNEL_OFFSET(c) + 0x18)


/*----- Shared Memory (SHM) +++ CORE SIDE +++ -----*/
#define SHM_BA				0xC0001000
#define SMC_STS 			HW_BYTE((SHM_BA+0x00))				// Shared Memory Core Status
#define SMC_CTL 			HW_BYTE((SHM_BA+0x01))				// Shared Memory Core Control
#define FLASH_SIZE			HW_BYTE((SHM_BA+0x02))				// Flash Memory Size
#define FWH_ID_LPC			HW_BYTE((SHM_BA+0x03))				// FWH ID on LPC bus
#define FLASH_BASE1			HW_BYTE((SHM_BA+0x04))				// Flash Memory Base Address 1
#define FLASH_BASE2			HW_BYTE((SHM_BA+0x05))				// Flash Memory Base Address 2
#define WIN_PROT			HW_BYTE((SHM_BA+0x06))				// RAM Windows Protection
#define WIN_SIZE			HW_BYTE((SHM_BA+0x07))				// RAM Windows Size
#define CORE_SHAW1_SEM		HW_BYTE((SHM_BA+0x08))				// Shared Access Window 1, Semaphore
#define CORE_SHAW2_SEM		HW_BYTE((SHM_BA+0x09))				// Shared Access Window 2, Semaphore
#define WIN_BASE1			HW_WORD((SHM_BA+0x0A))				// RAM Window 1 Base Address
#define WIN_BASE2			HW_WORD((SHM_BA+0x0C))				// RAM Window 2 Base Address
#define SMCORP0 			HW_WORD((SHM_BA+0x10))				// SHM Core Overrider Read protect 0 
#define SMCORP1 			HW_WORD((SHM_BA+0x12))				// SHM Core Overrider Read protect 1 
#define SMCORP2 			HW_WORD((SHM_BA+0x14))				// SHM Core Overrider Read protect 2 
#define SMCORP3 			HW_WORD((SHM_BA+0x16))				// SHM Core Overrider Read protect 3
#define SMCOWP0 			HW_WORD((SHM_BA+0x18))				// SHM Core Overrider Write Protect 0
#define SMCOWP1 			HW_WORD((SHM_BA+0x1A))				// SHM Core Overrider Write Protect 1
#define SMCOWP2 			HW_WORD((SHM_BA+0x1C))				// SHM Core Overrider Write Protect 2
#define SMCOWP3 			HW_WORD((SHM_BA+0x1E))				// SHM Core Overrider Write Protect 3


/*----- Timer Control (TMR) -----*/
#define TMR1_BA 			0xF0008000
#define TMR2_BA 			0xF0009000
#define TMR3_BA 			0xF000A000
#define TMR_P_TO_OFFSET(m)	((m/2)*0x20 + (m%2)*4)
#define TMR_M_TO_BA(n)		((n-1) *0x1000)

#define TICR_1(n)			HW_DWORD((TMR1_BA+0x08 + TMR_P_TO_OFFSET(n)))
#define TISR_1				HW_DWORD((TMR1_BA+0x18))			// Timer Interrupt Status for Timer 1
#define WTCR_1				HW_DWORD((TMR1_BA+0x1C))			// Watchdog Timer Control for Timer 1
#define TCSR_1(n)			HW_DWORD((TMR1_BA +TMR_P_TO_OFFSET(n)+0 ))
#define TDR_1(n)			HW_DWORD((TMR1_BA +TMR_P_TO_OFFSET(n)+0x10))
#define TISR_2				HW_DWORD((TMR2_BA+0x18))			// Timer Interrupt Status for Timer 2
#define WTCR_2				HW_DWORD((TMR2_BA+0x1C))			// Watchdog Timer Control for Timer 2
#define TICR_2(n)				HW_DWORD((TMR2_BA+0x0C +TMR_P_TO_OFFSET(n)))
#define TCSR_2(n)			HW_DWORD((TMR2_BA +TMR_P_TO_OFFSET(n)+0 ))
#define TDR_2(n)			HW_DWORD((TMR2_BA +TMR_P_TO_OFFSET(n)+0x10))

#define TISR_3				HW_DWORD((TMR3_BA+0x18))			// Timer Interrupt Status for Timer 3
#define WTCR_3				HW_DWORD((TMR3_BA+0x1C))			// Watchdog Timer Control for Timer 3
#define TICR_3(n)				HW_DWORD((TMR3_BA+0x28+TMR_P_TO_OFFSET(n)))
#define TCSR_3(n)			HW_DWORD((TMR3_BA +TMR_P_TO_OFFSET(n)+0 ))
#define TDR_3(n)			HW_DWORD((TMR3_BA +TMR_P_TO_OFFSET(n)+0x10))
// #define TCSR_1(n)			HW_DWORD((TMR1_BA+((##n==0) ? 0x00 : (##n==1) ? 0x04 : (##n==2) ? 0x20 : (##n==3) ? 0x24 :0x40)))
// #define TICR_1(n)			HW_DWORD((TMR1_BA+((##n==0) ? 0x08 : (##n==1) ? 0x0C : (##n==2) ? 0x28 : (##n==3) ? 0x2C :0x48)))
// #define TDR_1(n)			HW_DWORD((TMR1_BA+((##n==0) ? 0x10 : (##n==1) ? 0x14 : (##n==2) ? 0x30 : (##n==3) ? 0x34 :0x50)))
// #define TCSR_2(n)			HW_DWORD((TMR2_BA+((##n==0) ? 0x00 : (##n==1) ? 0x04 : (##n==2) ? 0x20 : (##n==3) ? 0x24 :0x40)))
// #define TICR_2(n)			HW_DWORD((TMR2_BA+((##n==0) ? 0x08 : (##n==1) ? 0x0C : (##n==2) ? 0x28 : (##n==3) ? 0x2C :0x48)))
// #define TDR_2(n)			HW_DWORD((TMR2_BA+((##n==0) ? 0x10 : (##n==1) ? 0x14 : (##n==2) ? 0x30 : (##n==3) ? 0x34 :0x50)))
// #define TCSR_3(n)			HW_DWORD((TMR3_BA+((##n==0) ? 0x00 : (##n==1) ? 0x04 : (##n==2) ? 0x20 : (##n==3) ? 0x24 :0x40)))
// #define TICR_3(n)			HW_DWORD((TMR3_BA+((##n==0) ? 0x08 : (##n==1) ? 0x0C : (##n==2) ? 0x28 : (##n==3) ? 0x2C :0x48)))
// #define TDR_3(n)			HW_DWORD((TMR3_BA+((##n==0) ? 0x10 : (##n==1) ? 0x14 : (##n==2) ? 0x30 : (##n==3) ? 0x34 :0x50)))
#define TCSR(n,m)			HW_DWORD((TMR1_BA +TMR_P_TO_OFFSET(n)+0 + TMR_M_TO_BA(m)))
#define TISR(n)				HW_DWORD((TMR1_BA+0x18)+(TMR_M_TO_BA(n)))		// Timer Interrupt Status
#define TICR(n,m)			HW_DWORD((TMR1_BA +TMR_P_TO_OFFSET(n)+8 + TMR_M_TO_BA(m)))
#define WTCR(n)				HW_DWORD((TMR1_BA+0x1C)+(TMR_M_TO_BA(n)))		// Watchdog Timer Control
#define TDR(n,m)			HW_DWORD((TMR1_BA +TMR_P_TO_OFFSET(n)+0x10 + TMR_M_TO_BA(m)))


/*----- Advanced Interrupt Controller (AIC) -----*/
#define AIC_BA				0xF0018000 
#define AIC_SCR1			HW_DWORD((AIC_BA+0x004))			// Source Control 1
#define AIC_SCR2			HW_DWORD((AIC_BA+0x008))			// Source Control 2
#define AIC_SCR3			HW_DWORD((AIC_BA+0x00C))			// Source Control 3
#define AIC_SCR4			HW_DWORD((AIC_BA+0x010))			// Source Control 4
#define AIC_SCR5			HW_DWORD((AIC_BA+0x014))			// Source Control 5
#define AIC_SCR6			HW_DWORD((AIC_BA+0x018))			// Source Control 6
#define AIC_SCR7			HW_DWORD((AIC_BA+0x01C))			// Source Control 7
#define AIC_SCR8			HW_DWORD((AIC_BA+0x020))			// Source Control 8
#define AIC_SCR9			HW_DWORD((AIC_BA+0x024))			// Source Control 9
#define AIC_SCR10			HW_DWORD((AIC_BA+0x028))			// Source Control 10
#define AIC_SCR11			HW_DWORD((AIC_BA+0x02C))			// Source Control 11
#define AIC_SCR12			HW_DWORD((AIC_BA+0x030))			// Source Control 12
#define AIC_SCR13			HW_DWORD((AIC_BA+0x034))			// Source Control 13
#define AIC_SCR14			HW_DWORD((AIC_BA+0x038))			// Source Control 14
#define AIC_SCR15			HW_DWORD((AIC_BA+0x03C))			// Source Control 15
#define AIC_SCR16			HW_DWORD((AIC_BA+0x040))			// Source Control 16
#define AIC_SCR17			HW_DWORD((AIC_BA+0x044))			// Source Control 17
#define AIC_SCR18			HW_DWORD((AIC_BA+0x048))			// Source Control 18
#define AIC_SCR19			HW_DWORD((AIC_BA+0x04C))			// Source Control 19
#define AIC_SCR20			HW_DWORD((AIC_BA+0x050))			// Source Control 20
#define AIC_SCR21			HW_DWORD((AIC_BA+0x054))			// Source Control 21
#define AIC_SCR22			HW_DWORD((AIC_BA+0x058))			// Source Control 22
#define AIC_SCR23			HW_DWORD((AIC_BA+0x05C))			// Source Control 23
#define AIC_SCR24			HW_DWORD((AIC_BA+0x060))			// Source Control 24
#define AIC_SCR25			HW_DWORD((AIC_BA+0x064))			// Source Control 25
#define AIC_SCR26			HW_DWORD((AIC_BA+0x068))			// Source Control 26
#define AIC_SCR27			HW_DWORD((AIC_BA+0x06C))			// Source Control 27
#define AIC_SCR28			HW_DWORD((AIC_BA+0x070))			// Source Control 28
#define AIC_SCR29			HW_DWORD((AIC_BA+0x074))			// Source Control 29
#define AIC_SCR30			HW_DWORD((AIC_BA+0x078))			// Source Control 30
#define AIC_SCR31			HW_DWORD((AIC_BA+0x07C))			// Source Control 31
#define AIC_GEN 			HW_DWORD((AIC_BA+0x084))			// Interrupt Group Enable Control
#define AIC_GRSR			HW_DWORD((AIC_BA+0x088))			// Interrupt Group Raw Status
#define AIC_IRSR			HW_DWORD((AIC_BA+0x100))			// Interrupt Raw Status
#define AIC_IASR			HW_DWORD((AIC_BA+0x104))			// Interrupt Active Status
#define AIC_ISR 			HW_DWORD((AIC_BA+0x108))			// Interrupt Status
#define AIC_IPER			HW_DWORD((AIC_BA+0x10C))			// Interrupt Priority Encoding
#define AIC_ISNR			HW_DWORD((AIC_BA+0x110))			// Interrupt Source Number
#define AIC_IMR 			HW_DWORD((AIC_BA+0x114))			// Output Mask Status
#define AIC_OISR			HW_DWORD((AIC_BA+0x118))			// Output Interrupt Status
#define AIC_MECR			HW_DWORD((AIC_BA+0x120))			// Mask Enable Command
#define AIC_MDCR			HW_DWORD((AIC_BA+0x124))			// Mask Disable Command
#define AIC_SSCR			HW_DWORD((AIC_BA+0x128))			// Source Set Command
#define AIC_SCCR			HW_DWORD((AIC_BA+0x12C))			// Source Clear Command
#define AIC_EOSCR			HW_DWORD((AIC_BA+0x130))			// End Of Service Command
#define AIC_TEST			HW_DWORD((AIC_BA+0x200))			// ICE/Debug Mode

#define AIC_SCR(n)			HW_DWORD((AIC_BA+(4*##n)))			// Source Control


/*----- Pulse Width Modulation (PWM) -----*/
#define PWMM0_BA			0xF0103000
#define PWMM1_BA			0xF0104000
#define	PWM_M_BASE(n)		((n)*0x1000)
#define	PWM_MULT_C(m)		((m)*0x0C)
#define	PWM_MULT_4(m)		((m)*0x04)
#define PPR(n)				HW_DWORD((PWMM0_BA+0x00) + PWM_M_BASE(n))	// PWM Pre-Scale 0
#define CSR(n)				HW_DWORD((PWMM0_BA+0x04) + PWM_M_BASE(n))	// PWM Clock Select
#define PCR(n)				HW_DWORD((PWMM0_BA+0x08) + PWM_M_BASE(n))	// PWM Control
#define CNR0(n) 			HW_DWORD((PWMM0_BA+0x0C) + PWM_M_BASE(n))	// PWM Counter 0
#define CMR0(n) 			HW_DWORD((PWMM0_BA+0x10) + PWM_M_BASE(n))	// PWM Comparator 0
#define PDR0(n) 			HW_DWORD((PWMM0_BA+0x14) + PWM_M_BASE(n))	// PWM Data 0
#define CNR1(n) 			HW_DWORD((PWMM0_BA+0x18) + PWM_M_BASE(n))	// PWM Counter 1
#define CMR1(n) 			HW_DWORD((PWMM0_BA+0x1C) + PWM_M_BASE(n))	// PWM Comparator 1
#define PDR1(n) 			HW_DWORD((PWMM0_BA+0x20) + PWM_M_BASE(n))	// PWM Data 1
#define CNR2(n) 			HW_DWORD((PWMM0_BA+0x24) + PWM_M_BASE(n))	// PWM Counter 2
#define CMR2(n) 			HW_DWORD((PWMM0_BA+0x28) + PWM_M_BASE(n))	// PWM Comparator 2
#define PDR2(n) 			HW_DWORD((PWMM0_BA+0x2C) + PWM_M_BASE(n))	// PWM Data 2
#define CNR3(n) 			HW_DWORD((PWMM0_BA+0x30) + PWM_M_BASE(n))	// PWM Counter 3
#define CMR3(n) 			HW_DWORD((PWMM0_BA+0x34) + PWM_M_BASE(n))	// PWM Comparator 3
#define PDR3(n) 			HW_DWORD((PWMM0_BA+0x38) + PWM_M_BASE(n))	// PWM Data 3
#define PIER(n) 			HW_DWORD((PWMM0_BA+0x3C) + PWM_M_BASE(n))	// PWM Timer Interrupt Enable
#define PIIR(n) 			HW_DWORD((PWMM0_BA+0x40) + PWM_M_BASE(n))	// PWM Timer Interrupt Identification
#define PWDR0(n)			HW_DWORD((PWMM0_BA+0x44) + PWM_M_BASE(n))	// PWM Watchdog 0
#define PWDR1(n)			HW_DWORD((PWMM0_BA+0x48) + PWM_M_BASE(n))	// PWM Watchdog 1
#define PWDR2(n)			HW_DWORD((PWMM0_BA+0x4C) + PWM_M_BASE(n))	// PWM Watchdog 2
#define PWDR3(n)			HW_DWORD((PWMM0_BA+0x50) + PWM_M_BASE(n))	// PWM Watchdog 3
#define CNR(m,n) 			HW_DWORD((PWMM0_BA+0x0C) + PWM_MULT_C(m) + PWM_M_BASE(n))	// PWM Counter
#define CMR(m,n) 			HW_DWORD((PWMM0_BA+0x10) + PWM_MULT_C(m) + PWM_M_BASE(n))	// PWM Comparator
#define PDR(m,n) 			HW_DWORD((PWMM0_BA+0x14) + PWM_MULT_C(m) + PWM_M_BASE(n))	// PWM Data
#define PWDR(m,n)			HW_DWORD((PWMM0_BA+0x44) + PWM_MULT_4(m) + PWM_M_BASE(n))	// PWM Watchdog


/*----- Tachometers (MFT 0 and 1) -----*/
#define MFT0_BA 			0xF0180000
#define MFT1_BA 			0xF0181000
#define MFT2_BA 			0xF0182000
#define MFT3_BA 			0xF0183000
#define MFT4_BA 			0xF0184000
#define MFT5_BA 			0xF0185000
#define MFT6_BA 			0xF0186000
#define MFT7_BA 			0xF0187000

#define	MFT_M_BASE(n)		((n)*0x1000)
#define TnCNT1(n)			HW_WORD(((MFT0_BA+0x00)+MFT_M_BASE(n)))		// Timer/Counter 1
#define TnCRA(n)			HW_WORD(((MFT0_BA+0x02)+MFT_M_BASE(n)))		// Reload/Capture A
#define TnCRB(n)			HW_WORD(((MFT0_BA+0x04)+MFT_M_BASE(n)))		// Reload/Capture B
#define TnCNT2(n)			HW_WORD(((MFT0_BA+0x06)+MFT_M_BASE(n)))		// Timer/Counter 2
#define TnPRSC(n)			HW_BYTE(((MFT0_BA+0x08)+MFT_M_BASE(n)))		// Clock prescaler
#define TnCKC(n)			HW_BYTE(((MFT0_BA+0x0A)+MFT_M_BASE(n)))		// Clock Until control
#define TnMCTRL(n)			HW_BYTE(((MFT0_BA+0x0C)+MFT_M_BASE(n)))		// Timer mode control
#define TnICTRL(n)			HW_BYTE(((MFT0_BA+0x0E)+MFT_M_BASE(n)))		// Timer interrupt control
#define TnICLR(n)			HW_BYTE(((MFT0_BA+0x10)+MFT_M_BASE(n)))		// Timer interrupt clear
#define TnIEN(n)			HW_BYTE(((MFT0_BA+0x12)+MFT_M_BASE(n)))		// Timer interrupt enable
#define TnCPA(n)			HW_WORD(((MFT0_BA+0x14)+MFT_M_BASE(n)))		// Compare A
#define TnCPB(n)			HW_WORD(((MFT0_BA+0x16)+MFT_M_BASE(n)))		// Compare B
#define TnCPCFG(n)			HW_BYTE(((MFT0_BA+0x18)+MFT_M_BASE(n)))		// Compare configuration
#define TnINASEL(n)			HW_BYTE(((MFT0_BA+0x1A)+MFT_M_BASE(n)))		// Input select A
#define TnINBSEL(n)			HW_BYTE(((MFT0_BA+0x1C)+MFT_M_BASE(n)))		// Input select B


/*----- Peripheral SERIAL PHERIPHERAL INTERFACE (PSPI) -----*/
#define PSPI1_BA			0xF0200000
#define PSPI2_BA			0xF0201000
#define	PSPI_M_BASE(n)		((n)*0x1000)
#define PSPI_DATA(n)		HW_WORD((PSPI1_BA+0x00) + MFT_M_BASE(n-1))	// PSPI Data In/Out 
#define PSPI_CTL1(n)		HW_WORD((PSPI1_BA+0x02) + MFT_M_BASE(n-1))	// PSPI control 1
#define PSPI_STAT(n)		HW_BYTE((PSPI1_BA+0x04) + MFT_M_BASE(n-1))	// PSPI Status
#define PSPI_TEST(n)		HW_WORD((PSPI1_BA+0x08) + MFT_M_BASE(n-1))	// PSPI Test


/*----- SDHC/MMC -----*/
#define SD_BA				0xF0840000
#define MMC_BA				0xF0842000
#define	SD_M_BASE(n)		((n)*0x2000)
#define SADD_ARG2(n)		HW_DWORD((SD_BA+0x00) + SD_M_BASE(n))
#define BLKSIZ(n)			HW_WORD((SD_BA+0x04) + SD_M_BASE(n))
#define BLKCNT(n)			HW_WORD((SD_BA+0x06) + SD_M_BASE(n))
#define ARG1REG(n)			HW_DWORD((SD_BA+0x08) + SD_M_BASE(n))
#define TRNSMOD(n)			HW_WORD((SD_BA+0x0C) + SD_M_BASE(n))
#define SD_CMD(n)			HW_WORD((SD_BA+0x0E) + SD_M_BASE(n))
#define BUFDATPORT(n)		HW_DWORD((SD_BA+0x20) + SD_M_BASE(n))
#define PRSNTSTATE(n)		HW_DWORD((SD_BA+0x24) + SD_M_BASE(n))
#define HSTCONT1(n)			HW_BYTE((SD_BA+0x28) + SD_M_BASE(n))
#define PWRCTRL(n)			HW_BYTE((SD_BA+0x29) + SD_M_BASE(n))
#define BLKGAPCTRL(n)		HW_BYTE((SD_BA+0x2A) + SD_M_BASE(n))
#define WAKECTRL(n)			HW_BYTE((SD_BA+0x2B) + SD_M_BASE(n))
#define CLKCTRL(n)			HW_WORD((SD_BA+0x2C) + SD_M_BASE(n))
#define TMOUTCTRL(n)		HW_BYTE((SD_BA+0x2E) + SD_M_BASE(n))
#define SWRST(n)			HW_BYTE((SD_BA+0x2F) + SD_M_BASE(n))
#define NORMINTST(n)		HW_WORD((SD_BA+0x30) + SD_M_BASE(n))
#define ERRINTST(n)			HW_WORD((SD_BA+0x32) + SD_M_BASE(n))
#define NORMINTEN(n)		HW_WORD((SD_BA+0x34) + SD_M_BASE(n))
#define ERRINTEN(n)			HW_WORD((SD_BA+0x36) + SD_M_BASE(n))
#define NORMINTSIGEN(n)		HW_WORD((SD_BA+0x38) + SD_M_BASE(n))
#define ERRINTSIGEN(n)		HW_WORD((SD_BA+0x3A) + SD_M_BASE(n))
#define AUTOCMDERRST(n)		HW_WORD((SD_BA+0x3C) + SD_M_BASE(n))
#define HSTCTRL2(n)			HW_WORD((SD_BA+0x3E) + SD_M_BASE(n))
#define FEVAUTOCMDERR(n)	HW_WORD((SD_BA+0x50) + SD_M_BASE(n))
#define FEVERRINT(n)		HW_WORD((SD_BA+0x52) + SD_M_BASE(n))
#define ADMAERRST(n)		HW_BYTE((SD_BA+0x54) + SD_M_BASE(n))
#define BOOTTOCTRL(n)		HW_DWORD((SD_BA+0x70) + SD_M_BASE(n))
#define SLOTINTST(n)		HW_WORD((SD_BA+0xFC) + SD_M_BASE(n))
#define HCVER(n)			HW_DWORD((SD_BA+0xFE) + SD_M_BASE(n))

/*********************	ESPI slave interface module  core registers	************************************/
#define ESPI_BA 0xF009F000
#define	ESPI_4M_BASE(n)	((n)*4)
#define ESPIID				HW_DWORD(ESPI_BA+0x00)
#define ESPICFG				HW_DWORD(ESPI_BA+0x04)
#define ESPISTS				HW_DWORD(ESPI_BA+0x08)
#define ESPIIE				HW_DWORD(ESPI_BA+0x0C)
#define ESPIWE				HW_DWORD(ESPI_BA+0x10)
#define VWREGIDX			HW_DWORD(ESPI_BA+0x14)
#define VWREGDATA			HW_DWORD(ESPI_BA+0x18)
#define OOBRXRDHEAD			HW_DWORD(ESPI_BA+0x1C)
#define OOBRXWRHEAD			HW_DWORD(ESPI_BA+0x20)
//#define OOBCTL				HW_DWORD(ESPI_BA+0x24)
#define FLASHRXRDHEAD		HW_DWORD(ESPI_BA+0x28)
#define FLASHTXWRHEAD		HW_DWORD(ESPI_BA+0x2C)
//#define FLASHCRC			HW_DWORD(ESPI_BA+0x30)
//#define FLASHCFG			HW_DWORD(ESPI_BA+0x34)
//#define FLASHCTL			HW_DWORD(ESPI_BA+0x38)
#define ESPIERR				HW_DWORD(ESPI_BA+0x3C)
#define VWEVMS0				HW_DWORD(ESPI_BA+0x140)
#define VWEVMS1				HW_DWORD(ESPI_BA+0x144)
#define VWEVMS2				HW_DWORD(ESPI_BA+0x148)
#define VWEVSM0				HW_DWORD(ESPI_BA+0x100)
#define VWEVSM1				HW_DWORD(ESPI_BA+0x104)
#define VWEVSM2				HW_DWORD(ESPI_BA+0x108)
#define VWEVMS3				HW_DWORD(ESPI_BA+0x14C)
#define VWEVMS4				HW_DWORD(ESPI_BA+0x150)
#define VWEVMS5				HW_DWORD(ESPI_BA+0x154)
#define VWEVMS6				HW_DWORD(ESPI_BA+0x158)
#define VWEVMS7				HW_DWORD(ESPI_BA+0x15C)
#define VWEVSM3				HW_DWORD(ESPI_BA+0x10C)
#define VWEVSM4				HW_DWORD(ESPI_BA+0x110)
#define VWEVSM5				HW_DWORD(ESPI_BA+0x114)
#define VWPING				HW_DWORD(ESPI_BA+0x2F8)
#define VWCTL				HW_DWORD(ESPI_BA+0x2FC)
#define OOBRXBUF(n)			HW_BYTE((ESPI_BA+0x00) + 0x300 + ESPI_4M_BASE(n))
#define OOBTXBUF(n)			HW_BYTE((ESPI_BA+0x00) + 0x380 + ESPI_4M_BASE(n))
#define OOBCTL				HW_DWORD(ESPI_BA+0x3FC)

#define FLASHRXBUF(n)		HW_BYTE((ESPI_BA+0x00) + 0x400 + ESPI_4M_BASE(n))
#define FLASHTXBUF(n)		HW_BYTE((ESPI_BA+0x00) + 0x480 + ESPI_4M_BASE(n))
#define FLASHBASE			HW_DWORD(ESPI_BA+0x4F0)
#define FLASHCRC			HW_DWORD(ESPI_BA+0x4F4)
#define FLASHCFG			HW_DWORD(ESPI_BA+0x4F8)
#define FLASHCTL			HW_DWORD(ESPI_BA+0x4FC)

/************************  VDM-RX Register *****************************/
#define VDMX_BA 0xE0800000
#define VDMX_IEN			HW_DWORD(VDMX_BA+0x4)
#define VDMX_RXFILT			HW_DWORD(VDMX_BA+0x14)

/*----- Serial IO Expander Interface (SIOX) -----*/
#define SIOXIF0_BA			0xF0101000
#define SIOXIF1_BA			0xF0102000
#define	SIOXIF_M_BASE(n)	((n)*0x1000)
#define	SIOXIF_2M_BASE(n)	((n)*2)
#define	SIOXIF_1M_BASE(n)	((n))
#define XDOUT0(n)			HW_BYTE((SIOXIF0_BA+0x00) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 0
#define XDOUT1(n)			HW_BYTE((SIOXIF0_BA+0x01) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 1
#define XDOUT2(n)			HW_BYTE((SIOXIF0_BA+0x02) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 2
#define XDOUT3(n)			HW_BYTE((SIOXIF0_BA+0x03) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 3
#define XDOUT4(n)			HW_BYTE((SIOXIF0_BA+0x04) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 4
#define XDOUT5(n)			HW_BYTE((SIOXIF0_BA+0x05) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 5
#define XDOUT6(n)			HW_BYTE((SIOXIF0_BA+0x06) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 6
#define XDOUT7(n)			HW_BYTE((SIOXIF0_BA+0x07) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out 7
#define XDIN0(n)			HW_BYTE((SIOXIF0_BA+0x08) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 0
#define XDIN1(n)			HW_BYTE((SIOXIF0_BA+0x09) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 1
#define XDIN2(n)			HW_BYTE((SIOXIF0_BA+0x0A) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 2
#define XDIN3(n)			HW_BYTE((SIOXIF0_BA+0x0B) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 3
#define XDIN4(n)			HW_BYTE((SIOXIF0_BA+0x0C) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 4
#define XDIN5(n)			HW_BYTE((SIOXIF0_BA+0x0D) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 5
#define XDIN6(n)			HW_BYTE((SIOXIF0_BA+0x0E) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 6
#define XDIN7(n)			HW_BYTE((SIOXIF0_BA+0x0F) + SIOXIF_M_BASE(n))	// I/O Expansion Data In 7
#define XEVCFG0(n)			HW_WORD((SIOXIF0_BA+0x10) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 0
#define XEVCFG1(n)			HW_WORD((SIOXIF0_BA+0x12) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 1
#define XEVCFG2(n)			HW_WORD((SIOXIF0_BA+0x14) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 2
#define XEVCFG3(n)			HW_WORD((SIOXIF0_BA+0x16) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 3
#define XEVCFG4(n)			HW_WORD((SIOXIF0_BA+0x18) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 4
#define XEVCFG5(n)			HW_WORD((SIOXIF0_BA+0x1A) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 5
#define XEVCFG6(n)			HW_WORD((SIOXIF0_BA+0x1C) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 6
#define XEVCFG7(n)			HW_WORD((SIOXIF0_BA+0x1E) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration 7
#define XEVSTS0(n)			HW_BYTE((SIOXIF0_BA+0x20) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 0
#define XEVSTS1(n)			HW_BYTE((SIOXIF0_BA+0x21) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 1
#define XEVSTS2(n)			HW_BYTE((SIOXIF0_BA+0x22) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 2
#define XEVSTS3(n)			HW_BYTE((SIOXIF0_BA+0x23) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 3
#define XEVSTS4(n)			HW_BYTE((SIOXIF0_BA+0x24) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 4
#define XEVSTS5(n)			HW_BYTE((SIOXIF0_BA+0x25) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 5
#define XEVSTS6(n)			HW_BYTE((SIOXIF0_BA+0x26) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 6
#define XEVSTS7(n)			HW_BYTE((SIOXIF0_BA+0x27) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status 7
#define IOXCTS(n)			HW_BYTE((SIOXIF0_BA+0x28) + SIOXIF_M_BASE(n))	// I/O Expansion Control and Status
#define IOXINDR(n)			HW_BYTE((SIOXIF0_BA+0x29) + SIOXIF_M_BASE(n))	// IOX Index
#define IOXCFG1(n)			HW_BYTE((SIOXIF0_BA+0x2A) + SIOXIF_M_BASE(n))	// I/O Expansion Configuration 1
#define IOXCFG2(n)			HW_BYTE((SIOXIF0_BA+0x2B) + SIOXIF_M_BASE(n))	// I/O Expansion Configuration 2
#define IOXDATR(n)			HW_BYTE((SIOXIF0_BA+0x2D) + SIOXIF_M_BASE(n))	// IOX Data
#define XGRPLKIEN(n)		HW_BYTE((SIOXIF0_BA+0x2E) + SIOXIF_M_BASE(n))	// Group Lock Interrupt Enable
#define XGRPLKST(n)			HW_BYTE((SIOXIF0_BA+0x2F) + SIOXIF_M_BASE(n))	// Group Lock Interrupt Status
#define XGRPLK0(n)			HW_BYTE((SIOXIF0_BA+0x30) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 0 CFG
#define XGRPLK1(n)			HW_BYTE((SIOXIF0_BA+0x32) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 1 CFG
#define XGRPLK2(n)			HW_BYTE((SIOXIF0_BA+0x34) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 2 CFG
#define XGRPLK3(n)			HW_BYTE((SIOXIF0_BA+0x36) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 3 CFG
#define XGRPLK4(n)			HW_BYTE((SIOXIF0_BA+0x38) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 4 CFG
#define XGRPLK5(n)			HW_BYTE((SIOXIF0_BA+0x3A) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 5 CFG
#define XGRPLK6(n)			HW_BYTE((SIOXIF0_BA+0x3C) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 6 CFG
#define XGRPLK7(n)			HW_BYTE((SIOXIF0_BA+0x3E) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock 7 CFG

#define XDOUT(n,m)			HW_BYTE(SIOXIF0_BA + 0x00 + SIOXIF_1M_BASE(m) + SIOXIF_M_BASE(n))	// I/O Expansion Data Out
#define XDIN(n,m)			HW_BYTE(SIOXIF0_BA + 0x08 + SIOXIF_1M_BASE(m) + SIOXIF_M_BASE(n))	// I/O Expansion Data In
#define XEVCFG(n,m)			HW_WORD(SIOXIF0_BA + 0x10 + SIOXIF_2M_BASE(m) + SIOXIF_M_BASE(n))	// I/O Expansion Event Configuration
#define XEVSTS(n,m)			HW_BYTE(SIOXIF0_BA + 0x20 + SIOXIF_1M_BASE(m) + SIOXIF_M_BASE(n))	// I/O Expansion Event Status
#define XGRPLK(n,m)			HW_WORD(SIOXIF0_BA + 0x30 + SIOXIF_2M_BASE(m) + SIOXIF_M_BASE(n))	// I/O Expansion Group Lock CFG


/*----- ADC -----*/
#define ADC_BA					0xF000C000
#define ADC_CON					HW_DWORD(ADC_BA + 0x00)					// ADC Control
#define ADC_DATA				HW_DWORD(ADC_BA + 0x04)					// ADC Data


/*----- PCI mailbox -----*/
#define PCI_MAILBOX_ID		0x07501050
// Host registers:
#define HMBXSTAT			0x4000								// Host Mailbox status
#define HMBXCTL 			0x4004								// Host Mailbox Control
#define HMBXCMD 			0x4008								// Host Mailbox Command
// Core registers:
#define PCIMBX_BA			0xF0848000
#define BMBXSTAT			HW_DWORD((PCIMBX_BA+0x4000))		// BMC Mailbox status
#define BMBXCTL 			HW_DWORD((PCIMBX_BA+0x4004))		// BMC Mailbox Control
#define BMBXCMD 			HW_DWORD((PCIMBX_BA+0x4008))		// BMC Mailbox Command
// PCI registers (MB Configuration space registers):
#define MB_VENDOR_ID		0
#define MB_DEVICE_ID		2
#define MB_COMMAND_REG		4
#define MB_STATUS_REG		6
#define MB_DEV_REV_ID		8
#define MB_PCI_CLASS_CODE	9
#define MB_PCI_CACHE_LINE	0xC
#define MB_PCI_LATENCY_TMR	0xD
#define MB_PCI_HEADER_TYPE	0xE
#define MB_BAR1				0x10
#define MB_SUB_VENDOR_ID	0x2C
#define MB_CAP_PTR			0x34
#define MB_INT_LINE			0x3C
#define MB_INT_PIN			0x3D
#define MB_MIN_GRANT		0x3E
#define MB_VDIDW			0x44
#define MB_CCW				0x48
#define MB_SIDW				0x4C
#define MB_PM_ID			0x78
#define MB_PM_CSR			0x7C
#define PCI_MAILBOX_CMD_STS MB_COMMAND_REG
#define PCI_MAILBOX_BAR1	MB_BAR1


// GFX PCI registers (GFX Configuration space registers):
#define GFX_VENDOR_ID			0x0
#define GFX_DEVICE_ID			0x2
#define GFX_COMMAND_REG			0x4
#define GFX_STATUS_REG			0x6
#define GFX_DEV_REV_ID			0x8
#define GFX_PCI_CLASS_CODE		0x9
#define GFX_PCI_CACHE_LINE		0xC
#define GFX_PCI_LATENCY_TMR		0xD
#define GFX_PCI_HEADER_TYPE		0xE
#define GFX_BAR2				0x10
#define GFX_BAR1				0x14
#define GFX_SUB_VENDOR_ID		0x2C
#define GFX_ROMBASE				0x30
#define GFX_CAP_PTR				0x34
#define GFX_INT_LINE			0x3C
#define GFX_INT_PIN				0x3D
#define GFX_MIN_GRANT			0x3E
#define GFX_OPT1				0x40
#define GFX_MGA_INDEX			0x44
#define GFX_MGA_DATA			0x48
#define GFX_SIDW				0x4C
#define GFX_OPT2				0x50
#define GFX_PM_ID				0xDC
#define GFX_PM_CSR				0xE0



/*----- SHA1-2 -----*/
#define SHA_BA					0xF085A000
#define	SHA_N_BASE(n)			((n)*4)
#define HASH_DATA_IN			HW_DWORD(SHA_BA + 0x00)					// Data FIFO
#define HASH_CTR_STS			HW_BYTE(SHA_BA + 0x04)					// Control/Status
#define HASH_CFG				HW_BYTE(SHA_BA + 0x08)					// Configuration
#define HASH_VER				HW_BYTE(SHA_BA + 0x0C)					// HASH version
#define HASH_DIG_H0				HW_DWORD(SHA_BA + 0x20)					// Digest of message H0
#define HASH_DIG_H1				HW_DWORD(SHA_BA + 0x24)					// Digest of message H1
#define HASH_DIG_H2				HW_DWORD(SHA_BA + 0x28)					// Digest of message H2
#define HASH_DIG_H3				HW_DWORD(SHA_BA + 0x2C)					// Digest of message H3
#define HASH_DIG_H4				HW_DWORD(SHA_BA + 0x30)					// Digest of message H4
#define HASH_DIG_H5				HW_DWORD(SHA_BA + 0x34)					// Digest of message H5
#define HASH_DIG_H6				HW_DWORD(SHA_BA + 0x38)					// Digest of message H6
#define HASH_DIG_H7				HW_DWORD(SHA_BA + 0x3C)					// Digest of message H7
#define HASH_DIG(n)				HW_DWORD(SHA_BA + 0x20 + SHA_N_BASE(n))	// Digest of message (n)

/*------- AES ---------*/
#define AES_BA						0xF0858000
#define AES_KEY_0					HW_DWORD(AES_BA+0x400)
#define AES_IV_0					HW_DWORD(AES_BA+0x440)
#define AES_CTR0					HW_DWORD(AES_BA+0x460)
#define AES_BUSY					HW_DWORD(AES_BA+0x470)
#define AES_SK						HW_DWORD(AES_BA+0x478)
#define AES_PREV_IV_0				HW_DWORD(AES_BA+0x490)
#define AES_DIN_DOUT				HW_DWORD(AES_BA+0x4A0)
#define AES_CONTROL					HW_DWORD(AES_BA+0x4C0)
#define AES_VERSION					HW_DWORD(AES_BA+0x4C4)
#define AES_HW_FLAGS				HW_DWORD(AES_BA+0x4C8)
#define AES_RGB_SEED				HW_DWORD(AES_BA+0x4D0)
#define AES_DFA_IS_ON				HW_DWORD(AES_BA+0x4F0)
#define AES_SW_RESET				HW_DWORD(AES_BA+0x4F4)
#define AES_DFA_ERROR_STATUS		HW_DWORD(AES_BA+0x4F8)
#define AES_RGB_SEEDING_READY		HW_DWORD(AES_BA+0x4FC)
#define AES_FIFO_DATA				HW_DWORD(AES_BA+0x500)
#define AES_FIFO_STATUS				HW_DWORD(AES_BA+0x600)

/*------- DES ---------*/
#define DES_BA						0xF0859000
#define DES_KEY_0					HW_DWORD(DES_BA+0x208)
#define DES_KEY_1					HW_DWORD(DES_BA+0x20C)
#define DES_KEY_2					HW_DWORD(DES_BA+0x210)
#define DES_KEY_3					HW_DWORD(DES_BA+0x214)
#define DES_KEY_4					HW_DWORD(DES_BA+0x218)
#define DES_KEY_5					HW_DWORD(DES_BA+0x21C)
#define DES_CONTROL_0				HW_DWORD(DES_BA+0x220)
#define DES_CONTROL_1				HW_DWORD(DES_BA+0x224)
#define DES_IV_0					HW_DWORD(DES_BA+0x228)
#define DES_IV_1					HW_DWORD(DES_BA+0x22C)
#define DES_VERSION					HW_DWORD(DES_BA+0x230)
#define DES_DIN_HOST_DATA0			HW_DWORD(DES_BA+0x250)
#define DES_DIN_HOST_DATA1			HW_DWORD(DES_BA+0x254)
#define DES_DOUT_HOST_DATA0			HW_DWORD(DES_BA+0x260)
#define DES_DOUT_HOST_DATA1			HW_DWORD(DES_BA+0x264)
#define DES_BUSY					HW_DWORD(DES_BA+0x270)
#define DES_SW_RESET				HW_DWORD(DES_BA+0x280)
#define DES_FIFO_DATA				HW_DWORD(DES_BA+0x300)
#define DES_FIFO_STATUS				HW_DWORD(DES_BA+0x400)

/*------------- PKA ----------*/
#define PKA_BA						0xF085B000	
#define PKA_CNTR_STS				HW_DWORD(PKA_BA+0x40)
#define PKA_CFG						HW_DWORD(PKA_BA+0x44)
#define MME_J0						HW_DWORD(PKA_BA+0x48)
#define PKA_OPCODE					HW_DWORD(PKA_BA+0x50)
#define PKA_VER						HW_DWORD(PKA_BA+0x14)
//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//					HOST SIDE REGISTERS 															//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define FID 				0x1A

/*----- Logic Device Numbers (LDN) -----*/
#define SP2 				0x02		// Serial port 2
#define SP1 				0x03		// Serial port 1
#define SWC 				0x04		// System Wake-up Control
#define MOUSE				0x05		// Keyboard and Mouse Controller (KBC) - Mouse interface
#define KEYBOARD			0x06		// Keyboard and Mouse Controller (KBC) - Keyboard interface
#define HGPIO				0x07		// Host General-Purpose I/O Ports
#define SHM 				0x0F		// Shared Memory
#define KCS1				0x11		// Keyboard Controller Style I/F Channel 1
#define KCS2				0x12		// Keyboard Controller Style I/F Channel 2
#define KCS3				0x17		// Keyboard Controller Style I/F Channel 3
#define BT					0x18		// Block Transfer Interface

/*----- Configuration Registers -----*/
#define LDN 				0x07		// Logical Device Number select
#define SID_LPC 			0x20		// SuperI/O ID
#define SIOCF1				0x21		// SuperI/O Configuration 1
#define HIDDEN				0x22		// SuperI/O hidden register
#define SIOCF5				0x25		// SuperI/O Configuration 5
#define SIOCF6				0x26		// SuperI/O Configuration 6
#define SRID				0x27		// SuperI/O Revision ID
#define HIDDEN2 			0x2C		// SuperI/O hidden register 2
#define TSTSTRP 			0x2C		// SuperI/O Test straps register
#define SIOMTEST			0x2E		// SuperI/O "M" test control
#define SIOATEST			0x2F		// SuperI/O "A" test control

/*----- Poleg General -----*/
#define ACTIVATE			0x30		// Activate
#define BASE0H				0x60		// I/O Port Base Address Bits (15-8) Descriptor 0
#define BASE0L				0x61		// I/O Port Base Address Bits (7-0)  Descriptor 0
#define BASE1H				0x62		// I/O Port Base Address Bits (15-8) Descriptor 1
#define BASE1L				0x63		// I/O Port Base Address Bits (7-0)  Descriptor 1
#define IRQ 				0x70		// Interrupt Number and Wake-Up on IRQ Enable
#define IRT 				0x71		// Interrupt Request Type Select
#define DMA0				0x74		// DMA Channel Select 0
#define DMA1				0x75		// DMA Channel Select 1

/*----- Poleg Base Address for Host Modules -----*/
#define KBC_DAT_BA			0x0060
#define KBC_CMD_BA			0x0064
#define FDC_BA				0x03f0
#define SP2_BA				0x02f8
#define SP1_BA				0x03f8
#define PP_BA				0x0278
#define SWC_BA				0x0700
#define SHM_HOST_BA 		0x0800
#define HGPIO_BA			0x7000
#define KCS1_BA 			0x8000
#define KCS2_BA 			0x8100
#define KCS3_BA 			0x8200
#define HSIOX_BA			0x8300
#define BT_HOST_BA			0x00E4


/*----- BIOS Post Codes FIFO -----*/
#define BIOS_POST_BA		0xF0007000
#define BPCFA2L 			HW_BYTE((BIOS_POST_BA+0x42))			// BPCF Address 2 LSB
#define BPCFA2M 			HW_BYTE((BIOS_POST_BA+0x44))			// BPCF Address 2 MSB
#define BPCFEN				HW_BYTE((BIOS_POST_BA+0x46))			// BPCF Enable
#define BPCFSTAT			HW_BYTE((BIOS_POST_BA+0x48))			// BPCF Status
#define BPCFDATA			HW_BYTE((BIOS_POST_BA+0x4A))			// BPCF Data
#define BPCFMSTAT			HW_BYTE((BIOS_POST_BA+0x4C))			// BPCF Miscellaneous Status
#define BPCFA1L 			HW_BYTE((BIOS_POST_BA+0x50))			// BPCF Address 1 LSB
#define BPCFA1M 			HW_BYTE((BIOS_POST_BA+0x52))			// BPCF Address 1 MSB

/*----- Universal Asynchronous Receiver Transmitter (UART) -----*/
#define SP_RXD				0x00
#define SP_TXD				0x00
#define SP_IER				0x01
#define SP_EIR				0x02
#define SP_FCR				0x02
#define SP_LCR				0x03
#define SP_BSR				0x03
#define SP_MCR				0x04
#define SP_LSR				0x05
#define SP_MSR				0x06
#define SP_SPR				0x07
#define SP_ASCR 			0x07
#define SP_LBGD_L			0x00
#define SP_LBGD_H			0x01
#define SP_LCR_BSR			0x03
#define SP_BGD_L			0x00
#define SP_BGD_H			0x01
#define SP_EXCR1			0x02
#define SP_EXCR2			0x04
#define SP_TXFLV			0x06
#define SP_RXFLV			0x07
#define SP_MRID 			0x00
#define SP_SH_LCR			0x01
#define SP_SH_FCR			0x02


/*----- Shared Memory (SHM) -----  +++ CORE SIDE +++ */
#define SMHAP0				(SHM_HOST_BA+0x00)
#define SMHAP1				(SHM_HOST_BA+0x01)
#define SMHAP2				(SHM_HOST_BA+0x02)
#define SMHAP3				(SHM_HOST_BA+0x03)
#define SMIMA0				(SHM_HOST_BA+0x04)
#define SMIMA1				(SHM_HOST_BA+0x05)
#define SMIMA2				(SHM_HOST_BA+0x06)
#define SMIMD				(SHM_HOST_BA+0x07)
#define HOST_SHAW1_SEM		(SHM_HOST_BA+0x00)
#define HOST_SHAW2_SEM		(SHM_HOST_BA+0x01)
#define SHM_CFG 			0xF0
#define WIN_CFG 			0xF1
#define SHAW1BA_0			0xF4
#define SHAW1BA_1			0xF5
#define SHAW1BA_2			0xF6
#define SHAW1BA_3			0xF7
#define SHAW2BA_0			0xF8
#define SHAW2BA_1			0xF9
#define SHAW2BA_2			0xFA
#define SHAW2BA_3			0xFB

/*----- Keyboard and Mouse controller -----*/
// Host part:
#define DBBOUT				0x600			// Changed from 60h
#define STATUS				0x604			// Changed from 64h
#define DBBIN				0x600			// Changed from 60h
#define COMMAND 			0x604			// Changed from 64h
// Core part:
#define KBC_BA				0xF0007000
#define HICTRL				HW_BYTE((KBC_BA+0x00))
#define HIIRQC				HW_BYTE((KBC_BA+0x02))
#define HIKMST				HW_BYTE((KBC_BA+0x04))
#define HIKDO				HW_BYTE((KBC_BA+0x06))
#define HIMDO				HW_BYTE((KBC_BA+0x08))
#define HIKMDI				HW_BYTE((KBC_BA+0x0A))
#define HIGLUE				HW_BYTE((KBC_BA+0x4E))


/*----- Keyboard Controller Style (KCS) -----*/
// Host part:
#define KCS_DATA_BA 			0x602			// Changed from 62h
#define KCS_STATUS_BA			0x606			// Changed from 66h
#define	KCS_HOST_SELECT(n)		((n==1) ? 0x00 : (n==2) ? 0x06 : (n==3) ? 0x08 : 0x00)
#define KCSn_DBBOUT(n)			(KCS_DATA_BA + KCS_HOST_SELECT(n))
#define KCSn_STATUS(n)			(KCS_STATUS_BA + KCS_HOST_SELECT(n))
#define KCSn_DBBIN(n)			(KCS_DATA_BA + KCS_HOST_SELECT(n))
#define KCSn_COMMAND(n) 		(KCS_STATUS_BA + KCS_HOST_SELECT(n))


//#define KCSn_DBBOUT(n)		(KCS_DATA_BA+((##n==1) ? 0x00 : (##n==2) ? 0x06 : (##n==3) ? 0x08 : 0x00))
//#define KCSn_STATUS(n)		(KCS_STATUS_BA+((##n==1) ? 0x00 : (##n==2) ? 0x06 : (##n==3) ? 0x08 : 0x00))
//#define KCSn_DBBIN(n)			(KCS_DATA_BA+((##n==1) ? 0x00 : (##n==2) ? 0x06 : (##n==3) ? 0x08 : 0x00))
//#define KCSn_COMMAND(n) 		(KCS_STATUS_BA+((##n==1) ? 0x00 : (##n==2) ? 0x06 : (##n==3) ? 0x08 : 0x00))

 // Core part:
#define KCS_BA				0xF0007000

#define	KCS_M_BASE(n)		((n)*0x12)
#define KCSnST(n)			HW_BYTE((KCS_BA+0x0C) + KCS_M_BASE(n-1))	// KCS Channel n Status
#define KCSnDO(n)			HW_BYTE((KCS_BA+0x0E) + KCS_M_BASE(n-1))	// KCS Channel n Data Out Buffer
#define KCSnDI(n)			HW_BYTE((KCS_BA+0x10) + KCS_M_BASE(n-1))	// KCS Channel n Data In Buffer
#define KCSnDOC(n)			HW_BYTE((KCS_BA+0x12) + KCS_M_BASE(n-1))	// KCS Channel n Data Out Buffer With SCI
#define KCSnDOM(n)			HW_BYTE((KCS_BA+0x14) + KCS_M_BASE(n-1))	// KCS Channel n Data Out Buffer With SMI
#define KCSnDIC(n)			HW_BYTE((KCS_BA+0x16) + KCS_M_BASE(n-1))	// KCS Channel n Data In Buffer With SCI
#define KCSnCTL(n)			HW_BYTE((KCS_BA+0x18) + KCS_M_BASE(n-1))	// KCS Channel n Control
#define KCSnIC(n)			HW_BYTE((KCS_BA+0x1A) + KCS_M_BASE(n-1))	// KCS Channel n Interrupt Control
#define KCSnIE(n)			HW_BYTE((KCS_BA+0x1C) + KCS_M_BASE(n-1))	// KCS Channel n Interrupt Enable

#define KCS1ST   			KCSnST(1)
#define KCS1DO    			KCSnDO(1)
#define KCS1DI    			KCSnDI(1)
#define KCS1DOC   			KCSnDOC(1)
#define KCS1DOM  			KCSnDOM(1)
#define KCS1DIC  			KCSnDIC(1)
#define KCS1CTL  			KCSnCTL(1)
#define KCS1IC  			KCSnIC(1)
#define KCS1IE  			KCSnIE(1)

#define KCS2ST   			KCSnST(2)
#define KCS2DO    			KCSnDO(2)
#define KCS2DI    			KCSnDI(2)
#define KCS2DOC   			KCSnDOC(2)
#define KCS2DOM  			KCSnDOM(2)
#define KCS2DIC  			KCSnDIC(2)
#define KCS2CTL  			KCSnCTL(2)
#define KCS2IC  			KCSnIC(2)
#define KCS2IE  			KCSnIE(2)

#define KCS3ST   			KCSnST(3)
#define KCS3DO    			KCSnDO(3)
#define KCS3DI    			KCSnDI(3)
#define KCS3DOC   			KCSnDOC(3)
#define KCS3DOM  			KCSnDOM(3)
#define KCS3DIC  			KCSnDIC(3)
#define KCS3CTL  			KCSnCTL(3)
#define KCS3IC  			KCSnIC(3)
#define KCS3IE  			KCSnIE(3)

	/*----- Host General Purpose I/O (HGPIO) -----*/
#define HGPSEL				0xF0
#define HGPCFG1 			0xF1
#define HGPEVR				0xF2
#define HGPCFG2 			0xF3
#define HGPDO				(HGPIO_BA+0x00)
#define HGPDI				(HGPIO_BA+0x01)
#define HGPEVEN 			(HGPIO_BA+0x02)
#define HGPEVST 			(HGPIO_BA+0x03)

/*----- Host Serial IO Expander Interface (HSIOX) -----*/
#define HSIOX_XDOUT0			0x00	// I/O Expansion Data Out 0
#define HSIOX_XDOUT1			0x01	// I/O Expansion Data Out 1
#define HSIOX_XDOUT2			0x02	// I/O Expansion Data Out 2
#define HSIOX_XDOUT3			0x03	// I/O Expansion Data Out 3
#define HSIOX_XDOUT4			0x04	// I/O Expansion Data Out 4
#define HSIOX_XDOUT5			0x05	// I/O Expansion Data Out 5
#define HSIOX_XDOUT6			0x06	// I/O Expansion Data Out 6
#define HSIOX_XDOUT7			0x07	// I/O Expansion Data Out 7
#define HSIOX_XDIN0			0x08	// I/O Expansion Data In 0
#define HSIOX_XDIN1			0x09	// I/O Expansion Data In 1
#define HSIOX_XDIN2			0x0A	// I/O Expansion Data In 2
#define HSIOX_XDIN3			0x0B	// I/O Expansion Data In 3
#define HSIOX_XDIN4			0x0C	// I/O Expansion Data In 4
#define HSIOX_XDIN5			0x0D	// I/O Expansion Data In 5
#define HSIOX_XDIN6			0x0E	// I/O Expansion Data In 6
#define HSIOX_XDIN7			0x0F	// I/O Expansion Data In 7
#define HSIOX_XEVCFG0			0x10	// I/O Expansion Event Configuration 0
#define HSIOX_XEVCFG1			0x11	// I/O Expansion Event Configuration 1
#define HSIOX_XEVCFG2			0x12	// I/O Expansion Event Configuration 2
#define HSIOX_XEVCFG3			0x13	// I/O Expansion Event Configuration 3
#define HSIOX_XEVCFG4			0x14	// I/O Expansion Event Configuration 4
#define HSIOX_XEVCFG5			0x15	// I/O Expansion Event Configuration 5
#define HSIOX_XEVCFG6			0x16	// I/O Expansion Event Configuration 6
#define HSIOX_XEVCFG7			0x17	// I/O Expansion Event Configuration 7
#define HSIOX_XEVCFG8			0x18	// I/O Expansion Event Configuration 8
#define HSIOX_XEVCFG9			0x19	// I/O Expansion Event Configuration 9
#define HSIOX_XEVCFGA			0x1A	// I/O Expansion Event Configuration A
#define HSIOX_XEVCFGB			0x1B	// I/O Expansion Event Configuration B
#define HSIOX_XEVCFGC			0x1C	// I/O Expansion Event Configuration C
#define HSIOX_XEVCFGD			0x1D	// I/O Expansion Event Configuration D
#define HSIOX_XEVCFGE			0x1E	// I/O Expansion Event Configuration E
#define HSIOX_XEVCFGF			0x1F	// I/O Expansion Event Configuration F
#define HSIOX_XEVSTS0			0x20	// I/O Expansion Event Status 0
#define HSIOX_XEVSTS1			0x21	// I/O Expansion Event Status 1
#define HSIOX_XEVSTS2			0x22	// I/O Expansion Event Status 2
#define HSIOX_XEVSTS3			0x23	// I/O Expansion Event Status 3
#define HSIOX_XEVSTS4			0x24	// I/O Expansion Event Status 4
#define HSIOX_XEVSTS5			0x25	// I/O Expansion Event Status 5
#define HSIOX_XEVSTS6			0x26	// I/O Expansion Event Status 6
#define HSIOX_XEVSTS7			0x27	// I/O Expansion Event Status 7
#define HSIOX_IOXCTS			0x28	// I/O Expansion Control and Status
#define HSIOX_IOXINDR			0x29	// IOX Index
#define HSIOX_IOXCFG1			0x2A	// I/O Expansion Configuration 1
#define HSIOX_IOXCFG2			0x2B	// I/O Expansion Configuration 2
#define HSIOX_IOXDATR			0x2D	// IOX Data
#define HSIOX_XGRPLKIEN			0x2E	// Group Lock Interrupt Enable
#define HSIOX_XGRPLKST			0x2F	// Group Lock Interrupt Status
#define HSIOX_XGRPLKA0			0x30	// I/O Expansion Group Lock A 0 CFG
#define HSIOX_XGRPLKB0			0x31	// I/O Expansion Group Lock B 0 CFG
#define HSIOX_XGRPLKA1			0x32	// I/O Expansion Group Lock A 1 CFG
#define HSIOX_XGRPLKB1			0x33	// I/O Expansion Group Lock B 1 CFG
#define HSIOX_XGRPLKA2			0x34	// I/O Expansion Group Lock A 2 CFG
#define HSIOX_XGRPLKB2			0x35	// I/O Expansion Group Lock B 2 CFG
#define HSIOX_XGRPLKA3			0x36	// I/O Expansion Group Lock A 3 CFG
#define HSIOX_XGRPLKB3			0x37	// I/O Expansion Group Lock B 3 CFG
#define HSIOX_XGRPLKA4			0x38	// I/O Expansion Group Lock A 4 CFG
#define HSIOX_XGRPLKB4			0x39	// I/O Expansion Group Lock B 4 CFG
#define HSIOX_XGRPLKA5			0x3A	// I/O Expansion Group Lock A 5 CFG
#define HSIOX_XGRPLKB5			0x3B	// I/O Expansion Group Lock B 5 CFG
#define HSIOX_XGRPLKA6			0x3C	// I/O Expansion Group Lock A 6 CFG
#define HSIOX_XGRPLKB6			0x3D	// I/O Expansion Group Lock B 6 CFG
#define HSIOX_XGRPLKA7			0x3E	// I/O Expansion Group Lock A 7 CFG
#define HSIOX_XGRPLKB7			0x3F	// I/O Expansion Group Lock B 7 CFG

#define HSIOX_XDOUT(m)			0x00+##m		// I/O Expansion Data Out
#define HSIOX_XDIN(m)			0x08+##m		// I/O Expansion Data In
#define HSIOX_XEVCFG(m)			0x10+##m		// I/O Expansion Event Configuration
#define HSIOX_XEVSTS(m)			0x20+##m		// I/O Expansion Event Status
#define HSIOX_XGRPLKA(m)		0x30+(##m*2)		// I/O Expansion Group A Lock CFG
#define HSIOX_XGRPLKB(m)		0x31+(##m*2)		// I/O Expansion Group A Lock CFG





/*----- System Wake-Up controller (SWC) -----*/
#define SWC_SCI_STS 			(SWC_BA+0x00)
#define SWC_SCI_EN				(SWC_BA+0x02)
#define SWC_SMI_STS 			(SWC_BA+0x04)
#define SWC_SMI_EN				(SWC_BA+0x06)
#define SWC_PRD_SMI				(SWC_BA+0x07)
	
/*----- Video Capture and Differentiation (VCD) -----*/
#define VCD_BA				0xF0810000
#define DIFF_TBL			(VCD_BA+0x0000)					// Start address of difference table
#define FBA_ADR 			HW_DWORD(VCD_BA+0x8000)			// Frame buffer A address
#define FBB_ADR 			HW_DWORD(VCD_BA+0x8004) 		// Frame buffer B address
#define FB_LP				HW_DWORD(VCD_BA+0x8008) 		// Frame buffers line pitch
#define CAP_RES 			HW_DWORD(VCD_BA+0x800C) 		// Capture resolution
#define DVO_DEL 			HW_DWORD(VCD_BA+0x8010) 		// DVO delay
#define VCD_MODE			HW_DWORD(VCD_BA+0x8014) 		// VCD mode
#define VCD_CMD 			HW_DWORD(VCD_BA+0x8018) 		// VCD command
#define VCD_STAT			HW_DWORD(VCD_BA+0x801C) 		// VCD status
#define VCD_INTE			HW_DWORD(VCD_BA+0x8020) 		// VCD interrupt enable
#define VCD_BSD1 			HW_DWORD(VCD_BA+0x8024) 		// VCD blue screen detect 1
#define VCD_RCHG			HW_DWORD(VCD_BA+0x8028) 		// VCD resolution change detect
#define HOR_CYC_TIM			HW_DWORD(VCD_BA+0x802C) 		// Horizontal Cycle timer
#define HOR_CYC_LST			HW_DWORD(VCD_BA+0x8030) 		// Horizontal Cycle Last
#define HOR_HI_TIM			HW_DWORD(VCD_BA+0x8034) 		// Horizontal High timer
#define HOR_HI_LST			HW_DWORD(VCD_BA+0x8038) 		// Horizontal High Last
#define VER_CYC_TIM			HW_DWORD(VCD_BA+0x803C) 		// Vertical Cycle timer
#define VER_CYC_LST			HW_DWORD(VCD_BA+0x8040) 		// Vertical Cycle Last
#define VER_HI_TIM			HW_DWORD(VCD_BA+0x8044) 		// Vertical High timer
#define VER_HI_LST			HW_DWORD(VCD_BA+0x8048) 		// Vertical High Last
#define HOR_AC_TIM			HW_DWORD(VCD_BA+0x804C) 		// Horizontal active timer
#define HOR_AC_LST			HW_DWORD(VCD_BA+0x8050) 		// Horizontal Active Count Last
#define HOR_LIN_TIM			HW_DWORD(VCD_BA+0x8054) 		// Horizontal Line Timer
#define HOR_LIN_LST			HW_DWORD(VCD_BA+0x8058) 		// Horizontal Line Last
#define VCD_FIFO			HW_DWORD(VCD_BA+0x805C) 		// VCD FIFO setting
#define VCD_DVO_Bypass_CNT	HW_DWORD(VCD_BA+0x8060) 		// VCD DVOBPC
#define VCD_DVO_Bypass_DATA	HW_DWORD(VCD_BA+0x8064) 		// VCD DVOBPD
//#define VCD_MEM_BIST		HW_DWORD(VCD_BA+0x8068) 		// VCD MEMBIST
#define VCD_TEST			HW_DWORD(VCD_BA+0x806C) 		// VCD TEST
#define VCD_DIAG			HW_DWORD(VCD_BA+0x8070) 		// VCD DIAG
#define VCD_BSD2 			HW_DWORD(VCD_BA+0x8074) 		// VCD blue screen detect 2
#define VCD_BSD3 			HW_DWORD(VCD_BA+0x8078) 		// VCD blue screen detect 2

#define VCD_BSDn(n)			HW_DWORD(VCD_BA + ((n == 1) ? 0x8024 : (n == 2) ? 0x8074 :  0x8078) )





/*----- Graphics Core Information Interface (GFXI) -----*/
// All GFXI registers are Read Only
#define	GFXI_BA				0xF000E000
#define	DISPHDST	HW_BYTE(GFXI_BA + 0x0)			//	Display Head Status (DISPHDST)	
#define	FBADL 		HW_BYTE(GFXI_BA + 0x4)			//	Frame Buffer Address Low (FBADL)	
#define	FBADM 		HW_BYTE(GFXI_BA + 0x8)			//	Frame Buffer Address Middle (FBADM)	
#define	FBADH 		HW_BYTE(GFXI_BA + 0xC)			//	Frame Buffer Address High (FBADH)	
#define	HVCNTL		HW_BYTE(GFXI_BA + 0x10)			//	Horizontal Visible Counter Low (HVCNTL)	
#define	HVCNTH		HW_BYTE(GFXI_BA + 0x14)			//	Horizontal Visible Counter High (HVCNTH)	
#define	HBPCNTL		HW_BYTE(GFXI_BA + 0x18)			//	 Horizontal Back-Porch Counter Low (HBPCNTL)	
#define	HBPCNTH		HW_BYTE(GFXI_BA + 0x1C)			//	 Horizontal Back-Porch Counter High (HBPCNTH)	
#define	VVCNTL		HW_BYTE(GFXI_BA + 0x20)			//	Vertical Visible Counter Low (VVCNTL)	
#define	VVCNTH		HW_BYTE(GFXI_BA + 0x24)			//	Vertical Visible Counter High (VVCNTH)	
#define	VBPCNTL		HW_BYTE(GFXI_BA + 0x28)			//	 Vertical Back-Porch Counter Low (VBPCNTL)	
#define	VBPCNTH		HW_BYTE(GFXI_BA + 0x2C)			//	 Vertical Back-Porch Counter High (VBPCNTH)	
#define	CURPOSXL	HW_BYTE(GFXI_BA + 0x30)			//	 Cursor Position X Low (CURPOSXL)	
#define	CURPOSXH	HW_BYTE(GFXI_BA + 0x34)			//	 Cursor Position X High (CURPOSXH)	
#define	CURPOSYL	HW_BYTE(GFXI_BA + 0x38)			//	 Cursor Position Y Low (CURPOSYL)	
#define	CURPOSYH	HW_BYTE(GFXI_BA + 0x3C)			//	 Cursor Position Y High (CURPOSYH)	
#define	GPLLINDI	HW_BYTE(GFXI_BA + 0x40)			//	V  Graphics PLL Input Divider (GPLLINDIV)	
#define	GPLLFBDI	HW_BYTE(GFXI_BA + 0x44)			//	V  Graphics PLL Feedback Divider (GPLLFBDIV)	
#define	GPLLST		HW_BYTE(GFXI_BA + 0x48)			//	Graphics PLL Status (GPLLST)	
#define	KVMHDST		HW_BYTE(GFXI_BA + 0x4C)			//	 KVM Head status	
#define	COLDEP		HW_BYTE(GFXI_BA + 0x50)			//	Color Depth (COLDEP)	
#define	VDISPEND	HW_BYTE(GFXI_BA + 0x54)			//	General Purpose register 0,Vertical Display End Low (VDISPEND)	
#define	VBLANKSTR	HW_BYTE(GFXI_BA + 0x58)			//	General Purpose register 1, Vertical Blank Start Low (VBLANKSTR)	
#define	VBLANKEND	HW_BYTE(GFXI_BA + 0x5C)			//	General Purpose register 2, Vertical Blank End (VBLANKEND)	
#define	VTOTAL		HW_BYTE(GFXI_BA + 0x60)			//	General Purpose register 3, Vertical Total Low (VTOTAL)	
#define	VHIGH		HW_BYTE(GFXI_BA + 0x64)			//	General Purpose register 4, Vertical High (VHIGH)	
#define	HDISPEND	HW_BYTE(GFXI_BA + 0x68)			//	GPR5 RO General Purpose register 5,	Horizontal Display End (HDISPEND) 
#define	HBLANKSTR	HW_BYTE(GFXI_BA + 0x6C)			//	GPR6 RO General Purpose register 6,	Horizontal Blank Start Low (HBLANKSTR) 
#define	HBLANKEND	HW_BYTE(GFXI_BA + 0x70)			//	GPR7 RO General Purpose register 7,	Horizontal Blank End (HBLANKEND) 
#define	HTOTAL		HW_BYTE(GFXI_BA + 0x74)			//	GPR8 RO General Purpose register 8,	Horizontal Total (HTOTAL) 
#define	CURWIDTH	HW_BYTE(GFXI_BA + 0x78)			//	GPR9 RO General Purpose register 9	Cursor Width (CURWIDTH) 
#define	CURHEIGHT	HW_BYTE(GFXI_BA + 0x7C)			//	GPR10 RO General Purpose register 1	0 Cursor Height (CURHEIGHT) 
#define	CURHSXL		HW_BYTE(GFXI_BA + 0x80)			//	GPR11 RO General Purpose register 1	1 Cursor Hot Spot X Position Low (CURHSXL)
#define	GPR12		HW_BYTE(GFXI_BA + 0x84)			//	GPR12 RO General Purpose register 1	2 PLL Reset Counter (PLLRSTCNT) 
#define	GPR13		HW_BYTE(GFXI_BA + 0x88)			//	GPR13 RO General Purpose register 1	3 Cursor Hot Spot High (CURHSH) 
#define	GPR14		HW_BYTE(GFXI_BA + 0x8C)			//	GPR14 RO General Purpose register 1	4 PLL Reset Counter (PLLRSTCNT) 
#define	GPR15		HW_BYTE(GFXI_BA + 0x90)			//	GPR15 RO General Purpose register 1	5 Cursor Hot Spot High (CURHSH)

/*----- Encoding Compression Engine (ECE) -----*/
#define ECE_BA				0xF0820000
#define DDA_CTRL			HW_DWORD(ECE_BA+0x0000)			// Data access control register
#define DDA_STS				HW_DWORD(ECE_BA+0x0004)			// Data access status register
#define FBR_BA				HW_DWORD(ECE_BA+0x0008)			// Base address of data frame buffer
#define ED_BA				HW_DWORD(ECE_BA+0x000C)			// Base address of the encoded data
#define RECT_XY				HW_DWORD(ECE_BA+0x0010)			// Offset of the rec. in the framebuffer. The input data is pushed into FIFO 4 levels depth (MSW)
#define RECT_DIMEN			HW_DWORD(ECE_BA+0x0014)			// Rect. dimensions in the terms of tiles. Input data is pushed into FIFO 4 levels depth (LSW)
#define TILE_SIZE			HW_WORD (ECE_BA+0x0018)			// Size of Tile register
#define RESOL				HW_DWORD(ECE_BA+0x001C)			// Screen resolution
#define INT_RAM_BA			HW_DWORD(ECE_BA+0x0020)			// Internal RAM source/destination base address
#define DTC_CFG				HW_DWORD(ECE_BA+0x0024)			// Data transfer control register
#define HEX_CTRL			HW_DWORD(ECE_BA+0x0040)			// Encoder engine control register
#define HEX_STS				HW_DWORD(ECE_BA+0x0044)			// Encoder engine status register
#define HEX_RECT_OFFSET		HW_DWORD(ECE_BA+0x0048)			// Current offset of the encoded rectangle
#define HEX_RECT_SIZE		HW_DWORD(ECE_BA+0x0104)			// Number of encoded bytes in the rectangle
#define HEX_HEAD1_B1		HW_DWORD(ECE_BA+0x03F8)			// Hextile encoding part of header of buffer 1
#define HEX_HEAD2_B1		HW_DWORD(ECE_BA+0x03FC)			// Hextile encoding part of header of buffer 1
#define HEX_DATA_B1			HW_DWORD(ECE_BA+0x0400)			// Raw/Encoded data RAM buffer 1 (tile 512 bytes)
#define HEX_HEAD1_B2		HW_DWORD(ECE_BA+0x0BF8)			// Hextile encoding part of header of buffer 2
#define HEX_HEAD2_B2		HW_DWORD(ECE_BA+0x0BFC)			// Hextile encoding part of header of buffer 2
#define HEX_DATA_B2			HW_DWORD(ECE_BA+0x0C00)			// Raw/Encoded data RAM buffer 2 (tile 512 bytes)
#define DATA_RECT			HW_DWORD(ECE_BA+0x1400)			// Rectangles of tile RAM buffer (color, x, y, w, h)

/*----- Fuse Arrays Configuration registers -----*/

#define 	OTP1_BA  	0xF0189000  /* Keys Array */ 
#define		FST1		HW_DWORD(OTP1_BA + 0x00) /* Fuse Array 1 Status Register */
#define		FADDR1		HW_DWORD(OTP1_BA + 0x04) /* Fuse Array 1 Address Register */
#define		FDATA1		HW_DWORD(OTP1_BA + 0x08) /* Fuse Array 1 Data Register */
#define 	FCFG1		HW_DWORD(OTP1_BA + 0x0C) /* Fuse Array 1 Configuration Register */
#define 	FKEYIND		HW_DWORD(OTP1_BA + 0x10) /* Fuse Key Index Register */
#define 	FCTL1		HW_DWORD(OTP1_BA + 0x14) /* Fuse Array 1 Control Register */

#define		OTP2_BA		0xF018A000 /* Fuse Array */
#define		FST2		HW_DWORD(OTP2_BA + 0x00) /* Fuse Array 2 Status Register */
#define		FADDR2		HW_DWORD(OTP2_BA + 0x04) /* Fuse Array 2 Address Register */
#define		FDATA2		HW_DWORD(OTP2_BA + 0x08) /* Fuse Array 2 Data Register */
#define 	FCFG2		HW_DWORD(OTP2_BA + 0x0C) /* Fuse Array 2 Configuration Register */
#define 	FUSTRAP		HW_DWORD(OTP2_BA + 0x10) /* Fuse Strap Register */
#define 	FCTL2		HW_DWORD(OTP2_BA + 0x14) /* Fuse Array 2 Control Register */

#define		OTP_BA(n)	(0xF0189000 + 0x1000*(n - 1))
#define		FST(n)		HW_DWORD(OTP_BA(n) + 0x00) /* Fuse Array n Status Register */
#define		FADDR(n)	HW_DWORD(OTP_BA(n) + 0x04) /* Fuse Array n Address Register */
#define		FDATA(n)	HW_DWORD(OTP_BA(n) + 0x08) /* Fuse Array n Data Register */
#define 	FCFG(n)		HW_DWORD(OTP_BA(n) + 0x0C) /* Fuse Array n Configuration Register */
#define 	FCTL(n)		HW_DWORD(OTP_BA(n) + 0x14) /* Fuse Array n Control Register */
#define		AHB2PCI_BA			0xf0400000
#define		WR_PCI_ADDR			HW_DWORD(AHB2PCI_BA+0x0)
#define		WR_AHB_IBYTE		HW_DWORD(AHB2PCI_BA+0x4)
#define		WR_CONTROL			HW_DWORD(AHB2PCI_BA+0x8)
#define		RD_PCI_ADDR			HW_DWORD(AHB2PCI_BA+0x20)
#define		RD_AHB_IBYTE		HW_DWORD(AHB2PCI_BA+0x24)
#define		RD_CONTROL			HW_DWORD(AHB2PCI_BA+0x28)
#define		AHB_IMASK			HW_DWORD(AHB2PCI_BA+0x40)
#define		AHB_ISTATUS			HW_DWORD(AHB2PCI_BA+0x44)
#define		DATA_REG			HW_DWORD(AHB2PCI_BA+0x400)
///////////////////////////////////////////////////////////////////////////////////
///					CO-PROCESSOR (CR16) REGISTERS								///
///////////////////////////////////////////////////////////////////////////////////

/*---- CP Interrupt Control Unit (ICU) -----*/
#define CP_ICU_REGS_BASE 0xFFFE00
#define IVCT         HW_BYTE(CP_ICU_REGS_BASE+0x00) /* Interrupt Vector   RO */
#define EXNMI        HW_BYTE(CP_ICU_REGS_BASE+0x04) /* External NMI Interrupt Control and Status   Varies per bit */
#define NMIMNTR      HW_BYTE(CP_ICU_REGS_BASE+0x06) /* Non-Maskable Interrupt Status Monitor   RO */
#define ISTAT0       HW_WORD(CP_ICU_REGS_BASE+0x0A) /* Interrupt Status 0   RO */
#define ISTAT1       HW_WORD(CP_ICU_REGS_BASE+0x0C) /* Interrupt Status 1   RO */
#define IENAM0       HW_WORD(CP_ICU_REGS_BASE+0x0E) /* Interrupt Enable and Mask 0   R/W */
#define IENAM1       HW_WORD(CP_ICU_REGS_BASE+0x10) /* Interrupt Enable and Mask 1   R/W */
#define IECLR0       HW_WORD(CP_ICU_REGS_BASE+0x12) /* Edge Interrupt Clear 0   WO */
#define IECLR1       HW_WORD(CP_ICU_REGS_BASE+0x14) /* Edge Interrupt Clear 1   WO */
#define IDBG         HW_WORD(CP_ICU_REGS_BASE+0x1A) /* Interrupt Debug   RO */
#define ITEST0       HW_WORD(CP_ICU_REGS_BASE+0x1C) /* Interrupt Test 0   W */
#define ITEST1       HW_WORD(CP_ICU_REGS_BASE+0x1E) /* Interrupt Test 1   W */


/*---- CP Timer and Watchdog (TWD) -----*/
#define CP_TWD_REGS_BA 0xFFF800
#define TWCFG        HW_BYTE(CP_TWD_REGS_BA+0x0) /* Timer and Watchdog Configuration Register   Varies per bit or RUP */
#define TWCP         HW_BYTE(CP_TWD_REGS_BA+0x2) /* Timer and Watchdog Clock Pre-Scaler Register   R/W or RUP */
#define TWDT0        HW_WORD(CP_TWD_REGS_BA+0x4) /* TWD Timer 0 Register   R/W or RUP */
#define T0CSR        HW_BYTE(CP_TWD_REGS_BA+0x6) /* TWDT0 Control and Status Register   Varies per bit */
#define WDCNT        HW_BYTE(CP_TWD_REGS_BA+0x8) /* Watchdog Count Register   WO or Touch*/
#define WDSDM        HW_BYTE(CP_TWD_REGS_BA+0xA) /* Watchdog Service Data Match Register   WO */
#define TWMT0        HW_WORD(CP_TWD_REGS_BA+0xC) /* TWD Timer 0 Internal Counter   RO */
#define TWMT1        HW_BYTE(CP_TWD_REGS_BA+0xE) /* TWD Timer 1 Internal Counter   RO */
#define TWMWD        HW_BYTE(CP_TWD_REGS_BA+0xE) /* Watchdog Internal Counter   RO */
#define WDCP		 	HW_BYTE(CP_TWD_REGS_BA+0x10) /*Watchdog clock Pre-Scaler Register  R/W or RUP */

/*----- CP Control Registers ------------*/
#define CP_CONTROL_REGS_BA 0x00FFF880               
#define	B2CPST0	HW_WORD(CP_CONTROL_REGS_BA+0x00) /* BMC to CP Status Register 0*/
#define B2CPST1	HW_WORD(CP_CONTROL_REGS_BA+0x02) /* BMC to CP Status Register 1*/
#define CP2BNT0	HW_WORD(CP_CONTROL_REGS_BA+0x04) /* CP to BMC Notification Register 0 */
#define CP2BNT1	HW_WORD(CP_CONTROL_REGS_BA+0x06) /* CP to BMC Notification Register 0 */
#define CPSTATR	HW_WORD(CP_CONTROL_REGS_BA+0x08) /* CoProcessor Status Register */
#define CPCFGR	HW_WORD(CP_CONTROL_REGS_BA+0x0A) /* CP Configuration Register */
#define LKREG1	HW_WORD(CP_CONTROL_REGS_BA+0x0C) /* Window Lock Register 1 */
#define LKREG2	HW_WORD(CP_CONTROL_REGS_BA+0x0E) /* Window Lock Register 2 */
#define INTDIR1	HW_WORD(CP_CONTROL_REGS_BA+0x10) /* Interrupt Disable Integration Register 1 */
#define INTDIR2	HW_WORD(CP_CONTROL_REGS_BA+0x12) /* Interrupt Disable Integration Register 2 */

#define SRAMWINC	HW_WORD(CP_CONTROL_REGS_BA+0x14) /* SRAM Access Window Control Register */
#define SPI0WINC	HW_WORD(CP_CONTROL_REGS_BA+0x16) /* SPI0 Access Window Control Register */
#define SPI3WINC	HW_WORD(CP_CONTROL_REGS_BA+0x18) /* SPI3 Access Window Control Register */
#define SPIXWINC	HW_WORD(CP_CONTROL_REGS_BA+0x1A) /* SPIX Access Window Control Register */
#define MISCWINC	HW_WORD(CP_CONTROL_REGS_BA+0x1C) /* Miscellaneous Access Windows Control Register (MISCWINC */

                                                    


/*-----CP Peripheral Access Windows------*/
#define PWIN0C1	HW_WORD(CP_CONTROL_REGS_BA+0x20) /*Peripheral Access Window 0 Control 1 Register*/
#define PWIN0C2 HW_WORD(CP_CONTROL_REGS_BA+0x22) /*Peripheral Access Window 0 Control 2 Register*/
#define PWIN1C1 HW_WORD(CP_CONTROL_REGS_BA+0x24) /*Peripheral Access Window 1 Control 1 Register*/
#define PWIN1C2 HW_WORD(CP_CONTROL_REGS_BA+0x26) /*Peripheral Access Window 1 Control 2 Register*/
#define PWIN2C1 HW_WORD(CP_CONTROL_REGS_BA+0x28) /*Peripheral Access Window 2 Control 1 Register*/
#define PWIN2C2 HW_WORD(CP_CONTROL_REGS_BA+0x2A) /*Peripheral Access Window 2 Control 2 Register*/
#define PWIN3C1 HW_WORD(CP_CONTROL_REGS_BA+0x2C) /*Peripheral Access Window 3 Control 1 Register*/
#define PWIN3C2 HW_WORD(CP_CONTROL_REGS_BA+0x2E) /*Peripheral Access Window 3 Control 2 Register*/
#define PWIN4C1 HW_WORD(CP_CONTROL_REGS_BA+0x30) /*Peripheral Access Window 4 Control 1 Register*/
#define PWIN4C2 HW_WORD(CP_CONTROL_REGS_BA+0x32) /*Peripheral Access Window 4 Control 2 Register*/
#define PWIN5C1 HW_WORD(CP_CONTROL_REGS_BA+0x34) /*Peripheral Access Window 5 Control 1 Register*/
#define PWIN5C2 HW_WORD(CP_CONTROL_REGS_BA+0x36) /*Peripheral Access Window 5 Control 2 Register*/
#define PWIN6C1 HW_WORD(CP_CONTROL_REGS_BA+0x38) /*Peripheral Access Window 6 Control 1 Register*/
#define PWIN6C2 HW_WORD(CP_CONTROL_REGS_BA+0x3A) /*Peripheral Access Window 6 Control 2 Register*/
#define PWIN7C1 HW_WORD(CP_CONTROL_REGS_BA+0x3C) /*Peripheral Access Window 7 Control 1 Register*/
#define PWIN7C2 HW_WORD(CP_CONTROL_REGS_BA+0x3E) /*Peripheral Access Window 7 Control 2 Register*/

/*-----CP System Access Windows------*/

#define SWIN0C1	HW_WORD(CP_CONTROL_REGS_BA+0x40) /*System Access Window 0 Control 1 Register*/
#define SWIN0C2	HW_WORD(CP_CONTROL_REGS_BA+0x42) /*System Access Window 0 Control 2 Register*/
#define SWIN1C1	HW_WORD(CP_CONTROL_REGS_BA+0x44) /*System Access Window 1 Control 1 Register*/
#define SWIN1C2	HW_WORD(CP_CONTROL_REGS_BA+0x46) /*System Access Window 1 Control 2 Register*/
#define SWIN2C1	HW_WORD(CP_CONTROL_REGS_BA+0x48) /*System Access Window 2 Control 1 Register*/
#define SWIN2C2	HW_WORD(CP_CONTROL_REGS_BA+0x4A) /*System Access Window 2 Control 2 Register*/
#define SWIN3C1	HW_WORD(CP_CONTROL_REGS_BA+0x4C) /*System Access Window 3 Control 1 Register*/
#define SWIN3C2	HW_WORD(CP_CONTROL_REGS_BA+0x4E) /*System Access Window 3 Control 2 Register*/
#define SWIN4C1	HW_WORD(CP_CONTROL_REGS_BA+0x50) /*System Access Window 4 Control 1 Register*/
#define SWIN4C2	HW_WORD(CP_CONTROL_REGS_BA+0x52) /*System Access Window 4 Control 2 Register*/

/*----------- Debugger Interface  ---------*/
#define CP_DGB_DEVICE_ID	0x0FE2201F
#define CP_DBG_IF_BA		0xFFF000
#define DBG_DID			HW_DWORD(CP_DBG_IF_BA) /*  DBG_DID is scanned by the JTAG2 interface during the IDCODE instruction */




////////////////////////////////
#endif
