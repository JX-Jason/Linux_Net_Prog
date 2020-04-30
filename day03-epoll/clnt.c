#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define BUF_SIZE 30
 
void error_handling(char *message)
{
    perror(message);
    exit(1);
}

int main(int argc,char **argv[])
{
    int clntfd;
    int str_len;
    struct sockaddr_in clntaddr;
    char message[30];

    if(argc != 3)
    {
        printf("Usage: %s <IP> <Port>\n",argv[0]);
        exit(1);
    }

    clntfd = socket(AF_INET,SOCK_STREAM,0);

    memset(&clntaddr,0,sizeof(clntaddr));

    clntaddr.sin_family = AF_INET;
    clntaddr.sin_addr.s_addr = inet_addr(argv[1]);
    clntaddr.sin_port = htons(atoi(argv[2]));

	if(connect(clntfd, (struct sockaddr *)&clntaddr, sizeof(clntaddr)) == -1) {
		error_handling("connect() error!");
	}else {
		puts("Connected...");
	}

	while(1) {
		fputs("Input message(Q to quit): ",stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "q\n") | !strcmp(message, "Q\n")) {
			break;
		}

		write(clntfd, message, strlen(message));
		str_len = read(clntfd, message, BUF_SIZE-1);
		message[str_len] = 0;
		printf("Message from server: %s \n", message);
	}

    close(clntfd);

    return 0;
}