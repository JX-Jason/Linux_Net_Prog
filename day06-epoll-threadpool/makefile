all: server
.PHONY:all
obj_serv = http_conn.h threadpool.h locker.h server.cpp 
server:$(obj_serv)
	g++ -std=c++11 -o server $(obj_serv) -g -lpthread
.PHONY:clean
clean:
	-rm -rf *.o server