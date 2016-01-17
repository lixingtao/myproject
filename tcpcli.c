#include <unistd.h>
#include	"unp.h"

int
main(int argc, char **argv)
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
    FILE *fp = fopen("in.dat", "r");
    int pre_timestamp = 12300000;
	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Writen(sockfd, sendline, strlen(sendline));
        usleep(1000000);
        tmp = read(sockfd, recvline, MAXLINE);
        printf("%s tmp=%d\n", recvline, tmp);

		/*if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

		Fputs(recvline, stdout);*/
	}

	exit(0);
}
