# 2nd September, 2019
#Author Esha Sarkar

import numpy as np
import sys
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import mutual_info_classif
import os
import tensorflow as tf

'''
python find_top_tags.py X.txt y.txt Positions 0 1000
'''
#start_time = time.time()
XTrain_file = sys.argv[1]
YTrain_file = sys.argv[2]

Xtr = np.loadtxt(XTrain_file,delimiter=',').astype('int')
ytr = np.loadtxt(YTrain_file,delimiter=',').astype('int')


print ("Training on: ",Xtr.shape,ytr.shape)


train_from = sys.argv[4]
train_till = sys.argv[5]

positions_file = sys.argv[3]+'_'+train_from+'_'+train_till+'.txt'

fh = open(positions_file,'w+')
fh.close()


TrainAccuracy = []
AvgTrainAccuracy = []
top_tags = 10
positions = np.zeros((1,top_tags))
for i in np.arange(int(train_from),int(train_till)):
#for i in np.arange(int(train_from),2):
	print ("Top tags: ",top_tags,"i value: ",i)
	print ("Running for Target SNP: ",i)

	ytr_one = ytr[:,i].astype('int')
	ytr_one_ohe = tf.keras.utils.to_categorical(ytr_one,3)
	Selector = SelectKBest(score_func=mutual_info_classif, k=top_tags)
	Tops = Selector.fit(Xtr, ytr_one)
	Indices_sorted = ((np.sort(np.argsort(Tops.scores_)[::-1][:top_tags])).reshape((1, top_tags)))
	positions = np.concatenate((positions, Indices_sorted))
print ("--------------------Feature extraction complete-------------------------")
pf = open(positions_file,'a')
np.savetxt(pf, positions[1:,:], delimiter=',', fmt = '%d')
pf.close()
