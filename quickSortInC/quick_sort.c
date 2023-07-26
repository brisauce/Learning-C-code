#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void quickSort(double[], int);
void quickSortRecursion(double[], int, int);
bool sortCheck(double[], int, int, bool);
void printElements(double[], int);
int swap(double[], int, int);
void randArrayFill(double[], int);
bool checkIfUsed (double[], int, double);
double randRange(double, double);

//  I'm going to modify this to sort arrays of double and then possibly strings
//  have now made it work for doubles. on to strings. the question is, how would I sort them?

//  idea for sorting strings - create a randomized string of random length (within some bounds) 
//  since character values have a numerical value associated with them, add up the values of the characters in a given string and store them.
//  send these values through quicksort and get the sorted order, then have a different function align the strings with their respective
int main (void){

    int length = 128;

    double array[length];

    randArrayFill(array, length);

    printf("this is the array before:");

    printElements(array, length);
 
    quickSort(array, length);
    
    bool sorted = sortCheck(array, 0, length - 1, true);

    printf("\n\nthis is the array after. It %ssorted%s", (sorted) ? "is " : "is NOT ", (sorted) ? "!!" : "????");

    printElements(array, length);

    return 0;
}






void quickSort(double array[], int length){
    quickSortRecursion(array, 0, length - 1);
}






void quickSortRecursion(double array[], int leftMostIndex, int rightMostIndex){

    //  if the array to be sorted is size 1, or rightmostindex is less than leftmost index, return
    if (leftMostIndex == rightMostIndex || rightMostIndex < leftMostIndex)
    {
        return;
    }

    int pivotIndex = leftMostIndex + (rightMostIndex - leftMostIndex) / 2;

    //  put the left, pivot, and right elements in order 
    if (array[leftMostIndex] > array[pivotIndex])
    {
        swap(array, leftMostIndex, pivotIndex);
    }
    if (array[pivotIndex] > array[rightMostIndex])
    {
        swap(array, pivotIndex, rightMostIndex);
    }
    if (array[leftMostIndex] > array[pivotIndex])
    {
        swap(array, leftMostIndex, pivotIndex);
    }

    //  for the case that the array subsect is smaller than 3 units
    if (rightMostIndex - leftMostIndex <= 2)
    {
        return;
    }

    //  put the pivot at the rightmost index
    swap(array, pivotIndex, rightMostIndex);
    pivotIndex = rightMostIndex;

    int leftPick = leftMostIndex, rightPick = rightMostIndex;
    int i = leftMostIndex, j = rightMostIndex;

    while (true)
    {
        //  find an element starting from the left that is bigger than the pivot
        //  find an element starting from the right that is bigger than the pivot

        for (; i <= rightMostIndex; i++)
        {
            leftPick = i;
            if (array[i] > array[pivotIndex])
            {
                break;
            }
        }

        for (; j >= leftMostIndex; j--)
        {
            rightPick = j;
            if (array[j] < array[pivotIndex])
            {
                break;
            }
        }

        //  if the leftpick is earlier in the array than the rightpick
        //  swap the larger and smaller elements
        if (leftPick < rightPick)
        {
            swap(array, leftPick, rightPick);
        }
        else 
        {
            break;
        }
    }

    //  after the partition, swap the pivot and the leftpick
    //  and set the pivot to the position of leftpick
    
    swap(array, leftPick, pivotIndex);
    pivotIndex = leftPick;
    
    //  recursively run the algorithm until it's all sorted
    quickSortRecursion(array, leftMostIndex, pivotIndex - 1);
    quickSortRecursion(array, pivotIndex + 1, rightMostIndex);
}



void printElements (double array[], int length) {

    //  simple algo to print elements in the array chosen

    printf("\n");

    for (int i = 0; i < length; i++){
        printf("%lf ", array[i]);
    }
}




double temp;

double *pTemp = &temp;

int swap (double array[], int index1, int index2)
{
    *pTemp = array[index1];
    array[index1] = array[index2];
    array[index2] = *pTemp;
    return 0;
}






bool sortCheck(double array[], int startIndex, int endIndex, bool helpful){

    //simple algo to check if array elements are sorted 
    int length = endIndex - startIndex;


    if (startIndex == endIndex)
    {
        printf("\ntried to check sortedness of an array of size 1");
        return true;
    }

    int i = startIndex;

    while ((array[i] <= array[i + 1]) && (i < endIndex))
    {
        i++;
    }
    
    
    if ((helpful) && (i < endIndex && (i + 1 != endIndex)))
    {
        printf("\nsortCheck stopped at element %d of %d", i, endIndex);
        sortCheck(array, i + 1, endIndex, helpful);
    }
    

    return i == endIndex;
}







void randArrayFill(double array[], int length)
{
    //create a random number seed based on the current time
    srand(time(0));

    //  counter
    int i = 0;

    //  minimum value which will be generated
    double min = 0;

    //  maximum value to be generated
    double max = 128;

    double guess;

    //fill given array with random numbers, but make them unique
    while (i < length)
    {
        guess = randRange(min, max);

        if (checkIfUsed(array, i, guess))
        {
            array[i] = guess;
            i++;
        }
        else
        {
            continue;
        }
    }
}

double randRange (double min, double max)
{
    //  creates a random number in the double precision range

    //  create a number between 0 and 1 by dividing a random double number by the maximum number which can be represented by a double
    double random = ((double) rand()) / RAND_MAX;

    //  multiply this number by the desired range of numbers
    double range = (max - min) * random;

    //  shift the number by adding the lowest value
    double number = range + min;

    return number;
}

bool checkIfUsed (double array[], int stop, double guess)
{
    //  checks if an array element before the stop value matches the guess value
    for (int i = 0; i < stop; i++)
    {
        if (array[i] == guess)
        {
            return false;
        }
    }
    return true;
}