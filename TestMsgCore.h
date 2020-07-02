
// TestMsg for Core

#ifndef __TestMsgCore__
#define __TestMsgCore__

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned long  UINT32;

#define _in
#define _out
#define _inout

#define TRUE 1
#define FALSE 0
#define ESC_KEY 27

extern void _startup (void);

#define PostCode(x) WRITE_REG(HW_DWORD(&_startup),x)// this override code test !!!!

// foreground colors
#define COLOR_BLACK		0  
#define COLOR_RED		1 
#define COLOR_GREEN		2 
#define COLOR_YELLOW	3 
#define COLOR_BLUE		4 
#define COLOR_MAGENTA	5 
#define COLOR_CYAN		6
#define COLOR_WHITE		7
#define COLOR_DEFAULT   9

#define COLOR_FOREGROUND 30 
#define COLOR_BACKGROUND 40 


extern void TestMsgInit (void);
extern void LogWarning (_in const char *pFormat, ...);
extern void LogMessage (_in const char *pFormat, ...);
extern void LogMessageColor (UINT8 ForeColor, _in const char *pFormat, ...);
extern void LogError (_in const char *pFormat, ...);
extern void LogTitle (_in const char *pFormat, ...);
extern void LogHeader (_in const char *pFormat, ...);
extern void LogPass (_in const char *pFormat, ...);
extern void SetColor (UINT8 ForeColor, UINT8 BackColor);
extern int GetUserParam_HEX_8BIT (UINT8 *Param, UINT8 IsPreset);
extern int GetUserParam_HEX_32BIT (UINT32 *Param, UINT8 IsPreset);
extern int GetUserParam_INT_32BIT (long int *Param, UINT8 IsPreset);

extern void UartInit (void); // Fixed to UART3 connected to SI2 @ 115K
extern void UartWaitForEmpty (void);
extern void UartSetNum (int UART_Num);
extern void Uart_AutoDetect (void);
extern UINT8 UartReviceBuff (void *BuffIn, UINT32 NumberOfBytesToreceive, UINT32 *NumberOfBytesreceived, UINT32 IntervalTimeout, UINT32 TotalTimeout);
extern void UartSendBuff (const void *BuffOut, UINT32 NumberOfBytesToSend);
extern void UartSendStr (const char *Str);
extern void UartSendChar (const char data);
extern int KbHit (void);
extern char GetChar (void); // check KbHit before using GetChar
extern char GetString (char *string, UINT32 MaxStrLen, UINT8 IsPreset);


#define LOG_ERROR(format) ( LogError("\n***** FAILURE ***** at:\n%s(%d)(%s):\n", __FILE__, __LINE__, __FUNCTION__), (LogError format), LogMessage("\n"))

#endif
