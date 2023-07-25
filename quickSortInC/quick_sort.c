#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void quickSort(int [], int);
void quickSortRecursion(int[], int, int);
bool sortCheck(int[], int, int, bool);
void printElements(int[], int);
int swap(int[], int, int);
void randArrayFill(int[], int);
void printElementsHighlight (int[], int, int, int, int);
bool checkIfUsed (int[], int, int);


int main (void){

    int length = 128;

    int array[length];

    randArrayFill(array, length);

    printf("this is the array before:");

    printElements(array, length);

    quickSort(array, length);

    bool sorted = sortCheck(array, 0, length, true);

    printf("\n\nthis is the array after. It %ssorted%s", (sorted) ? "is " : "is NOT ", (sorted) ? "!!" : "????");

    printElements(array, length);
    return 0;
}






void quickSort(int array[], int length){
    quickSortRecursion(array, 0, length - 1);
}






void quickSortRecursion(int array[], int leftMostIndex, int rightMostIndex){

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









void printElementsHighlight (int array[], int length, int highlight1, int highlight2, int pivot) {

    //  simple algo to print elements in the array chosen and "box" a highlight

    printf("\n");

    for (int i = 0; i < length; i++){
        if (i == highlight1 || i == highlight2)
        {
            printf("{");
        }
        if (i == pivot)
        {
            printf("{");
        }
        
        
        printf("%d", array[i]);

        if (i == pivot)
        {
            printf("}");
        }
        if (i == highlight1 || i == highlight2)
        {
            printf("} ");
        }
        printf(" ");
    }
}





void printElements (int array[], int length) {

    //  simple algo to print elements in the array chosen

    printf("\n");

    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
}





int temp;

int *pTemp = &temp;

int swap (int array[], int index1, int index2)
{
    if (array[index1] == array[index2])
    {
        return 1;
    }
    *pTemp = array[index1];
    array[index1] = array[index2];
    array[index2] = *pTemp;
    return 0;
}






bool sortCheck(int array[], int startIndex, int endIndex, bool helpful){

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
        printElementsHighlight(array, length, i, i, i);
        sortCheck(array, i + 1, endIndex, helpful);
    }
    

    return i == endIndex;
}







void randArrayFill(int array[], int length)
{
    //create a random number seed based on the current time
    srand(time(0));

    int i = 0;
    int guess;

    //fill given array with random numbers, but make them unique
    while (i < length)
    {
        // use % to pick your number range
        guess = (rand() % 128) + 1;

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

bool checkIfUsed (int array[], int stop, int guess)
{
    for (int i = 0; i < stop; i++)
    {
        if (array[i] == guess)
        {
            return false;
        }
    }
    return true;
}