#ifndef _MY_PRINTF_H
#define _MY_PRINTF_H

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


int my_printf(const char * format, ...);
int my_putString (char * string);
void printBadSpec(const char * start, const char * end);
void printFormatSpecs(formatSpec * fmtSpecs, int numSpecs, bool test);


#endif