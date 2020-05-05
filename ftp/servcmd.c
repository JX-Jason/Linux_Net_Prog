#include<stdio.h>
#include "servcmd.h"
#include<fcntl.h>
#include<dirent.h>
#include<string.h>

#define FILEBUFSIZE 1024

char re[MAXLINE];
DIR *dp;
struct dirent *entry;
int n;


// int ftp_put_cd(char *para, data *mdata){

//   printf("para = %s\n", para);
//   char *current_path = mdata->current_path;
//   char temp[MAXLINE];
//   int flag = 0;
//   int indeep = mdata->indeep;
//   // if( (strcmp(para, ".")) == 0)
//   //   return 0;
//   if( (strcmp(para, "..")) == 0){
//     printf("int strcmp\n");
//     printf("indeep = %d\n", indeep);
//     if(indeep != 0){

//       // printf("deal\n");

//       indeep--;
//       memset(temp, 0, sizeof(temp));
//       int i = strlen(current_path)-1;
//       for(; i>=0; i--){
//         if(current_path[i] == '/')
//           break;
//       }
//       for(int j=0; j<i; j++)
//         temp[j] = current_path[j];

//       strcpy(current_path, temp);
//     }
//   }
//   else{
//     indeep++;
//     //判断para这个文件夹存不存在
//     dp = opendir(current_path);
//     while( (entry = readdir(dp)) != NULL){
//       if( (strcmp(entry->d_name, para) == 0) && (entry->d_type & DT_DIR)){
//         flag = 1;
//         break;
//       }
//     }
//     if(flag){
//       strcat(current_path, "/");
//       strcat(current_path, para);
//     }
//     else{
//       //当前目录没有这个文件
//       int num = -1;
//       memcpy(mdata->rebuf, &num, sizeof(int));
//       strcat(mdata->rebuf, "no such file or file not a dir\n");
//       mdata->size = strlen("no such file or file not a dir\n") + sizeof(int);
//       return 1;
//     }
//   }
//   printf("%s\n", current_path);
//   int num = 1;
//   memcpy(mdata->rebuf, &num, sizeof(int));
//   mdata->indeep = indeep;
//   return 1;
// }
  //接收客户端传来的文件
int put_do(data *mdata)
{
	char filename[MAXLINE];
	char filepath[MAXLINE];
	int filefd;
	int filesize;
	int i,j;
	memset(filename, 0, sizeof(filename));

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
	
	strcat(filepath, mdata->current_path);
	//strcat(filepath, "/");
	strcat(filepath, filename);

		
	//调用O_TRUNC 新上传文件会覆盖旧文件
	if( (filefd = open("tmp.txt", O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
	{
		return err("put_do open file error\n");
	}
	else
	{
		write(filefd, (mdata->rebuf) + i + 1, FILEBUFSIZE - i - 1);

		while (read(mdata->fd, mdata->rebuf, FILEBUFSIZE) >= 0)
		{
		if( (write(filefd, mdata->rebuf, FILEBUFSIZE)) < 0)
		{
			close(filefd);
			return err("put_do write to file error\n");
		}
		} 
	}
	close(filefd);
	return 1;

}
//向客户端传送文件
int get_do(data *mdata)
{  
	char send[FILEBUFSIZE];
	int filefd;
	int sendsize, len;
	char filepath[MAXLINE];
	char filename[MAXLINE];
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
	//strcat(filepath, "/");
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
		if( (write(mdata->fd, send, FILEBUFSIZE)) < 0)
		{
			close(filefd);
			return err("put file error\n");
		}
		memset(send, 0, sizeof(send));
	}
	return 0;
}

void ls_do(data *mdata)
{
	char *current_path = mdata->current_path;
	memset(re, 0, sizeof(re));
	if( (dp = opendir(current_path)) != NULL){
		while( (entry = readdir(dp)) != NULL){
		if( (strcmp(entry->d_name, ".") == 0) || \
			(strcmp(entry->d_name, "..") == 0) )
			continue;
		strcat(re, entry->d_name);
		strcat(re, "\t");
		}
		strcat(re, "\n");
	}
	strcpy(mdata->rebuf, re);
	mdata->size = strlen(re);
}
