
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MAX 100
#define LEN 100

//shell指令单个管道结构体
struct cmd_list{  
    int argc;  //单个管道参数个数
    char *argv[MAX];
};

struct cmd_list *cmdv[MAX];  //shell指令
int num;//shell管道个数
int flagdo;//是否为后台处理命令标记

//执行外部命令
void execute(char *argv[])
{
        int error;
        error=execvp(argv[0],argv);
        if (error==-1)  printf("failed!\n");
        exit(1);
}

//切分单个管道
void split_cmd(char *line)
{
     struct cmd_list * cmd = (struct cmd_list *)malloc(sizeof(struct cmd_list));
     cmdv[num++] = cmd;
     cmd->argc = 0;
     char *save;
     char *arg = strtok_r(line, " \t", &save);//切分空格
     while (arg)
     {
        cmd->argv[cmd->argc] = arg;
        arg = strtok_r(NULL, " \t", &save);
        cmd->argc++;
     }
     cmd->argv[cmd->argc] = NULL;
}

//切分管道
void split_pipe(char *line)
{
    char *save;
    char * cmd = strtok_r(line, "|", &save);
    while (cmd) {
        split_cmd(cmd);
        cmd = strtok_r(NULL, "|", &save);
    }
}

//执行管道命令
void do_pipe(int index)
{
    if (index == num - 1)
        execute(cmdv[index]->argv);
    int fd[2];
    pipe(fd);//创建管道，0读，1写
    if (fork() == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execute(cmdv[index]->argv);
    }
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    do_pipe(index + 1);
}

//执行内部指令
int inner(char *line)
{
    char *save,*tmp[MAX];
    char t[LEN],p[LEN];
    strcpy(t,line);
    char *arg = strtok_r(line, " \t", &save);//切分空格
    int i=0;
    while (arg) {
        tmp[i] = arg;
        i++;//记录命令个数
        arg = strtok_r(NULL, " \t", &save);
     }
    tmp[i] = NULL;
    if (strcmp(tmp[i-1],"&")==0)//判断是否为后台处理命令
    {
        flagdo=1;
        i--;
    }
    if (strcmp(tmp[0],"exit")==0)//exit
    {
        exit(0);
        return 1;
    }
    else
    if (strcmp(tmp[0],"pwd")==0)//pwd
    {
        char buf[LEN];
        getcwd(buf,sizeof(buf));//得到当前路径
        printf("Current dir is:%s\n",buf);
        return 1;
    }
    else
    if (strcmp(tmp[0],"cd")==0)//cd
    {
        char buf[LEN];
        if (chdir(tmp[1])>=0)
        {
            getcwd(buf,sizeof(buf));
            printf("Current dir is:%s\n",buf);
        }
        else
        {
            printf("Error path!\n");
        }
         return 1;
    }
	else 
	    if (strcmp(tmp[0],"ls")==0)//ls
    {
		 DIR *dir_p;

        struct dirent *direct_p; 

        if((dir_p=opendir("."))==NULL)

          fprintf(stderr,"cannot open the file %s\n",tmp[1]);

        else

          while ((direct_p=(readdir(dir_p)))!=NULL)

          printf("%s  ",direct_p->d_name);

        printf("\n");

        close(dir_p);		
       
         return 1;
    }
    else 
        if (strcmp(tmp[0],"echo")== 0)//echo
        {
           int times;
            for(times = 1;times<i;times++)
                printf("%s%s",tmp[times],(times<i-1)?" ":"");
            printf("\n");
           return 1; 
        }
    else 
        if(strcmp(tmp[0],"cat")==0)//cat 
        {   int c;
            FILE *ifp;
            FILE *ofp;
            if(i == 1)
            {
               while((c = getc(stdin)) !=EOF)
                putc(c,stdout);
            }else
             {
					ifp = fopen(tmp[1],"r");      
				{
				   while((c = getc(ifp)) !=EOF)
					putc(c,stdout);
					fclose(ifp);
				}			
			 }
			 return 1;
        }
	  else 
        if (strcmp(tmp[0],"mkdir")== 0)//mkdir
        {
           if(mkdir(tmp[1],0770)!=0)
			  perror("mkdir"),exit(-1);			 
			  printf("mkdir directory  success .\n");            
           return 1; 
        }
	else 
        if (strcmp(tmp[0],"rm")== 0)//rm
        {
			if(remove(tmp[1]) == 0)
            printf("removed %s  success !\n",tmp[1]);   
			 else perror("remove"),exit(-1);			 
			           
           return 1; 
        }	
	else 
        if (strcmp(tmp[0],"wc")== 0)//wc
     {  FILE *ifp;
		int characters, lines, words, state;
        char c;
		ifp = fopen(tmp[1],"r");
        state = characters = lines = words = 0;
         while((c = getc(ifp)) !=EOF)
		{
        characters++;
        if(c == '\n') {
            lines++;
            state = 0;
            continue;
        } else if(c == ' ') {
            state = 0;
            continue;
        } else if(c == '\t') {
            state = 0;
            continue;
        } else {
            if(state == 0) {
                state = 1;
                words++;
            }
            continue;
        }
      }

    printf("%d characters. %d words. %d lines.\n", characters, words, lines);  
			return 1;
    }		
		
	
    return 0;
}
int main(int arc,char * arv[])
{
	    printf("\n---------User Commands--------\n");
		printf("|---ls:          List all files.\n");
		printf("|---echo xxx:    Print something behind.\n");
		printf("|---cat xxx.x:   Display the files.\n");
		printf("|---mkdir xxx.x: Create new directory.\n");
		printf("|---rm xxx.x:    Delete the files.\n");
		printf("|---cd xxx.x:    Change directory.\n");
		printf("|---pwd:         Print current working directory.\n");
		printf("|---wc xxx.x:    Count the file's lines.\n");
		printf("|---ls|grep xx:  Pipe grep value things.\n");
		printf("|---ls|wc &:     Background run.\n");
		printf("|---exit:        Escape command line.\n");
		printf("------------------------------------\n");
    int i,pid;
    char buf[LEN],p[LEN];
    while (1)
    {

        fgets(buf,LEN,stdin);//读入shell指令
        if (buf[0]=='\n') continue;
        buf[strlen(buf)-1]='\0';
        strcpy(p,buf);
        int inner_flag;
        inner_flag=inner(buf);//内置指令执行
        if (inner_flag==0)
        {
            pid=fork();//建立新的进程
            if (pid==0)
            {
                split_pipe(p);//管道的切割
                //如果是后台处理命令将&符号删除
                if (strcmp(cmdv[num-1]->argv[cmdv[num-1]->argc-1],"&")==0)
                {
                    cmdv[num-1]->argc--;
                    cmdv[num-1]->argv[cmdv[num-1]->argc]=NULL;
                }
                do_pipe(0);//执行管道
                exit(0);
            }
            if (flagdo==0)//非后台处理命令主进程才需等待子进程处理
                waitpid(pid,NULL,0);
        }
    }
    return 0;
}
