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
void split(struct Node *a,struct Node* b)
{
    data_type b_size = (int)(a->size/2);
    struct Tuple *q;
    struct Node *c;
    int i,j;
    if(a->leaf)
    {
        b = create_node(a->leaf);
        for(i=b_size+1;i<a->size;++i)
        {
            q = ((struct Node_h*)(a->my_nodes))->values[i];
            insert_node(b,q);
            ((struct Node_h*)(a->my_nodes))->values[i] = NULL;
        }
    }
    else
    {
        b = create_node(a->leaf);
        for(i=b_size+1;i<a->size;++i)
        {
            c = ((struct Node_nh*)(a->my_nodes))->values[i];
            insert_node(b,c);
            ((struct Node_nh*)(a->my_nodes))->values[i] = NULL;
        }
    }
    a->size = a->size - b->size;
    printf("%d \n",b->size);
    for (i=0;i<b->size;i++)
    {
        for (j=0;j<Dim;j++)
        {
            printf("%d,", ((struct Node_h*)b->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n");
    printf("%d \n",a->size);
    for (i=0;i<a->size;i++)
    {
        for (j=0;j<Dim;j++)
        {
            printf("%d,", ((struct Node_h*)a->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n");
    updatebox(a);
    updatebox(b);
}

//crear nuevo root y agregar p y q, y actualizar bbox del nuevo root
void create_newroot(struct Rtree *t,struct Node* p,struct Node* q)
{

    struct Node *n = t->root;
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
