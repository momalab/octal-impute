import sys

def main():
    if len(sys.argv) < 3:
        print("Inform input and output files")
        exit(1)
    filein = sys.argv[1]
    fileout = sys.argv[2]
    with open(fileout, 'w') as fout:
        fout.write("0,1,2\n")
        with open(filein) as fin:
            for line in fin:
                if line.strip() != "-":
                    try:
                        if 'nan' in line:
                            probs = [1.0, 0.0, 0.0]
                        else:
                            probs = [ float(x) for x in line.strip().split(' ')]
                        fout.write("{},{},{}\n".format(probs[0], probs[1], probs[2]))
                    except:
                        fout.write("1.0,0.0,0.0\n")

if __name__ == "__main__":
    main()
