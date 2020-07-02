/**************************************************************************//*****
 * @file     printf.c
 * @brief    Implementation of several stdio.h methods, such as printf(),
 *           sprintf() and so on. This reduces the memory footprint of the
 *           binary when using those methods, compared to the libc implementation.
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


/**
 * @brief  Writes a character inside the given string. Returns 1.
 *
 * @param  pStr	Storage string.
 * @param  c    Character to write.
 */
static signed int PutChar(char *pStr, char c)
{
    *pStr = c;
    return 1;
}


/**
 * @brief  Writes a string inside the given string.
 *
 * @param  pStr     Storage string.
 * @param  pSource  Source string.
 * @return  The size of the written
 */
static signed int PutString (char *pStr, const char *pSource)
{
    signed int num = 0;

    while (*pSource != 0)
    {
        *pStr++ = *pSource++;
        num++;
    }
    return num;
}


/**
 * @brief  Writes an unsigned int inside the given string, using the provided fill &
 *         width parameters.
 *
 * @param  pStr  Storage string.
 * @param  fill  Fill character.
 * @param  width  Minimum integer width.
 * @param  value  Integer value.
 */
static signed int PutUnsignedInt(char *pStr, char fill, signed int width, unsigned long int value)
{
    signed int num = 0;

    /* Take current digit into account when calculating width */
    width--;

    /* Recursively write upper digits */
    if ((value / 10) > 0)
    {
        num = PutUnsignedInt (pStr, fill, width, value / 10);
        pStr += num;
    }

    /* Write filler characters */
    else
    {
        while (width > 0)
        {
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }

    /* Write lower digit */
    num += PutChar(pStr, (value % 10) + '0');

    return num;
}


/**
 * @brief  Writes a signed int inside the given string, using the provided fill & width
 *         parameters.
 *
 * @param pStr   Storage string.
 * @param fill   Fill character.
 * @param width  Minimum integer width.
 * @param value  Signed integer value.
 */
static signed int PutSignedInt(char *pStr, char fill, signed int width, signed long int value)
{
    signed int num = 0;
    unsigned long int absolute;

    /* Compute absolute value */
    if (value < 0)
        absolute = -value;
    else
        absolute = value;

    /* Take current digit into account when calculating width */
    width--;

    /* Recursively write upper digits */
    if ((absolute / 10) > 0)
    {
        if (value < 0)
        {
            num = PutSignedInt(pStr, fill, width, -(absolute / 10));
        }
        else
        {
            num = PutSignedInt(pStr, fill, width, absolute / 10);
        }
        pStr += num;
    }
    else
    {

        /* Reserve space for sign */
        if (value < 0)
        {
            width--;
        }

        /* Write filler characters */
        while (width > 0)
        {
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }

        /* Write sign */
        if (value < 0)
        {
            num += PutChar(pStr, '-');
            pStr++;
        }
    }

    /* Write lower digit */
    num += PutChar(pStr, (absolute % 10) + '0');

    return num;
}


/**
 * @brief  Writes an hexadecimal value into a string, using the given fill, width &
 *         capital parameters.
 *
 * @param pStr   Storage string.
 * @param fill   Fill character.
 * @param width  Minimum integer width.
 * @param maj    Indicates if the letters must be printed in lower- or upper-case.
 * @param value  Hexadecimal value.
 *
 * @return  The number of char written
 */
static signed int PutHexa (char *pStr, char fill, signed int width, unsigned char maj, unsigned long int value)
{
    signed int num = 0;

    /* Decrement width */
    width--;

    /* Recursively output upper digits */
    if ((value >> 4) > 0)
    {

        num += PutHexa(pStr, fill, width, maj, value >> 4);
        pStr += num;
    }
    /* Write filler chars */
    else
    {

        while (width > 0)
        {
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }

    /* Write current digit */
    if ((value & 0xF) < 10)
    {
        PutChar(pStr, (value & 0xF) + '0');
    }
    else if (maj)
    {
        PutChar(pStr, (value & 0xF) - 10 + 'A');
    }
    else
    {
        PutChar(pStr, (value & 0xF) - 10 + 'a');
    }
    num++;

    return num;
}



/* Global Functions ----------------------------------------------------------- */


/**
 *  Write formatted data from variable argument list to sized buffer
 *
 * @brief  Stores the result of a formatted string into another string.
 *         Format arguments are given in a va_list instance.
 *
 * @param pStr    Destination string.
 * @param length  Length of Destination string.
 * @param pFormat Format string.
 * @param ap      Argument list.
 *
 * @return  The number of characters written.
 */
signed int vsnprintf (char *pStr, size_t length, const char *pFormat, va_list ap)
{
    char          fill;
    unsigned char width;
    signed int    num = 0;
    signed int    size = 0;
    unsigned char Size_Prefixes = 0;

    if (pStr==NULL)
    	return 0;

    /* Clear the string */
    *pStr = 0;

    /* Phase string */
    while (*pFormat != 0 && size < length)
    {
        /* Normal character */
        if (*pFormat != '%')
        {
            *pStr++ = *pFormat++;
            size++;
        }
        /* Escaped '%' */
        else if (*(pFormat+1) == '%')
        {
            *pStr++ = '%';
            pFormat += 2;
            size++;
        }
        /* Token delimiter */
        else
        {
            fill = ' ';
            width = 0;
            pFormat++;

            /* Parse filler */
            if (*pFormat == '0')
            {
                fill = '0';
                pFormat++;
            }

            /* Parse width */
            while ((*pFormat >= '0') && (*pFormat <= '9'))
            {

                width = (width*10) + *pFormat-'0';
                pFormat++;
            }

            /* Check if there is enough space */
            if (size + width > length)
            {
                width = length - size;
            }

            // check Size Prefixes (long int) (goes with d, i,u,x and X type)
            if (*pFormat == 'l')
            {
            	Size_Prefixes = 1;
            	pFormat++;
            }


            /* Parse type */
            switch (*pFormat)
            {
				case 'd':
				case 'i':
						if (Size_Prefixes==1)
							num = PutSignedInt(pStr, fill, width, va_arg(ap, signed long int));
						else
							num = PutSignedInt(pStr, fill, width, va_arg(ap, signed int));
						break;
				case 'u':
					if (Size_Prefixes==1)
						num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned long int));
					else
						num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned int));
					break;
				case 'x':
					if (Size_Prefixes==1)
						num = PutHexa(pStr, fill, width, 0, va_arg(ap, unsigned long int));
					else
						num = PutHexa(pStr, fill, width, 0, va_arg(ap, unsigned int));
					break;
				case 'X':
					if (Size_Prefixes==1)
						num = PutHexa(pStr, fill, width, 1, va_arg(ap, unsigned long int));
					else
						num = PutHexa(pStr, fill, width, 1, va_arg(ap, unsigned int));
					break;
				case 's': num = PutString(pStr, va_arg(ap, char *)); break;
				case 'c': num = PutChar(pStr, va_arg(ap, unsigned int)); break;
				default:
					return EOF;
            }

            pFormat++;
            pStr += num;
            size += num;
        }
    }

    /* NULL-terminated (final \0 is not counted) */
    if (size < length)
    {
        *pStr = 0;
    }
    else
    {
        *(--pStr) = 0;
        size--;
    }

    return size;
}


/**
 * @brief  Stores the result of a formatted string into another string.
 *         Format arguments are given in a va_list instance.
 *
 * @param pStr    Destination string.
 * @param length  Length of Destination string.
 * @param pFormat Format string.
 * @param ...     Other arguments
 *
 * @return  The number of characters written.
 */
signed int snprintf (char *pString, size_t length, const char *pFormat, ...)
{
    va_list    ap;
    signed int rc;

    va_start(ap, pFormat);
    rc = vsnprintf(pString, length, pFormat, ap);
    va_end(ap);

    return rc;
}


signed int vsprintf(char *pString, const char *pFormat, va_list ap)
{
	return (0);
}
signed int sprintf(char *pStr, const char *pFormat, ...)
{
	return (0);
}
signed int vprintf(const char *pFormat, va_list ap)
{
	return (0);
}
signed int vfprintf(FILE *pStream, const char *pFormat, va_list ap)
{
	return (0);
}
signed int fprintf(FILE *pStream, const char *pFormat, ...)
{
	return 0;
}
signed int printf(const char *pFormat, ...)
{
	return (0);
}
signed int puts(const char *pStr)
{
   return (0);
}
signed int fputc(signed int c, FILE *pStream)
{
	 return EOF;
}
signed int fputs(const char *pStr, FILE *pStream)
{
	return EOF;
}


// Get string Length
size_t strlen (const char *pStr)
{
	size_t num = 0;
	while (*pStr != 0)
	{
		num++;
		if (num <= 0)
			return (EOF); // error, string is longer then 32767
		pStr++;
	}
	return (num);
}



/* --------------------------------- End Of File ------------------------------ */
