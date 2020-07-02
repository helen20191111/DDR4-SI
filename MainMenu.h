#ifndef __MainMenu_h___
#define __MainMenu_h___



extern char g_KeyPress;
extern int g_SkipDataAbort;
extern UINT32 g_TlbsOriginAddr;
extern int g_IsMMUenable;

extern const char g_Text_AbortedByUser[];
extern const char g_Text_InvalidFormat[];

#define ESC_KEY 27

BOOL DisplayInfo (void);
int CheckForAbort (void);
BOOL MainMenu (void);
void MPR_Readout_RawData (UINT8 Page, UINT16 *RawData);
int WriteModeReg (UINT8 Index, UINT32 Data);
#endif

