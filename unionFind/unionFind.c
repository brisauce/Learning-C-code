#include <stdio.h>

/*
Time to make a union-find algorithm, which, starting out, I have no idea how to do. I have never set up a tree data structure before. Time to learn!
*/

size_t find (size_t[], size_t);
void Union (size_t[], size_t, size_t);
void findReplace (size_t[], size_t, size_t);
void findCompress (size_t[], size_t);

#define PARENTSIZE 50

int main ()
{
    //  using __SIZE_MAX__ to define the root, as The Book requested
    size_t parent [PARENTSIZE] = {__SIZE_MAX__, 0, 1, 1, 1, 2, 2};

    parent[40] = (size_t) __SIZE_MAX__;
    parent[41] = (size_t) 40;
    parent[42] = (size_t) 41;
    parent[43] = (size_t) 42;
    parent[44] = (size_t) 40;

    Union(parent, 3, 43);
    
    for (size_t i = 0; i < 7; i++)
    {
        printf("%ld\n", parent[i]);
    }
    puts("\n");
    for (size_t i = 40; i <= 44; i++)
    {
        printf("%ld\n", parent[i]);
    }
    
    

    return 0;
}




void findCompress (size_t parent[], size_t startNode)
{
    //  find the root of the given tree
    size_t destRoot = find(parent, startNode);

    //  set all nodes in parent[] to connect directly to the root
    for (size_t i = 0; i < PARENTSIZE; i++)
    {
        if ((find(parent, startNode) == destRoot) && (i != destRoot))
        {
            parent[i] = destRoot;
        }
    }
}



void findReplace (size_t parent[], size_t destNode, size_t startNode)
{
    //  if the current node doesn't already point to destnode, recursively call the function to the node it points to
    //  and make the current node point to the desired destination
    //  if the node is a root, make it point to the destnode
    if (parent[startNode] == __SIZE_MAX__)
    {
        parent[startNode] = destNode;
        return;
    }
    else if (startNode == destNode)
    {
        return;
    }

    findReplace(parent, destNode, parent[startNode]);
    parent[startNode] = destNode;
}




//  lowercase "u" union means something else in C. The Book says I will learn about this later
void Union (size_t parent[], size_t destNode, size_t set2Node)
{

    //  this version sets all nodes of both trees to point to the root of the destination tree
    size_t destRoot = find(parent, destNode);
    size_t set2Root = find(parent, set2Node);

    findReplace(parent, destNode, set2Node);
    findCompress(parent, destNode);

    parent[set2Root] = destRoot;
}




size_t find (size_t parent[], size_t node)
{
    //  if the parent of the current node is the size max macro, return current node
    if (parent[node] == __SIZE_MAX__)
    {
        return node;
    }
    else
    {
        //  else, find the node referenced in the element
        find(parent, parent[node]);
    }
}