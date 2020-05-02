#include<stdio.h>
#include "ls.h"

#include<dirent.h>
#include<string.h>
char re[MAXLINE];
DIR *dp;
struct dirent *entry;
int n;
//void ls_do(char dirname[]);

 

// int main(int ac,char *av[])

// {

//         if(ac==1)

//           ls_do(".");

//         else

//           while(--ac){

//                 ls_do(*++av);

//                 }

// return 0;

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
