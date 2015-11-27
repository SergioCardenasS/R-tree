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

struct Node* find(struct Node* n,struct Tuple *t)
{
    
    if (!within_bounding(t,n->my_box))
        return NULL;
    int i;
    if (n->leaf)
    {
        struct Node_h *p = (struct Node_h*)(n->my_nodes);
        for(i=0 ; i<n->size ; i++)
            if (compare_tuple(p->values[i],t))
                return n;
        return NULL;    
    }else{
        struct Node* q;
        struct Node_nh *p = (struct Node_nh*)(n->my_nodes);
        for(i=0 ; i<n->size ; i++)
        {
            q = find(p->values[i],t);
            if (q)
                return q;
        }
        return NULL;
    }
}

/*el nodo a es el nodo sobrecargado y el nodo b recibira la mitad de sus elementos,
se debe actualizar los bboxes de ambos nodos con 'updatebox'*/
void split(struct Node *a,struct Node* b)
{
    pick_seed(a);
    if(a->leaf)
    { 
        struct Node_h *p = (struct Node_h*)(a->my_nodes);
        struct Node *tmp = create_node(a->leaf);
        insert_node(b,p->values[M_]);
        p->values[M_] = NULL;
        a->size -= 1;
        translate_node(a,tmp); 
        updatebox(a);
        updatebox(b);
        updatebox(tmp);
        while (a->size < m_)
        {
            pick_next(tmp,a);
            pick_next(tmp,b);
        }
        while (tmp->size > 0)
            pick_next_xor(tmp,a,b);
        delete_node(tmp);    
    }
    else
    {
        struct Node_nh *p = (struct Node_nh*)(a->my_nodes);
        struct Node *tmp = create_node(a->leaf);       
        insert_node(b,p->values[M_]);
        p->values[M_] = NULL;
        a->size -= 1;
        translate_node(a,tmp); 
        updatebox(a);
        updatebox(b);
        updatebox(tmp);
        while (a->size < m_)
        {
            pick_next(tmp,a);
            pick_next(tmp,b);
        }
        while (tmp->size > 0)
            pick_next_xor(tmp,a,b);
        delete_node(tmp);
    }    
}

//crear nuevo root y agregar p y q, y actualizar bbox del nuevo root
void create_newroot(struct Rtree *t,struct Node* q)
{
    struct Node* p = t->root;
    struct Node *n;
    n = create_node(0);
    t->root = n;
    insert_node(n,p);
    insert_node(n,q);
    updatebox(n);
}

void delete_root(struct Rtree *t)
{
    if (t->root->size == 1 && !t->root->leaf)
    {   
        struct Node *p = t->root;
        struct Node_nh *q = (struct Node_nh*)(p->my_nodes);
        t->root = q->values[0];
        q->values[0] = NULL;
        delete_node(p);
    }
}

//insercion en el arbol desde la raiz
void insert_tree(struct Rtree *t, data_type *d)
{ 
    struct Node *p;
    struct Node *q = NULL;
    struct Tuple *r = create_tuple(d);
    if (find(t->root,r))
    {
        delete_tuple(r);
        return;
    }
    p = choose_leaf(t->root,r); 
    insert_node(p,r);       //p es nodo hoja

    int key = 0; //nos dice si hubo un split anterior
    
    while (p != NULL)
    {
        if (key)
            insert_node(p,q);
            
        if (p->size > M_)
        {   
            q = create_node(p->leaf);
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
        create_newroot(t,q);
}

void re_insert(struct Rtree *t, struct Node* n)
{
    int i;
    if (n->leaf)
    {
        struct Node_h* p = (struct Node_h*)(n->my_nodes);
        for (i=0 ; i<n->size; i++)
            insert_tree(t,p->values[i]->values);
    }
    else
    {
        struct Node_nh *p= (struct Node_nh*) n->my_nodes;
        for (i=0 ; i<n->size; i++)
            re_insert(t,p->values[i]);
    }    
}

void delete_tree(struct Rtree *t, data_type *d)
{   
    if (t->root->size == 0)
        return;       
    struct Tuple *r = create_tuple(d);
    struct Node *p = find(t->root,r);
    struct Node *q;
    if (!p)
    {
        delete_tuple(r);
        return;
    }
    int key = 0; //nos dice si hubo un underflow
    delete_tuple_node(p,r);
    updatebox(p);
    while (p != NULL)
    {
        if (key)
        {
            delete_tuple_node(p,q);
            updatebox(p);
            re_insert(t,q);
            delete_node(q);         
        }
        if (p->size < m_) //underflow
        {   
            q = p;
            key = 1;
        }
        else
        {
            updatebox(p);
            key = 0;
        }        
        p = p->father;
    }
       
    if (key)
        delete_root(t);
}

