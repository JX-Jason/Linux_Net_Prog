#include "clntcmd.h"
#include <fcntl.h>
#include "data.h"
char para[MAXLINE];
//返回需要参数的命令的参数
//如 cd ， get ， put
char *get_para(char *buf, int n){
  memset(para, 0, sizeof(para));
  int j=0;
  for(int i=n; i<strlen(buf) - 1 ; i++)
    para[j++] = buf[i];
  //para[j] = '\0';
  return para;
}
//上传文件
int ftp_get_put(int sockfd, char *cmd)
{
  char send[FILEBUFSIZE];
  int filefd;
  int sendsize;
  char returnbuf[10];
  if( (filefd = open(get_para(cmd, 4), O_RDONLY)) == -1)
    return err("ftp_get_put open error\n");

  puts("open file success!");

  //告诉服务器准备接受数据
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_put write error\n");

  puts("ready to send file\n");

  //准备传输数据
  while( (sendsize = read(filefd, send, FILEBUFSIZE)) > 0){
    //memcpy(send, &sendsize, sizeof(int));
    if( (write(sockfd, send, FILEBUFSIZE)) < 0){
      close(filefd);
      return err("put file error\n");
    }
    memset(send, 0, sizeof(send));
  }
  close(filefd);
  printf("upload file success\n");
  return 0;
}


int ftp_get_get(int sockfd, char *cmd)
{
  char *filename = get_para(cmd, 4);
  char recv[FILEBUFSIZE];
  char filepath[MAXLINE];
  int filesize;
  int filefd, flag;
  char recv_path[MAXLINE] = "./recvftp";
  //当前目录下还没有创建recv_path这个文件夹
  if(!opendir(recv_path))
    mkdir("recvftp", 0777);

  strcat(filepath, recv_path);
  strcat(filepath, "/");
  strcat(filepath, filename);

  if(write(sockfd, cmd, strlen(cmd)) < 0)
    return err("ftp_get_get write error\n");

  if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
  {
    return err("ftp_get_get open file error\n");
  }
  int num;
  while( (num = (read(sockfd, recv, MAXLINE))) >= 0)
  {
      printf("read num : %d \n", num);
        if( (write(filefd, recv, num)) < 0)
        {
          close(filefd);
          return err("ftp_get_get write to file error\n");
        }
  }        
  printf("file received success\n");
  return 0;
}