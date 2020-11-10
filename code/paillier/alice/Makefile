ID=10# database ID
KEYSIZE=3072#1024
N_THREADS=8
N_INDIVIDUALS=501
N_INPUTS=30
N_OUTPUTS=3

# COMPILER
CC=g++
FLAGS=-O3 -std=c++17
INCS=-I../lib/common -I../lib/gmp/include -I../lib/paillier
LIBS=-L../lib/gmp/lib -lgmp -pthread

# TOOLS
PYTHON=python3

# FILES
EXT=.exe# file extension
DATAEXT=.data
KEYEXT=.key
PYEXT=.py
TMPEXT=.tmp

# C++ programs
DECRYPTOR=decryptor
ENCRYPTOR=encryptor
KEYGEN=keygen
PARSER=parser${PYEXT}

# Data files
QUERY=query_tag_SNPs_${ID}_genotypes${DATAEXT}
QUERY_PARSED=queryTagSNPsParsed${ID}${TMPEXT}
ENCRYPTED_QUERY=encryptedQuery${ID}${TMPEXT}
ENCRYPTED_TARGETS=encryptedProbs${ID}${TMPEXT}
DECRYPTED_TARGETS=decryptedProb${ID}${DATAEXT}
POSITIONS_PREAMBLE=positions
POSITIONS=${POSITIONS_PREAMBLE}${ID}${DATAEXT}

# DIRECTORIES
BOB=../bob

# KEYS
PUBLIC_KEY=public${KEYEXT}
SECRET_KEY=secret${KEYEXT}

all: compile run


%: %.cpp
	${CC} ${FLAGS} ${INCS} $< -o $@${EXT} ${LIBS}

clean:
	rm -f *${EXT} *${KEYEXT} *${TMPEXT}
	rm -f ${POSITIONS_PREAMBLE}*${DATAEXT}

compile:
	make ${KEYGEN}
	make ${ENCRYPTOR}
	make ${DECRYPTOR}

decrypt:
	./${DECRYPTOR}${EXT} ${ENCRYPTED_TARGETS} ${DECRYPTED_TARGETS} ${N_THREADS} ${KEYSIZE} ${N_INDIVIDUALS} ${N_INPUTS} ${N_OUTPUTS}

encrypt:
	./${ENCRYPTOR}${EXT} ${QUERY_PARSED} ${ENCRYPTED_QUERY}

generate-keys:
	./${KEYGEN}${EXT} ${KEYSIZE}

parse:
	${PYTHON} ${PARSER} ${QUERY} ${POSITIONS} ${QUERY_PARSED}

request-positions:
	cp ${BOB}/${POSITIONS} .

run: request-positions parse generate-keys encrypt send

send:
	cp ${ENCRYPTED_QUERY} ${PUBLIC_KEY} ${BOB}/
