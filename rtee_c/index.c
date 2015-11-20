#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>
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

void printTuples(struct Tuple *points,data_type size){
	printf("(");
    int i;
    for (i=0;i<Dim;i++){
   		printf("%d,", points->values[i]);
   	}
   	printf(")");
}

void print(struct Node* p,int level){
    if (p->leaf){
        int j;
    	for (j=0;j<level;j++){
            printf("     ");
        }
        printf("[" );
    	int i=0;
        for (i=0;i<p->size;i++){
        	printTuples( ((struct Node_h*)p->my_nodes)->values[i],p->size);
        	printf(",");
        }
        printf(" size: %d,",p->size);
        printf(" type: leaf");
        printf("]\n" );


        return;
    }
    else{
    	int j;
        for (j=0;j<level;j++){
                printf("     ");
        }
        printf("%s\n","RECTANGULO" );
    	int i;
        for(i=0;i<M_;i++){
            print(((struct Node_nh*)p->my_nodes)->values[i],level+1);
        }
    }
}//recibe el root
void show_all(struct Node *n)
{


}


//=============================================================================
int main (int argc, char* argv[])
{
    struct Rtree *tree;
    tree = create_rtree();
    data_type dim = 2;
    data_type a[]={1,1};
    data_type b[]={2,2};
    insert_tree(tree,a);
    insert_tree(tree,b);

    data_type c[]={3,3};
    insert_tree(tree,c);

    data_type d[]={4,4};
    insert_tree(tree,d);

    data_type e[]={5,5};
    insert_tree(tree,e);


    print(tree->root,0);
    destroy_rtree(tree);
    return 0;
}
