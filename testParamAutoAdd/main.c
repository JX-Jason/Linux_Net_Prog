#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#define BUF_SIZE 1024
//后来经过一上午的调试发现是lanuch.json里面cwd设置出错了：正确的应该是设置成"cwd": "${fileDirname}",
//而我设置成了${workspacedir}，所以出现相对路径读取不到文件，而绝对路径却可以读取到
int ftp_get_put(int sockfd, const char *cmd)
{
    int filefd;
    if( (filefd = open(cmd, O_RDONLY)) == -1)
        return err("ftp_get_put open error\n");
}
int main(int argc, char **argv)
{
    char message[30];
    int clntfd = 5;
    puts("Please Input:::::::: put aaa.txt:");
    fgets(message, BUF_SIZE, stdin);
    
    if(message[0] == 'p'&& message[1] == 'u'&& message[2] == 't')
    {
        char filename[strlen(message)-5]; 
        strncpy(filename, message + 4, strlen(message)-5);  
        ftp_get_put(clntfd, filename);                     
    } 
    return 0;   
}