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
python HEImputation.py 10k DatasetX_10k.txt DatasetY.txt
Get Model type: like 1k data or 10k data. There may be two options: 1k or 10k
Get the query file Tag SNPs
Get the true labels
The outputs will be written in Predictions_Probability.csv
'''

model_type = sys.argv[1]
XTest_file = sys.argv[2]
YTest_file = sys.argv[3]
no_of_targets = 500
XTest = np.loadtxt(XTest_file,delimiter=',')
YTest = np.loadtxt(YTest_file,delimiter=',')
fh_pred_proba = open('Predictions_Probability.csv','w+')
fh_pred_proba.write('0,1,2\n')

fh_formatted_ytest = open(YTest_file[:-4]+'_MAUC.csv','w+')
fh_formatted_ytest.write('class\n')
#fh_pred_proba.close()
if model_type == '10k':
	#XTest = np.loadtxt('DatasetX_10k.txt',delimiter=',')
	#YTest = np.loadtxt('DatasetY.txt',delimiter=',')
	Weights = np.loadtxt('Weights_10k_10.txt',delimiter=',')
	#Weights2 = np.loadtxt('weights_scaled.txt',delimiter=',')
	Biases = np.loadtxt('bias_10k_10.txt',delimiter=',')
	#Biases = np.loadtxt('biases_scaled.txt',delimiter=',')
	Positions = np.loadtxt('Positions_10k_10.txt',delimiter=',')
elif model_type == '1k':
	#XTest = np.loadtxt('DatasetX_1k.txt',delimiter=',')
	#YTest = np.loadtxt('DatasetY.txt',delimiter=',')
	Weights = np.loadtxt('Weights_1k_10.txt',delimiter=',')
	Biases = np.loadtxt('bias_1k_10.txt',delimiter=',')
	Positions = np.loadtxt('Positions_1k_10.txt',delimiter=',')

#print (Weights2.shape, Weights.shape)
Results = np.ones((XTest.shape[0],no_of_targets)) 
for tar_snp_no in range(no_of_targets): #no_of_targets No. of Target SNPs
	if tar_snp_no%100 == 0:
		print ("Evaluating Target SNP No. :", tar_snp_no)
	scale_weights = 1
	Tag_positions = Positions[tar_snp_no].astype('int')
	
	XTestp = np.zeros((XTest.shape[0],Tag_positions.shape[0]))
	for num,i in enumerate(Tag_positions):
		XTestp[:,num] = XTest[:,i]
	
	Xp = (OneHotEncoder(XTestp)).reshape((XTestp.shape[0],XTestp.shape[1]*3))
	Weights_per_tag = ((Weights[tar_snp_no*30:(tar_snp_no+1)*30])*scale_weights).astype('float')
	Biases_per_tag = ((Biases[tar_snp_no*3:(tar_snp_no+1)*3])*scale_weights).astype('float')
	temp = (np.dot(Xp,Weights_per_tag)+Biases_per_tag)
	results_per_tag = np.argmax(temp,1)
	preds_per_tag = temp
	#print (temp[0])
	#if np.sum(temp,1)[:,None]!=0:
	temp1 = temp/(np.sum(temp,1)[:,None])
	temp1 = np.nan_to_num(temp1)
	#else:
	#	temp1 = np.array([0,0,0])
	#print (temp1[0])
	np.savetxt(fh_pred_proba,temp1,fmt="%0.4f",delimiter=',')
	#results_per_tag = np.argmax((np.dot(Xp,Weights_per_tag)),1)
	Results[:,tar_snp_no] = results_per_tag
	np.savetxt(fh_formatted_ytest,(YTest[:,tar_snp_no]),fmt = '%1d')
	
print (Results.shape, YTest.shape)
Accuracy = (np.count_nonzero(Results==YTest[:,:no_of_targets]))/(XTest.shape[0]*Results.shape[1])

print (Accuracy)
fh = open('yPreds.csv','w+')
np.savetxt(fh,Results, fmt="%1d",delimiter=',')
fh.close()
fh_pred_proba.close()
fh_formatted_ytest.close()
