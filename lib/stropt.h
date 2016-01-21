#ifndef stropt
#define stropt
#include <map>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>


void trim(char s[]); 

int explode(char s[], std::vector<std::string> &vet, char split_ch = '\t'); 

#endif
