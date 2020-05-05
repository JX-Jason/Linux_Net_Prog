#include<stdio.h>
#include "servcmd.h"
#include<fcntl.h>
#include<dirent.h>
#include<string.h>

#define SENDFILESIZE 1024
char buf[MAXLINE];
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

int ftp_put_put(char *para, data *mdata)
{
  int sockfd = mdata->fd;
  char *current_path =  mdata->current_path;
  //接收客户端传来的文件
  memset(re, 0, sizeof(re));
  char filename[MAXLINE];
  char filepath[MAXLINE];
  int filefd;
  int filesize;

  for(int i=strlen(para)-1; i>=0; i--){
    if(para[i] == '/'){
      for(int j=0; i<strlen(para); j++)
        filename[j] = para[++i];
      break;
    }
  }

  strcat(filepath, current_path);
  strcat(filepath, "/");
  strcat(filepath, filename);

  // printf("para = %s\n", para);

  //调用O_TRUNC 新上传文件会覆盖旧文件
  if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0){
    return err("ftp_put_put open file error\n");
  }
  else{
    // printf("create file success\n");
    // printf("ready 4 read\n");
    while(read(sockfd, buf, SENDFILESIZE) >= 0){
      memcpy(&filesize, buf, sizeof(int));

      printf("filesize = %d\n", filesize);

      if( (write(filefd, buf+sizeof(int), filesize)) < 0){
        close(filefd);
        return err("ftp_put_put write to file error\n");
      }

      //文件接收完毕
      if(filesize < (SENDFILESIZE-sizeof(int)))
        break;
    }
  }
  close(filefd);
  return 1;

}

// int ftp_put_get(char *para, data *mdata){
//   // printf("this is ftp_put_get\n");

//   //向客户端传送文件
//   init();
//   char send[SENDFILESIZE];
//   int filefd;
//   int sendsize, len;
//   char filepath[MAXLINE];
//   int sockfd;
//   char *current_path = mdata->current_path;
//   strcat(filepath, current_path);
//   strcat(filepath, "/");
//   strcat(filepath, para);

//   // printf("serverfilepath = %s\n", filepath);

//   sockfd = mdata->fd;

//   if( (filefd = open(filepath, O_RDONLY)) == -1){
//     int flag = -1;
//     memcpy(mdata->rebuf, &flag, sizeof(int));
//     strcat(mdata->rebuf, "not exist this file\n");
//     mdata->size = sizeof(int) + strlen("not exist this file\n");
//     // printf("mdata->rebuf = %s\n", mdata->rebuf);

//     return 1;
//   }
//   // printf("open file success\n");

//   //准备传输数据
//   while( (sendsize = read(filefd, (send+2*sizeof(int)), (SENDFILESIZE-2*sizeof(int)))) > 0){

//     // printf("sendsize = %d\n", sendsize);

//     //头标志，告知这是正常数据
//     int flag = 1;
//     memcpy(send, &flag, sizeof(int));
//     memcpy(send+sizeof(int), &sendsize, sizeof(int));
//     if( (write(sockfd, send, SENDFILESIZE)) < 0){
//       close(filefd);
//       return err("put file error\n");
//     }
//     memset(send, 0, sizeof(send));
//   }

//   return 0;
// }

 

void ls_do(data *mdata)

{

  // printf("this is ls\n");
  char *current_path = mdata->current_path;
  // printf("current_path = %s\n", current_path);

  memset(re, 0, sizeof(re));
  // int num = 0;
  if( (dp = opendir(current_path)) != NULL){
    while( (entry = readdir(dp)) != NULL){
      // printf("readdir\n");
      if( (strcmp(entry->d_name, ".") == 0) || \
        (strcmp(entry->d_name, "..") == 0) )
        continue;
      // num++;
      // printf("entry->d_name = \"%s\"\n", entry->d_name);
      strcat(re, entry->d_name);
      strcat(re, "\t");
    }
    // if(num)
      strcat(re, "\n");
  }
  strcpy(mdata->rebuf, re);
  mdata->size = strlen(re);
}
