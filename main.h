#ifndef __main_h__
#define __main_h__

extern int g_SkipDataAbort;
void Check_Clocks (void);
UINT32 Check_BMC_PLL (DWORD PllCon, DWORD PllIndex);


#define IOW32(addr,data) WRITE_REG(HW_DWORD(addr),data)
#define IOR32(addr)      READ_REG(HW_DWORD(addr))
#define REG_SET_FIELD(address,width,position,value) IOW32(address, (IOR32(address) & (~((((UINT32)1<<(width))-1)<<(position))))  | ((value)<<(position)))
#define REG_READ_FIELD(address,width,position) 	   ((IOR32(address)>>(position)) & ((((UINT32)1)<<(width))-1))


#endif