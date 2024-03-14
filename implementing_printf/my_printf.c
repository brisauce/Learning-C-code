/*
*
* My version of printf. currently has support for the following output formats:
*  - %c single-char
*  - %d integer/bool
*  - %% percent symbol
*  - %f single floating point (being reworked) (being put on hold)
*  - %s string literals
*  - %ld long integers
*  - %lu long unsigned int
*  - %x lowercase hexadecimal 
*  - %X uppercase hexadecimal 
*  - %o octal
*  - %lld long long int
*  - %llu long long unsigned int
*
*
* Like the implementation of printf included with stdio.h, this implementation of printf 
* can take more arguments to the format specifier between the % and the letter indicating 
* the data output type. Like the stdio.h printf, the arguments can be in this order:
* 
* %[flags][width][precision][char(s) indicating data output type]
*
* Partially mirroring the original implementation, the flags are:
*      -  Field width left justify.
*      +  Field width right justify.
*      #  Print hexadecimal number with "0x" or "0X" prefix, depending on whether uppercase 
*         or lowercase hexadecimal is specified.
*      0  To indicate filling space not taken up by the printed number with zeroes, when field
*         width is set to left justify
*
* The width and precision arguments are seperated by a period (.)
* A few examples of properly formatted format specifiers are:
*      %d       Integer output with no further arguments.
*      %10.5u   Unsigned integer output with field width argument 10 and precision argument 5.
*      %#5.3x   Lowercase hexadecimal output with hexadecimal prefix flag, 5 field width, 3 precision.
*      %-5%     Percent symbol output with left justify flag and 5 field width.
*      %08o     Octal output with left justifying zeroes flag and 8 field width.
*
*
* There are some cases where some format specifier arguments will be ignored.
*
*      %c, %%, %s     These format specifiers only change outputs from a no argument case
*                     in the case there is a field width or justify argument.
*
*      %x, %X         Only the hexadecimal format specifiers will accept the hex prefix argument.
*                     The program will be halted otherwise.
*
*                     In the case where there is a '0' argument in a format specifier, the rest of
*                     the field for that format specifier will only be filled with zeroes in the
*      %0d, etc       case where there is no precision argument. If there is, the space making
*                     up the difference between either the number of spaces equal to the precision
*                     arument or to the number of spaces needed to display the partcular number will
*                     be filled with zeroes, depending on whichever is larger.
*
* There is a floating point format specifier on the way eventually.
*/

#ifndef SUB_PRINT_FUNCTIONS_H
#include "sub-print_functions.h"
#endif

#ifndef _STDARG_H
#include <stdarg.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDINT_H
#include <stdint.h>
#endif

#ifndef _LIMITS_H
#include <limits.h>
#endif


void enqueue (int * queue, int size, int to_queue);
int dequeue (int * queue, int size);
void printQueue (int * queue, int size);
void clearQueue (int * queue, int size);

const char * GLOBAL_printf_format = NULL;

int my_printf(const char * format, ...)
{
  va_list args;
  va_start(args, format);
  int printed = 0;

  GLOBAL_printf_format = format;

  while (*GLOBAL_printf_format)
  {
    switch (*GLOBAL_printf_format)
    {
      case '%':
        GLOBAL_printf_format ++;
        printed += formatSpecProc(&args);
        break;
      default :
        printed += my_putChar(*GLOBAL_printf_format);
    }
    GLOBAL_printf_format ++;
  }

  done:
  
  va_end(args);
  fflush(stdout);
  return printed;
}

//  my version of puts
 
int my_putString (char * string)
{
  int written_chars = 0;
  while (*string)
  {
    written_chars += my_putChar(*string);
    string ++;
  }
  written_chars += my_putChar('\n');
  return written_chars;
}