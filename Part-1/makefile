OBJS	= greedy.o main.o graph.o helper.o pruning.o vamana_al.o
SOURCE	= ./src/greedy.cpp ./src/main.cpp ./src/graph.cpp ./src/helper.cpp ./src/pruning.cpp ./src/vamana_al.cpp
HEADER	= header.h
OUT		= vamana
CC	 	= g++
FLAGS	= -g3 -c -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	mv $(OBJS) ./build
	mv $(OUT) ./bin

greedy.o: ./src/greedy.cpp
	$(CC) $(FLAGS) ./src/greedy.cpp

main.o: ./src/main.cpp
	$(CC) $(FLAGS) ./src/main.cpp

graph.o: ./src/graph.cpp
	$(CC) $(FLAGS) ./src/graph.cpp

helper.o: ./src/helper.cpp
	$(CC) $(FLAGS) ./src/helper.cpp

pruning.o: ./src/pruning.cpp
	$(CC) $(FLAGS) ./src/pruning.cpp

vamana_al.o: ./src/vamana_al.cpp
	$(CC) $(FLAGS) ./src/vamana_al.cpp

clean:
	rm -f *.o
	rm -f ./build/*.o
	rm -f ./bin/$(OUT)
