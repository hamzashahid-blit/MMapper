all:
	g++ -std=c++23 -Werror -pedantic -Wconversion main.cpp state.cpp -lm -lraylib

