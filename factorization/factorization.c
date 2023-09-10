/*
*
*
*   create a program that recieves a number on the command line and prints the prime factors of the number,
*   then extend this program to recieve any number of numbers tobe factored and print them on their own lines
*
*
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool isPrime (int num);


int main (int argc, char* argv[argc + 1])
{
    char *ptr;
    int half_num;
    bool prime_number = true;
    printf("argc = %d\n", argc);

    for (size_t j = 1; j < argc; j++)
    {
        //int length = strlen(argv[j]);
        size_t num = strtoul(argv[j], &ptr, 10);
        
        printf("\nargv[%d] = %lu\n", j, num);

        half_num = num / 2;
        bool * factors = calloc(half_num + 1, sizeof(bool));
        int factor;

        for (size_t i = 2; i < half_num + 1; i++)
        {
            factor = num / i;
            if (factor * i == num && isPrime(i))
            {
                factors[i] = true;
                if (prime_number == true)
                {
                    prime_number == false;
                }
            }
        }

        if (prime_number == false)
        {
            printf("%d is prime", num);
        }
        else
        {
            printf("factors of %lu include", num);
            for (size_t i = 2; i <= half_num + 1; i++)
            {
                if (factors[i])
                {
                    printf(" %lu", i);
                }
            }
        }
        free(factors);
    }
}




bool isPrime (int num)
{
    int factor;
    for (size_t i = 2; i < num / 2; i++)
    {
        factor = num / i;
        if (factor * i == num)
        {
            return false;
        }
    }
    return true;
}
