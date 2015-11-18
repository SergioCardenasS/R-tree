#ifndef RTREE_H_INCLUDED
#define RTREE_H_INCLUDED

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

/// Funcion para ordenar un vector de pair
bool myfunction (pair<int,int> i,pair<int,int> j)
{
    return ( (i.second < j.second) && (i.first < j.first) );
}

/// Block sera el unidad minima, es decir un bloque pequeño que sera un punto en el espacio D.
template<int D>
struct Block
{
    vector<int> point;
    Block()
    {
        for(int i=0;i<D;++i)
            point.push_back(0);
    }
    Block(vector<int> p)
    {
        for(int i=0;i<D;++i)
            point.push_back(p[i]);
    }
    void print_block()
    {
        for(int i=0;i<D;++i)
            cout<<point[i]<< " ";
        cout<<endl;
    }
    Block<D>& operator = (Block<D> obj)
    {
        for(int i=0;i<D;++i)
            (*this)[i]=obj[i];
        return (*this);
    }
    bool operator == (Block<D> obj)
    {
        for(int i=0;i<D;++i)
        {
            if((*this)[i]!=obj[i])
                return 0;
        }
        return 1;
    }
    int& operator [](int pos)
    {
        return point[pos];
    }
};

template<int D>
bool operator < (Block<D>& one,Block<D>& two)
{
    for(int i=0;i<D;++i)
    {
        if((one)[i]>(two)[i])
            return 0;
    }
    return 1;
}
/// Para el nodo se penso en:
/// 1) El nodo puede ser de 2 tipos : PADRE(Espacio) Y HIJO.
/// Entonces guardaremos posicion X y Y para el padre y ademas un vector de nodos.
/// Para el hijo guardaremos solamente el punto. (Aca guardaremos la data si es que la hay).
template<int size_,int D>
struct node
{
    bool type;
    int node_size;
    int tam; /// De elementos que hay en el nodo;
    node<size_,D>* bgn;
    /// Si es padre es 1
    Block<D> fi_point,se_point;
    vector<node<size_,D>*> f_nodes;
    /// Si es hijo es 0
    vector<Block<D>> s_nodes;
    ///Funciones y constructores:
    node(){tam=0;type = 0;node_size=size_;} /// inicializamos todas las variables.
    node(node<size_,D>* n){bgn = n;tam=0;type = 0;node_size=size_;} /// contructor del nodo begin
    bool in(Block<D>& X)
    {
        for(int i=0;i<D;++i)
        {
            if((X[i]>(fi_point)[i])or (X[i]<(se_point)[i]))
                return 0;
        }
        return 1;
    }
    void create_MBB()
    {
        Block<D> nmin,nmax;
        nmin = s_nodes[0];
        nmax = s_nodes[0];
        for(int i=0;i<D;++i)
        {
            for(int j=1;j<s_nodes.size();++j)
            {
                if((nmin)[i]>(s_nodes[j])[i])
                    (nmin)[i] = (s_nodes[j])[i];
                if(nmax[i]<(s_nodes[j])[i])
                    (nmax)[i] = (s_nodes[j])[i];
            }
        }
        fi_point = nmin;
        se_point = nmax;
    }
};

template<int D>
double eu_distance(Block<D> f,Block<D> s)
{
    double acum=0;
    for(int i=0;i<D;++i)
        acum+=pow(f[i]-s[i],2);
    return sqrt(acum);
}

template<typename P,int size_,int D>
class RTree
{
private:
    typedef node<size_,D> Node;
    typedef Block<D> block;
    typedef typename vector<Node>::iterator node_iter;
    typedef typename vector<block>::iterator block_iter;
    Node* root;
public:
    RTree()
    {
        root = new Node(nullptr);
    }
    bool find_element2(block& c_elem,Node** &tmp)
    {
        if(*tmp)
        {
            cout<<"Estamos buscando el elemnto: ";
            c_elem.print_block();
            cout<<" El de tipo: "<<(*tmp)->type<<endl;
            /// Hacemos la busqueda 2, para eso tenemos que buscar en todo el arbol hasta llegar los hijos y preguntar
            /// si esta el dato. para eso el primer if quwe si es de tipo hijo pregunte si existe el dato
            if(!(*tmp)->type)
            {
                for(int i=0;i<(*tmp)->s_nodes.size();++i)
                {
                    if((*tmp)->s_nodes[i]==c_elem)
                        return 1;
                }
            }
            else
            {
                /// Pero si es de tipo padre, este tendra que preguntar si tiene hijos y entras a cada uno de ellos,
                /// Lo que necesitamos en este caso es un puntero el padre, no al hijo por lo tanto vamos a crear
                /// 2 temporales uno que sea el puntero al padre (el que queremos y otro al hijo).
                cout<<"Si es PADRE"<<endl;
                Node** m_temp=tmp;
                Node** r_temp=tmp;
                for(unsigned int i=0;i<(*tmp)->f_nodes.size();++i)
                {
                    tmp=&((*tmp)->f_nodes[i]);
                    if((*tmp)->in(c_elem))
                    {
                        r_temp=tmp;
                        if(find_element2(c_elem,tmp))
                            return 1;
                    }
                    tmp=m_temp;
                }
                tmp=r_temp;
            }
            return 0;
        }
    }
    bool find_element(P elem)
    {
        Node** tmp=&root;
        block c_elem(elem); /// Construccion del elemento ingresado a bloque.
        int pos;
        return find_element2(c_elem,tmp,pos);
    }
    bool distan_r(block &c_elem,block a,block b)
    {
        double acum;
        for(int i=0;i<D;++i)
        {
            acum+=pow(a[i]-c_elem[i],2) + pow(b[i]-c_elem[i],2);
        }
        return sqrt(acum);

    }
    void choose_sub_tree(block& c_elem,Node** tmp)
    {
//        Node* aux;
//        int pos = (*tmp)->f_nodes.size()-1;
//        while(((*tmp)->type))
//        {
//            aux=(*tmp)->f_nodes[pos];
//            aux->s_nodes[0].print_block();
//            double dis = distan_r(c_elem,aux->fi_point,aux->se_point);
//            for(int i=1;i<(*tmp)->f_nodes.size();++i)
//            {
//                if(dis>distan_r(c_elem,(*tmp)->f_nodes[i]->fi_point,(*tmp)->f_nodes[i]->se_point))
//                {
//                    dis = distan_r(c_elem,(*tmp)->f_nodes[i]->fi_point,(*tmp)->f_nodes[i]->se_point);
//                    aux=(*tmp)->f_nodes[i];
//                }
//            }
//            tmp=&(aux);
//            cout<<" Datos : "<<endl;
//            (*tmp)->s_nodes[0].print_block();
//            (*tmp)->s_nodes[1].print_block();
//            break;

//        }
        Node* aux;
        while(((*tmp)->type))
        {
            aux=(*tmp)->f_nodes[0];
            for(int i=0;i!=(*tmp)->f_nodes.size();++i)
            {
                if(eu_distance(c_elem,aux->fi_point)>eu_distance(c_elem,(*tmp)->f_nodes[i]->fi_point))
                    aux=(*tmp)->f_nodes[i];
            }
            tmp=&(aux);
        }
        (*tmp)->s_nodes[0].print_block();
        (*tmp)->s_nodes[1].print_block();
    }

    void min_area(Node** tmp,vector<pair<int,int>> &distances)
    {
        block* bg = new block();
        int j,dis=100000;
        int dis_point;
        /// Encontraremos el punto mas cercano al origen para tomarlo como punto base
        for(int i=0;i<(*tmp)->s_nodes.size();++i)
        {
            dis_point = eu_distance((*tmp)->s_nodes[i],*bg);
            if(dis>dis_point)
            {
                dis = dis_point;
                j = i;
            }
        }
        (*tmp)->s_nodes[j].print_block();
        /// Crearemos un vector de distancias desde el punto mas cercano al origen al resto, para luego realizar un sort,
        /// Por su distancia, el pair es de <distancia, posicion dentro de vector de nodos>
        int aux_dis;
        for(int i=0;i<(*tmp)->s_nodes.size();++i)
        {
            aux_dis = eu_distance((*tmp)->s_nodes[i],(*tmp)->s_nodes[j]);
            distances.push_back(make_pair(aux_dis,i));
        }
        sort(distances.begin(),distances.end(),myfunction);
        int p = distances[1].second;
        cout<< distances[1].first<< " ";
        (*tmp)->s_nodes[p].print_block();
    }
//    void split2(Node*& nod)
//        {
//            if(nod)
//            {
//                if(nod->type)
//                {
//                    Node* aux = nod->f_nodes[0];
//                    for(int i=0;i!=nod->f_nodes.size();++i)
//                        split2(nod->f_nodes[i]);
//                    nod->MBB();
//                }
//                else
//                    nod->create_MBB();
//            }
//        }
    void split(Node** &tmp)
    {
        /// Creamos 2 nodos 2 de tipo hijo
        Node* f_node = new Node(*tmp);
        Node* s_node = new Node(*tmp);
        (*tmp)->type = 1;
        /// Luego vamos a dividir en 2 el tmp que es un nodo hijo (raiz) y pasamos a padre
        /// Tamaños de los 2 nodos, este puede varian dependiendo del split
        int size_s_node = (*tmp)->s_nodes.size()/2;
        int size_s_node2 = (*tmp)->s_nodes.size() - size_s_node;
        /// Hacer un buen split
        /// Primero con los puntos que tenemos, tenemos que crear un buen MBB o MBR
        vector<pair<int,int>> points;
        min_area(tmp,points);
        ///Ahora que tenemos los vectores de pairs , sacaremos los elementos dependiendo del size_s_node
        block aux;
        int des=1;
        for(int i=size_;i>=size_s_node2;--i)
        {
            int fac = points[i].second; /// posicion donde se encuntra el block dentro del temp
            cout<<"Estamos sacando: ";
            (*tmp)->s_nodes[fac].print_block();
            aux = (*tmp)->s_nodes[fac]; /// copiamos el bloque de la ultima posicion
            s_node->s_nodes.push_back(aux); /// Lo agregamos a nuestra nuevo Nodo
            /// Ahora eliminamos ese bloque del otro nodo
            (*tmp)->s_nodes.erase((*tmp)->s_nodes.end());
        }
        for(int i=0;i<size_s_node;++i)
        {
            int fac = points[i].second; /// posicion donde se encuntra el block dentro del temp
            cout<<"Estamos sacando: ";
            (*tmp)->s_nodes[fac].print_block();
            aux = (*tmp)->s_nodes[fac]; /// copiamos el bloque de la ultima posicion
            f_node->s_nodes.push_back(aux); /// Lo agregamos a nuestra nuevo Nodo
            /// Ahora eliminamos ese bloque del otro nodo
            (*tmp)->s_nodes.erase((*tmp)->s_nodes.end());
        }

        f_node->create_MBB();
        s_node->create_MBB();
        (*tmp)->s_nodes.clear();
        (*tmp)->f_nodes.push_back(f_node);
        (*tmp)->f_nodes.push_back(s_node);

        for(int j=0;j<root->f_nodes.size();++j)
        {
            cout<< "El hijo numero : "<<j+1<<endl;
            for(int i=0;i<root->f_nodes[j]->s_nodes.size();++i)
            {
                root->f_nodes[j]->s_nodes[i].print_block();
            }
        }
    }

    bool insert_element(P elem)
    {
        Node** tmp = &root;
        block c_elem(elem); /// La entrada es un vector, lo convertimos a un tipo block
        if(find_element2(c_elem,tmp))
        {
            cout<<"Ya existe el nodo"<<endl;
            return 0;
        }
        if(((*tmp)->type))
        {
            cout<<"Escogiendo un sub tree para : ";
            c_elem.print_block();
            choose_sub_tree(c_elem,tmp);
            cout<<" Datos : "<<endl;
            (*tmp)->s_nodes[0].print_block();
            (*tmp)->s_nodes[1].print_block();
        }
        (*tmp)->s_nodes.push_back(c_elem);
        if((*tmp)->s_nodes.size()>size_)
        {
            cout<<"Entrando a hacer split"<<endl;
            split(tmp);
        }
        //ssplit2(root);

    }
};
#endif // RTREE_H_INCLUDED
