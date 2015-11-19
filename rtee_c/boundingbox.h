
struct Bounding_box     
{
    data_type min_boundary[Dim];
    data_type max_boundary[Dim];
};


struct Bounding_box * create_bbox ()
{
    struct Bounding_box *b;
    b = (struct Bounding_box*)malloc(sizeof(struct Bounding_box));
    assert(b);
    int i;
    for (i=0 ; i<Dim ;i++)
    {
        b->min_boundary[i] = 0;
        b->max_boundary[i] = 0;
    }    
    return b;
}

void delete_bbox (struct Bounding_box *b)
{
    if( b != NULL )
        free(b);
}


int within_bounding (struct Tuple *a, struct Bounding_box *b) 
{    
    int i;
    for (i=0 ; i< Dim; i++)
        if ( (a->values[i] < b->min_boundary[i]) || (a->values[i] > b->max_boundary[i]) )
            return 0;
    return 1;
}







