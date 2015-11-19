#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"

#define M_ 4     //numero maximo de elementos por nodo
#define m_ 2     //numero minimo de elementos por nodo
#define Dim 2   //numero de dimensiones 
 
typedef int data_type;
typedef double dist_type;

#include "tuple.h"
#include "boundingbox.h"
#include "node.h"
#include "heap.h"
#include "rtree.h"


//recibe el root
void show_all(struct Node *n)
{
   
}


//=============================================================================
int main (int argc, char* argv[])
{  
    
    struct Rtree *tree;
    tree = create_rtree();
    data_type a[]={1,2};
    data_type b[]={4,2};
    insert_tree(tree,a);
    insert_tree(tree,b);
    
    show_all(tree->root);
    destroy_rtree(tree); 

    return 0;
}


