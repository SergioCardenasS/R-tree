#include <iostream>
#include "R-tree.h"

using namespace std;

typedef vector<int> vec;
int main()
{
    vec points;
    RTree<vec,3,2> tree;
    int op,num1,num2;
    while(1)
    {
        cout<<"1.- Ingrese sus puntos"<<endl;
        cin>>op;
        if(op == 0)
            return 0;
        cout<<"Ingrese sus puntos x , y : ";
        cin>>num1;
        cin>>num2;
        points.push_back(num1);
        points.push_back(num2);
        tree.insert_element(points);
        points.clear();
    }

}
