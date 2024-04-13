#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "sub-print_functions.h"
#include "my_printf.h"

void rndmzFmtSpec (formatSpec * fmtSpecs);
int printFloat32 (const uint32_t float_bits, const char whichPart);

int main(void)
{
  /*  putting this aside for now. still needs:
  *   - to be stress tested.
  *   - float printing implemented.
  *   - implement something so it prints an entire line at a time like printf
  *     instead of one character at a time like it currently does.
  *   - move away from relying on heap memory allocation so it isn't almost 10x
  *     slower than printf.
  * 
  *   In general, i'm sure there is a lot that could be done here. Given that it's
  *   one of my first, if not my first ever *real* programming project i'm certain 
  *   it could be better
  */
  

}




int printFloat32 (const uint32_t float_bits, const char whichPart)
{

  /*  Prints the chosen bits of a 32 bit float whose bits are passed as an integer.*/

  //  start_bit and end_bit are the offsets from the first bit, since we are bit shifting
  //  1 to the left (start_bit - end_bit) number of times
  int start_bit;
  int end_bit;
  int trueBit, and_mask;

  int printed = 0;

  if (whichPart == 's')
  {
    /* sign*/
    start_bit = 31;
    end_bit = 31;
  }
  else if (whichPart == 'e')
  {
    /* exponent*/
    start_bit = 30;
    end_bit = 23;
  }
  else if (whichPart == 'm')
  {
    /* mantissa*/
    start_bit = 22;
    end_bit = 0;
  }
  else if (whichPart == 'a')
  {
    /*  just print the whole thing out*/
    start_bit = 31;
    end_bit = 0;
  }
  else
  {
    puts("SILLY BILLY");
    abort();
  }
  

  for (int i = start_bit; i >= end_bit; i--)
  {
    and_mask = 1 << i;
    trueBit = float_bits & and_mask;
    
    if (trueBit)
    {
      printed += my_printf("1");
    }
    else
    {
      printed += my_printf("0");
    }
  }
  return printed;
}




void rndmzFmtSpec (formatSpec * fmtSpecs)
{
  /*  Requires srand() to be set in main
  *   just initialize it with time(0), like old times
  */
  fmtSpecs->type = (unsigned) (rand() % 14);
  fmtSpecs->start = (char *) rand();
  fmtSpecs->precision = (unsigned) rand();
  fmtSpecs->length = (unsigned int) rand();
  fmtSpecs->L_just_0 = (bool) ((unsigned int)(rand() % 2));
  fmtSpecs->justify = ((unsigned int) (rand() % 2)) ? '+' : '-';
  fmtSpecs->hex_prefix = (bool) ((unsigned int)(rand() % 2));
  fmtSpecs->field_width = (unsigned) rand();
}