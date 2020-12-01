#!/usr/bin/env bash
valid=( 1 10 20 40 80 )
cmp=0
isValid(){
    for i in "${valid[@]}"; do
        if [[ $i -eq $1 ]]; then
            cmp=1
            return
        fi
    done
    cmp=0
}

isValid $1
if [[ ${cmp} -eq 1 ]]; then
    echo "Running ${1}k"
    if [[ $1 -eq 1 || $1 -eq 10 ]]; then
        nIndividuals=501
    else
        nIndividuals=1000
    fi
    export ID=${1}
    export N_INDIVIDUALS=${nIndividuals}
    export N_THREADS=${2}
    cd paillier && make run ID=${ID} N_INDIVIDUALS=${N_INDIVIDUALS} N_THREADS=${N_THREADS}
    cp alice/decryptedProb${ID}.data ../decryptedProb${ID}.tmp
    # cp time_memory_encryption.tmp /results/time_memory_encryption${ID}.txt
    # cp time_memory_query.tmp /results/time_memory_query${ID}.txt
    # cp time_memory_decryption.tmp /results/time_memory_decryption${ID}.txt
    cd ..
    python3 format.py decryptedProb${ID}.tmp ${ID}k_proba_formatted.tmp
    # python3 evaluation.py -i ${ID}k_proba_formatted.tmp -t YTrue_${ID}k_formatted.txt -o /results/auc_test_${ID}k.png
    python3 evaluation.py -i ${ID}k_proba_formatted.tmp -t YTrue_${ID}k_formatted.txt -o auc_test_${ID}k.png
else
    echo "Invalid input."
fi
