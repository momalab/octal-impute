# 11th November, 2020
#Author Esha Sarkar

import numpy as np
import keras
import sys
from keras.models import Sequential
from keras.layers import Dense
from keras import optimizers
from keras.layers import Dense, Flatten
import tensorflow as tf
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import mutual_info_classif
from keras.constraints import maxnorm, nonneg
import os

'''
python Training.py 10
'''
XTrain_file = 'X.txt'
YTrain_file = 'y.txt'

Xtr = np.loadtxt(XTrain_file,delimiter=',').astype('int')
ytr = np.loadtxt(YTrain_file,delimiter=',').astype('int')


print ("Training on: ",Xtr.shape,ytr.shape)


def OneHotEncoder(Matrix):
	Temp = np.zeros((Matrix.shape[0],Matrix.shape[1],3))
	for num, i in enumerate(Matrix):
		Temp[num] = (tf.keras.utils.to_categorical(i,3))
	return (Temp)

train_from = int(sys.argv[1])
train_till = train_from+1

weights_file = 'weights_'+str(train_from)+'_'+str(train_till)+'.txt'
bias_file = 'bias_'+str(train_from)+'_'+str(train_till)+'.txt'

fh = open(weights_file,'w+')
fh.close()
fh = open(bias_file,'w+')
fh.close()

wf=open(weights_file,'ab')
bf=open(bias_file,'ab')
top_tags = 10
i = train_from
print ("Top tags: ",top_tags,"i value: ",i)
print ("Running for Target SNP: ",i)
pf = (np.loadtxt('features/Positions_'+str(i)+'_'+str(i+1)+'.txt',delimiter=',')).astype('int')
ytr_one = ytr[:,i].astype('int')
ytr_one_ohe = tf.keras.utils.to_categorical(ytr_one,3)
Xtr_temp = Xtr[:,pf]
X_train = OneHotEncoder(Xtr_temp).reshape((Xtr_temp.shape[0],top_tags*3))
TagNos = X_train.shape[1]
model_10k = Sequential()
model_10k.add(Dense(ytr_one_ohe.shape[1], input_dim=TagNos,bias_constraint=keras.constraints.NonNeg(),kernel_constraint=keras.constraints.NonNeg()))
model_10k.compile(loss='categorical_crossentropy', optimizer='adam')
model_10k.summary()  
reduce_lr = keras.callbacks.ReduceLROnPlateau(monitor='loss', factor=0.5,patience=3, min_lr=0.00001)
model_10k.fit(X_train, ytr_one_ohe, epochs=30, verbose=1,callbacks=[reduce_lr,keras.callbacks.TerminateOnNaN()])

W,b = model_10k.layers[0].get_weights()
W[W==-0.0]=0.0
b[b==-0.0]=0.0
np.savetxt(wf,W,delimiter=',',fmt='%1.8f')
np.savetxt(bf,b,delimiter=',',fmt='%1.8f')
wf.close()
bf.close()


print ("----------------------Training complete-------------------------")
