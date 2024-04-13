#ifndef _SUB_PRINT_FUNCTIONS_H
#define _SUB_PRINT_FUNCTIONS_H 

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>



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

//  add types above ^^

typedef struct 
{
  const char * start;
  unsigned int length;
  int type;
  int field_width;
  unsigned int precision;
  bool hex_prefix;
  char justify;
  bool L_just_0;
}formatSpec;

#define CHAR_EMPTY_SPACE 21

int my_printChar(char to_print, formatSpec * fmtSpec);
int my_putChar (char print_me);
int my_putInt(long long int print_me, formatSpec * fmtSpec);
//int my_putFloat(float print_me, int field_width, int precision);
int my_stringPrint (const char * print_me, formatSpec * fmtSpec);
unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit);
int my_putUInt(unsigned long long print_me, formatSpec * fmtSpec);

int DEPRECATEDformatSpecPrint (va_list * argsP, formatSpec * spec);

void printQueue (int * queue, int size);
void enqueue (int * queue, int size, int to_queue);
void clearQueue (int * queue, int size);
int dequeue (int * queue, int size);

int charEnqueue (char * queue, int size, char to_queue);
char charDequeue (char * queue, int size);
void clearCharQueue (char * queue, int size);
void printCharQueue (char * queue, int size);
char charStackPop (char * stack, int stack_size);

int stackToInt (int * queue, int queue_size);
int stackPop (int * stack, int queue_size);

int readInt (const char ** format);


#endif