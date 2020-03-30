import numpy
import sys

def main():
    if len(sys.argv) < 3:
        print('Usage: python normalizer.py filein fileout')
        exit(1)
    m = numpy.loadtxt(sys.argv[1],delimiter=' ')
    m = m/numpy.sum(m,0)
    numpy.savetxt(sys.argv[2], m, fmt='%.6f', delimiter=' ')

if __name__ == '__main__':
    main()
