import sys

us = 1/10**6

def main():
    if len(sys.argv) < 5:
        print('Usage: python3 timer.py time_encryption time_querier time_decryption time_parser time_keygen output')
        exit(1)

    filenames = sys.argv[1:-1]
    fileout = sys.argv[-1]

    totaltime = 0
    runtimes = []
    for filename in filenames:
        with open(filename) as fin:
            runtime = float( fin.read() ) * us
            totaltime += runtime
            runtimes.append( runtime )

    with open(fileout, 'w') as fout:
        fout.write( '{}'.format('%.2f' % totaltime) )
        for runtime in runtimes[:3]:
            fout.write( ',{}'.format('%.2f' % runtime) )

if __name__ == '__main__':
    main()
