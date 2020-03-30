import numpy as np
W = (np.loadtxt('W_Logreg.txt',delimiter=' ')).transpose()
b = (np.loadtxt('b_Logreg.txt',delimiter=' ')).transpose()
X = np.loadtxt('XTest_Logreg.txt',delimiter=' ')

def scale(Matrix,min_value,max_value):
    #min_value = np.min(Matrix)
    #max_value = np.max(Matrix)
    ScaledMatrix = (Matrix-min_value)/(max_value-min_value)
    return (ScaledMatrix,min_value,max_value)
def InverseScale(Scaled_Matrix,min_value,max_value):
    Matrix = (ScaledMatrix*(max_value-min_value))+min_value
    return (Matrix)

ulti_max = np.max([np.min(W),np.max(W),np.min(b),np.max(b)])
ulti_min = np.min([np.min(W),np.max(W),np.min(b),np.max(b)])

W_scaled,W_min,W_max = scale(W,ulti_min,ulti_max)
b_scaled,b_min,b_max = scale(b,ulti_min,ulti_max)

np.savetxt('W_Logreg_pos.txt', W_scaled, fmt='%.6f', delimiter=' ')
np.savetxt('b_Logreg_pos.txt', b_scaled, fmt='%.6f', delimiter=' ')


############################################################# Both positive and negative weights ##########################
z = np.dot(X,W)+b
O = np.exp(z)/(np.sum(np.exp(z),1))[:,None]
###########################################################################################################################
'''
Formula explanation
z = | z11 z12 z13 |
	| z21 z22 z23 |
	| z31 z32 z33 |
	|
	|
	|_			 _|

O = | (e^z11)/(e^z11+e^z12+e^13)	(e^z12)/(e^z11+e^z12+e^13)	(e^z13)/(e^z11+e^z12+e^13) |
	| (e^z21)/(e^z21+e^z22+e^23)	(e^z22)/(e^z21+e^z22+e^23)	(e^z23)/(e^z21+e^z22+e^23) |
	|
	|
	|_																					  _|

'''

############################################################# Only positive weights #######################################
z_scaled = np.dot(X,W_scaled)+b_scaled
O_scaled = np.exp(z_scaled)/(np.sum(np.exp(z_scaled),1))[:,None]
###########################################################################################################################
np.savetxt('Output_prob_one_target.txt',O_scaled,delimiter=' ')

output_scaled = np.argmax(O_scaled,1)
output = np.argmax(O,1)

output = np.argmax(O,1)

print ("No of values which are different:",np.count_nonzero(output!=output_scaled))
