import pandas
import numpy as np

TagGenotypes_10k = np.array(pandas.read_csv('sorted_tag_SNPs_10k_genotypes.txt', sep='\s+', delimiter=None, header=None, names=None, index_col=None))
TagGenotypes_1k = np.array(pandas.read_csv('sorted_tag_SNPs_1k_genotypes.txt', sep='\s+', delimiter=None, header=None, names=None, index_col=None))
TargetGenotypes = np.array(pandas.read_csv('sorted_target_SNP_genotypes.txt', sep='\s+', delimiter=None, header=None, names=None, index_col=None))
DatasetX1_10k = TagGenotypes_10k[:,4:].transpose()
DatasetX1_1k = TagGenotypes_1k[:,4:].transpose()
DatasetY1 = TargetGenotypes[:,4:].transpose()
ShufflingVector = np.random.permutation(DatasetX1_10k.shape[0])
DatasetX_10k = DatasetX1_10k[ShufflingVector]
DatasetX_1k = DatasetX1_1k[ShufflingVector]
DatasetY = DatasetY1[ShufflingVector]

print (DatasetX_10k.shape, DatasetX_1k.shape, DatasetY.shape)

np.savetxt('DatasetX_10k.txt',DatasetX_10k,delimiter=',',fmt='%d')
np.savetxt('DatasetX_1k.txt',DatasetX_1k,delimiter=',',fmt='%d')
np.savetxt('DatasetY.txt',DatasetY,delimiter=',',fmt='%d')