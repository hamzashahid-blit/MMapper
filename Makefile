all:
	g++ -std=c++23 -g -Werror -pedantic -Wconversion main.cpp state.cpp -lm -lraylib

