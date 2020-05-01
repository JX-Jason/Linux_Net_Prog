all: client server
.PHONY:all
obj_serv = server.c
obj_clnt = client.c
server:$(obj_serv)
	gcc -o server $(obj_serv) -g -std=c99 -lpthread -D_REENTRANT
client:$(obj_clnt)
	gcc -o client $(obj_clnt) -g -std=c99 -lpthread -D_REENTRANT

.PHONY:clean
clean:
	rm -rf *.o server client
