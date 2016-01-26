#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
class A
{
public:
    int data[100];
    A()
    {
        //cout<<"construct"<<endl;
        for (int i = 0; i < 100; i++) data[i] = i;
    }
    ~A()
    {
        //cout<<"destruct"<<endl;
    }
};

class Base {
    virtual void func() {
        cout<<"Base"<<endl;
    }
};
class Derived{
    virtual void func() {
        cout<<"derive"<<endl;
    }
};

int main(int argc,char* argv[])
{
    /*int n = 100000;
    //for(;;)
    {
        int * p = new int[n];
        for (int i = 0; i < n; i++) p[i] = i;
        //A *p  = new A;
        //for (int i = 0; i < n; i++) 
        //int *ip = (int *)p;
        //cout<<*(ip-1)<<endl;
        //delete []p;
        //cout<<p->data[9]<<endl;
        //delete (A *)p;
        A *t  = (A *) (p+1);
        delete[] t;
    }*/
    return 0;
}
