# Octal-Impute
The repository consists of all the scripts, data and source code for fast genotype imputation performed on GWAS data. GWAS data has 500 target SNPs to be predicted for 2.5k individuals. We use machine learning for performing imputation, develop ML models for the same and perform private inference using Paillier. This is the repo for weights, biases, scripts for both plain-text and encrypted imputation.  
## Plain-text
### Dependencies
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


