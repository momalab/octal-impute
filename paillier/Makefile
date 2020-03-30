ID=1
QUERY=query_tag_SNPs_${ID}_genotypes.data
OUTPUT_PROBS=ypred${ID}.data
OUTPUT_TARGETS=target${ID}.data

TIME_ENCRYPTION=time_encryption.tmp
TIME_COMPUTATION=time_computation.tmp
TIME_DECRYPTION=time_decryption.tmp

N_THREADS=8

ALICE=alice
BOB=bob
TOOL=tool

all: compile run

clean:
	cd ${ALICE} && make clean
	cd ${BOB} && make clean

compile:
	cd ${ALICE} && make compile
	cd ${BOB} && make compile

run:
	cd ${ALICE} && { time make run QUERY=$(abspath ${QUERY}) ID=${ID} ; } 2> $(abspath .)/${TIME_ENCRYPTION}
	cd ${BOB} && { time make run ID=${ID} N_THREADS=${N_THREADS} ; } 2> $(abspath .)/${TIME_COMPUTATION}
	cd ${ALICE} && { time make decrypt QUERY_DECRYPTED_PROBS=$(abspath .)/${OUTPUT_PROBS} QUERY_DECRYPTED_TARGETS=$(abspath .)/${OUTPUT_TARGETS} ID=${ID} ; } 2> $(abspath .)/${TIME_DECRYPTION}
