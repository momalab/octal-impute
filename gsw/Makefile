ID=10

SRC=../../../src
CGT=cgt.exe
BOB=bob.exe

PYTHON=python3
NORMALIZER=normalizer.py

LOCAL=../tests/user/esha_logreg
CFG=${LOCAL}/cgt.cfg

QUERY=XTest_Logreg_1.txt
WEIGHTS=W_Logreg_pos.txt
BIASES=b_Logreg_pos.txt
ENCRYPTED_QUERY=encrypted_query${ID}.tmp
ENCRYPTED_PROBS=encrypted_probs${ID}.tmp
DECRYPTED_PROBS=decrypted_probs${ID}.tmp
NORMALIZED_PROBS=normalized_probs${ID}.tmp

all: compile encrypt run

compile:
	cd ${SRC} && make TFHE=1 USER=${LOCAL}

run:
	cd ${SRC} && ./${BOB} ${LOCAL}/${ENCRYPTED_QUERY} ${LOCAL}/${WEIGHTS} ${LOCAL}/${BIASES} ${LOCAL}/${ENCRYPTED_PROBS}

decrypt:
	cd ${SRC} && ./${CGT} dec -c ${CFG} < ${LOCAL}/${ENCRYPTED_PROBS} > ${LOCAL}/${DECRYPTED_PROBS}
	${PYTHON} ${NORMALIZER} ${DECRYPTED_PROBS} ${NORMALIZED_PROBS}

encrypt:
	cd ${SRC} && ./${CGT} enc -c ${CFG} < ${LOCAL}/${QUERY} > ${LOCAL}/${ENCRYPTED_QUERY}

clean:
	cd ${SRC} && make c
	rm -f *.tmp
