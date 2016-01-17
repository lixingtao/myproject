#include <iostream>
#include <cstring>
#include <unordered_map>
#include "configfile.h"
#include "configitem.h"
#include "mysqlopt.h"
#include "slide_window.h"
extern "C" {
#include "unp.h"
}

using namespace configlib; 
using namespace std;
using namespace SlideWindow;

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

int main(int argc, char *argv[])
{	
    //
    if (argc < 2) {
        cout<<"input configure file name"<<endl;
        return -1;
    }
    string conf_file = argv[1];

    //读取配置文件
    configfile g_File(conf_file);
    configitem<int> mysql_port(g_File, "mysql", "port", "", 0);
    configitem<std::string> mysql_ip(g_File, "mysql", "ip", "s=-", "Default");
    configitem<std::string> mysql_user(g_File, "mysql", "user", "s=-", "Default");
    configitem<std::string> mysql_password(g_File, "mysql", "password", "s=-", "Default");
    configitem<std::string> mysql_dbname(g_File, "mysql", "dbname", "s=-", "Default");

    configitem<int> server_port(g_File, "server", "port", "", 0);

    configitem<int> slidewindow_size(g_File, "slidewindow", "windowsize", "", 0);

    configitem<int> socket_maxbuflen(g_File, "socket", "maxbuflen", "", 0);
    configitem<int> socket_maxclientopen(g_File, "socket", "maxclientopen", "", 0);
    configitem<int> socket_listenq(g_File, "socket", "listenq", "", 0);
	g_File.read();


    //初始化，连接数据库
    Mysql *mysql = new Mysql();
    Mysql_conf *mysql_conf = new Mysql_conf();
    snprintf(mysql_conf->ip, 80, "%s", ((string)mysql_ip).c_str());
    snprintf(mysql_conf->user_name, 80, "%s", ((string)mysql_user).c_str());
    snprintf(mysql_conf->password, 80, "%s", ((string)mysql_password).c_str());
    snprintf(mysql_conf->db_name, 80, "%s", ((string)mysql_dbname).c_str());
    mysql_conf->port = (int)mysql_port;
    //cout<<mysql_conf->ip<<","<<mysql_conf->user_name<<","<<mysql_conf->password<<","<<mysql_conf->db_name<<","<<mysql_conf->port<<endl;
    if(mysql->init(mysql_conf)){
        cout<<"init error"<<endl;
        return 1;
    }

    //数据库操作
    char sql[sql_maxlen];
    snprintf(sql, sql_maxlen, "select * from test");
    vector<vector<string> > ret;
    mysql->select(sql, ret, 4);
    /*for (int i = 0; i < ret.size(); i++) {
        for (int j = 0; j < ret[i].size(); j++) {
            printf("%s\t", ret[i][j].c_str());
        }
        cout<<endl;
    }*/

    //创建套接字
	/*int					listenfd, connfd;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons((int)server_port);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    //监听
	Listen(listenfd, LISTENQ);

    char buf[sock_maxbuflen];
    ssize_t n;
    Slide_window m_window((int)slideWindow_size);
    for(;;) {
        socklen_t clilen = sizeof(cliaddr);
        //连接建立
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
        while ( (n = read(connfd, buf, sock_maxbuflen)) > 0) {
            trim(buf);
            vector<string> ret;
            mysql->explode(buf, ret);
            Request req;
            if (ret.size() < 5) continue;
            req.ip = ret[0]; req.uid = ret[1]; req.time = atoi(ret[2].c_str()); 
            req.reference = ret[3]; req.pn = atoi(ret[3].c_str());
            m_window.push_window(req);
            memset(buf, 0, sizeof(buf));
            m_window.print_window();
            strncpy(buf, "request successed", 100);
            Writen(connfd, buf, 100);
        }

        Close(connfd);		
        cout<<"this connect closed"<<endl;
    }*/
    unordered_map<string, Slide_window*> sw_map; //存储滑动窗口的map

    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    const int sck_mbuflen = (int)socket_maxbuflen;
    const int sck_mcliopen = (int)socket_maxclientopen;
    const int sck_listenq = (int)socket_listenq;
    char buf[sck_mbuflen];
    socklen_t clilen;
    struct pollfd client[sck_mcliopen];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    Listen(listenfd, sck_listenq);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < sck_mcliopen; i++) {
        client[i].fd = -1;
    }
    maxi = 0;

	for ( ; ; ) {
		nready = Poll(client, maxi+1, INFTIM);

		if (client[0].revents & POLLRDNORM) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif

			for (i = 1; i < sck_mcliopen; i++)
				if (client[i].fd < 0) {
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == sck_mcliopen)
				err_quit("too many clients");

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

        Request req;
		for (i = 1; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ( (n = read(sockfd, &req, sck_mbuflen)) < 0) {
					if (errno == ECONNRESET) { /*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
						Close(sockfd);
						client[i].fd = -1;
					} else
						err_sys("read error");
				} else if (n == 0) { /*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
					Close(sockfd);
					client[i].fd = -1;
				} else {
                    trim(buf);
                    vector<string> ret;
                    mysql->explode(buf, ret);
                    Request req;
                    if (ret.size() < 5) continue;
                    req.ip = ret[0]; req.uid = ret[1]; req.time = atoi(ret[2].c_str()); 
                    req.reference = ret[3]; req.pn = atoi(ret[3].c_str());

                    Slide_window *p_window;
                    if (sw_map.find(req.ip) == sw_map.end()) {
                        sw_map[req.ip] = new Slide_window(slidewindow_size);
                    }
                    p_window = sw_map[req.ip];

                    p_window->push_window(req);

                    p_window->print_window();
                    strncpy(buf, "request successed\n", sck_mbuflen);
                    printf("%s", buf);
                    Writen(sockfd, buf, strlen(buf));
                    memset(buf, 0, sizeof(buf));
                }

                if (--nready <= 0) break;		/* no more readable descriptors */
            }
        }
    }

    Close(listenfd);
    return 0;
}
