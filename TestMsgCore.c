
#include "TestMsgCore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "Poleg.h"
#include "CoreRegisters.h"
#include "Utility.h"

signed int snprintf (char *pString, size_t length, const char *pFormat, ...);

const char pErrorMsg[] = "Error: message length exceed FormattedTextBuffer size. \n\r";
static char FormattedTextBuffer [0x100] __attribute__ ((aligned (4)));  

//-------------------------------------------------------------------
void TestMsgInit (void)
{
	UartInit ();

	// Reset all terminal settings to default.
	//UartSendChar (0x1B);
	//UartSendChar ('c');
}


//-----------------------------------------
static void __log_message (_in const char *pFormat, va_list ap)
{
    int len;

	FormattedTextBuffer[0] = 0; // clear the string

	/* Write formatted string in buffer */
	len = vsnprintf((char*)&FormattedTextBuffer[0], sizeof(FormattedTextBuffer)-1, pFormat, ap); // len is not counting the terminating null character.
	if (len >= (sizeof(FormattedTextBuffer)-1) )
		UartSendStr (pErrorMsg);
	else
		UartSendStr ((char *)&FormattedTextBuffer[0]);

	return;

}
//-------------------------------------------------------------
void LogMessage (_in const char *pFormat, ...)
{
    va_list ap;
    va_start(ap, pFormat);
    __log_message (pFormat, ap);
    va_end(ap);
}
//------------------------------------------------------------
void LogMessageColor (UINT8 ForeColor, _in const char *pFormat, ...)
{
	va_list ap;
	SetColor (ForeColor, COLOR_DEFAULT);
	va_start(ap, pFormat);
	__log_message (pFormat, ap);
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
	va_end(ap);
}
//-------------------------------------------------------------
void LogError (_in const char *pFormat, ...)
{
    va_list ap;
	SetColor (COLOR_RED, COLOR_DEFAULT);
    va_start(ap, pFormat);
    __log_message (pFormat, ap);
    va_end(ap);
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
}
//-------------------------------------------------------------
void LogWarning (_in const char *pFormat, ...)
{
	va_list ap;
	SetColor (COLOR_MAGENTA,COLOR_DEFAULT );
	va_start(ap, pFormat);
	__log_message (pFormat, ap);
	va_end(ap);
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
}
//-------------------------------------------------------------
void LogTitle (_in const char *pFormat, ...)
{
	va_list ap;
	SetColor (COLOR_BLUE, COLOR_DEFAULT);
	UartSendStr ("Title: ");
    va_start(ap, pFormat);
    __log_message (pFormat, ap);
    va_end(ap);
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);

}
//-------------------------------------------------------------
void LogHeader (_in const char *pFormat, ...)
{
    va_list ap;
	SetColor (COLOR_CYAN, COLOR_DEFAULT);
	UartSendStr ("***************************************************\n");
    va_start(ap, pFormat);
    __log_message (pFormat, ap);
    va_end(ap);
	UartSendStr ("\n***************************************************\n");
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
}
//-------------------------------------------------------------
void LogPass (_in const char *pFormat, ...)
{
    va_list ap;
	SetColor (COLOR_GREEN, COLOR_DEFAULT);
    va_start(ap, pFormat);
    __log_message (pFormat, ap);
	SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
    va_end(ap);
}
//-------------------------------------------------------------
// Tested with VT100@ZOC terminal 
void SetColor (UINT8 ForeColor, UINT8 BackColor)
{
	/*
	if (ForeColor == COLOR_DEFAULT)
		ForeColor = COLOR_BLACK;
	if (BackColor == COLOR_DEFAULT)
		BackColor = COLOR_WHITE;
	*/

	snprintf (FormattedTextBuffer,sizeof(FormattedTextBuffer)-1,"%c[%u;%um",0x1B,ForeColor+COLOR_FOREGROUND,BackColor+COLOR_BACKGROUND);
	UartSendStr (FormattedTextBuffer);
}
//-------------------------------------------------------------

//-----------------------------------------------------------------------
extern int GetUserParam_HEX_8BIT (UINT8 *Param, UINT8 IsPreset)
{
	char key_string[5];
	UINT32 NewValue;
	char KeyPress;

	if (IsPreset==TRUE)
		snprintf (key_string,sizeof(key_string),"0x%02X",*Param);
	KeyPress = GetString (key_string,sizeof(key_string), IsPreset);

	if ((KeyPress==ESC_KEY) || (key_string[0]==0/*EMPTY*/))
	{
		LogWarning (" - aborted by user.\n");
		return (-1); 
	}
	if ((sscanf (key_string,"0x%lx",&NewValue) != 1) || (NewValue>0xFF) )
	{
		LogError (" - Invalid format, abort.\n");
		return (-2); 
	}
	*Param = (UINT8)NewValue;
	return (0);
}
//-----------------------------------------------------------------------
extern int GetUserParam_HEX_32BIT (UINT32 *Param, UINT8 IsPreset)
{
	char key_string[11];
	UINT32 NewValue;
	char KeyPress;

	if (IsPreset==TRUE)
		snprintf (key_string,sizeof(key_string),"0x%08lX",*Param);
	KeyPress = GetString (key_string,sizeof(key_string), IsPreset);

	if ((KeyPress==ESC_KEY) || (key_string[0]==0/*EMPTY*/))
	{
		LogWarning (" - aborted by user.\n");
		return (-1); 
	}
	if (sscanf (key_string,"0x%lx",&NewValue) != 1)
	{
		LogError (" - Invalid format, abort.\n");
		return (-2); 
	}
	*Param = (UINT32)NewValue;
	return (0);
}
//-----------------------------------------------------------------------
extern int GetUserParam_INT_32BIT (long int *Param, UINT8 IsPreset)
{
	char key_string[11];
	long int NewValue;
	char KeyPress;

	if (IsPreset==TRUE)
		snprintf (key_string,sizeof(key_string),"%ld",*Param);
	KeyPress = GetString (key_string,sizeof(key_string), IsPreset);

	if ((KeyPress==ESC_KEY) || (key_string[0]==0/*EMPTY*/))
	{
		LogWarning (" - aborted by user.\n");
		return (-1); 
	}
	if (sscanf (key_string,"%ld",&NewValue) != 1)
	{
		LogError (" - Invalid format, abort.\n");
		return (-2); 
	}
	*Param = NewValue;
	return (0);
}
//---------------------------------------------------------------------

extern void Uart_AutoDetect (void)
{
	UINT32 uart_num,WLS_filed;
	UINT32 TimeOut, loop_countdown;
	//---------------------------------
	loop_countdown = 10;
	while (1)
	{
		//-------------------------------
		// Send message to all UARTs ports (0...3)
		for (uart_num=0; uart_num<4; uart_num++)
		{
			WLS_filed = READ_REG(UART_LCR(uart_num)) & 0x03; // Word Length Select
			if (WLS_filed==0x03/*8 bits*/ || WLS_filed==0x02/*7 bits*/)
			{ // if this UART channel is configure to 5 or 6 bits, we assume it is not init. Support only 6 or 7 bits.
				UartSetNum(uart_num);
				LogMessage ("\n Press 'ENTER' to start ... ");
				UartWaitForEmpty ();
			}
		}
		//-------------------------------
		// check for 'ENTER' key from all UARTs ports  
		TimeOut = 500000;
		while (TimeOut!=0)
		{
			for (uart_num=0; uart_num<4; uart_num++)
			{
				WLS_filed = READ_REG(UART_LCR(uart_num)) & 0x03; // Word Length Select
				if (WLS_filed==0x03/*8 bits*/ || WLS_filed==0x02/*7 bits*/)
				{ // if this UART channel is configure to 5 or 6 bits, we assume it is not init. Support only 6 or 7 bits.
					UartSetNum(uart_num);
					if ((KbHit()==TRUE) && (GetChar()==0x0D))
					{
						LogMessage ("Use UART%d ",uart_num);
						return;
					}
				}
			}
			TimeOut--;
		}
		//-------------------------------
		// after ~3 second, configure UART3<-->SI2 and UART0<-->BSP @ 8N1, 115Kb/s; 
		if (loop_countdown==0)
		{
			UartSetNum(3);
			TestMsgInit(); // Init UART3<-->SI2 
			LogMessage ("Configure UART3<-->SI2; Init USRT3 to 8N1 @ 115Kb/s.\n");

			UartSetNum(0); // 17/NOV/2016: added support for UART0 configuration
			TestMsgInit(); // Init UART0<-->BSP 
			LogMessage ("Configure UART0<-->BSP; Init USRT0 to 8N1 @ 115Kb/s.\n");
		}
		loop_countdown--;
		//-------------------------------
	}
}
//---------------------------------------------------------------
// Poleg UART Configuration

// This code assuming ROM CODE (Flash UART Programming) already configure UART3 module via SI2 pins; It use the same UART module and interface pins;
// It /*update baud rate to 115384bps and*/ disable interrupts. 

UINT32 UART_NUM_g;
extern void UartSetNum (int UART_Num)
{
	UART_NUM_g = UART_Num;
}

//-------------------------------------------------------------
extern void UartInit (void)
{
	UINT8 LCR;

	while ((READ_REG(UART_LSR(UART_NUM_g))&0x40) == 0x00); // wait for THRE (Transmitter Holding Register Empty) and TSR (Transmitter Shift Register) to be empty.
	Sleep (10000);

	WRITE_REG (UART_IER(UART_NUM_g),0x00); // disable all interrupts; use polling mode	
	WRITE_REG (UART_FCR(UART_NUM_g),0x07); // reset TX and RX FIFO
	
	
	LCR = 0x03; // 8N1
	LCR |= 0x80; // Set DLAB bit; Accesses the Divisor Latch Registers (DLL, DLM).
	WRITE_REG (UART_LCR(UART_NUM_g), LCR);
	WRITE_REG (UART_DLL(UART_NUM_g), 11); // Baud Rate = UART Clock 24MHz / (16 * (11+2)) = 115384
	WRITE_REG (UART_DLM(UART_NUM_g), 0x00); 
	LCR &= 0x7F; // Clear DLAB bit; Accesses RBR, THR or IER registers.
	WRITE_REG (UART_LCR(UART_NUM_g), LCR);
	
	if (UART_NUM_g==3)
	{
		// connect SI2 to BMC UART3 
		WRITE_REG (SPSWC, (READ_REG(SPSWC)&0xFFFFFFF8) | 0x02); 

		// Set SI2 Mux pins
		SET_BIT_REG (MFSEL1,11);
	}
	else if (UART_NUM_g==0) // BSP
	{
		// Set BSP Mux pins
		SET_BIT_REG (MFSEL1,9);
	}
	
}
//-------------------------------------------------------------
extern void UartSendBuff (const void *BuffOut, UINT32 NumberOfBytesToSend)
{
	UINT8 *pBuff;
	pBuff = (UINT8*) BuffOut;

	while (NumberOfBytesToSend!=0)
	{
		while ((READ_REG(UART_LSR(UART_NUM_g))&0x20) == 0x00); // wait for THRE (Transmitter Holding Register Empty) to be set.
		WRITE_REG (UART_THR(UART_NUM_g), *pBuff);
		pBuff++;
		NumberOfBytesToSend--;
	}
}

//-------------------------------------------------------------	
extern UINT8 UartReviceBuff (void *BuffIn, UINT32 NumberOfBytesToreceive, UINT32 *NumberOfBytesreceived, UINT32 IntervalTimeout, UINT32 TotalTimeout) 
{
	UINT8 *pBuff;
	pBuff = (UINT8*) BuffIn;
	UINT32 l_IntervalTimeout, l_NumberOfBytesreceived;
	UINT8 l_LSR;
	UINT8 status;

	l_IntervalTimeout = IntervalTimeout;
	l_NumberOfBytesreceived = 0;

	while (1)
	{
		if (NumberOfBytesToreceive == 0) // end
		{
			status = 0;
			break;
		}

		l_LSR = READ_REG(UART_LSR(UART_NUM_g));

		if ((l_LSR&0x01) == 0x01) // check RFDR; RxFIFO contains at least one received data word
		{
			*pBuff = READ_REG (UART_THR(UART_NUM_g));
			pBuff++;
			l_IntervalTimeout = IntervalTimeout;
			l_NumberOfBytesreceived++;
			NumberOfBytesToreceive--;
		}

		if ((l_LSR&0x1E) != 0) // OEI (Overrun Error Indicator); PEI (Parity Error Indicator); FEI (Framing Error Indicator); BII (Break Interrupt Indicator)
		{
			status = 1;
			break;
		}

		if (l_IntervalTimeout == 0) // IntervalTimeout
		{
			status = 2;
			break;
		}

		if (TotalTimeout == 0) // TotalTimeout
		{
			status = 3;
			break;
		}

		TotalTimeout--;
		l_IntervalTimeout--;

	}

	if (NumberOfBytesreceived != NULL)
		*NumberOfBytesreceived = l_NumberOfBytesreceived;

	return (status);
}
//-------------------------------------------------------------	

//----------------------------------
// characters and string
//----------------------------------

//----------------------------------
// Output to Console
//----------------------------------

//-------------------------------------
extern void UartSendStr (const char *Str)
{
	while (*Str != 0)
	{
		while ((READ_REG(UART_LSR(UART_NUM_g))&0x20) == 0x00); // wait for THRE (Transmitter Holding Register Empty) to be set.
		WRITE_REG (UART_THR(UART_NUM_g), *Str);
		if (*Str == 0x0A)
		{
			while ((READ_REG(UART_LSR(UART_NUM_g))&0x20) == 0x00); // wait for THRE (Transmitter Holding Register Empty) to be set.
			WRITE_REG (UART_THR(UART_NUM_g), 0x0D);  // add CR and LF
		}
		Str++;
	}
}
//-------------------------------------------------------------

//-------------------------------------------------------------
extern void UartSendChar (const char data)
{
	while ((READ_REG(UART_LSR(UART_NUM_g))&0x20) == 0x00); // wait for THRE (Transmitter Holding Register Empty) to be set.
	WRITE_REG (UART_THR(UART_NUM_g), data);
}
//-------------------------------------------------------------
// use this function to wait until UART send all it's buffer before change clock freq
extern void UartWaitForEmpty (void)
{
	while ((READ_REG(UART_LSR(UART_NUM_g))&0x40) == 0x00); // wait for THRE (Transmitter Holding Register Empty) and TSR (Transmitter Shift Register) to be empty.
	// some delay. notice needed some delay so UartUpdateTool will pass w/o error log
	volatile unsigned int delay;
	for (delay=0;delay<10000;delay++);
}
//----------------------------------
// Input from Console
//----------------------------------

//-----------------------------------
extern int KbHit (void)
{

	if ( (READ_REG(UART_LSR(UART_NUM_g))&0x01) == 0x01) // check RFDR; RxFIFO contains at least one received data word
		return (TRUE); // key was received.
	else
		return (FALSE); // no key is waiting.
}
//------------------------------------
extern char GetChar (void)
{
	char data;

	data = READ_REG (UART_THR(UART_NUM_g));

	return (data);
}
//------------------------------------
// Note: user should use ENTER key to end the string
extern char GetString (char *string, UINT32 MaxStrLen, UINT8 IsPreset) 
{
	char data;
	UINT32 index = 0;

	MaxStrLen--; // keep one space for 0x0 (end of string).

	SetColor (COLOR_YELLOW, COLOR_DEFAULT);

	if (IsPreset==TRUE)
	{
		while (*string!=0)
		{
			UartSendChar (*string);
			string++;
			index++;
		}		
	}
	
	*string = 0; 

	while (1)
	{
		while (KbHit()!=TRUE); // wait for key
		data = GetChar ();
		//-----------------------------
		if (data==0x1B /*ESC*/)  // ESC key or spacial keys
		{
			char tmp = 0; // spacial keys has several char
			UartReviceBuff (&tmp, 1, NULL, 1000, 1000);
			if (tmp=='[')
			{
				UartReviceBuff (&tmp, 1, NULL, 1000, 1000);
				if (tmp=='D') // back 
				{
					if (index!=0) 
					{
						UartSendChar (0x1B); 
						UartSendChar ('['); 
						UartSendChar ('D'); 
						index--;
						string--;	
					}
				}
				if (tmp=='C') // forward 
				{
					if (*string!=0) 
					{
						UartSendChar (0x1B); 
						UartSendChar ('['); 
						UartSendChar ('C'); 
						index++;
						string++;	
					}
				}
			}
			else
			{
				// end the string by press ESC key
				SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
				UartSendStr ("\n");
				return (data);
			}
		}
		//-----------------------------
		else if (data==0x0A || data==0x0D )
		{ // end the string by press ENTER key
			SetColor (COLOR_DEFAULT, COLOR_DEFAULT);
			UartSendStr ("\n");
			return (data);
		}
		//-----------------------------
		else if (data==0x08)  // back
		{
			if (index!=0) 
			{
				UartSendStr ("\b \b");
				index--;
				if (*string==0)
				{
					string--;	
					*string = 0;
				}
				else
				{
					string--;
				}
			}
		}
		//-----------------------------
		else if (index==0 && data==' ')
		{
			// ignore any prefix space 
		}
		//-----------------------------
		else if (index<MaxStrLen) // limit typing when reach to end of buffer
		{
			UartSendChar (data); // ECHO
			index++;
			if (*string==0)
			{
				*string = data;
				string++;
				*string = 0;
			}
			else
			{
				*string = data;
				string++;
			}	
		}
		//-----------------------------
	}
}
//---------------------------------------------------------------------------