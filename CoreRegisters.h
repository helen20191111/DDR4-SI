/******************************************************************************
    Copyright:   2008 Nuvoton (TA)

    created:    11 7, 2005   15:57
    filename:   CoreRegisters.h
    author:     Avi Fishman

    purpose:    Macros for accessing Core Registers
******************************************************************************/

#ifndef __CoreRegisters_h__
#define __CoreRegisters_h__


#define _BIT(reg,bit)                (1 << bit)
#define HW_BYTE(add)                 (*(( volatile unsigned char *)(add)))
#define HW_WORD(add)                 (*(( volatile unsigned short *)(add)))
#define HW_DWORD(add)                (*(( volatile unsigned long *)(add)))
#define _READ_REG(RegName)           (RegName)
#define _WRITE_REG(RegName,data)     (RegName=data)
#define _SET_BIT_REG(RegName, Bit)   (RegName|=(1<<Bit))
#define _CLEAR_BIT_REG(RegName, Bit) (RegName&=(~(1<<Bit)))
#define _READ_BIT_REG(RegName, Bit)  ((RegName>>Bit)&1)


/******************************************************************************
* Macro: READ_REG()
*
* Purpose:  Read core register regardless of it size (BYTE, WORD or DWORD)
*
* Params:   RegisterName - Core register name as defined in the chip header file
*
* Returns:  The value of the register according to the size of register
*
* Comments: RegisterName must be HW_BYTE(x), HW_WORD(x) or HW_DWORD(x)
*
******************************************************************************/
#define READ_REG(RegisterName) _READ_REG(RegisterName)


/******************************************************************************
* Macro: WRITE_REG()
*
* Purpose:  Write to core register regardless of it size (BYTE, WORD or DWORD)
*
* Params:   RegisterName - Core register name as defined in the chip header file
*           Value - The value to write into the register according its size
*
* Returns:  none
*
* Comments: RegisterName must be HW_BYTE(x), HW_WORD(x) or HW_DWORD(x)
*
******************************************************************************/
#define WRITE_REG(RegisterName, Value) _WRITE_REG(RegisterName, Value)


/******************************************************************************
* Macro: SET_BIT_REG()
*
* Purpose:  Set a bit in core register
*
* Params:   RegisterName - Core register name as defined in the chip header file
*           BitNum - The number of bit to set
*
* Returns:  none
*
* Comments: RegisterName must be HW_BYTE(x), HW_WORD(x) or HW_DWORD(x)
*           This macro is doing WRITE_REG(RegName, READ_REG(RegName)|(1<<Bit))
*
******************************************************************************/
#define SET_BIT_REG(RegisterName, BitNum) _SET_BIT_REG(RegisterName, BitNum)


/******************************************************************************
* Macro: CLEAR_BIT_REG()
*
* Purpose:  Clear a bit in core register
*
* Params:   RegisterName - Core register name as defined in the chip header file
*           BitNum - The number of bit to clear
*
* Returns:  none
*
* Comments: RegisterName must be HW_BYTE(x), HW_WORD(x) or HW_DWORD(x)
*           This macro is doing WRITE_REG(RegName, READ_REG(RegName)&(~(1<<Bit)))
*
******************************************************************************/
#define CLEAR_BIT_REG(RegisterName, BitNum) _CLEAR_BIT_REG(RegisterName, BitNum)


/******************************************************************************
* Macro: READ_BIT_REG()
*
* Purpose:  Read a bit from core register
*
* Params:   RegisterName - Core register name as defined in the chip header file
*           BitNum - The number of bit to read
*
* Returns:  The value of the bit.
*
* Comments: RegisterName must be HW_BYTE(x), HW_WORD(x) or HW_DWORD(x)
*           This macro is doing ((READ_REG(RegName)>>Bit)&1)
*
******************************************************************************/
#define READ_BIT_REG(RegisterName, BitNum) _READ_BIT_REG(RegisterName, BitNum)


#define WRITE_FIELD_REG(RegisterName,width,position,value) WRITE_REG(RegisterName, (READ_REG(RegisterName) & (~((((UINT32)1<<(width))-1)<<(position)))) | ((value)<<(position)))
#define READ_FIELD_REG(address,width,position) 	   ((READ_REG(RegisterName)>>(position)) & ((((UINT32)1)<<(width))-1))

#define	SET_BIT(var, bitNum, data)	(SET_FIELD(var, bitNum, bitNum, data))

// Check if bits are set or cleared
#define BIT_IS_SET(data, bitNum)	((data >> bitNum) & 1)
#define BIT_IS_CLEARED(data, bitNum) (!((data >> bitNum) & 1))

// Creates a mask from a single bit
#define BIT_MASK(bit)					(1 << bit)

// Creates a mask of a range of bits (field)
#define FIELD_MASK(firstBit, lastBit)	((((1 << (lastBit - firstBit + 1)) - 1)) << firstBit)

// Write a variable's specific bits (field) with a value (if the data value is greater than the max value the field can hold, the data is truncated)
#define SET_FIELD(var, firstBit, lastBit, data)	(var = ((var & ~(FIELD_MASK(firstBit, lastBit))) | ((data << firstBit) & FIELD_MASK(firstBit, lastBit))))

// Reads a variable's specific bits (field)
#define GET_FIELD(var, firstBit, lastBit)	((var & FIELD_MASK(firstBit, lastBit)) >> firstBit)

// Read a variable's specific bit
#define GET_BIT(var, bitNum)		(GET_FIELD(var, bitNum, bitNum))

// Write a register's specific bits (field) with a value
#define SET_FIELD_REG(reg, firstBit, lastBit, data)	(WRITE_REG(reg, (READ_REG(reg) & (~(FIELD_MASK(firstBit, lastBit)))) | (data << firstBit)))

// Read specific bits (field) from a register
#define GET_FIELD_REG(reg, firstBit, lastBit)	(GET_FIELD(READ_REG(reg), firstBit, lastBit))

#endif //__CoreRegisters_h__
