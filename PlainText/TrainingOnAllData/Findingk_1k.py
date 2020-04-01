# 2nd September, 2019
#Author Esha Sarkar
import pandas
import numpy as np
import keras

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

# Xtr = np.loadtxt('XTrain_1k.txt',delimiter=',').astype('int')
# Xts = np.loadtxt('XTest_1k.txt',delimiter=',').astype('int')
# ytr = np.loadtxt('YTrain_1k.txt',delimiter=',').astype('int')
# yts = np.loadtxt('YTest_1k.txt',delimiter=',').astype('int')

Xtr = np.loadtxt('DatasetX_1k.txt',delimiter=',').astype('int')
ytr = np.loadtxt('DatasetY.txt',delimiter=',').astype('int')


print (Xtr.shape,ytr.shape)

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
fh = open('Positions_1k_10.txt','w+')
fh.close()
fh = open('Weights_1k_10.txt','w+')
fh.close()
fh = open('bias_1k_10.txt','w+')
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
		#for po in Indices_sorted:
		fh = open('Positions_1k_10.txt','a')
		np.savetxt(fh,Indices_sorted,delimiter=',',fmt  = "%4d")
		#fh.write(string_to_write)
		#fh.write('\n')
		
		fh.close()

		X_train = OneHotEncoder(Tops.transform(Xtr)).reshape((Xtr.shape[0],top_tags*3))
		#X_train = Tops.transform(Xtr)
		TagNos = X_train.shape[1]

		from keras import backend as K
		K.clear_session()

		model_1k = Sequential()
		model_1k.add(Dense(ytr_one_ohe.shape[1], input_dim=TagNos,bias_constraint=keras.constraints.NonNeg(),kernel_constraint=keras.constraints.NonNeg()))
		opt = keras.optimizers.Adam(lr=0.0008, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False)
		model_1k.compile(loss='categorical_crossentropy', optimizer=opt)
		model_1k.summary()  
		reduce_lr = keras.callbacks.ReduceLROnPlateau(monitor='loss', factor=0.5,patience=3, min_lr=0.00001)
		filepath="Model{epoch}.hdf5"
		model_1k.fit(X_train, ytr_one_ohe, epochs=30, verbose=1,callbacks=[reduce_lr,keras.callbacks.TerminateOnNaN()])

		W,b = model_1k.layers[0].get_weights()
		W[W==-0.0]=0.0
		b[b==-0.0]=0.0
		f=open('Weights_1k_10.txt','ab')
		np.savetxt(f,W,delimiter=',',fmt='%1.4f')
		f.close()
		f=open('bias_1k_10.txt','ab')
		np.savetxt(f,b,delimiter=',',fmt='%1.4f')
		f.close()

		#from keras import backend as K
		#K.clear_session()

		#model_ = Sequential()
		#model_.add(Dense(ytr_one_ohe.shape[1], input_dim=TagNos,bias_constraint=keras.constraints.NonNeg(),kernel_constraint=keras.constraints.NonNeg()))
		#opt = keras.optimizers.Adam(lr=0.0008, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False)
		#model_.compile(loss='categorical_crossentropy', optimizer=opt)


		#files = os.listdir(os.getcwd())
		#models = [filename for filename in files if filename.startswith('Model') and filename.endswith('.hdf5')]
		#model_numbers = []
		#for model_name in models:
		#    model_numbers.append(int(model_name[model_name.index('l')+1:model_name.index('.')])) 
		#latest_model = 'Model'+str(np.sort(model_numbers)[-2])+'.hdf5'
		#print (latest_model)
		#model_.load_weights(latest_model)

		X_train = OneHotEncoder(Tops.transform(Xtr)).reshape((Xtr.shape[0],top_tags*3))
		#X_test = Tops.transform(Xts)
		ytr_one = ytr[:,i].astype('int')
		ytr_hat = np.argmax(model_1k.predict(X_train),1)
		acc = (np.count_nonzero(ytr_one==ytr_hat))/X_train.shape[0]
		#print (acc)
		TrainAccuracy.append(acc)
		print ("Till now: ",i, "Mean Test Accuracy is: ", np.mean(TrainAccuracy))
	AvgTrainAccuracy.append(np.mean(TrainAccuracy))
print ("Average Test Accuracy over 500 SNPs: ", np.mean(AvgTrainAccuracy))
