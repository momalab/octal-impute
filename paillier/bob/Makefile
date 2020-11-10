ID=10# database ID
N_THREADS=8
KEYSIZE=3072#1024

# COMPILER
CC=g++
FLAGS=-O3 -std=c++17
INCS=-I../lib/common -I../lib/gmp/include -I../lib/paillier
LIBS=-L../lib/gmp/lib -lgmp -pthread

# FILES
EXT=.exe# file extension
DATAEXT=.data
KEYEXT=.key
TMPEXT=.tmp

# C++ programs
QUERIER=querier

# Data files
ENCRYPTED_QUERY=encryptedQuery${ID}${TMPEXT}
ENCRYPTED_TARGET=encryptedProbs${ID}${TMPEXT}

POSITIONS_PREAMBLE=positions
POSITIONS=${POSITIONS_PREAMBLE}${ID}${DATAEXT}
WEIGHTS_PREAMBLE=weights
WEIGHTS=${WEIGHTS_PREAMBLE}${ID}${DATAEXT}

# DIRECTORIES
ALICE=../alice

all: compile run

%: %.cpp
	${CC} ${FLAGS} ${INCS} $< -o $@${EXT} ${LIBS}

clean:
	rm -f *${EXT} *${KEYEXT} *${TMPEXT}

clean-all: clean
	rm -f ${POSITIONS_PREAMBLE}*${DATAEXT}
	rm -f ${WEIGHTS_PREAMBLE}*${DATAEXT}

compile:
	make ${QUERIER}

query:
	./${QUERIER}${EXT} ${ENCRYPTED_QUERY} ${WEIGHTS} ${ENCRYPTED_TARGET} ${N_THREADS} ${KEYSIZE}

run: query send-result

send-result:
	cp ${ENCRYPTED_TARGET} ${ALICE}
