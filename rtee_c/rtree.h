struct Rtree
{
    struct Node *root;
};

struct Rtree* create_rtree ()
{
    struct Rtree *t;
    t = (struct Rtree*)malloc(sizeof(struct Rtree));
    assert(t);
    t->root = create_node(1);
    return t;
}

void destroy_rtree(struct Rtree *t)
{
    if (t) 
        delete_node(t->root);    
}




/*el nodo a es el nodo sobrecargado y el nodo b recibira la mitad de sus elementos,
se debe actualizar los bboxes de ambos nodos con 'updatebox'*/
void split(struct Node *a,struct Node*b)
{
    
}

//crear nuevo root y agregar p y q, y actualizar bbox del nuevo root
void create_newroot(struct Rtree *t,struct Node* p,struct Node* q)
{
    struct Node *n;
    n = create_node(0);
    insert_node(n,p);
    insert_node(n,q);
    t->root = n;
    updatebox(n);
}

//insercion en el arbol desde la raiz
void insert_tree(struct Rtree *t, data_type *d)
{
    //struct Heap *my_heap;   //almace los nodos no hojas desde el root hasta el destino a insertar
    struct Node *p = t->root;
    struct Node *q = NULL;
    struct Tuple *r = create_tuple(d);
    
    p = choose_leaf(p,r); // p tiene el lugar donde es mejor insertar r    
    insert_node(p,r);
    int key = 0; //nos dice si hubo un split anterior
    
    if (p == t->root){
        if (p->size > M_)
        {
            split(p,q);
            key = 1;
        }           
        else        
            updateboxtuple(p,r);
    }
    while (p != t->root)
    {    
        if (key)
            insert_node(p,q);    
        if (p->size > M_)
        {
            split(p,q);
            key = 1;
        }           
        else
        {
            updateboxtuple(p,r);
            key = 0;
        }  
        p = p->father;        
    }
    if (key)
        create_newroot(t,p,q);
}


