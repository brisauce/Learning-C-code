#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

void quickSort(int);
void quickSortRecursion(int, int);
bool sortCheck(int, bool);
void printElements();
int swap(int, int);
void randArrayFill();
char randChar();
void getValue();

//  I'm going to modify this to sort arrays of double and then possibly strings
//  have now made it work for doubles. on to strings. the question is, how would I sort them?

//  idea for sorting strings - create a randomized string of random length (within some bounds) 
//  since character values have a numerical value associated with them, add up the values of the characters in a given string and store them in 
//  an array of structs, with the structs containing a space for a string and a space for an int

#define STRINGS 20

#define SIZE 20

typedef struct 
{
    char string[SIZE];
    unsigned int value;
} stringAndValue;
    
stringAndValue manyStrings [STRINGS];

int main (void){   

    randArrayFill();

    getValue();

    printElements();

    quickSort(STRINGS);

    bool sorted = sortCheck(0, true);

    printElements();

    printf("The array of structs %ssorted %s", (sorted) ? "is " : "is NOT ", (sorted) ? "=)" : "=(");

    return 0;
}






void quickSort(int length){
    quickSortRecursion(0, length - 1);
}






void quickSortRecursion(int leftMostIndex, int rightMostIndex){

    //  if the array to be sorted is size 1, or rightmostindex is less than leftmost index, return
    if (leftMostIndex == rightMostIndex || rightMostIndex < leftMostIndex)
    {
        return;
    }

    int pivotIndex = leftMostIndex + (rightMostIndex - leftMostIndex) / 2;

    //  put the left, pivot, and right elements in order 
    if (manyStrings[leftMostIndex].value > manyStrings[pivotIndex].value)
    {
        swap(leftMostIndex, pivotIndex);
    }
    if (manyStrings[pivotIndex].value > manyStrings[rightMostIndex].value)
    {
        swap(pivotIndex, rightMostIndex);
    }
    if (manyStrings[leftMostIndex].value > manyStrings[pivotIndex].value)
    {
        swap(leftMostIndex, pivotIndex);
    }

    //  for the case that the array subsect is smaller than 3 units
    if (rightMostIndex - leftMostIndex <= 2)
    {
        return;
    }

    //  put the pivot at the rightmost index
    swap(pivotIndex, rightMostIndex);
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
            if (manyStrings[i].value > manyStrings[pivotIndex].value)
            {
                break;
            }
        }

        for (; j >= leftMostIndex; j--)
        {
            rightPick = j;
            if (manyStrings[j].value < manyStrings[pivotIndex].value)
            {
                break;
            }
        }

        //  if the leftpick is earlier in the array than the rightpick
        //  swap the larger and smaller elements
        if (leftPick < rightPick)
        {
            swap(leftPick, rightPick);
        }
        else 
        {
            break;
        }
    }

    //  after the partition, swap the pivot and the leftpick
    //  and set the pivot to the position of leftpick
    
    swap(leftPick, pivotIndex);
    pivotIndex = leftPick;
    
    //  recursively run the algorithm until it's all sorted
    quickSortRecursion(leftMostIndex, pivotIndex - 1);
    quickSortRecursion(pivotIndex + 1, rightMostIndex);
}



void printElements () {

    //  simple algo to print elements in the array chosen

    printf("\n");

    for (int i = 0; i < STRINGS; i++){
        printf("%s\n", manyStrings[i].string);
    }
}




int temp;

int *pTemp = &temp;

char tempC[SIZE];

int swap (int index1, int index2)
{   
    //  move the "value" to its new home
    *pTemp = manyStrings[index1].value;
    manyStrings[index1].value = manyStrings[index2].value;
    manyStrings[index2].value = *pTemp;

    //  move the string to its new home
    strcpy(tempC, manyStrings[index1].string);
    strcpy(manyStrings[index1].string, manyStrings[index2].string);
    strcpy(manyStrings[index2].string, tempC);
    return 0;
}




bool sortCheck(int start, bool helpful){

    //simple algo to check if array elements are sorted 

    int i = start;
    
    while ((manyStrings[i].value <= manyStrings[i + 1].value))
    {
        i++;
    }
    
    
    if ((helpful) && (i < STRINGS))
    {
        printf("\nsortCheck stopped at element %d of %d", i, STRINGS);
        printf("\ni     String: %s\nValue: %d", manyStrings[i].string, manyStrings[i].value);
        printf("\ni + 1 String: %s\nValue: %d\n", manyStrings[i + 1].string, manyStrings[i + 1].value);
        sortCheck(i + 1, helpful);
    }


    


    return i == STRINGS;
}







void randArrayFill()
{
    //create a random number seed based on the current time
    srand(time(0));

    //  counter
    int i = 0;

    //random string size
    int k;

    char l;
    //fill the array with random chars determined by randChar
    while (i < STRINGS)
    {
        k = rand() % SIZE;
        for (int j = 0; j < k; j++)
        {
            manyStrings[i].string[j] = randChar();
        }
        i++;
    }
}

char randChar ()
{
    double a;
    a = ((double) rand()) / RAND_MAX;
    a *= (127 - 33);
    a += 33;
    a = (char) a;
    return a;
}






void getValue ()
{
    for (int i = 0; i < STRINGS; i++)
    {
        manyStrings[i].value = 0;
        for (int j = 0; manyStrings[i].string[j] != '\0'; j++)
        {
            manyStrings[i].value += manyStrings[i].string[j];
        }
    }
}