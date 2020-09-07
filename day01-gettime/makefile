all: client server
.PHONY:all
obj_serv = server_main.c
obj_clnt = client_main.c
server:$(obj_serv)
	gcc -o server $(obj_serv) -g
client:$(obj_clnt)
	gcc -o client $(obj_clnt) -g

.PHONY:clean
clean:
	rm -rf *.o server client
