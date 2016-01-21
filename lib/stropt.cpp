#include "stropt.h"

void trim(char s[]) {/*{{{*/
    if (!s) return;
    int cur = 0, i = 0;
    int len = strlen(s);

    i = len - 1;
    while(i>0 && '\n'==s[i]) s[i--] = 0;
    i = 0;
    while ('\n' == s[i]) ++i;
    for (; i < len; ++i) {
        s[cur++] = s[i];
    }
    s[cur] = 0;
}/*}}}*/

int explode(char s[], std::vector<std::string> &vet, char split_ch) { //'\t' 分割字符川
    //vet.clear();
    std::vector<std::string>().swap(vet);
    std::string str = "";
    for(int i=0; i < strlen(s); i++) {
        if(s[i] == split_ch) {
            vet.push_back(str);
            str = "";
        } else {
            str = str + s[i];
        }
    }

    vet.push_back(str);
    return vet.size();
}
