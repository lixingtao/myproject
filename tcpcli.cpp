#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "slide_window.h"
extern "C" {
#include	"unp.h"
}
using namespace std;
using namespace SlideWindow;


int explode(char s[], std::vector<std::string> &vet, char split_ch = '\t') { //'\t' 分割字符川
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

void trim(char s[]) {
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
}

int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
    int tmp;
    int pre_timestamp;
    int timestamp;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	//servaddr.sin_port = htons(12387);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	char sendline[MAXLINE], recvline[MAXLINE];
    //FILE *fp = fopen("lighttpdsorted", "r");
    FILE *fp = fopen("error", "r");
    pre_timestamp = 1530000000;
	while (Fgets(sendline, MAXLINE, fp) != NULL) {

        Request req;
        trim(sendline);
        vector<string> ret;
        explode(sendline, ret);
        if (ret.size() < 6) continue;
        req.time = atoi(ret[0].c_str()); req.ip = ret[1]; req.uid = ret[2];
        req.preurl = ret[3]; req.qt = ret[4]; req.pn = atoi(ret[5].c_str());

        timestamp = req.time;
        //sleep(max(0, timestamp - pre_timestamp));
        usleep(100000);
        pre_timestamp = timestamp;

        Writen(sockfd, sendline, sizeof(sendline));
        tmp = read(sockfd, recvline, MAXLINE);
        printf("%s tmp=%d\n", recvline, tmp);

        /*if (Readline(sockfd, recvline, MAXLINE) == 0)
          err_quit("str_cli: server terminated prematurely");

          Fputs(recvline, stdout);*/
    }

    exit(0);
}
