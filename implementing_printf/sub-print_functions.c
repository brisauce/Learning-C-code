#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

/*  Flags used by internal printf functions to identify format specifier data*/

#define NO_TYPE 0
#define INT_TYPE 1
#define LONG_INT_TYPE 2
#define UNSIGNED_INT_TYPE 3
#define HEXADECIMAL_LOWER 4
#define HEXADECIMAL_UPPER 5
#define OCTAL 6
#define UNSIGNED_LONG_TYPE 7
#define UNSIGNED_LONG_LONG_TYPE 8
#define LONG_LONG_INT_TYPE 9
#define PERCENT_SYMBOL 10
#define CHAR_TYPE 11
#define STRING_TYPE 12
#define POINTER_TYPE 13


typedef struct 
{
  const char * start;
  unsigned int length;
  int type;
  int field_width;
  int precision;
  bool hex_prefix;
  char justify;
  bool L_just_0;
}formatSpec;


int my_printChar(char to_print, formatSpec * fmtSpec);
int my_putChar (char print_me);
int my_putInt(long long int print_me, formatSpec * fmtSpec);
int my_putFloat(float print_me, int field_width, int precision);
int my_stringPrint (const char * print_me, formatSpec * fmtSpec);
unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit);
int my_putUInt(unsigned long long print_me, formatSpec * fmtSpec);

int DEPRECATEDformatSpecPrint (va_list * argsP, formatSpec * fmtSpecs);

void printQueue (int * queue, int size);
void enqueue (int * queue, int size, int to_queue);
void clearQueue (int * queue, int size);
int dequeue (int * queue, int size);

void clearCharQueue (char * queue, int size);
void printCharQueue (char * queue, int size);
int charEnqueue (char * queue, int size, char to_queue);
char charDequeue (char * queue, int size);
char charStackPop (char * stack, int stack_size);

int stackToInt (int * queue, int queue_size);
int stackPop (int * stack, int stack_size);


#ifndef EMPTY_SPACE
#define EMPTY_SPACE INT_MAX
#endif

#ifndef CHAR_EMPTY_SPACE
#define CHAR_EMPTY_SPACE 21
#endif

#define IS_DIGIT(c) ((unsigned int) ((c) - '0') < 10)



//  Functions my printf implementation relies on.


int DEPRECATEDformatSpecPrint (va_list * argsP, formatSpec * fmtSpecs)
{

  int printed = 0;

  switch (fmtSpecs->type)
  {
  case PERCENT_SYMBOL :
    printed += my_printChar('%', fmtSpecs);
    break;
  case CHAR_TYPE :
    printed += my_printChar((char) va_arg(*argsP, int), fmtSpecs);
    break;
  case INT_TYPE :
    printed += my_putInt(va_arg(*argsP, int), fmtSpecs);
    break;
  /*
  case 'f' :
    printed += my_putFloat((float) va_arg(*args, double));
    break;
  */
  case STRING_TYPE :
    printed += my_stringPrint(va_arg(*argsP, char *), fmtSpecs);
    break;
  case UNSIGNED_INT_TYPE :
    printed += my_putUInt(va_arg(*argsP, unsigned int), fmtSpecs);
    break;
  case HEXADECIMAL_LOWER :
    printed += my_putUInt(va_arg(*argsP, unsigned int), fmtSpecs);
    break;
  case HEXADECIMAL_UPPER :
    printed += my_putUInt(va_arg(*argsP, unsigned int), fmtSpecs);
    break;
  case OCTAL :
    printed += my_putInt(va_arg(*argsP, int), fmtSpecs);
    break;
  case LONG_INT_TYPE : //  "long" types
    printed += my_putInt(va_arg(*argsP, long int), fmtSpecs);
    break;
  case UNSIGNED_LONG_TYPE :
    printed += my_putInt(va_arg(*argsP, unsigned long int), fmtSpecs);
    break;
  case LONG_LONG_INT_TYPE :
    printed += my_putInt(va_arg(*argsP, long long int), fmtSpecs);
    break;
  case UNSIGNED_LONG_LONG_TYPE :
    printed += my_putUInt(va_arg(*argsP, unsigned long long), fmtSpecs);
    break;
  case POINTER_TYPE :
    printed += my_putUInt((unsigned long long int) va_arg(*argsP, void *), fmtSpecs);
    break;
  }
  return printed;
}

int readInt (const char ** format)
{
  /*  Stolen from https://codebrowser.dev/glibc/glibc/stdio-common/printf-parse.h.html#read_int */

  /*  assumes passed first character is a number*/
  int value = **format - '0';

  /* ++ before (the symbol) means (the symbol) is iterated before it is otherwise operated on.
    I suppose this means operators are read inside-out?
  */
  while (IS_DIGIT(* ++ (*format)))
  {
    if (value >= 0)
    {
      if (INT_MAX / 10 < value)
      {
        /*  prevents overflows. more or less an error code*/
        value = -1;
      }
      else
      {
        int newDigit = **format - '0';

        value *= 10;
        if (INT_MAX - newDigit < value)
        {
          /*  prevents overflows*/
          value = -1;
        }
        else
        {
          value += newDigit;
        }
      }
    }
  }
  return value;
}

int stackPop (int * stack, int stack_size)
{
  int to_return;
  size_t i = 0;
  for (; i < stack_size; i++)
  {
    if (stack[i] == EMPTY_SPACE)
    {
      i--;
      break;
    }
    to_return = stack[i];
  }
  stack[i] = EMPTY_SPACE;
  return to_return;
}

int charEnqueue (char * queue, int size, char to_queue)
{
  for(int i = 0; i < size; i++)
  {
    if (queue[i] == CHAR_EMPTY_SPACE)
    {
      queue[i] = to_queue;
      return 1;
    }
  }
  return 0;
}

char charDequeue (char * queue, int size)
{
  char dequeued = queue[0];
  int one_less = size - 1;

  for (int i = 0; i < one_less; i++)
  {
    queue[i] = queue[i + 1];
  }
  queue[one_less] = CHAR_EMPTY_SPACE;
  return dequeued;
}

void clearCharQueue (char * queue, int queue_size)
{
  for (size_t i = 0; i < queue_size; i++)
  {
    queue[i] = CHAR_EMPTY_SPACE;
  }
}

void printCharQueue (char * queue, int size)
{
  for (size_t i = 0; i < size; i++)
  {
    my_putChar(queue[i]);
    my_putChar(' ');
  }
}

char charStackPop (char * stack, int stack_size)
{
  char to_return;
  size_t i = 0;
  for (; i < stack_size; i++)
  {
    if (stack[i] == CHAR_EMPTY_SPACE)
    {
      i--;
      break;
    }
    to_return = stack[i];
  }
  stack[i] = CHAR_EMPTY_SPACE;
  return to_return;
}

int my_printChar(char to_print, formatSpec * fmtSpec)
{
  int printed = 0;

  if (fmtSpec->justify != '-' /* justify == +, because anything other than '-' is assumed to be '+' */)
  {
    for (size_t i = 1; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }
    
    printed += my_putChar(to_print);
  }
  else
  {
    printed += my_putChar(to_print);

    for (size_t i = 1; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }
  }
  
  return printed;
}

int my_putChar (char print_me)
{
  int printed = write(STDOUT_FILENO, &print_me, 1);
  if (printed == -1)
  {
    perror("my_putChar failed:");
    return 0;
  }
  else
  {
    return printed;
  }
}

int my_putInt(long long int print_me, formatSpec * fmtSpec)
{
  int printed = 0;
  int number_base = 10;
  bool is_negative;

  if (print_me < 0)
  {
    is_negative = true;
    print_me *= -1;
  }
  
    
  //  create a buffer that functions like a queue to store all numerical characters, 
  //  zeroes to be printed from precision argument, and hex prefix

  int print_buffer_size = 20 + fmtSpec->precision;

  char print_buffer[print_buffer_size];
  clearCharQueue(print_buffer, print_buffer_size);

  const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  const char uppercase[] = {'A', 'B', 'C', 'D', 'E', 'F'};

  int digits_to_print = 0;
  
  do
  {    
    unsigned int coefficient = print_me % number_base;
    print_me /= number_base;

    if (fmtSpec->type == HEXADECIMAL_UPPER && coefficient > 9)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, uppercase[coefficient - '9']);
    }
    else
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, digits[coefficient]);
    } 
  } while(print_me != 0);


  //  enqueue zeroes to be printed from precision argument
  for (size_t i = digits_to_print; i < fmtSpec->precision; i++)
  {
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
  }

  if (is_negative)
  {
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, '-');
  }
  
    
  //  print order:

  //  >>> '+' <<<
  //  if there is a non-zero precision argument:
  //  [spaces][negative sign][zeroes][number]

  //  field width > 0, zero precision argument, argument calls for left justify zeroes:
  //  [negative sign][zeroes][number]

  //  field width > 0, zero precision argument, argument doesn't call 
  //  for left justify zeroes:
  //  [spaces][negative sign][number]



  
  //  >>> '-' <<<
  //  non-zero precision argument, non-zero field width and field width > precision:
  //  [negative sign][zeroes][number][spaces]

  //  if there is a zero precision argument, regardless of whether the format specifier
  //  calls for left justfifying zeroes:
  //  [number][spaces]

  
  if (fmtSpec->justify != '-' /* justify == '+', because argument could be '\0' (defaults to '+' if not specified) */)
  {
    if (fmtSpec->precision == 0 && fmtSpec->L_just_0 == true)
    {
      if (is_negative == true)
      {
        printed += my_putChar('-');
        digits_to_print ++;
      }

      for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
      {
        printed += my_putChar('0');
      }
    }
    else
    {
      for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
      {
        printed += my_putChar(' ');
      }
    }
    
    
    while (print_buffer[0] != CHAR_EMPTY_SPACE)
    {
      char to_print = charStackPop(print_buffer, print_buffer_size);
      printed += my_putChar(to_print);
    }
    
  }
  else
  {
    //  code for justify == '-'

    if (is_negative == true && fmtSpec->precision == 0)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, '-');
    }
    

    while (print_buffer[0] != CHAR_EMPTY_SPACE)
    {
      char to_print = charStackPop(print_buffer, print_buffer_size);
      printed += my_putChar(to_print);
    }

    for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }

  }
  
  return printed;
}


int my_putUInt(unsigned long long print_me, formatSpec * fmtSpec)
{
  int printed = 0;
  int number_base;

  if (print_me == 0)
  {
    fmtSpec->hex_prefix = false;
  }


  if (fmtSpec->type == HEXADECIMAL_LOWER || fmtSpec->type == HEXADECIMAL_UPPER || fmtSpec->type == POINTER_TYPE)
  {
    number_base = 16;
  }
  else if (fmtSpec->type == OCTAL)
  {
    number_base = 8;
  }
  else
  {
    number_base = 10;
  }

  //  create a buffer that functions like a queue to store all numerical characters, 
  //  zeroes to be printed from precision argument, and hex prefix

  /*  to replicate printf output ¯\_(ツ)_/¯*/
  if (fmtSpec->type == POINTER_TYPE)
  {
    fmtSpec->hex_prefix = true;
  }
  

  const unsigned int print_buffer_size = 20 + fmtSpec->precision + (fmtSpec->hex_prefix * 2);

  char print_buffer[print_buffer_size];
  clearCharQueue(print_buffer, print_buffer_size);

  const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  const char uppercase[] = {'A', 'B', 'C', 'D', 'E', 'F'};

  int digits_to_print = 0;  
  
  do
  {    
    unsigned int coefficient = print_me % number_base;
    print_me /= number_base;

    if (fmtSpec->type == HEXADECIMAL_UPPER && coefficient > 9)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, uppercase[coefficient - '9']);
    }
    else
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, digits[coefficient]);
    } 
  } while(print_me != 0);

  //  enqueue zeroes to be printed from precision argument
  for (size_t i = digits_to_print; i < fmtSpec->precision; i++)
  {
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
  }
  
  if (fmtSpec->hex_prefix == true)
  {
    if (fmtSpec->type == HEXADECIMAL_LOWER || fmtSpec->type == HEXADECIMAL_UPPER)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, (fmtSpec->type == HEXADECIMAL_UPPER) ? 'X' : 'x');
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
    }
    else if (fmtSpec->type == OCTAL)
    {
      /*  As it turns out, there is a prefix that gets printed with octal type numbers.
        Need to append a '0' to the beginning of octal numbers if hex_prefix is flagged
      */
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
    }
  }
  
  //  INTENDED print order (copying printf):

  //  >>> '+' <<<
  //  if there is a non-zero precision argument:
  //  [spaces][hex prefix/octal prefix][zeroes][number]

  //  field width > 0, zero precision argument, argument calls for left justify zeroes:
  //  [negative sign][zeroes][number]

  //  field width > 0, zero precision argument, argument doesn't call 
  //  for left justify zeroes:
  //  [spaces][hex prefix/octal prefix][number]


  
  //  >>> '-' <<<
  //  non-zero precision argument, non-zero field width and field width > precision:
  //  [hex prefix/octal prefix][zeroes][number][spaces]

  //  if there is a zero precision argument, regardless of whether the format specifier
  //  calls for left justfifying zeroes:
  //  [hex prefix/octal prefix][number][spaces]

  
  if (fmtSpec->justify != '-' /* justify == '+', because argument could be '\0' (defaults to '+' if not specified) */)
  {
    if (fmtSpec->precision == 0 && fmtSpec->L_just_0 == true)
    {
      for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
      {
        printed += my_putChar('0');
      }
    }
    else
    {
      for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
      {
        printed += my_putChar(' ');
      }
    }
    
    
    while (print_buffer[0] != CHAR_EMPTY_SPACE)
    {
      char to_print = charStackPop(print_buffer, print_buffer_size);
      printed += my_putChar(to_print);
    }
    
  }
  else
  {
    //  code for justify == '-'
    while (print_buffer[0] != CHAR_EMPTY_SPACE)
    {
      char to_print = charStackPop(print_buffer, print_buffer_size);
      printed += my_putChar(to_print);
    }

    
    for (size_t i = digits_to_print; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }
    
  }
  
  return printed;
}


int my_stringPrint (const char * print_me, formatSpec * fmtSpec)
{
  //  puts, used internally by my printf to print string literals 
  //  passed by %s without adding a newline on the end
  int printed = 0;
  
  int len = strlen(print_me);

  if (fmtSpec->justify != '-' /* Anything that isn't '-' is considered '+' */)
  {
    for (int i = len; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }
    
    while (*print_me)
    {
      printed += my_putChar(*print_me);
      print_me ++;
    }
  }
  else
  {
    // code for justify == '-'
    while (*print_me)
    {
      printed += my_putChar(*print_me);
      print_me ++;
    }

    for (int i = len; i < fmtSpec->field_width; i++)
    {
      printed += my_putChar(' ');
    }
  }

  return printed;
}



unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit)
{
  //  attempts to read an IEEE 754 complying float to derive the value of a given float by reading
  //  the integer value of its binary representation
  assert(start_bit >= end_bit);
  
  int is_true, and_mask;
  unsigned int value = 0;

  for (int i = start_bit; i >= end_bit; i--)
  {
    and_mask = 1 << i;
    is_true = float_bits & and_mask;
    if (is_true)
    {
      value += pow(2, i - end_bit);
    }
  }
  return value;
}


int my_putFloat(float print_me, int field_width, int precision)
{
  /*
  *
  *  so, as it turns out, I am WAY out of my depth with dealing with floating point numbers like this.
  *  I'm going to put this project on hold until I Get Smart.
  *
  */

  //  floating point bits basics
  //  https://docs.oracle.com/cd/E19957-01/806-3568/ncg_math.html

  //  floating point bits to decimal numbers
  //  https://youtu.be/LXF-wcoeT0o

  /*  First, determine endianness*/

  unsigned int signVal, mantissa;
  int exponent, printed = 0;
  uint32_t float_bits = * (uint32_t*) &print_me;
  float value;
  
  signVal = bitValue(float_bits, 31, 31);
  exponent = bitValue(float_bits, 30, 23);
  mantissa = bitValue(float_bits, 22, 0);

  if (exponent > 0 && exponent < 255)
  {
    //  "normal" numbers
    int trueExponent = exponent - 127;
    
    
  }
  else if (exponent == 0 && mantissa != 0)
  {
    //  "subnormal" numbers
    int trueExponent = exponent - 126;

  }
  else if (exponent == 0 && mantissa == 0)
  {
    //  signed zero, depending on sign bit
    
  }
  else if (signVal == 0 && exponent == 255 && mantissa == 0)
  {
    //  positive infinity
  }
  else if (signVal == 1 && exponent == 255 && mantissa == 0)
  {
    //  negative infinity
  }
  else if (exponent == 255 && mantissa != 0)
  {
    //  NaN (not a number)
  }
  
  return printed;
}