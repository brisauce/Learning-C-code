#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>



double magnitude (double vector[]);
double vectorMult (double vector1[], double vector2[]);
void matrixMult (int m1rows, int m1cols, int m2rows, int m2cols, int resRows, int resCols, int matrix1[m1rows][m1cols], int matrix2[m2rows][m2cols], int result[resRows][resCols]);
void printMatrix(int x, int y, double matrix[x][y], int bracket);
void printVector(double vector[]);
void gaussElim (int x, int y, double matrix[x][y]);
int isTriForm (int x, int y, double matrix[x][y], int RRechelon);
void intPrintMatrix(int x, int y, int matrix[x][y], int bracket); 







//  write a program which does vector to vector products
//  done
//  write a program that does matrix multiplication
//  done
//  make the matrix multiplication program work for matrixes of any size (allow for vector to vector and matrix to vector products)
//  done
//  implement gaussian elimination

char const * const truth[2] =
{
    "false",
    "true"
};


int main (void) 
{
    
    double vector1 [2] = 
    {
        2, 3
    };
    double vector2 [2] = 
    {
        55, 6
    };
    

    puts("vector1:");

    printVector(vector1);

    printf("vector1's magnitude: %-5.2lf\n", magnitude(vector1));

    puts("\nvector2:");

    printVector(vector2);

    printf("vector2's magnitude: %-5.2lf", magnitude(vector2));

    printf("\n\nvector1 * vector2 = %6.2lf\n\n", vectorMult(vector1, vector2));

    

    //  for the matrix multiplication to work properly, matrix1 must be size [n][p] and matrix2 [p][m]

    int matrix1[4][3] =
    {
        {10, 11, 12},
        {13, 14, 15},
        {16, 17, 18},
        {19, 20, 21}
    };

    int matrix2[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    

    
    int m1Rows = sizeof(matrix1) / sizeof(matrix1[0]);
    int m1Cols = sizeof(matrix1[0]) / sizeof(matrix1[0][0]);

    int m2Rows = sizeof(matrix2) / sizeof(matrix2[0]);
    int m2Cols = sizeof(matrix2[0]) / sizeof(matrix2[0][0]);

    int resRows = m1Rows, resCols = m2Cols;

    int result[resRows][resCols];
    
    
    puts("\nmatrix1:");
    intPrintMatrix(m1Rows, m1Cols, matrix1, 0);
    puts("\nmatrix2:");
    intPrintMatrix(m2Rows, m2Cols, matrix2, 0);
    
    
    matrixMult(m1Rows, m1Cols, m2Rows, m2Cols, resRows, resCols, matrix1, matrix2, result);

    puts("\nresult of multiplying above matrices:");

    intPrintMatrix(resRows, resCols, result, 1);
    

    double gaussElimMatrix[3][4] =
    {
        {2, 1,-1, 8},
        {-3, -1, 2, -11},
        {-2, 1, 2, -3}
    };

    int geRows = sizeof(gaussElimMatrix) / sizeof(gaussElimMatrix[0]);
    int geCols = sizeof(gaussElimMatrix[0]) / sizeof(gaussElimMatrix[0][0]);

    puts("gaussElimMatrix BEFORE it is operated on");
    printMatrix(geRows, geCols, gaussElimMatrix, 1);
    gaussElim(geRows, geCols, gaussElimMatrix);
    int finished_eliminating = isTriForm(geRows, geCols, gaussElimMatrix, 1);

    if (finished_eliminating == 1)
    {
        puts("\ngaussElim ran successfully");
    }
    else
    {
        puts("\ngaussElim was interrupted");
        return 1;
    }
    
    puts("\nafter gaussElim:");
    printMatrix(geRows, geCols, gaussElimMatrix, 1);

    return 0;
}








void gaussElim (int x, int y, double matrix[x][y])
{
    const short int X1before = x - 1;
    const short int Y1before = y - 1;
    int first_half_iterations = 0;
    int second_half_iterations = 0;
    size_t Ypick;
    size_t Xpick;
    size_t elimPick;
    int Xoffset;

    while (1)
    {
        /*
        *
        *   the idea is to get the matrix to a point where its in triangle form here
        * 
        */
        Xpick = __SIZE_MAX__;
        Ypick = __SIZE_MAX__;
        elimPick = __SIZE_MAX__;
        Xoffset = 1;

        /*
        *
        *   starting from the bottom leftmost element, check to see if the particular element is zero going up through the matrix
        * 
        */
        for (size_t i = 0; (i < y) && (Xpick == __SIZE_MAX__); i++)
        {
            for (int j = X1before; j >= Xoffset; j--)
            {
                if (matrix[j][i])
                {
                    Xpick = j;
                    Ypick = i;
                    break;
                }
            }
            Xoffset++;
        }

        /*
        *
        *   find the next row without a zero in that place and make that the elimpick
        * 
        */
        for (int i = Xpick - 1; i >= 0; i--)
        {
            if (matrix[i][Ypick])
            {
                elimPick = i;
                break;
            }
        }

        /*
        *
        *   after you have found the pick for elimination, perform the matrix algebra which will eliminate the term
        * 
        */
        if (elimPick == __SIZE_MAX__)
        {
            break;
        }
        

        double * coeff = (double*) malloc(1);
        *coeff = matrix[Xpick][Ypick] / matrix[elimPick][Ypick];

        if (*coeff < 0)
        {
            /*
            *
            *   if there is a negative proportion between the pick and elim, make the coefficient positive and add
            *   elim row * coeff to the pick row
            *
            */
            *coeff *= -1;
            for (size_t i = 0; i < y; i++)
            {
                matrix[Xpick][i] += (matrix[elimPick][i] * *coeff);
            }
        }
        else
        {
            /*
            *
            *   otherwise, subtract (elim row * coeff) from the picked row
            *
            */
            for (size_t i = 0; i < y; i++)
            {
                matrix[Xpick][i] -= (matrix[elimPick][i] * *coeff);
            }
        }

        free(coeff);        
        first_half_iterations++;
    }
    /*
    *
    *  logic for reduced row echelon form
    *
    * 
    *  the idea here is essentially to do what you did to get the program into triangle form, but from a different perspective
    *  make sure to ensure your variables (in the equation which is now a matrix element) are ending up being one at the end of the calculation
    * 
    */
    while (1)
    {
        Xpick = __SIZE_MAX__;
        Ypick = __SIZE_MAX__;
        elimPick = __SIZE_MAX__;
        //  find a non zero element in the upper portion of the matrix, starting from the top right element one index away from the rightmost element
        Xoffset = 0;
        for (int i = y - 2; (i > 0) && (Xpick == __SIZE_MAX__); i--)
        {
            Xoffset++;
            for (size_t j = 0; j < (x - Xoffset); j++)
            {
                if (matrix[j][i])
                {
                    Xpick = j;
                    Ypick = i;
                    break;
                }
            }
        }


        if(Xpick == __SIZE_MAX__)
        {
            break;
        }

        /*
        *
        *  as before, find a suitable elimpick
        *  find the next row without a zero in that place and make that the elimpick
        * 
        */
        
        for (int i = Xpick + 1; i < x; i++)
        {
            if (matrix[i][Ypick])
            {
                elimPick = i;
                break;
            }
        }

        

        double * coeff = (double*) malloc(1);
        *coeff = matrix[Xpick][Ypick] / matrix[elimPick][Ypick];

        if (*coeff < 0)
        {
            /*
            *
            *   if there is a negative proportion between the pick and elim, make the coefficient positive and add
            *   elim row * coeff to the pick row
            *
            */
            *coeff *= -1;
            for (size_t i = 0; i < y; i++)
            {
                matrix[Xpick][i] += (matrix[elimPick][i] * *coeff);
            }
            
        }
        else
        {
            /*
            *
            *   otherwise, subtract (elim row * coeff) from the picked row
            *
            */
            for (size_t i = 0; i < y; i++)
            {
                matrix[Xpick][i] -= (matrix[elimPick][i] * *coeff);
            }
        }

        free(coeff);        
        second_half_iterations++;
    }
    /*
    *
    *   now that the matrix has been reduced, make each of the coefficients 1 by dividing the coefficient's row
    *   by the coefficient
    *
    */
    for (size_t i = 0; i < x; i++)
    {
        if (matrix[i][i] != 1.0)
        {
            double * coeff = (double*) malloc(1);
            *coeff = matrix[i][i];
            for (size_t j = 0; j < y; j++)
            {
                matrix[i][j] /= *coeff;
            }
            free(coeff);
        }
    }

    printf("\nfirst_half_iterations = %d", first_half_iterations);
    printf("\nsecond_half_iterations = %d", second_half_iterations);
    return;
}







int isTriForm (int x, int y, double matrix[x][y], int RRechelon)
{
    int Yoffset;

    if (RRechelon)
    {
        if (!isTriForm(x, y, matrix, 0))
        {
            printf("\nMake sure the matrix is in triangle form first before determining its reverse row echelon status!");
            return 0;
        }
        
        //  checks if a given matrix is in reduced row echelon form by looping through all spaces which should be 0
        Yoffset = 1;
        int Xoffset = 1;
        for (int i = y - 2; i > 0; i--)      
        {
            for (size_t j = 0; j < x - Xoffset; j++)
            {
                if (matrix[j][i])
                {
                    //  if a space which should be 0 is not 0, return 0
                    return 0;
                }
            }
            Xoffset++;
        }
        //  if it loops through without issue, return 1
        return 1;
    }
    else
    {
        //  checks if a given matrix is in triangle form by looping through all spaces which should be 0
        Yoffset = 2;
        for (int i = (x - 1); i > 0; i--)
        {
            for (size_t j = 0; j < y - Yoffset; j++)
            {
                //  if a space which should be 0 is not 0, return 0
                if (matrix[i][j])
                {
                    //printf("\nisTriForm ended at matrix[%d][%d] = %.2lf", i, j, matrix[i][j]);
                    return 0;
                }
            }
        Yoffset++;
        }
        //  if it loops through without issue, return 1
        return 1;
    }
}








void matrixMult (int m1rows, int m1cols, int m2rows, int m2cols, int resRows, int resCols, int matrix1[m1rows][m1cols], int matrix2[m2rows][m2cols], int result[resRows][resCols])
{
    if (m1cols != m2rows)
    {
        double s = (double) m1cols / m2rows;
        printf("\nMatrix must be size: matrix1[n][p] matrix2[p][m]");
        printf("\nSystem is instead in form matrix1[n][p] matrix2[%.1lfp][m]", s);
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < resRows; i++)
    {
        //  i is the counter for the vertical component of the result matrix and vertical component for the first matrix
        for (size_t j = 0; j < m1rows; j++)
        {   
            //  j is the counter for the horizontal component of the result and the second matrix
            sum = 0;
            for (size_t k = 0; k < m2rows; k++)
            {
                //  k is the counter for the horizontal component of the first matrix and the vertical component of the second matrix    
                sum += matrix1[i][k] * matrix2[k][j];
            }
            //  after the sum is totalled, add it to the result matrix
            result[i][j] = sum;
        }
    }
}







double vectorMult (double vector1[], double vector2[])
{
    // one method of multiplying vectors is to multiply the x, then y coordinates of a vector and add them together to make a scalar
    double a = vector1[0] * vector2[0];
    double b = vector1[1] * vector2[1];
    return a + b;
}








void printVector(double vector[])
{
    //  prints vector
    for (size_t i = 0; i < 2; i++)
    {
        printf("%-6.2lf ", vector[i]);
    }
    puts("");
}








double magnitude (double vector[])
{
    //  the magnitude of a vector can be found by using the distance equation on its x and y coordinates
    double x = vector[0] * vector[0];
    double y = vector[1] * vector[1];

    return sqrt(x + y);
}








void printMatrix(int x, int y, double matrix[x][y], int bracket)
{
    //  prints matrix
    puts("");
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < y; j++)
        {
            //  creates cool looking brackets!!!
            if (bracket)
            {
                if ((i == 0) && (j == 0))
                {
                    printf("/  ");
                }
                else if ((j == 0) && i < (x - 1))
                {
                    printf("|  ");
                }
                else if ((j == 0) && (i == x - 1))
                {
                    printf("\\  ");
                }
            }
            
            
            printf("%6.2lf ", matrix[i][j]);

            // brackets!!! :DDDDDD

            if (bracket)
            {
                if ((i == 0) && (j == (y - 1)))
                {
                    printf("  \\");
                }
                else if ((j == (y - 1)) && i < (x - 1))
                {
                    printf("  |");
                }
                else if ((j == (y - 1)) && (i == x - 1))
                {
                    printf("  /");
                }
            }
        }
        printf("\n");
    }
}





void intPrintMatrix(int x, int y, int matrix[x][y], int bracket)
{
    //  prints matrix
    puts("");
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < y; j++)
        {
            //  creates cool looking brackets!!!
            if (bracket == 1)
            {
                if ((i == 0) && (j == 0))
                {
                    printf("/  ");
                }
                else if ((j == 0) && i < (x - 1))
                {
                    printf("|  ");
                }
                else if ((j == 0) && (i == x - 1))
                {
                    printf("\\  ");
                }
            }
            
            
            printf("%3d ", matrix[i][j]);

            // brackets!!! :DDDDDD

            if (bracket == 1)
            {
                if ((i == 0) && (j == (y - 1)))
                {
                    printf("  \\");
                }
                else if ((j == (y - 1)) && i < (x - 1))
                {
                    printf("  |");
                }
                else if ((j == (y - 1)) && (i == x - 1))
                {
                    printf("  /");
                }
            }
        }
        printf("\n");
    }
}