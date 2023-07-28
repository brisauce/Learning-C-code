#include <stdio.h>
#include <math.h>


//  time to make a numerical derivative program!!!!

//  i'm going to try and implement the symmetric differences quotient

//  from the book - 

/*
See if you can implement the numerical derivative:
double f (double x)
for a function:
double F (double x)

implement this with an example F for the function that you use for this exercise. 
A good primary choice for F would be a function for which you know the derivative, such as 
sin(x), cos(x), or sqrt(x). this allows you to check your answers for correctness.

*/

//  original function (anti-derivative)

double F (double x)
{
    x = cos(x);
    return x;
}

//  derivative

double f (double x)
{
    double h = 1/1e3;
    x = (F(x + h) - F(x - h));
    x /= (2.0 * h);
    return x;
}


int main ()
{   
    double a = 8;

    printf("F(%.0lf) = %.16lf", a, F(a));
    printf("\nf(%.0lf) = %.16lf", a, f(a));
    return 0;
}