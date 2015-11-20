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
    struct Node *father;
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
    n->father = NULL;
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
    if (n->leaf) //entonces d es un tupla
    {
        struct Tuple *q = (struct Tuple*)d;
        ((struct Node_h*)(n->my_nodes))->values[n->size] = q;
        
    }else{ //entonces d es un nodo
        struct Node *q = (struct Node*)d;
        ((struct Node_nh*)(n->my_nodes))->values[n->size] = q;
        q->father = n;
    }
    n->size +=1;
}

//actualizar tamaño de los limites
void updatebox(struct Node *n)
{

}

//actualizar tamaño de los limites con rsepecto a un nodo
void updateboxtuple(struct Node *n, struct Tuple *a)
{

}

dist_type calculate_area(struct Bounding_box *b,struct Tuple *d)
{
    data_type mi[Dim];
    data_type ma[Dim];
    data_type dist[Dim];
    data_type result = 1.0;
    int i;
    for (i=0;i<Dim;i++)
    {
        if (b->min_boundary[i] < d->values[i])
            mi[i] = b->min_boundary[i];
        else
            mi[i] = d->values[i];
            
        if (b->max_boundary[i] > d->values[i])
            ma[i] = b->max_boundary[i];
        else
            ma[i] = d->values[i];
        
        dist[i] = ma[i]-mi[i];
    }
    
    for (i=0; i<Dim;i++)
        result *= dist[i];
    
    return result;
}

struct Node * choose_leaf(struct Node * n,struct Tuple *d)
{
    struct Node *p = n;
    int i,pos;  // pos es la posicion de nodo elejido
    int s;  // s numero de elemntos de ese nodo
    dist_type area,area2; //expansion del area
    while(!p->leaf)
    {        
        s = M_+1;
        area = -1.0;        
        for (i=0 ; i < p->size ; i++) //recorrido por todos los hijos
        {
            if (within_bounding (d,p->my_box))
            {
                if (s > p->size)
                {   
                    area = 0.0;
                    pos = i;
                    s = p->size;
                }                   
            }
        }
        if(area != 0.0)
        {
            for (i=0 ; i < p->size ; i++)
            {
                if (area == -1.0)
                {
                    area = calculate_area(p->my_box,d);
                    pos = i;
                    s = p->size;
                }   
                else{
                    area2 = calculate_area(p->my_box,d);
                    if (area2 < area)
                    {                        
                        area = area2;
                        pos = i;
                        s = p->size;
                    }else if(area2 == area && s > p->size)
                    {
                        area = area2;
                        pos = i;
                        s = p->size;
                    }
                }
                    
            }
        }
        p = ((struct Node_nh*)(p->my_nodes))->values[pos];
    }
    return p;
}


