/*
*
*   Prompt: 
*
*   The adjacency matrix of graph G is a matrix A that holds a value "true" or "false" if element A[i][j]
*   if ther is an arc from node i to node j.
*
*   Use an adjacency matrix to:
*   -   conduct a breadth-first search in graph G - DONE
*   -   find connected components - DONE (use BFS)
*   -   find a spanning tree - done by definition of what my BFS implementation does, spanning_tree() prints the 
*       connections of a viable spanning tree given an adjacency matrix
*
*   Use a distance matrix to find the shortest path between two nodes X and Y given as an input
*   
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMPTY_SPACE __SIZE_MAX__

size_t breadthFirstSearch (const size_t x, const size_t matrix[x][x], const size_t start_node, const size_t sought_node);
//  Breadth first search which finds a node in an adjacency matrix. returns the node if found and __SIZE_MAX__ if not

void printQueue(const size_t * queue, const size_t size);
//  prints the members of a given queue. prints "%" for empty queue spots. Can adjust outputs to reflect the actual value of nodes rather than 
//  numerical designation in the program if desired

bool inQueue (const size_t queue[], const int size, const int num);
//  determines if a number is present in a given queue

void clearQueue (size_t * queue, const size_t size);
//  sets all values in a queue to the value which represents an empty spot

void enqueue (size_t queue[], const size_t node, const size_t size);
//  appends a given node to the front of a queue

size_t dequeue (size_t queue[], const size_t size);
//  detaches a node from the end of a queue. returns node detached in this way

bool isEmpty (size_t queue[], const size_t size);
//  determines if a queue is empty

size_t peek (size_t queue[], const size_t size);
//  returns the next item in a queue to be dequeued

void spanningTree (const size_t x, size_t matrix[x][x]);
//  prints the nodes represented in an adjacency matrix with connections that could be constructed to make a hypothetical spanning tree
//  spanning tree visualization is left as an exercise to the user

void printMatrix(const size_t x, size_t matrix[x][x]);
//  prints a square matrix 

void printPath(size_t * path, size_t path_size, bool isNode);
//  prints the path taken by the shortestDistance function

size_t * shortestDistance (unsigned int matrix_size, const size_t distance_Matrix[matrix_size][matrix_size], unsigned int start_node, unsigned int sought_node, size_t * distance_ptr);
//  implementation of djikstra's algorithm, returns a path to the sought node in the form of a queue
//  returns distance travelled by reference through the size_t pointer passed to the function
//  will normally return a queue with the start node at the front of the queue and the sought node at the end of the queue
//  if a path is not possible because either node passed is outside the scope of the graph, returns a pointer to size_t = __SIZE_MAX__
//  if a path is not possible because the start node and sought nodes are the same node, returns a pointer to the start node
//  if a path is not possible because the nodes are not part of a connected graph, returns the path without the sought node at the end of the queue

void testing(void);





int main () 
{
    size_t adjacency_Matrix[6][6] =
    {
        {1, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 1},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0}
    };

    size_t distance_matrix[6][6] =
    {
        {EMPTY_SPACE, 10         , EMPTY_SPACE, EMPTY_SPACE, 55         , EMPTY_SPACE},
        {10         , EMPTY_SPACE, 39         , EMPTY_SPACE, 46         , EMPTY_SPACE},
        {EMPTY_SPACE, 39         , EMPTY_SPACE, 80         , EMPTY_SPACE, EMPTY_SPACE},
        {EMPTY_SPACE, EMPTY_SPACE, 80         , EMPTY_SPACE, 30         , 10         },
        {55         , 46         , EMPTY_SPACE, 30         , EMPTY_SPACE, EMPTY_SPACE},
        {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, 10         , EMPTY_SPACE, EMPTY_SPACE} 
    };

    size_t test_distance_matrix[6][6] =
    {
        {EMPTY_SPACE, 10         , 20         , EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE},
        {10         , EMPTY_SPACE, 30         , EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE},
        {20         , 30         , EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE},
        {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, 10         , 20         },
        {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, 10         , EMPTY_SPACE, 30         },
        {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, 20         , 30         , EMPTY_SPACE}
    };

    //printMatrix(6, distance_matrix);
    unsigned int start_node;
    unsigned int end_node;
    char buffer[10];

    printf("What node would you like to start at?\nStart at node : ");
    scanf("%d", &start_node);
    printf("\nyou chose to start at node %d", start_node);
    fgets(buffer, 10, stdin);
    printf("\nWhat node would you like to find?\nFind node : ");
    scanf("%d", &end_node);
    printf("\nyou chose to end at node %d",end_node);

    size_t distance_matrix_size = sizeof(distance_matrix) / sizeof(distance_matrix[0]);

    printMatrix(distance_matrix_size, test_distance_matrix);

    size_t * distance_to_node = calloc(1, sizeof(size_t));

    //  malloc with extra steps lol
    size_t * path_to_nodes = shortestDistance(distance_matrix_size, test_distance_matrix, start_node, end_node, distance_to_node);

    if ((path_to_nodes[0] == end_node) && (*distance_to_node != 0))
    {
        printf("\nFound path from %u to %u!\n", start_node, end_node);
        printf("Path covered %lu units\n", *distance_to_node);
        printf("path : \n");
        printPath(path_to_nodes, distance_matrix_size, false);
    }
    else if ((*distance_to_node == 0) && (path_to_nodes[0] == __SIZE_MAX__))
    {
        puts("\nTried to find a node outside the scope of the graph!");
        printf("Graph extends from node 0 to node %lu\n", distance_matrix_size - 1);
        printf("Tried to connect to node %u", (start_node > end_node) ? start_node : end_node);
    }
    else if ((*distance_to_node == 0) && (path_to_nodes[0] == start_node))
    {
        printf("\nPath :");
        printPath(path_to_nodes, distance_matrix_size, false);
        puts("Looked for a path to a node you started at! whoops!");
    }
    else if (path_to_nodes[0] == __SIZE_MAX__)
    {
        printf("\nPath :");
        printPath(path_to_nodes, distance_matrix_size, false);
        printf("\nPath from %u to %u not possible! graph may be unconnected!", start_node, end_node);
    }
    
    free(path_to_nodes);
    free(distance_to_node);
    
    return 0;
}





size_t * shortestDistance (unsigned int matrix_size, const size_t distance_Matrix[matrix_size][matrix_size], unsigned int start_node, unsigned int sought_node, size_t * distance_ptr)
{
    //  edge cases
    if (start_node == sought_node)
    {
        size_t * path = malloc(matrix_size * sizeof(size_t));
        clearQueue(path, matrix_size);
        path[0] = start_node;
        return path;
    }
    else if (start_node >= matrix_size || sought_node >= matrix_size)
    {
        size_t * path = malloc(matrix_size * sizeof(size_t));
        clearQueue(path, matrix_size);
        printQueue(path, matrix_size);
        return path;
    }
    
    
    //  dijkstra's algorithm 

    //  pretty easy to implement, it's on wikipedia
    size_t total_distance = 0;
    size_t minimum_distance_node;
    size_t current_node = start_node;

    size_t * path = malloc(matrix_size * sizeof(size_t));
    size_t * potential_arcs = malloc(matrix_size * sizeof(size_t));

    clearQueue(path, matrix_size);
    clearQueue(potential_arcs, matrix_size);

    printf("BFS path initialized with malloc then cleared with clearqueue :");
    printPath(path, matrix_size, false);
    
    while (true)
    {
        enqueue(path, current_node, matrix_size);

        for (size_t i = 0; i < matrix_size; i++)
        {
            //  looks for undiscovered arcs in adjacency matrix
            if ((distance_Matrix[current_node][i] != EMPTY_SPACE) && (! inQueue(path, matrix_size, i)))
            {
                if (i == sought_node)
                {
                    *distance_ptr = total_distance + distance_Matrix[current_node][i];
                    free(potential_arcs);
                    enqueue(path, i, matrix_size);
                    return path;
                }
                enqueue(potential_arcs, i, matrix_size);
            }
        }

        if (potential_arcs[0] == EMPTY_SPACE)
        {
            enqueue(path, __SIZE_MAX__, matrix_size);
            *distance_ptr = total_distance;
            free(potential_arcs);
            return path;
        }
        
        
        //  from the discovered arcs, find the one with the least distance to the next node
        
        minimum_distance_node = potential_arcs[0];
        
        for (size_t i = 1; i < matrix_size; i++)
        {
            if (potential_arcs[i] == EMPTY_SPACE)
            {
                break;
            }
            
            if (distance_Matrix[current_node][potential_arcs[i]] < distance_Matrix[current_node][minimum_distance_node])
            {
                minimum_distance_node = potential_arcs[i];
            }
        }
        total_distance += distance_Matrix[current_node][minimum_distance_node];
        current_node = minimum_distance_node;
        clearQueue(potential_arcs, matrix_size);
    }
}




void printPath(size_t * path, size_t path_size, bool isNode)
{
    bool isPath = false;
    puts("");
    for (int i = (path_size - 1); i >= 0; i--)
    {
        if (path[i] != EMPTY_SPACE || isPath)
        {
            if (!isPath)
            {
                isPath = true;
            }
            
            if (path[i] != __SIZE_MAX__)
            {
                printf("%lu ", (isNode) ? path[i] + 1 : path[i]);
            }
            else
            {
                printf("END");
            }
            
            if (i != 0)
            {
                printf("=> ");
            }
        }
    }
    if (!isPath)
    {
        printf("\nno path printed");
    }
    
    puts("");
}





void spanningTree (const size_t x, size_t matrix[x][x])
{
    /*
    *
    *   spanning tree drawing algorithm
    *   essentially a modified BFS
    *   since "find a spanning tree" is fairly open to interpretation I will print the parent and children to the terminal
    *   and draw the connections irl to see if this is making a spanning tree
    * 
    */

    size_t discovery_queue[x];
    size_t discovered_queue[x];
    size_t current_node;
    bool hasChildren;
    clearQueue(discovered_queue, x);
    clearQueue(discovery_queue, x);

    //  put the root in the discovery queue
    enqueue(discovery_queue, 0, x);

    do {
        current_node = dequeue(discovery_queue, x);
        hasChildren = false;
        //  add it to the discovered queue
        enqueue(discovered_queue, current_node, x);
        printf("\nParent node %lu ", current_node + 1);
        for (int i = 0; i < x; i++)
        {
            if ((matrix[current_node][i] == 1) && (! inQueue(discovery_queue, x, i)) && (! inQueue(discovered_queue, x, i)))
            {
                //  if the current element is not in the discovered queue or discovery queue and describes an edge between two nodes,
                //  add it to the discovery queue
                enqueue(discovery_queue, i, x);

                if (!hasChildren)
                {
                    printf("has child(ren)");
                }
                else
                {
                    printf(" and");
                }

                printf(" %d", i + 1);
                hasChildren = true;
            }
        }

        if (!hasChildren)
        {
            printf("does not have children.");
        }
        else
        {
            printf(".");
        }
                
        //  will loop until the discovery queue is empty
    } while (! isEmpty(discovery_queue, x));
}




size_t breadthFirstSearch (const size_t x, const size_t matrix[x][x], const size_t start_node, const size_t sought_node)
{
    /*
    *
    *   breadth first search
    *   find the shortest path to a node from a given node
    *   returns the node satisfying a given property
    *   if it doesn't successfully find the node, returns __SIZE_MAX__
    * 
    */

    size_t discovery_queue[x];
    size_t discovered_queue[x];
    size_t current_node;
    size_t steps = 0;
    clearQueue(discovered_queue, x);
    clearQueue(discovery_queue, x);

    //  put the start node in the discovery queue
    enqueue(discovery_queue, start_node, x);

    do {
        current_node = dequeue(discovery_queue, x);
        if (current_node == sought_node)
        {
            //  if the current head of the discovery queue is the node in question, return it
            return steps;
        }
        else
        {
            //  else, add it to the discovered queue
            enqueue(discovered_queue, current_node, x);
        }
        
        for (int i = 0; i < x; i++)
        {
            if ((matrix[current_node][i] == 1) && (! inQueue(discovery_queue, x, i)) && (! inQueue(discovered_queue, x, i)))
            {
                //  if the current element is not in the discovered queue or discovery queue and describes an edge between two nodes,
                //  add it to the discovery queue
                enqueue(discovery_queue, i, x);
            }
        }
        steps++;
        //  will loop until the discovery queue is empty
    } while (! isEmpty(discovery_queue, x));

    //  if the loop stops, meaning all nodes in the graph were explored, returns __SIZE_MAX__
    return __SIZE_MAX__;
}




size_t peek (size_t queue[], const size_t size)
{
    //  returns the next item in the queue to be dequeued

    for (int i = 0; i < size; i++)
    {
        if (queue[i + 1] == EMPTY_SPACE )
        {
            if (queue[i] != EMPTY_SPACE)
            {
                return queue[i];
            }
            else
            {
                break;
            }
        }
    }
    printf("\npeek failed to find an element");
    return __SIZE_MAX__;
}





bool isEmpty (size_t queue[], const size_t size)
{
    //  checks to see if a queue is empty
    for (size_t i = 0; i < size; i++)
    {
        if (queue[i] != EMPTY_SPACE)
        {
            return false;
        }   
    }
    return true;
}




size_t dequeue (size_t queue[], const size_t size)
{
    //  removes the element at the front of a queue from the queue and returns that element
    
    size_t dequeued;
    for (size_t i = 0; i < size; i++)
    {
        if ((queue[i + 1] == EMPTY_SPACE) || (i + 1 == size))
        {
            if (queue[i] != EMPTY_SPACE)
            {
                dequeued = queue[i];
                queue[i] = EMPTY_SPACE;
                return dequeued;
            }
            else
            {
                break;
            }
        }
    }

    printf("\nFunction dequeue failed to find an element to dequeue");
    return __SIZE_MAX__;
}





void enqueue (size_t queue[], const size_t node, const size_t size)
{
    //  adds an element to the back of a queue
    
    for (int i = (size - 2); i >= 0; i--)
    {
        queue[i + 1] = queue[i];
    }


    queue[0] = node;
}




void clearQueue (size_t * queue, const size_t size)
{
    //  sets all elements of a given queue to the empty space value
    for (size_t i = 0; i < size; i++)
    {
        queue[i] = EMPTY_SPACE;
        if (queue[i + 1] == EMPTY_SPACE)
        {
            return;
        }
    }
}




void printQueue(const size_t * queue, const size_t size)
{
    puts("");

    
    for (size_t i = 0; i < size; i++)
    {
        if (queue[i] == EMPTY_SPACE)
        {
            printf("%% ");
        }
        else 
        {
            printf("%u ", queue[i]);
        }
    }
    
    
    puts("");
}




bool inQueue (const size_t queue[], const int size, const int num)
{
    /*
    *
    *   if a given number is not in a queue, return true
    * 
    */
    for (int i = 0; i < size; i++)
    {
        if (queue[i] == num)
        {
            return true;
        }
        else if (queue[i] == EMPTY_SPACE)
        {
            break;
        }
    }
    return false;
}




void printMatrix(const size_t x, size_t matrix[x][x])
{
    puts("");
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < x; j++)
        {
            if (matrix[i][j] == EMPTY_SPACE || matrix[i][j] == 0)
            {
                printf("** ");
            }
            else
            {
                printf("%lu ", matrix[i][j]);
            }
        }
        puts("");
    }
}