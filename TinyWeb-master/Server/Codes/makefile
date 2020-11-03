all: server
.PHONY:all
obj_serv = http_conn.h web_function.h web_thread.h main.cpp
server:$(obj_serv)
	g++ -std=c++11 -o server $(obj_serv) -g -lpthread
.PHONY:clean
clean:
	rm -rf *.o server