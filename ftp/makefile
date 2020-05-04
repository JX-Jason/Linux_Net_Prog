all: server client
.PHONY:all
CXX = gcc
CFLAGS = -g -std=c99
server:server.c servcmd.c
	$(CXX) -o server server.c servcmd.c $(CFLAGS)
client:client.c clntcmd.c
	$(CXX) -o client client.c clntcmd.c $(CFLAGS)

clean:
	rm -rf *.o $(all) 	