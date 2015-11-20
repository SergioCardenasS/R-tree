struct Node;
void delete_node (struct Node *a);

//=====================================================================
struct Node_h                   //datos nodo hoja
{
    struct Tuple *values[M_+1];
};

struct Node_h* create_nodeh()
{
    struct Node_h *n;
    n = (struct Node_h*)malloc(sizeof(struct Node_h));
    assert(n);
    int i;
    for (i=0 ; i <= M_ ; i++)
        n->values[i] = NULL;
    return n;
}

void delete_nodeh(struct Node_h* n)
{
    if (n)
    {
        int i;
        for (i=0 ; i<=M_ ; i++)
            delete_tuple(n->values[i]);
        free(n);
    }
}

//=============================================================================
struct Node_nh                   //datos nodo no hoja
{
    struct Node *values[M_+1];
};

struct Node_nh* create_nodenh()
{
    struct Node_nh *n;
    n = (struct Node_nh*)malloc(sizeof(struct Node_nh));
    assert(n);
    int i;
    for (i=0 ; i <= M_ ; i++)
        n->values[i] = NULL;
    return n;
}

//borrado es en cadena
void delete_nodenh(struct Node_nh* n)
{
    if (n)
    {
        int i;
        for (i=0 ; i<=M_ ; i++)
            delete_node(n->values[i]);
        free(n);
    }
}


//=============================================================================
struct Node
{
    int leaf;   //1 si es hoja
    struct Bounding_box *my_box;
    int size;
    void *my_nodes;
};

//inicializar nodo
struct Node * create_node(int l)
{
    struct Node *n;
    n = (struct Node*)malloc(sizeof(struct Node));
    assert(n);
    if (l)
        n->my_nodes = create_nodeh();
    else
        n->my_nodes = create_nodenh();
    n->leaf = l;
    n->my_box = NULL;
    n->size = 0;
    return n;
};



void delete_node (struct Node *a)
{
    if (a)
    {
        delete_bbox(a->my_box);
        if (a->leaf)
            delete_nodeh((struct Node_h*)a->my_nodes);
        else
            delete_nodenh((struct Node_nh*)a->my_nodes);
        free(a);
    }
}

//se puede insert un puntero a un nodo o puntero a tupla, depende del tipo de nodo (nodo->leaf?)
void insert_node (struct Node *n,void *d)
{
    if (n->leaf)
    {

    }else{

    }
}

//actualizar tamaño de los limites
void updatebox(struct Node *n)
{

}

//actualizar tamaño de los limites con rsepecto a un nodo
void updateboxtuple(struct Node *n, struct Tuple *a)
{

}

struct Node * choose_leaf(struct Node * n,struct Tuple *d)
{
    return NULL;
}
