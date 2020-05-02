.PHONY:all
CXX = gcc
all = server client
CFLAGS = -g -std=c99
server:
	$(CXX) -o server server.c ls.c $(CFLAGS)
client:
	$(CXX) -o client client.c $(CFLAGS)

clean:
	rm -rf *.o $(all) 	