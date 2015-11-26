struct Tuple
{
    data_type values[Dim];
};

// devuelve un puntero a una estructura tupla
struct Tuple * create_tuple (data_type *a)
{
    struct Tuple *t;
    t = (struct Tuple*)malloc(sizeof(struct Tuple));
    assert(t);
    int i;
    for (i=0 ; i<Dim ;i++)
    {
        t->values[i]=a[i];
    }

    return t;
}

void delete_tuple(struct Tuple *a)
{
    if( a != NULL )
        free(a);
}

// calculo de la distancia para valores numericos, vectores
dist_type distance(struct Tuple *a, struct Tuple *b)
{
    int i;
    dist_type sum = (dist_type) 0;
    for (i=0 ; i<Dim ; i++)
        sum += pow((dist_type)(a->values[i] - b->values[i]), 2);
    sum = sqrt(sum);
    return sum;
}

bool compare_tuple(struct Tuple *a, struct Tuple *b)
{
    int i;
    for (i=0 ; i<Dim ; i++)
        if(a->values[i]!=b->values[i])
            return 0;
    return 1;
}

