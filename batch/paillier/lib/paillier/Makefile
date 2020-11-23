# COMPILER
CC=g++
FLAGS=-O3 -std=c++17
INCS=-I../common -I../gmp/include
LIBS=-L../gmp/lib -lgmp -pthread
EXT=.exe# file extension

%: %.cpp
	${CC} ${FLAGS} ${INCS} $< -o $@${EXT} ${LIBS}
