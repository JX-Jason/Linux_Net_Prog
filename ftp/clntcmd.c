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
  para[j] = '\0';
  return para;
}
//上传文件
int ftp_get_put(int sockfd, char *cmd)
{
  char send[SENDFILESIZE];
  int filefd;
  int sendsize;
  
  if( (filefd = open(get_para(cmd, 4), O_RDONLY)) == -1)
    return err("ftp_get_put open error\n");

  puts("open file success!");

  //告诉服务器准备接受数据
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_put write error\n");

  puts("ready to send file\n");

  //准备传输数据
  while( (sendsize = read(filefd, (send+sizeof(int)), (SENDFILESIZE-sizeof(int)))) > 0){

    printf("sendsize = %d\n", sendsize);

    memcpy(send, &sendsize, sizeof(int));
    if( (write(sockfd, send, SENDFILESIZE)) < 0){
      close(filefd);
      return err("put file error\n");
    }
    memset(send, 0, sizeof(send));
  }
  close(filefd);
  printf("upload file success\n");
  return 0;
}


// int ftp_get_get(int sockfd, char *cmd)
// {
//   init();
//   char *filename = get_para(cmd, 4);
//   char filepath[MAXLINE];
//   int filesize;
//   int filefd, flag;

//   //当前目录下还没有创建recv_path这个文件夹
//   if(opendir(recv_path) == NULL)
//     mkdir("recvftp", 0777);

//   strcat(filepath, recv_path);
//   strcat(filepath, "/");
//   strcat(filepath, filename);

//   // printf("filepath = %s\n", filepath);

//   if(write(sockfd, cmd, strlen(cmd)) < 0)
//     return err("ftp_get_get write error\n");
//   else{
//     if(read(sockfd, buf, MAXLINE) >= 0){
//       //收到数据先判断是不是报错信息
//       memcpy(&flag, buf, sizeof(int));

//       // printf("buf = %s\n", buf);
//       // printf("flag = %d\n", flag);
//       if(flag == -1){
//         //返回的是出错信息
//         printf("%s\n", buf+sizeof(int));
//         return -1;
//       }
//       else{
//         // printf("filesize = %d\n", filesize);
//         // memcpy(&filesize, buf+sizeof(int), sizeof(int));
//         if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0){
//           return err("ftp_get_get open file error\n");
//         }

//         //循环接收文件
//         // printf("ready for receive file\n");

//         int con = 1;
//         while(con){

//           // printf("in while\n");

//           memcpy(&filesize, buf+sizeof(int), sizeof(int));

//           // printf("filesize = %d\n", filesize);

//           if( (write(filefd, buf+2*sizeof(int), filesize)) < 0){
//             close(filefd);
//             return err("ftp_get_get write to file error\n");
//           }

//           if(filesize < (SENDFILESIZE-2*sizeof(int))){
//             con = 0;
//             // printf("con = 0\n");
//           }
//           else
//             read(sockfd, buf, SENDFILESIZE);

//         }
//       }
//     }
//   }
//   printf("file received success\n");
//   return 0;
// }