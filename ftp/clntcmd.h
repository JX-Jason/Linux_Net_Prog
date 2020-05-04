#ifndef __CLNTCMD__H
#define __CLNTCMD__H
#define SENDFILESIZE 1024
int ftp_get_put(int sockfd, char *cmd);
int ftp_get_get(int sockfd, char *cmd);
#endif
