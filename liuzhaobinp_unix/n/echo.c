/*************************************************************************
	> File Name: echo.c
	> Author: 
	> Mail: 
	> Created Time: Friday, April 27, 2018 AM10:45:44 CST
 ************************************************************************/

#include<stdio.h>
int main(int argc,char *argv[]){
    int i;
    for(i=1;i<argc;i++)
        printf("%s%s",argv[i],(i<argc-1)?" ":"");
    printf("\n");
    return 0;


}
