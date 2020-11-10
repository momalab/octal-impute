ID=10
# QUERY=query_tag_SNPs_${ID}_genotypes.data
QUERY=XTest_${ID}k.txt
OUTPUT_PROBS=ypred${ID}.data
OUTPUT_TARGETS=target${ID}.data

# TIME_ENCRYPTION=time_encryption.tmp
# TIME_COMPUTATION=time_computation.tmp
# TIME_DECRYPTION=time_decryption.tmp

N_THREADS=8

ALICE=alice
BOB=bob
TOOL=tool

MEMORY_ENCRYPTION=time_memory_encryption.tmp
MEMORY_QUERY=time_memory_query.tmp
MEMORY_DECRYPTION=time_memory_decryption.tmp
MEMORY_REPORT=time_memory${ID}.data

PYTHON=python3
TIMER=timer.py
MEMORY=memory.py

LINUX_TIME=../tool/time

all: compile run

clean:
	cd ${ALICE} && make clean
	cd ${BOB} && make clean
	rm -f ${MEMORY_ENCRYPTION}
	rm -f ${MEMORY_QUERY}
	rm -f ${MEMORY_DECRYPTION}

compile:
	cd ${ALICE} && make compile
	cd ${BOB} && make compile

install:
	apt install python3
	pip3 install --upgrade numpy==1.18.5 pandas==1.1.2 tensorflow==2.3.0

run:
	cd ${ALICE} && { ${LINUX_TIME} -v make run QUERY=$(abspath ${QUERY}) ID=${ID} ; } 2> $(abspath .)/${MEMORY_ENCRYPTION}
	cd ${BOB} && { ${LINUX_TIME} -v make run ID=${ID} N_THREADS=${N_THREADS} ; } 2> $(abspath .)/${MEMORY_QUERY}
	cd ${ALICE} && { ${LINUX_TIME} -v make decrypt QUERY_DECRYPTED_PROBS=$(abspath .)/${OUTPUT_PROBS} QUERY_DECRYPTED_TARGETS=$(abspath .)/${OUTPUT_TARGETS} ID=${ID} ; } 2> $(abspath .)/${MEMORY_DECRYPTION}
