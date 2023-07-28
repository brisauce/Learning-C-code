#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
your next mission is to calculate pi to an arbitrary specificity

I got the idea from a jomatech video to assume a circle of radius 1 inside a box with sides of length 2, generate points randomly inside the box
and calculate pi from the ratio of points inside the circle to the total number of points

after building this - it is VERY cpu intensive and not at all efficient- this is essentially a gimmicky interview question type problem,
you don't lock down the first 4 digits until you're calculating for 20 million points, so there's a ratio of about 5 million points per 
accurate digit if this trend continues

scratch that - even at 80 million points, only the first 4 digits are guaranteed to be accurate. ah well, i tried.
If I go any higher than about ^^^ this my computer just kind of gives up, so that's the end of that for now
*/
double randCrd (double, double);
double distCalc0 (double, double);


#define NUMPTS 80000000

typedef struct
{
    double x;
    double y;
    double dist;
} XYcrd;

XYcrd pts[NUMPTS];

int main ()
{
    //  random number seed: defined
    srand(time(0));

    //  min and max values: determined
    double min = -1;
    double max = 1;
    double circlePts = 0;

    for (int i = 0; i < NUMPTS; i++)
    {
        //  random coordinates: deployed
        pts[i].x = randCrd(min, max);
        pts[i].y = randCrd(min, max);

        //  distance: deobfuscated
        pts[i].dist = distCalc0(pts[i].x, pts[i].y);

        if (pts[i].dist < 1)
        {
            circlePts++;
        }
    }

    double calc = (double) 4 * (circlePts / NUMPTS);

    printf("%lf", calc);
    
    return 0;
}



double distCalc0 (double x, double y)
{
    double xdist = x*x;
    double ydist = y*y;
    double dist = xdist + ydist;
    dist = sqrt(dist);
    return dist;
}




double randCrd (double min, double max)
{
    double x = (double) rand() / RAND_MAX;
    x *= (max - min);
    x += min;
    return x;
}