OBJS	= ./build/greedy.o ./build/main.o ./build/graph.o ./build/helper.o ./build/pruning.o ./build/vamana_al.o ./build/data_forming.o
SOURCE	= ./src/greedy.cpp ./src/main.cpp ./src/graph.cpp ./src/helper.cpp ./src/pruning.cpp ./src/vamana_al.cpp ./src/data_forming.cpp
HEADER	= ./include/header.h
OUT		= ./bin/vamana
CC	 	= g++
FLAGS	= -g3 -c -Wall -pg

# Add variables for test source and objects
# Remove main.o from the test object files to avoid conflict with main.cpp
TEST_SRC = ./testing/test.cpp
TEST_OBJS = ./build/greedy.o ./build/graph.o ./build/helper.o ./build/pruning.o ./build/vamana_al.o ./build/test.o
TEST_OUT = ./bin/test

all: build_dir $(OBJS)
	$(CC) -g -pg $(OBJS) -o $(OUT)

# Create build directory if it does not exist
build_dir:
	mkdir -p ./build
	mkdir -p ./bin

# Compile each source file into an object file
./build/greedy.o: ./src/greedy.cpp
	$(CC) $(FLAGS) ./src/greedy.cpp -o ./build/greedy.o

./build/main.o: ./src/main.cpp
	$(CC) $(FLAGS) ./src/main.cpp -o ./build/main.o

./build/graph.o: ./src/graph.cpp
	$(CC) $(FLAGS) ./src/graph.cpp -o ./build/graph.o

./build/helper.o: ./src/helper.cpp
	$(CC) $(FLAGS) ./src/helper.cpp -o ./build/helper.o

./build/pruning.o: ./src/pruning.cpp
	$(CC) $(FLAGS) ./src/pruning.cpp -o ./build/pruning.o

./build/vamana_al.o: ./src/vamana_al.cpp
	$(CC) $(FLAGS) ./src/vamana_al.cpp -o ./build/vamana_al.o

./build/data_forming.o: ./src/data_forming.cpp
	$(CC) $(FLAGS) ./src/data_forming.cpp -o ./build/data_forming.o

# Compile test source into object file
./build/test.o: $(TEST_SRC)
	$(CC) $(FLAGS) $(TEST_SRC) -o ./build/test.o

# Link object files (excluding main.o) and run tests
test: build_dir $(TEST_OBJS)
	$(CC) -g $(TEST_OBJS) -o $(TEST_OUT)
	./$(TEST_OUT)

# Clean command to remove object files and binary
clean:
	rm -f ./build/*.o
	rm -f $(OUT)
	rm -f $(TEST_OUT)
	rm -rf ./build
	rm -rf ./bin
