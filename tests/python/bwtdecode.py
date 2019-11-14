import os

#  https://www.student.cs.uwaterloo.ca/~cs240/f18/modules/module10.pdf

#  BWT-decoding(C[0..n − 1])
#   C: string of characters over alphabet Σc
#   A <- array of size n
#   for i = 0 to n − 1
#       A[i] <- (C[i], i)
#   Stably sort A by first entry
#   S <- empty string
#   for j = 0 to n
#       if C[j] = $ break
#   repeat
#       j <- second entry of A[j]
#       append C[j] to S
#   until C[j] = $
#   return S

def bwt_decode():
    # S O B R   D D B
    C = 'SOBR\x00DDB'
    A = 8 * [0]

    for i in range(8):
        A[i] = (C[i], i)

    A = sorted(A, key=lambda x: x[0])

    for j in range(len(C)):
        if C[j] == '\x00':
            break

    S = ''

    while True:
        j = A[j][1]
        S += C[j]

        if C[j] == '\x00':
            break;

    print(S)


def main(args):
    try:
        bwt_decode()
    except Exception as ex:
        print(ex)
    return 1


if __name__ == '__main__':
    main(sys.argv[1:])
    # buggy microft cannot fix the crap --- sys.exit(main(sys.argv[1:]))
