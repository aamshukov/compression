import os
import sys

from random import randint

def generate():
    pairs = [ [1,2], [1,3], [1,4], [2,1], [2,3], [2,4], [3,1], [3,2], [3,4], [4,1], [4,2], [4,3] ]
    with open(r'd:\tmp\pf.gen.txt', 'w') as stream:
        for k in range(1, 16 * 1024):
            pair = pairs[randint(0, 11)]
            stream.write(str(pair[0]) + str(pair[1]) + ' ')
            if k % 32 == 0:
                stream.write('\n')


def main(args):
    try:
        generate()
    except Exception as ex:
        print(ex)
    return 1


if __name__ == '__main__':
    main(sys.argv[1:])
    # buggy microft cannot fix the crap --- sys.exit(main(sys.argv[1:]))
