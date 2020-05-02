#include<stdio.h>

#include<dirent.h>

 

void ls_do(char dirname[]);

 

int main(int ac,char *av[])

{

        if(ac==1)

          ls_do(".");

        else

          while(--ac){

                ls_do(*++av);

                }

return 0;

}

 

void ls_do(char dirname[])

{

        DIR *dir_p;

        struct dirent *direct_p;

 

        if((dir_p=opendir(dirname))==NULL)

          fprintf(stderr,"cannot open the file %s\n",dirname);

        else

          while ((direct_p=(readdir(dir_p)))!=NULL)

          printf("%s  ",direct_p->d_name);

        printf("\n");

        close(dir_p);

 

}
