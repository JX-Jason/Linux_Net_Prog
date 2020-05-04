
CXX = g++

SRC_DIR = ./src/
TARGET = my_db
OBJ = bpt.o my_db.o
CFLAGS = -g
$(TARGET):$(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) -lpthread
	rm -rf $(OBJ)

bpt.o:
	$(CXX) -g -c $(SRC_DIR)bpt.cc 

my_db.o:
	$(CXX) -g -c -lpthread $(SRC_DIR)my_db.cpp 

clean:
	rm -rf $(OBJ) $(TARGET)