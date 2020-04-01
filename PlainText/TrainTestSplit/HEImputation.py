import numpy as np
import pandas
import tensorflow as tf
import sys
import time
import subprocess

def scale(Matrix):
    min_value = np.min(Matrix)
    max_value = np.max(Matrix)
    ScaledMatrix = (Matrix-min_value)/(max_value-min_value)
    return (ScaledMatrix)
	
def OneHotEncoder(Matrix):
    Temp = np.zeros((Matrix.shape[0],Matrix.shape[1],3))
    for num, i in enumerate(Matrix):
        Temp[num] = (tf.keras.utils.to_categorical(i,3))
    return (Temp)
'''
python HEImputation.py 10k
Get Model type: like 1k data or 10k data. There may be two options: 1k or 10k
Get the query file Tag SNPs
Get the name of the out file where the data will be written. outfile is the file where predictions will be written
Get the name of the time file: The time will state the total run time of the process
'''

model_type = sys.argv[1]

no_of_tags = 500
if model_type == '10k':
	XTest = np.loadtxt('XTest_10k.txt',delimiter=',')
	YTest = np.loadtxt('YTest_10k.txt',delimiter=',')
	Weights = np.loadtxt('Weights_10k_10.txt',delimiter=',')
	Positions = np.loadtxt('Positions_10k_10.txt',delimiter=',')
elif model_type == '1k':
	XTest = np.loadtxt('XTest_1k.txt',delimiter=',')
	YTest = np.loadtxt('YTest_1k.txt',delimiter=',')
	Weights = np.loadtxt('Weights_1k_10.txt',delimiter=',')
	Positions = np.loadtxt('Positions_1k_10.txt',delimiter=',')


Results = np.ones((XTest.shape[0],no_of_tags)) 
for tag_snp_no in range(500): #No. of Tag SNPs
	if tag_snp_no%100 == 0:
		print ("Evaluating Tag SNP No. :", tag_snp_no)
	scale_weights = 1
	Tag_positions = Positions[tag_snp_no].astype('int')
	
	XTestp = np.zeros((XTest.shape[0],Tag_positions.shape[0]))
	for num,i in enumerate(Tag_positions):
		XTestp[:,num] = XTest[:,i]
	
	Xp = (OneHotEncoder(XTestp)).reshape((XTestp.shape[0],XTestp.shape[1]*3))
	Weights_per_tag = ((Weights[tag_snp_no*30:(tag_snp_no+1)*30])*scale_weights).astype('float')
	results_per_tag = np.argmax(np.dot(Xp,Weights_per_tag),1)
	Results[:,tag_snp_no] = results_per_tag
Accuracy = (np.count_nonzero(Results==YTest))/(XTest.shape[0]*YTest.shape[0])

print (Accuracy)

