#ifndef __CLNTCMD__H
#define __CLNTCMD__H
#define FILEBUFSIZE 1024
int put_clnt(int sockfd, char *cmd);
int get_clnt(int sockfd, char *cmd);
#endif
