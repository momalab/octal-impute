import sys

def main():
    if len(sys.argv) < 5:
        print('Usage: python3 memory.py memory_encryption memory_querier memory_decryption memory_report')
        exit(1)

    inputs = sys.argv[1:-1]
    output = sys.argv[-1]
    MAX_RESIDENT_MEMORY_IDX = 9
    tags = ['Encryption', 'Query     ', 'Decryption']

    with open(output, 'w') as fout:
        for idx,input in enumerate(inputs):
            with open(input) as fin:
                line = fin.read().splitlines()[MAX_RESIDENT_MEMORY_IDX]
                fout.write( '{} : {}\n'.format(tags[idx], line) )

if __name__ == '__main__':
    main()
