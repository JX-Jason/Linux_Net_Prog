一、实验目的
	
	1.理解shell程序的功能
	2.学会shell的使用
	3.构造一个简单的shell	

二、实验内容
	
	内容一、C/C++编写一个简单的shell程序，实现以下基本的命令。
	1）浏览目录和文件的各种属性ls
	2）回显命令 echo
	3）显示文件内容 cat
	4）创建目录 mkdir
	5）删除文件 rm
	6）切换目录 cd
	7）显示当前目录 pwd
	8）文字统计 wc
	内容二、写一个makefile来管理
	内容三、写清楚编译和运行的过程


三、实验结果说明
1．	输入”ls|grep xx”时，ls与grep之间没有空格。
2．	管道命令调用了部分系统命令，但是剩下的命令均由C语言执行。
3．	部分内容参考了CSDN博客内容，由于版面原因就不一一列举出来作者姓名，在此对他们表示感谢。 
 https://blog.csdn.net/u012349696/article/details/50083787

	最终的程序实现了ls,echo,cat,mkdir,rm,cd,pwd,wc这些指
令的基本功能。还实现了“ls|grep”管道命令，“ls|wc &”后台命令
	
	makefile的内容为：
	testmake:main.0
		gcc -o stu0200_mysh main.o
	main.o:main.c
		gcc -c -o main.o main.c


四、实验编译与运行
	
	在lab2目录下执行make命令，生成main.o ,stu0200_mysh文件

	输入exit可以退出程序。