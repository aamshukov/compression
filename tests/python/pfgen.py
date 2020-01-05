import os
import sys
import itertools

from random import randint

def generate():
    pairs = [ [1,2], [1,3], [1,4], [2,1], [2,3], [2,4], [3,1], [3,2], [3,4], [4,1], [4,2], [4,3] ]
    with open(r'd:\tmp\pf.gen.txt', 'w') as stream:
        for k in range(1, 16 * 1024):
            pair = pairs[randint(0, 11)]
            stream.write(str(pair[0]) + str(pair[1]) + ' ')
            if k % 32 == 0:
                stream.write('\n')


def generate_vectors():
    from random import seed
    from random import randint
    n = 256
    r = 16
    with open(r'd:\tmp\pf.slices.gen.{}.txt'.format(n), 'w') as stream:
        for k in range(r):
            l = n - r * k
            z = [0] * l
            p = []
            ones = 0
            while True:
                c = randint(0, l - 1)
                if c not in p:
                    p.append(c)
                    z[c] = 1
                    ones += 1
                if ones == r:
                    break
            s = ''.join(str(x) for x in z) + '\n'
            print(s)
            stream.write(s)


def main(args):
    try:
        generate_vectors()
        generate()
    except Exception as ex:
        print(ex)
    return 1


def prime_factors(n):
    for i in itertools.chain([2], itertools.count(3, 2)):
        if n <= 1:
            break
        while n % i == 0:
            n //= i
            yield i


for n in prime_factors(10):
    print(n)


def generate_lzw_dictionary():
    for k in range(16):
        print("{0:b}".format(k))


if __name__ == '__main__':
    main(sys.argv[1:])
    # buggy microft cannot fix the crap --- sys.exit(main(sys.argv[1:]))
