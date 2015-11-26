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


//bool find_element2(struct Tuple *r,struct Node *tmp)
//{
//    int i,j;
//    printf("Estamos buscando el elemento: ");
//        for(i =0;i<Dim;++i)
//        {
//            printf("%d -> ",r->values[i]);
//        }
//        printf("\n");
//    if(tmp)
//    {
//            /// Hacemos la busqueda 2, para eso tenemos que buscar en todo el arbol hasta llegar los hijos y preguntar
//            /// si esta el dato. para eso el primer if quwe si es de tipo hijo pregunte si existe el dato
//        if(tmp->leaf)
//        {
//            for(i=0;i<tmp->size;++i)
//            {
//                for(j =0;j<Dim;++j)
//                {
//                    printf("%d -> ",((struct Node_h*)(tmp->my_nodes))->values[j]);
//                }
//                printf("\n");
//
////                if(((struct Node_h*)(tmp->my_nodes))->values[i] == r)
////                    return 1;
//            }
//        }
//        else
//        {
//                /// Pero si es de tipo padre, este tendra que preguntar si tiene hijos y entras a cada uno de ellos,
//                /// Lo que necesitamos en este caso es un puntero el padre, no al hijo por lo tanto vamos a crear
//                /// 2 temporales uno que sea el puntero al padre (el que queremos y otro al hijo).
//            printf("Es de tipo padre\n");
//            struct Node* m_temp=tmp;
//            struct Node* r_temp=tmp;
//            for(i=0;i<tmp->size;++i)
//            {
//                tmp=((struct Node_nh*)(tmp->my_nodes))->values[i];
//                if(within_bounding(r,tmp->my_box))
//                {
//                    printf("Esta dentro %d\n",i);
//                    r_temp=tmp;
//                    if(find_element2(r,tmp))
//                        return 1;
//                }
//                tmp=m_temp;
//            }
//            tmp=r_temp;
//        }
//    return 0;
//    }
//}


//bool find_element2(struct Node* p,struct Tuple *r)
//{
//    int j,i;
//    if(p)
//    {
//        if (p->leaf)
//        {
//            printf("%d \n",p->size);
//            for (i=0;i<p->size;i++)
//            {
//                for (j=0;j<Dim;j++){
//                    printf("%d,", ((struct Node_h*)p->my_nodes)->values[j]);
//                }
//                for (j=0;j<Dim;j++){
//                    printf("%d,", r->values[j]);
//                }
//                if(compare_tuple(((struct Node_h*)p->my_nodes)->values[i],r));
//                    return 1;
//            }
//        }
//        else
//        {
//            for(i=0;i<p->size;i++)
//            {
//                find_element2(((struct Node_nh*)p->my_nodes)->values[i],r);
//            }
//        }
//        return 0;
//    }
//}
//bool find_element(struct Rtree *t,data_type *elem)
//{
//    struct Node* tmp = t->root;
//    struct Tuple *r = create_tuple(elem);/// Construccion del elemento ingresado a bloque.
//    return find_element2(tmp,r);
//}
/*el nodo a es el nodo sobrecargado y el nodo b recibira la mitad de sus elementos,
se debe actualizar los bboxes de ambos nodos con 'updatebox'*/
void split(struct Node *a,struct Node* b)
{
    pick_seed(a);
    //int i;
    //int j;/*
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
    
    /*
    printf("%d \n",b->size);
    for (i=0 ; i<b->size ; i++)
    {
        for (j=0 ; j<Dim ; j++)
        {
            printf("%d,", ((struct Node_h*)b->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n");
    printf("%d \n",a->size);
    for (i=0 ; i<a->size ; i++)
    {
        for (j=0;j<Dim;j++)
        {
            printf("%d,", ((struct Node_h*)a->my_nodes)->values[i]->values[j]);
        }
        printf(" - ");
    }
    printf("\n ================================================================== \n");*/
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

//insercion en el arbol desde la raiz
void insert_tree(struct Rtree *t, data_type *d)
{ 
    struct Node *p;
    struct Node *q = NULL;
    struct Tuple *r = create_tuple(d);
/*    if (find(t->root,r))
    {
        delete_tuple(r);
        return;
    }*/
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


