#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Base{
    int n;
public:
    Base(int t) {
        n = t;
        cout<<"Base"<<endl;
    }
    Base(const Base &base) {
        n = base.n;
        cout<<"Base reference"<<endl;
    }
};

template <typename T>
void print_vet(vector<T> vet) {
    for (int i = 0; i < vet.size(); i++) cout<<vet[i]<<endl;
    cout<<endl;
}

int main(int argc, char *argv[])
{
    /*istream_iterator<int> in_iter(cin);
    istream_iterator<int> eof;
    vector<int>vet(in_iter, eof);*/
    /*ostream_iterator<string> out_iter(cout, "\n");
    istream_iterator<string> in_iter(cin), eof;
    while(in_iter != eof) {
        *out_iter++ = *in_iter++;
    }*/
    //print_vet(vet);
    string line = "first,middle,last";
    string ::reverse_iterator rcomma = find(line.rbegin(), line.rend(), ',');
    cout<<string(line.rbegin(), rcomma)<<endl;
    //cout<<string(rcomma, line.rbegin())<<endl;
    cout<<string(rcomma.base(), line.end()) <<endl;

    return 0;
}
