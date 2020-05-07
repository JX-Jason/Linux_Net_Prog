#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "clntcmd.h"
#define BUF_SIZE 1024
 
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
    char message[BUF_SIZE];

    if(argc != 3)
    {
        printf("Usage: %s <IP> <Port>\n",argv[0]);
        exit(1);
    }

    clntfd = socket(AF_INET,SOCK_STREAM,0);

    memset(&clntaddr,0,sizeof(clntaddr));

    clntaddr.sin_family = AF_INET;
    clntaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clntaddr.sin_port = htons(atoi("9000"));

	if(connect(clntfd, (struct sockaddr *)&clntaddr, sizeof(clntaddr)) == -1) {
		error_handling("connect() error!");
	}else {
		puts("Connected...");
	}

	while(1) {
		fputs("Please input cmd(Q to quit): ",stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "q\n") | !strcmp(message, "Q\n")) {
			break;
		}
        if((strncmp(message, "ls", 2)) == 0)
        {
            write(clntfd, "ls", 2);
            str_len = read(clntfd, message, BUF_SIZE-1);
            message[str_len] = 0;
            printf("[----ls----]:\n %s", message);
        }
        else if((strncmp(message, "put ", 4)) == 0)
        {
            if(strlen(message) < 5)
            {
                puts("client put error! Usage: put xxx.xxx");
                break;
            }
            else
            {
                put_clnt(clntfd, message);                      
            }
        }
        else if((strncmp(message, "get ", 4)) == 0)
        {
            if(strlen(message) < 5)
            {
                puts("client put error! Usage: get xxx.xxx");
                break;
            }
            else
            {
                get_clnt(clntfd, message);                      
            }
        }        


	}

    close(clntfd);

    return 0;
}