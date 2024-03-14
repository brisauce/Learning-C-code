#include <stdio.h>

#include "sub-print_functions.h"
#include "my_printf.h"

int main(void)
{
  my_printf("this is a test of my printf\n");
  my_printf("It isn't a perfect implementation, %s", "in fact, it's pretty far from it");
  char * test = "\nThere isn't a working %s that prints floats\n";
  char test1[] = "function\0";
  my_printf(test, test1);
  my_printf("there are also strange interactions with the C version of printf\nThey seem to interact with the output buffer differently");
  
}

