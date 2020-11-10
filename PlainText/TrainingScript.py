# 2nd September, 2019
#Author Esha Sarkar
import pandas
import numpy as np
import keras
import sys
from random import seed
from random import randint
from numpy import array
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from math import sqrt
from sklearn.metrics import mean_squared_error
from keras import optimizers
from random import seed
from random import randint
from numpy import array
from keras.models import Sequential
from keras.layers import Dense, Flatten
from keras.layers import LSTM,Dropout,Reshape
from math import sqrt
from sklearn.metrics import mean_squared_error
import tensorflow as tf
import pandas
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense
from keras import optimizers
from keras.layers import Dense, Flatten
from sklearn.metrics import mean_squared_error
import tensorflow as tf
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import mutual_info_classif
from keras.constraints import maxnorm, nonneg
import os
'''
python XTrain_10k.txt YTrain_10k.txt Positions_10k.txt Weights_10k.txt Bias_10k.txt 
'''
XTrain_file = sys.argv[1]
YTrain_file = sys.argv[2]

Xtr = np.loadtxt(XTrain_file,delimiter=',').astype('int')
ytr = np.loadtxt(YTrain_file,delimiter=',').astype('int')


print ("Training on: ",Xtr.shape,ytr.shape)

def OneHotEncoder(Matrix):
	Temp = np.zeros((Matrix.shape[0],Matrix.shape[1],3))
	for num, i in enumerate(Matrix):
		Temp[num] = (tf.keras.utils.to_categorical(i,3))
	return (Temp)
def Accuracy(X,y,ModelName):
	y_actual = np.argmax(y.reshape((y.shape[0],int(y.shape[1]/3),3)),2)
	print (y_actual.shape)
	y1 = (ModelName.predict(X))
	y_hat = np.argmax((y1.reshape((y1.shape[0],int(y1.shape[1]/3),3))),2)
	print (y_hat.shape)
	A = (np.count_nonzero(y_hat==y_actual))/(y_actual.shape[0]*y_actual.shape[1])
	return (A)

positions_file = sys.argv[3]
weights_file = sys.argv[4]
bias_file = sys.argv[5]

fh = open(positions_file,'w+')
fh.close()
fh = open(weights_file,'w+')
fh.close()
fh = open(bias_file,'w+')
fh.close()
import os
TT_list = [10]
TrainAccuracy = []
AvgTrainAccuracy = []
for top_tags in TT_list:
	for i in np.arange(0,int(ytr.shape[1])):#int(ytr.shape[1])
		print ("Top tags: ",top_tags,"i value: ",i)
		print ("Running for Target SNP: ",i)

		ytr_one = ytr[:,i].astype('int')
		ytr_one_ohe = tf.keras.utils.to_categorical(ytr_one,3)
		Selector = SelectKBest(score_func=mutual_info_classif, k=top_tags)
		Tops = Selector.fit(Xtr, ytr_one)
		Indices_sorted = (np.sort(np.argsort(Tops.scores_)[::-1][:top_tags])).reshape((1, top_tags))
		print (type(Indices_sorted),Indices_sorted.shape)
		string_to_write = np.array2string(Indices_sorted)[1:-1]
		fh = open(positions_file,'a')
		np.savetxt(fh,Indices_sorted,delimiter=',',fmt  = "%4d")
		
		fh.close()

		X_train = OneHotEncoder(Tops.transform(Xtr)).reshape((Xtr.shape[0],top_tags*3))
		TagNos = X_train.shape[1]

		from keras import backend as K
		K.clear_session()

		model_10k = Sequential()
		model_10k.add(Dense(ytr_one_ohe.shape[1], input_dim=TagNos,bias_constraint=keras.constraints.NonNeg(),kernel_constraint=keras.constraints.NonNeg()))
		#opt = keras.optimizers.Adam(lr=0.0008, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False)
		model_10k.compile(loss='categorical_crossentropy', optimizer='adam')
		model_10k.summary()  
		reduce_lr = keras.callbacks.ReduceLROnPlateau(monitor='loss', factor=0.5,patience=3, min_lr=0.00001)
		filepath="Model{epoch}.hdf5"
		model_10k.fit(X_train, ytr_one_ohe, epochs=30, verbose=1,callbacks=[reduce_lr,keras.callbacks.TerminateOnNaN()])

		W,b = model_10k.layers[0].get_weights()
		W[W==-0.0]=0.0
		b[b==-0.0]=0.0
		f=open(weights_file,'ab')
		np.savetxt(f,W,delimiter=',',fmt='%1.4f')
		f.close()
		f=open(bias_file,'ab')
		np.savetxt(f,b,delimiter=',',fmt='%1.4f')
		f.close()

		X_train = OneHotEncoder(Tops.transform(Xtr)).reshape((Xtr.shape[0],top_tags*3))
		ytr_one = ytr[:,i].astype('int')
		ytr_hat = np.argmax(model_10k.predict(X_train),1)
		acc = (np.count_nonzero(ytr_one==ytr_hat))/X_train.shape[0]
		TrainAccuracy.append(acc)
		print ("Till now: ",i, "Mean Train Accuracy is: ", np.mean(TrainAccuracy))
	AvgTrainAccuracy.append(np.mean(TrainAccuracy))

print ("----------------------Training complete-------------------------")
