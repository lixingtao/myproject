#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Test {
public:
    ~Test() {
        cout<<"destruct Test"<<endl;
    }
};

class Base{
public:
    int n;
    Base(int i) {
        n = i;
        cout<<"Base"<<endl;
    }
    Base(const Base &base) {
        n = base.n;
        cout<<"Base reference"<<endl;
    }
    ~Base() {
        cout<<"destruct Base"<<endl;
    }
    Test test;
};


template <typename T>
void print_vet(vector<T> vet) {
    for (int i = 0; i < vet.size(); i++) cout<<vet[i]<<endl;
    cout<<endl;
}


int main(int argc, char *argv[])
{
    Base base = 1;
    Base base2 = 2;
    return 0;
}
