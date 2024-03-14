#define SUB_PRINT_FUNCTIONS_H 

#ifndef _STDINT_H
#include <stdint.h>
#endif

#ifndef _STDARG_H
#include <stdarg.h>
#endif

#ifndef _STDBOOL_H
#include <stdbool.h>
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

#define CHAR_EMPTY_SPACE 21

int my_printPercent(int field_width, char justify);
int my_putChar (char print_me);
int my_putInt(long long int print_me, int type, int field_width, int precision, bool left_justify_zeroes, char justify);
int my_putFloat(float print_me);
int my_stringPrint (char * print_me, int field_width, char justify);
unsigned int bitValue (uint32_t float_bits, int start_bit, int end_bit);
int my_putUInt(unsigned long long print_me, int type, int field_width, int precision, bool hex_prefix, bool left_justify_zeroes, char justify);

int formatSpecProc (va_list * args);
int formatSpecProcLong (va_list * args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes);
int formatSpecProcLongLong(va_list * args, int field_width, int precision, char justify, bool hex_prefix, bool left_justify_zeroes);

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

