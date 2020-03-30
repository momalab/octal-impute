import numpy as np
import pandas
import sys
import tensorflow as tf

def oneHotEncoder(matrix):
    temp = np.zeros( (matrix.shape[0], matrix.shape[1], 3) )
    for num, i in enumerate(matrix):
        temp[num] = tf.keras.utils.to_categorical(i,3)
    return temp

def parse(filein, filepos, fileout):
    xtest = np.loadtxt(filein, delimiter=',')
    positions = np.loadtxt(filepos, delimiter=',')
    nTargets = positions.shape[0]
    with open(fileout, 'w') as fout:
        for t in range(nTargets):
            tagPositions = positions[t].astype('int')
            xtestp = np.zeros( (xtest.shape[0], tagPositions.shape[0]) )
            for num, i in enumerate(tagPositions):
                xtestp[:,num] = xtest[:,i]
            xp = ( oneHotEncoder(xtestp) ).reshape( (xtestp.shape[0], xtestp.shape[1]*3) )
            # np.savetxt( '{}.{}'.format(fileout,t) , xp, delimiter=' ',fmt='%1d' )
            np.savetxt( fout , xp, delimiter=' ',fmt='%1d' )
            fout.write('-\n')

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('Usage: python parser.py tagSNPs positions fileout')
        exit(1)
    tagSNPs = sys.argv[1]
    filepos = sys.argv[2]
    fileout = sys.argv[3]
    parse(tagSNPs, filepos, fileout)
