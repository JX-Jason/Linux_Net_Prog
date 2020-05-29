#include<stdio.h>
#include "servcmd.h"
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include <errno.h>
#define FILEBUFSIZE 1024

char re[MAXLINE];
DIR *dp;
struct dirent *entry;
int n;
typedef int ssize_t;
ssize_t                        /* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;        /* and call read() again */
            else
                return(-1);
        } else if (nread == 0)
            break;                /* EOF */

        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);        /* return >= 0 */
}
/* end readn */

ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t        n;

    if ( (n = readn(fd, ptr, nbytes)) < 0)
        err("readn error");
    return(n);
}
ssize_t                        /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;        /* and call write() again */
            else
                return(-1);            /* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        err("writen error");
}
//接收客户端传来的文件
int put_serv(data *mdata)
{
	char filename[MAXLINE];
	char filepath[MAXLINE];
	int filefd;
	int filesize;
	int i,j;
	int read_num = 0;
	memset(filename, 0, sizeof(filename));
	memset(filepath, 0, sizeof(filepath));
	for(i = 4; i<strlen(mdata->rebuf); i++)
	{
		if(mdata->rebuf[i] == '\n')
		{		  
			break;
		}			
		else
		{
			filename[j++] = mdata->rebuf[i];
		}        
	}  
	if(!opendir("./servRecv"))
		mkdir("servRecv", 0777);	
	strcat(filepath, mdata->current_path);
	strcat(filepath, "servRecv/");
	strcat(filepath, filename);		
	//调用O_TRUNC 新上传文件会覆盖旧文件
	if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
	{
		return err("put_serv open file error\n");
	}
	else
	{
		if( (write(filefd, mdata->rebuf + i + 1, (mdata->size - i - 1))) < 0)
		{
			close(filefd);
			return err("put_serv write to file error\n");
		}		
		while (1)
		{
			read_num = read(mdata->fd, mdata->rebuf, FILEBUFSIZE);
			printf("read_num : %d \n", read_num);
			if(read_num < 0 && errno == EAGAIN)
			{
				break;
			}
			if( (write(filefd, mdata->rebuf, read_num)) < 0)
			{
				close(filefd);
				return err("put_serv write to file error\n");
			}
			memset(mdata->rebuf, 0, sizeof(mdata->rebuf));	
		}
		
	}
	close(filefd);
	return 1;

}
//向客户端传送文件
int get_serv(data *mdata)
{  
	char send[FILEBUFSIZE];
	int filefd;
	int sendsize, len;
	char filepath[MAXLINE];
	char filename[MAXLINE];
	memset(filepath, 0, sizeof(filepath));
	memset(filename, 0, sizeof(filename));
	int j;
	for(int i = 4; i<strlen(mdata->rebuf); i++)
	{
		if(mdata->rebuf[i] == '\n')
		{		  
			break;
		}        
		else
		{
			filename[j++] = mdata->rebuf[i];
		}        
	}  

	strcat(filepath, mdata->current_path);
	strcat(filepath, filename);

	if( (filefd = open(filepath, O_RDONLY)) == -1)
	{
		// int flag = -1;
		// memcpy(mdata->rebuf, &flag, sizeof(int));
		// strcat(mdata->rebuf, "not exist this file\n");
		// mdata->size = sizeof(int) + strlen("not exist this file\n");
		return 1;
	}
	//准备传输数据
	while( (sendsize = read(filefd, (send), (FILEBUFSIZE))) > 0)
	{
		printf("sendsize: %d\n", sendsize);
		if( (write(mdata->fd, send, sendsize)) < 0)
		{
			close(filefd);
			return err("put file error\n");
		}
		memset(send, 0, sizeof(send));
	}
	return 0;
}

void ls_serv(data *mdata)
{
	char *current_path = mdata->current_path;
	memset(re, 0, sizeof(re));
	if( (dp = opendir(current_path)) != NULL){
		while( (entry = readdir(dp)) != NULL){
		if( (strcmp(entry->d_name, ".") == 0) || \
			(strcmp(entry->d_name, "..") == 0) )
			continue;
		strcat(re, "-->");
		strcat(re, entry->d_name);
		strcat(re, "\n");
		}
	}
	strcpy(mdata->rebuf, re);
	mdata->size = strlen(re);
}
