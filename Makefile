##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## make an executable
##

CC = g++

EXECUTABLE	= main
SRC_DIR = src
TEST_SRC_DIR = tests

SRC = $(shell find $(SRC_DIR) -name '*.cpp')
MAIN_SRC = main.cpp
MAIN_OBJ = $(MAIN_SRC:%.cpp=%.o)
OBJ = $(SRC:%.cpp=%.o)


CFLAGS += -std=c++17 -W -Wall -Wextra $(if $(DEBUG),-g3) $(if $(DEBUG),-DDEBUG)
LDFLAGS = -ldl -lcriterion
INC_FLAGS = -Iinclude

TEST_NAME = unit_tests
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)

TEST_SRC = $(shell find $(TEST_SRC_DIR) -name '*.c')


all: bin/$(EXECUTABLE)

run: all
	@./bin/$(EXECUTABLE) $(ARGS)

bin/$(EXECUTABLE): $(OBJ) $(MAIN_OBJ)
	@$(CC) -o $@ $(OBJ) $(MAIN_OBJ) $(CFLAGS) $(INC_FLAGS) $(LDFLAGS)
	
%.o: %.cpp
	@$(CC) $(INC_FLAGS) $(CFLAGS) -c $< -o $@
	@echo "\033[1;32mCompiled \033[1;37m'$<'\033[m"

.PHONY: clean
clean:
	@$(RM) -r $(OBJ) $(MAIN_OBJ)

.PHONY: fclean
fclean: clean
	@$(RM) -r $(BIN)/$(EXECUTABLE)
	@$(RM) -r vgcore*

.PHONY: re
re: fclean all

valgrind:
	@make re DEBUG=1
	valgrind --leak-check=full -s ./bin/$(EXECUTABLE) $(ARGS)

dll:
	@g++ -g3 -Wall -Wno-write-strings -g -fPIC -shared -I include ./Test.cpp -o test.so

tests: fclean
	@$(CC) -o $(TEST_NAME) $(INC_FLAGS) $(SRC) $(TEST_SRC) $(CFLAGS) $(LDFLAGS)
	@./$(TEST_NAME)
