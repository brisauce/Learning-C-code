/*
*
* My version of printf. currently has support for the following output formats:
*  - %c   single-char
*  - %d   integer/bool
*  - %%   percent symbol
*  - %f   single floating point (being reworked) (being put on hold)
*  - %s   string literals
*  - %ld  long integers
*  - %lu  long unsigned int
*  - %x   lowercase hexadecimal 
*  - %X   uppercase hexadecimal 
*  - %o   octal
*  - %lld long long int
*  - %llu long long unsigned int
*  - %p   pointer
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




#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "sub-print_functions.h"

void printBadSpec(const char * start, const char * end);
bool isEmptySpec(formatSpec * fmtSpec);
void printFormatSpecs(formatSpec * fmtSpecs, int numSpecs, bool test);


const char * GLOBAL_printf_format = NULL;

#ifndef EMPTY_SPACE
#define EMPTY_SPACE INT_MAX
#endif



#define CLEAR_FMT_SPEC()            \
do                                  \
{                                   \
  fmtSpecs->type = NO_TYPE;         \
  fmtSpecs->start = NULL;           \
  fmtSpecs->precision = 0;          \
  fmtSpecs->length = 0;             \
  fmtSpecs->L_just_0 = false;       \
  fmtSpecs->justify = '\0';         \
  fmtSpecs->hex_prefix = false;     \
  fmtSpecs->field_width = 0;        \
} while (0)
  



int my_printf(const char * format, ...)
{
  va_list args;
  va_start(args, format);
  int printed = 0;
  //  time to implement the format specifier pre-checker
  //  i have read the C implementation at https://codebrowser.dev/glibc/glibc/stdio-common/vfprintf-internal.c.html
  //  and it checks for format specifiers in the string, verifies it is valid,
  //  and if there are no format specifiers it basically turns into puts()

  /*  This will remain as the start of the format string until after the format specifiers are read,
   *  then it will become the iterated format string pointer
   *  jk! :^)
   */
  const char * fmtStart = format;

  #define SPECS_AT_START 10


  formatSpec * fmtSpecs = (formatSpec *) calloc(SPECS_AT_START, sizeof(formatSpec));
  
  if (fmtSpecs == NULL)
  {
    perror("Failed to allocate memory for fmtspecs in my_printf");
    abort();
  }

  formatSpec * fmtSpecStart = fmtSpecs;




  //  TODO: turn this below into a macro for readability  VVV
  int fmtSpecsOverflow = 0;

  #define FMT_SPEC_OFFSET 10

  /*  Loops through format string to find format specifiers*/
  while (*format)
  {
    
    /*  If format specifier symbol is detected... */
    if (*format == '%')
    {
      /*  
      *   ...Loop through and read the format specifier arguments
      *   After you make sure there is space to store their data
      */

      if (fmtSpecs - fmtSpecStart == SPECS_AT_START + fmtSpecsOverflow)
      {          
        fmtSpecsOverflow += FMT_SPEC_OFFSET;
        formatSpec * morefmtSpecs = (formatSpec *) realloc(fmtSpecStart, (SPECS_AT_START + fmtSpecsOverflow) * sizeof(formatSpec));
        if (morefmtSpecs == NULL)
        {
          free(fmtSpecs);
          perror("Failed to realloc format specifier struct array");
          abort();
        }
        fmtSpecStart = morefmtSpecs;

        formatSpec * fmtSpecLimit = fmtSpecStart + (SPECS_AT_START + fmtSpecsOverflow);
        fmtSpecs = fmtSpecLimit - FMT_SPEC_OFFSET;
        while (fmtSpecs < fmtSpecLimit)
        {
          CLEAR_FMT_SPEC();
          fmtSpecs ++;
        }
        fmtSpecs = fmtSpecLimit - FMT_SPEC_OFFSET;
      }

      fmtSpecs->start = format;

      format ++;

      bool precision_flag = false;

      /*Loop through format specifiers to find arguments*/
      while (*format)
      {
        if (*format == '%')
        {
          fmtSpecs->type = PERCENT_SYMBOL;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'c')
        {
          fmtSpecs->type = CHAR_TYPE;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'd')
        {
          fmtSpecs->type = INT_TYPE;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        /*
        else if (*format == 'f' )
          To be built eventually!
        */
        else if (*format == 's')
        {
          fmtSpecs->type = STRING_TYPE;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'u')
        {
          fmtSpecs->type = UNSIGNED_INT_TYPE;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'x')
        {
          fmtSpecs->type = HEXADECIMAL_LOWER;
          fmtSpecs->length = (format - fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'X')
        {
          fmtSpecs->type = HEXADECIMAL_UPPER;
          fmtSpecs->length = format - (fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'o')
        {
          fmtSpecs->type = OCTAL;
          fmtSpecs->length = format - (fmtSpecs->start) + 1;
          fmtSpecs ++;
          break;
        }
        else if (*format == 'p')
        {
          fmtSpecs->type = POINTER_TYPE;
          fmtSpecs->length = format - (fmtSpecs->start) + 1;
          fmtSpecs ++;
        }
        
        /*  add more cases here ^^^ */
        else if (*format == 'l') /*  "long" types*/
        {
          format ++;
          if (*format == 'd')
          {
            fmtSpecs->type = LONG_INT_TYPE;
            fmtSpecs->length = format - (fmtSpecs->start) + 1;
            fmtSpecs ++;
            break;
          }
          else if(*format == 'u')
          {
            fmtSpecs->type = UNSIGNED_LONG_TYPE;
            fmtSpecs->length = format - (fmtSpecs->start) + 1;
            fmtSpecs ++;
            break;
          }
          else if (*format == 'l')
          {
            /*  "long long" types*/
            format ++;
            if (*format == 'd')
            {
              fmtSpecs->type = LONG_LONG_INT_TYPE;
              fmtSpecs->length = format - (fmtSpecs->start) + 1;
              fmtSpecs ++;
              break;
            }
            else if(*format == 'u')
            {
              fmtSpecs->type = UNSIGNED_LONG_LONG_TYPE;
              fmtSpecs->length = format - (fmtSpecs->start) + 1;
              fmtSpecs ++;
              break;
            }
            else
            {
              /*  no format specifier identified*/
              goto noValidSpec;
            }
          }
          else
          {
            /*  no format specifier identified*/
            goto noValidSpec;
          }
        }
        /*  format specifier flags*/
        else if (*format == '-')
        {

          if (precision_flag || fmtSpecs->field_width)
          {
            goto noValidSpec;
          }

          fmtSpecs->justify = '-';

        }
        else if (*format == '+')
        {
          if (precision_flag || fmtSpecs->field_width)
          {
            goto noValidSpec;
          }

          fmtSpecs->justify = '+';

        }
        else if (*format == '#')
        {
          if (precision_flag || fmtSpecs->field_width)
          {
            goto noValidSpec;
          }

          fmtSpecs->hex_prefix = true;
        }
        else if (*format == '0')
        {

          if (fmtSpecs->field_width || precision_flag)
          {
            goto number_case;
          }
          
          fmtSpecs->L_just_0 = true;
        }
        else if (*format == '.')
        {
          if(precision_flag)
          {
            goto noValidSpec;
          }

          precision_flag = true;
        }
        else if (*format - '0' < 10 && *format - '0' > 0 /* *format is a numerical character */)
        {
          number_case:
          if (precision_flag == false)
          {
            fmtSpecs->field_width = readInt(&format);
          }
          else
          {
            fmtSpecs->precision = readInt(&format);
          }
          /*  have to do this because readint will make format point to the next non digit character,
              which will make the function parse the string incorrectly
          */
          format --;
        }
        else
        {
          /*  No valid format specifier character detected.*/
          noValidSpec:

          CLEAR_FMT_SPEC();

          break;
        }
        
        format ++;
      }

      if (fmtSpecs->start && !fmtSpecs->length)
      {
        CLEAR_FMT_SPEC();
      }
      

    }

    /*  No format specifier symbol*/

    format ++;
  }
  
  

  //  TODO: turn above into a macro ^^

  /*  re-orient the pointers to the starting points of the format string and format spec data structure  */

  format = fmtStart;
  fmtSpecs = fmtSpecStart;

  //printFormatSpecs(fmtSpecs, SPECS_AT_START + fmtSpecsOverflow);

  /*  if there are no format specifiers, print the string */

  if (fmtSpecs->start == NULL)
  {
    printed += my_stringPrint(format, fmtSpecs);
    goto done;
  }

  while (*format)
  {
    if (format == fmtSpecs->start)
    {
      //  TODO: TURN THIS INTO A MACRO FOR READABILITY VVV
      switch (fmtSpecs->type)
      {
      case PERCENT_SYMBOL :
        printed += my_printChar('%', fmtSpecs);
        break;
      case CHAR_TYPE :
        printed += my_printChar((char) va_arg(args, int), fmtSpecs);
        break;
      case INT_TYPE :
        printed += my_putInt(va_arg(args, int), fmtSpecs);
        break;
      /*
      case 'f' :
        printed += my_putFloat((float) va_arg(*args, double));
        break;
      */
      case STRING_TYPE :
        printed += my_stringPrint(va_arg(args, char *), fmtSpecs);
        break;
      case UNSIGNED_INT_TYPE :
        printed += my_putUInt(va_arg(args, unsigned int), fmtSpecs);
        break;
      case HEXADECIMAL_LOWER :
        printed += my_putUInt(va_arg(args, unsigned int), fmtSpecs);
        break;
      case HEXADECIMAL_UPPER :
        printed += my_putUInt(va_arg(args, unsigned int), fmtSpecs);
        break;
      case OCTAL :
        printed += my_putUInt(va_arg(args, unsigned int), fmtSpecs);
        break;
      case LONG_INT_TYPE :
        printed += my_putInt(va_arg(args, long int), fmtSpecs);
        break;
      case UNSIGNED_LONG_TYPE :
        printed += my_putInt(va_arg(args, unsigned long int), fmtSpecs);
        break;
      case LONG_LONG_INT_TYPE :
        printed += my_putInt(va_arg(args, long long int), fmtSpecs);
        break;
      case UNSIGNED_LONG_LONG_TYPE :
        printed += my_putUInt(va_arg(args, unsigned long long), fmtSpecs);
        break;
      case POINTER_TYPE :
        printed += my_putUInt((unsigned long long) va_arg(args, void *), fmtSpecs);
        break;
      default:
        printf("new type added incompletely or bad type information passed. Type reference number: %d", fmtSpecs->type);
        free(fmtSpecStart);
        abort();
        break;
      }
      //  TODO: TURN THIS INTO A MACRO FOR READABILITY ^^^
      format += fmtSpecs->length;
      fmtSpecs ++;
    }
    else
    {
      printed += my_putChar(*format);
      format ++;
    }
  }
  

  done:


  free(fmtSpecStart);
  va_end(args);
  fflush(stdout);
  return printed;
}

void printFormatSpecs(formatSpec * fmtSpecs, int numSpecs, bool test)
{
  for (int i = 0; i < numSpecs; i++)
  {
    printf("fmtSpecs #%d contains:\nstart: %p\nType: ", i + 1, fmtSpecs->start);

    switch (fmtSpecs->type)
    {
    case NO_TYPE:
      printf("no type");
      break;
    case INT_TYPE:
      printf("Int");
      break;
    case LONG_INT_TYPE:
      printf("Long int");
      break;
    case UNSIGNED_INT_TYPE:
      printf("Unsigned int");
      break;
    case HEXADECIMAL_LOWER:
      printf("Hexadecimal, lowercase");
      break;
    case HEXADECIMAL_UPPER:
      printf("Hexadecimal, uppercase");
      break;
    case OCTAL:
      printf("Octal");
      break;
    case UNSIGNED_LONG_TYPE:
      printf("Unsigned long");
      break;
    case UNSIGNED_LONG_LONG_TYPE:
      printf("Unsigned long long");
      break;
    case LONG_LONG_INT_TYPE:
      printf("Long long int");
      break;
    case PERCENT_SYMBOL:
      printf("Percent symbol");
      break;
    case CHAR_TYPE:
      printf("Char");
      break;
    case STRING_TYPE:
      printf("String");
      break;
    case POINTER_TYPE:
      printf("Pointer");
      break;
    default:
      printf("no valid type identified");
    }

    if (!test) /*it's only this way because i dont want to swap the code in the if/else around ¯\_(ツ)_/¯*/
    {
      printf("\nSpecifier Literal: \"");

      for (unsigned int i = 0; i < fmtSpecs->length; i++)
      {
        printf("%c%s", fmtSpecs->start[i], (i == fmtSpecs->length - 1) ? "\"" : "" );
      }
    }
    else
    {
      printf("\nLength: %d", fmtSpecs->length);
    }
    
    printf("\nField Width: %d\nPrecision: %d\nHex Prefix: %s\nLeft Justify Zeroes: %s\nJustify: %c\n\n", 
      fmtSpecs->field_width, 
      fmtSpecs->precision, 
      (fmtSpecs->hex_prefix) ? "True" : "False",
      (fmtSpecs->L_just_0) ? "True" : "False",
      (fmtSpecs->justify)? fmtSpecs->justify : '+');
    fmtSpecs ++;
  }
}

bool isEmptySpec(formatSpec * fmtSpec)
{
  return fmtSpec->start == NULL && fmtSpec->type == NO_TYPE && fmtSpec->precision == 0 && fmtSpec->length == 0 && fmtSpec->L_just_0 == false && fmtSpec->justify == '\0' && fmtSpec->hex_prefix == false && fmtSpec->field_width == 0;
}

void printBadSpec(const char * start, const char * end)
{
  const char * startSpec = start;
  putc('\"', stdout);
  while (startSpec < end || *startSpec)
  {
    putc(*startSpec, stdout);
    startSpec ++;
  }
  puts("\"");

  int distance = (end - start) + 1;

  for (int i = 0; i < distance; i++)
  {
    putc(' ', stdout);
  }
  puts("^");
  
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