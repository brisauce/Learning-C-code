#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDBOOL_H
#include <stdbool.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _MATH_H
#include <math.h>
#endif

#ifndef _ERRNO_H
#include <errno.h>
#endif

#ifndef _LIMITS_H
#include <limits.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STDINT_H
#include <stdint.h>
#endif

#ifndef _ASSERT_H
#include <assert.h>
#endif

#ifndef _STDARG_H
#include <stdarg.h>
#endif


#ifndef INT_TYPES
#define INT_TYPES

#define INT_TYPE 0
#define LONG_INT_TYPE 1
#define UNSIGNED_INT_TYPE 2
#define HEXADECIMAL_LOWER 3
#define HEXADECIMAL_UPPER 4
#define OCTAL 5
#define UNSIGNED_LONG_TYPE 6
#define UNSIGNED_LONG_LONG_TYPE 7
#define LONG_LONG_INT_TYPE 8

//  add types above ^^
#endif


int my_printChar(char to_print, int field_width, char justify);
int my_putChar (char print_me);
int my_putInt(long long int print_me, int type, int field_width, int precision, bool left_justify_zeroes, char justify);
int my_putFloat(float print_me, int field_width, int precision);
int my_stringPrint (char * print_me, int field_width, char justify);
unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit);
int my_putUInt(unsigned long long print_me, int type, int field_width, int precision, bool hex_prefix, bool left_justify_zeroes, char justify);

int formatSpecProc (va_list * argsP);
int formatSpecProcLong(va_list *args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes);
int formatSpecProcLongLong(va_list *args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes);


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



#define EMPTY_SPACE INT_MAX

#define CHAR_EMPTY_SPACE 21

extern const char * GLOBAL_printf_format;


//  Functions my printf implementation relies on.


int formatSpecProc (va_list * argsP)
{

  int printed = 0;
  char justify = '\0';

  bool precision_flag = false;
  bool hex_prefix = false;
  bool left_justify_zeroes = false;

  int prcsn_stack_size = 10;
  int FW_stack_size = 10;

  int prcsn_stack[prcsn_stack_size];
  int FW_stack[FW_stack_size];

  clearQueue(prcsn_stack, prcsn_stack_size);
  clearQueue(FW_stack, FW_stack_size);

  int field_width;
  int precision = 0;

  while (true)
  {
    switch (*GLOBAL_printf_format)
    {
    case '%' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      printed += my_printChar('%', field_width, justify);
      break;
    case 'c' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      printed += my_printChar((char) va_arg(*argsP, int), field_width, justify);
      break;
    case 'd' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += my_putInt(va_arg(*argsP, int), INT_TYPE, field_width, precision, left_justify_zeroes, justify);
      break;
    /*
    case 'f' :
      printed += my_putFloat((float) va_arg(*args, double));
      break;
    */
    case 's' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      char * string = va_arg(*argsP, char *);
      printed += my_stringPrint(string, field_width, justify);
      break;
    case 'u' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += my_putUInt(va_arg(*argsP, unsigned int), UNSIGNED_INT_TYPE, field_width, precision, hex_prefix, left_justify_zeroes, justify);
      break;
    case 'x' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += my_putUInt(va_arg(*argsP, unsigned int), HEXADECIMAL_LOWER, field_width, precision, hex_prefix, left_justify_zeroes, justify);
      break;
    case 'X' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += my_putUInt(va_arg(*argsP, unsigned int), HEXADECIMAL_UPPER, field_width, precision, hex_prefix, left_justify_zeroes, justify);
      break;
    case 'o' :
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += my_putInt(va_arg(*argsP, int), OCTAL, field_width, precision, left_justify_zeroes, justify);
      break;

    //  add more cases here ^^^
    case 'l' : //  "long" types
      GLOBAL_printf_format ++;
      field_width = stackToInt(FW_stack, FW_stack_size);
      precision = stackToInt(prcsn_stack, prcsn_stack_size);
      printed += formatSpecProcLong(argsP, field_width, precision, justify, hex_prefix, left_justify_zeroes);
      break;
    //  format specifier flags
    case '-' :
      if(justify != false)
      {
        printf("Invalid format specifier. There can be no more than one argument for format justification per format specifier.\nArgument was: %c", justify);
        abort();
      }
      justify = '-';
      GLOBAL_printf_format ++;
      continue;
    case '+' :
      if(justify != false)
      {
        printf("Invalid format specifier. There can be no more than one argument for format justification per format specifier.\nArgument was: %c", justify);
        abort();
      }
      justify = '+';
      GLOBAL_printf_format ++;
      continue;
    case '#' :
      if(hex_prefix != false)
      {
        printf("Invalid format specifier. There can be no more than one \"#\" signifying a hex prefix in a format specifier.");
        abort();
      }
      hex_prefix = true;
      GLOBAL_printf_format ++;
      continue;
    case '0' :
      if (FW_stack[0] != EMPTY_SPACE || prcsn_stack[0] != EMPTY_SPACE)
      {
        goto number_case;
      }
      left_justify_zeroes = true;
      GLOBAL_printf_format ++;
      continue;
    case '.' :
      if(precision_flag != false)
      {
        printf("Invalid format specifier. Format specifier can have no more than one period signifying precision.");
        abort();
      }
      precision_flag = true;
      GLOBAL_printf_format ++;
      continue;
    default :
      if(*GLOBAL_printf_format - '0' > 10 || *GLOBAL_printf_format - '0' < 0)
      {
        printf("Invalid format specifier. Invalid character \"%c\" detected", *GLOBAL_printf_format);
        abort();
      }
      number_case:
      if (precision_flag == false)
      {
        enqueue(FW_stack, FW_stack_size, *GLOBAL_printf_format - '0');
      }
      else
      {
        enqueue(prcsn_stack, prcsn_stack_size, *GLOBAL_printf_format - '0');
      }
      GLOBAL_printf_format ++;
      continue;
    }
    break;
  }
  return printed;
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

int stackToInt (int * stack, int stack_size)
{
  int value = 0;
  int i = 0;
  while (stack[0] != EMPTY_SPACE)
  {
    value += stackPop(stack, stack_size) * pow(10, i);
    i++;
  }
  return value;
}

int formatSpecProcLong(va_list *args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes)
{

  int printed = 0;
  //  "long" types
  switch (*GLOBAL_printf_format)
  {
    case 'd' : 
      printed += my_putInt(va_arg(*args, long int), LONG_INT_TYPE, field_width, precision, left_justify_zeroes, justify);
      break;
    case 'u' :
      printed += my_putUInt(va_arg(*args, unsigned long int), UNSIGNED_LONG_TYPE, field_width, precision, hex_prefix, left_justify_zeroes, justify);
      break;

    //  more "long" types go here ^^^
    case 'l' :
      //  "long long" types
      GLOBAL_printf_format ++;
      printed += formatSpecProcLongLong(args, field_width, precision, justify, hex_prefix, left_justify_zeroes);
      break;
    default :
      printed += my_putChar('%');
  }
  return printed;
}

int formatSpecProcLongLong(va_list *args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes)
{
  int printed = 0;
  switch (*GLOBAL_printf_format)
  {
  case 'd':
    printed += my_putInt(va_arg(*args, long long int), LONG_LONG_INT_TYPE, field_width, precision, left_justify_zeroes, justify);
    break;
  case 'u':
    printed += my_putUInt(va_arg(*args, unsigned long long int), UNSIGNED_LONG_LONG_TYPE, field_width, precision, hex_prefix, left_justify_zeroes, justify);
    break;
  default:
    printed += my_putChar('%');
  }
  return printed;
}


void enqueue (int * queue, int size, int to_queue)
{
  for(int i = 0; i < size; i++)
  {
    if (queue[i] == EMPTY_SPACE)
    {
      queue[i] = to_queue;
      return;
    }
  }
}

int dequeue (int * queue, int size)
{
  int dequeued = queue[0];
  int one_less = size - 1;

  for (int i = 0; i < one_less; i++)
  {
    queue[i] = queue[i + 1];
  }
  return dequeued;
}

void printQueue (int * queue, int size)
{
  for(int i = 0; i < size; i++)
  {
    if(queue[i] == EMPTY_SPACE)
    {
      printf("%%%% ");
    }
    else
    {
      printf("%2d ", queue[i]);
    }
  }
}

void clearQueue (int * queue, int size)
{
  for(int i = 0; i < size; i++)
  {
    queue[i] = EMPTY_SPACE;
  }
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

int my_printChar(char to_print, int field_width, char justify)
{
  int printed = 0;

  if (justify != '-' /* justify == +, because anything other than '-' is assumed to be '+' */)
  {
    for (size_t i = 1; i < field_width; i++)
    {
      printed += my_putChar(' ');
    }
    
    printed += my_putChar(to_print);
  }
  else
  {
    printed += my_putChar(to_print);

    for (size_t i = 1; i < field_width; i++)
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

int my_putInt(long long int print_me, int type, int field_width, int precision, bool left_justify_zeroes, char justify)
{
  int printed = 0;
  long long int integer_max;
  int number_base;
  bool is_negative;

  if (print_me < 0)
  {
    is_negative = true;
    print_me *= -1;
  }
  
  
  if (type == OCTAL)
  {
    number_base = 8;
  }
  else
  {
    number_base = 10;
  }
  
  //  create a buffer that functions like a queue to store all numerical characters, 
  //  zeroes to be printed from precision argument, and hex prefix

  int print_buffer_size = 20 + precision;

  char print_buffer[print_buffer_size];
  clearCharQueue(print_buffer, print_buffer_size);

  char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char uppercase[] = {'A', 'B', 'C', 'D', 'E', 'F'};
  int coefficient = 0;

  int digits_to_print = 0;
  
  do
  {    
    coefficient = print_me % number_base;
    print_me /= number_base;

    if (type == HEXADECIMAL_UPPER && coefficient > 9)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, uppercase[coefficient - '9']);
    }
    else
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, digits[coefficient]);
    } 
  } while(print_me != 0);

  //  enqueue zeroes to be printed from precision argument
  for (size_t i = digits_to_print; i < precision; i++)
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

  
  if (justify != '-' /* justify == '+', because argument could be '\0' (defaults to '+' if not specified) */)
  {
    if (precision == 0 && left_justify_zeroes == true)
    {
      if (is_negative == true)
      {
        printed += my_putChar('-');
        digits_to_print ++;
      }

      for (size_t i = digits_to_print; i < field_width; i++)
      {
        printed += my_putChar('0');
      }
    }
    else
    {
      for (size_t i = digits_to_print; i < field_width; i++)
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

    if (is_negative == true && precision == 0)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, '-');
    }
    

    while (print_buffer[0] != CHAR_EMPTY_SPACE)
    {
      char to_print = charStackPop(print_buffer, print_buffer_size);
      printed += my_putChar(to_print);
    }

    for (size_t i = digits_to_print; i < field_width; i++)
    {
      printed += my_putChar(' ');
    }

  }
  
  return printed;
}


int my_putUInt(unsigned long long print_me, int type, int field_width, int precision, bool hex_prefix, bool left_justify_zeroes, char justify)
{
  int printed = 0;
  int number_base;

  if (print_me == 0)
  {
    hex_prefix = false;
  }

  if (type != HEXADECIMAL_LOWER && type != HEXADECIMAL_UPPER)
  {
    assert(!hex_prefix);
  }  

  if (type == HEXADECIMAL_LOWER || type == HEXADECIMAL_UPPER)
  {
    number_base = 16;
  }
  else
  {
    number_base = 10;
  }

  //  create a buffer that functions like a queue to store all numerical characters, 
  //  zeroes to be printed from precision argument, and hex prefix

  int print_buffer_size = 20 + precision + hex_prefix * 2;

  char print_buffer[print_buffer_size];
  clearCharQueue(print_buffer, print_buffer_size);

  char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char uppercase[] = {'A', 'B', 'C', 'D', 'E', 'F'};
  int coefficient = 0;

  int digits_to_print = 0;  
  
  do
  {    
    coefficient = print_me % number_base;
    print_me /= number_base;

    if (type == HEXADECIMAL_UPPER && coefficient > 9)
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, uppercase[coefficient - '9']);
    }
    else
    {
      digits_to_print += charEnqueue(print_buffer, print_buffer_size, digits[coefficient]);
    } 
  } while(print_me != 0);

  //  enqueue zeroes to be printed from precision argument
  for (size_t i = digits_to_print; i < precision; i++)
  {
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
  }
  
  if (hex_prefix == true)
  {
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, (type == HEXADECIMAL_UPPER) ? 'X' : 'x');
    digits_to_print += charEnqueue(print_buffer, print_buffer_size, '0');
  }
  
  //  INTENDED print order:

  //  >>> '+' <<<
  //  if there is a non-zero precision argument:
  //  [spaces][hex prefix][zeroes][number]

  //  field width > 0, zero precision argument, argument calls for left justify zeroes:
  //  [negative sign][zeroes][number]

  //  field width > 0, zero precision argument, argument doesn't call 
  //  for left justify zeroes:
  //  [spaces][hex prefix][number]


  
  //  >>> '-' <<<
  //  non-zero precision argument, non-zero field width and field width > precision:
  //  [hex prefix][zeroes][number][spaces]

  //  if there is a zero precision argument, regardless of whether the format specifier
  //  calls for left justfifying zeroes:
  //  [hex prefix][number][spaces]

  
  if (justify != '-' /* justify == '+', because argument could be '\0' (defaults to '+' if not specified) */)
  {
    if (precision == 0 && left_justify_zeroes == true)
    {
      for (size_t i = digits_to_print; i < field_width; i++)
      {
        printed += my_putChar('0');
      }
    }
    else
    {
      for (size_t i = digits_to_print; i < field_width; i++)
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

    
    for (size_t i = digits_to_print; i < field_width; i++)
    {
      printed += my_putChar(' ');
    }
    
  }
  
  return printed;
}

#ifndef STRING_H
#include <string.h>
#endif

int my_stringPrint (char * print_me, int field_width, char justify)
{
  //  puts, used internally by my printf to print string literals 
  //  passed by %s without adding a newline on the end
  int printed = 0;
  
  int len = strlen(print_me);

  if (justify != '-' /* Anything that isn't '-' is considered '+' */)
  {
    for (size_t i = len; i < field_width; i++)
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

    for (size_t i = len; i < field_width; i++)
    {
      printed += my_putChar(' ');
    }
  }

  return printed;
}



unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit)
{
  //  attempts to comply with IEEE 754 standards to derive the value of a given float by reading its binary value
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

  unsigned int sign, exponent, mantissa;
  int printed = 0;
  uint32_t float_bits = * (uint32_t*) &print_me;
  float value;
  
  sign = bitValue(float_bits, 31, 31);
  exponent = bitValue(float_bits, 30, 23);
  mantissa = bitValue(float_bits, 22, 0);

  if (exponent > 0 && exponent < 255)
  {
    //  "normal" numbers
    int true_exponent = exponent - 127;
    
    
  }
  else if (exponent == 0 && mantissa != 0)
  {
    //  "subnormal" numbers
    int true_exponent = exponent - 126;

  }
  else if (exponent == 0 && mantissa == 0)
  {
    //  signed zero, depending on sign bit
  }
  else if (sign == 0 && exponent == 255 && mantissa == 0)
  {
    //  positive infinity
  }
  else if (sign == 1 && exponent == 255 && mantissa == 0)
  {
    //  negative infinity
  }
  else if (exponent == 255 && mantissa != 0)
  {
    //  NaN (not a number)
  }
  
  
}