#include <stdio.h>
#include <math.h>
#include <tgmath.h>


//  time to make a numerical derivative program!!!!

//  i'm going to try and implement the symmetric differences quotient (now with complex numbers - I didn't have to change much)

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

double complex F (double complex x)
{
    //  F(x) = x^2
    x *= x;
    return x;
}

//  derivative

double complex f (double complex x)
{
    double h = 1/1e3;
    x = (F(x + h) - F(x - h));
    x /= (2.0 * h);
    return x;
}


int main ()
{   

    double complex a = 3 + 8*I;

    double complex Fa = F(a);
    double complex FxDx = f(a);
    
    printf("Original:\n%.0lf + %.0lfi", creal(a), cimag(a));
    puts("\nF(x)");
    printf("%.2lf + %.2lfi", creal(Fa), cimag(Fa));
    printf("\nFx/dx:\n%.2lf + %.2lfi", creal(FxDx), cimag(FxDx));
    return 0;
}