# Octal-Impute
The repository consists of all the scripts, data and source code for fast genotype imputation performed on GWAS and GTEX data. GWAS data has 500 target SNPs to be predicted for 2504 individuals. We perform private genotype imputation on tag SNPs with genomic distance of 10k and 1k. We use machine learning for performing imputation, develop ML models for the same and perform private inference using Paillier. The weights, biases, scripts performing computation on these two different datasets are appended with '1k' or '10k' respectively.  
**Plain-text computation**  
There are two folders:  
1. TrainTestSplit consists of GWAS data split into training and test data (80-20 split).  
*Data*  
Training data: XTrain_1k.txt, YTrain_1k.txt, XTrain_10k.txt, YTrain_10k.txt   
Test data: Testing data: XTest_1k.txt, YTest_1k.txt, XTest_10k.txt, YTest_10k.txt  
*Trained parameters*  
The training is done using an MLP for every target SNP forming a neural network of 500 smaller MLPs. Further, specific 10 tag SNPs are selected using mutual information to effectively predict the target SNPs.  
Weights and biases: Weights_1k_10.txt, Weights_10k_10.txt, bias_1k_10.txt, bias_10k_10.txt  
Positions: The positions of the selected tag SNPs (using mutual information) Positions_1k_10.txt, Positions_10k_10.txt  
To perform imputation in plaintext: 
python HEImputation.py <model_type>  
model_type can be 10k or 1k  

2. TrainingOnAllData:  
It consists of all the scripts, data and parsers to pre-process data for entire GWAS dataset. The trained parameters can be used to predict target SNPs from other datasets as GTEX. 
*Data*
DatasetX, DatasetY  
To parse data in the form of individuals being represented along the rows, run:  
python Parser.py
Trained parameters follow the same convention as TrainTestSplit.  
Training Scipts for training weights and the top tag SNPs for targets:  
python Findingk_1k.py or Findingk_10k.py
