struct Node;
void delete_node (struct Node *a);

//=====================================================================
struct Node_h                   //datos nodo hoja
{
    struct Tuple* values[M_+1];
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
    n->my_box = create_bbox();
    n->size = 0;
    n->father = NULL;
    return n;
};



void delete_node (struct Node *a)
{
    a->father = NULL;
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
        struct Tuple *p;
        p = (struct Tuple*)d;
        ((struct Node_h*)(n->my_nodes))->values[n->size] = p;

    }else{ //entonces d es un nodo
        struct Node *p;
        p = (struct Node*)d;
        ((struct Node_nh*)(n->my_nodes))->values[n->size] = p;
        p->father = n;
    }
    n->size +=1;
}

//actualizar tamaño de los limites
void updatebox(struct Node *n)
{
    struct Bounding_box *bb = n->my_box;
    int i,j;
    if (n->leaf)
    {
        struct Node_h* nod = (struct Node_h*)n->my_nodes;
        struct Tuple* tup = nod->values[0];
        for(i=0 ; i<Dim ; ++i)
        {
            bb->min_boundary[i] = tup->values[i];
            bb->max_boundary[i] = tup->values[i];
        }

        for(i=1 ; i<n->size ; ++i)
        {
            tup = nod->values[i];
            for(j=0 ; j<Dim ; ++j)
            {
                if(bb->min_boundary[j] > tup->values[j])
                    bb->min_boundary[j] = tup->values[j];
                else if (bb->max_boundary[j] < tup->values[j])
                    bb->max_boundary[j] = tup->values[j];
            }
        }
    }
    else
    {
        struct Node_nh* nod = (struct Node_nh*)n->my_nodes;
        struct Bounding_box *box = nod->values[0]->my_box;
        for(i=0 ; i<Dim ; ++i)
        {
            bb->min_boundary[i] = box->min_boundary[i];
            bb->max_boundary[i] = box->max_boundary[i];
        }

        for(i=1 ; i<n->size ; ++i)
        {
            box = nod->values[i]->my_box;
            for(j=0 ; j<Dim ; ++j)
            {
                if (bb->min_boundary[j] > box->min_boundary[j])
                    bb->min_boundary[j] = box->min_boundary[j];
                if (bb->max_boundary[j] > box->max_boundary[j])
                    bb->max_boundary[j] = box->max_boundary[j];
            }
        }
    }
}

//actualizar tamaño de los limites con rsepecto a un nodo
void updateboxtuple(struct Node *n, struct Tuple *a)
{
    struct Bounding_box *box = n->my_box;
    if (within_bounding(a,box))
        return;
    int i;
    for(i=0;i<Dim;++i)
    {
        if(a->values[i] < box->min_boundary[i])
            box->min_boundary[i] = a->values[i];
        else if(a->values[i] > box->max_boundary[i])
            box->max_boundary[i] = a->values[i];
    }
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


