all: client serverLT serverET
.PHONY:all
obj_serv_LT = serverLT.c
obj_serv_ET = serverET.c
obj_clnt = clnt.c
serverLT:$(obj_serv)
	gcc -o serverLT $(obj_serv_LT) -g -std=c99
serverET:$(obj_serv)
	gcc -o serverET $(obj_serv_ET) -g -std=c99	
client:$(obj_clnt)
	gcc -o client $(obj_clnt) -g -std=c99

.PHONY:clean
clean:
	rm -rf *.o server client
