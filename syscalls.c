/**************************************************************************//*****
 * @file     stdio.c
 * @brief    Implementation of newlib syscall
 ********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "TestMsgCore.h"

#undef errno
extern int errno;
extern int  end; // from linker; end of the stack

extern void _exit (int status);

__attribute__ ((used))
caddr_t _sbrk ( int incr )
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

    if (heap == NULL)
    {
        heap = (unsigned char *)&end;
    }
    prev_heap = heap;
    heap += incr;
    return (caddr_t) prev_heap;
}

__attribute__ ((used))
int link(char *old, char *new)
{
    return -1;
}

__attribute__ ((used))
int _close(int file)
{
  return -1;
}

__attribute__ ((used))
int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

__attribute__ ((used))
int _isatty(int file)
{
  return 1;
}

__attribute__ ((used))
int _lseek(int file, int ptr, int dir)
{
  return 0;
}
__attribute__ ((used))
int _read(int file, char *ptr, int len)
{
  return 0;
}
__attribute__ ((used))
int _write(int file, char *ptr, int len)
{
  return len;
}

__attribute__ ((used))
void abort (void)
{
	LogMessage ("\n\n");
	LogError ("***************\n");
	LogError ("   Test abort  \n");
	LogError ("*************** \n");
	
	_exit (-1);
	//__asm__ ("BKPT 0x0000"); 
	while(1);
}

__attribute__ ((used))
void exit (int status)
{
  /* Abort called */
	if (status!=0)
	{
		LogMessage ("\n\n");
		LogError ("**********************************\n");
		LogError ("   Test exit with error num %d     \n",status);
		LogError ("**********************************\n");
	}
	else
	{
		 LogMessage ("\n\n");
		 LogPass ("**********************************\n");
		 LogPass ("  Test exit with no errors        \n");
		 LogPass ("**********************************\n"); 
	}

	//__asm__ ("BKPT 0x0000"); 
	_exit (status);
	while(1);
}
/* --------------------------------- End Of File ------------------------------ */
