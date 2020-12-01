# Octal-Impute
The repository consists of all the scripts, data and source code for fast genotype imputation performed on GWAS data. GWAS data has 500 target SNPs to be predicted for 2.5k individuals. We use machine learning for performing imputation, develop ML models for the same and perform private inference using Paillier. This is the repo for weights, biases, scripts for both plain-text and encrypted imputation.  
## Plain-text
### Dependencies
* python 3.6.5
* keras 2.3.1
* scikit-learn 0.23.2
* numpy 1.19.2

1. Clone octal-impute:
```
git clone https://github.com/momalab/octal-impute.git
```
2. Change directory to Plain-text:
```
cd PlainText
```
3. Find top features:
```
python find_top_tags.py X_train.txt y_train.txt Positions <starting_target_no> <end_target_no>
```
The positions will get stored in files called Positions_<starting_target_no>\_<end_target_no>.txt. Our extracted feature positions are also available in Positions/
4. Run Training script:
```
python Training.py <target_no>
```
The weights and biases will get stored in files called weights_<starting_target_no>\_<end_target_no>.txt. Our extracted parameters are also available in Weights/ and Bias/

# Private Inference using the Paillier Cryptosystem

## Setup
1. Install dependencies (sudo required):
```
cd ../paillier
./install.sh
```
2. Unzip data:
```
./unzip.sh
```

## Run

### TL;DR
1. Compile and run everything for all databases:
```
sh run [N_THREADS]
```
N_THREADS is a optional command that specifies the number of threads to be used. The default is 8.

### Step-by-Step
1. Compile
```
./compile.sh
```

2. Run for a specific database:
```
./run.sh ID N_THREADS
```
For example:
```
./run.sh 10 16
```
Will run for the 10k database with 16 threads.

The script will automatically call all steps: parse, encryption, query, decryption, and calculate AUC.

Files of interest:
* inputs: XTest_*k.txt in paillier/paillier/
* weights: weights*.data in paillier/paillier/bob/
* outputs: *k_proba_formatted.tmp in paillier/
* auc: auc_test_*k.png
