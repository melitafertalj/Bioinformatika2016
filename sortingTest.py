import sys
import time

start = time.clock()

if len(sys.argv) != 3:
    print 'Wrong number of parameters!'
    sys.exit()

with open(sys.argv[2], "r") as SAfile:
    SA = SAfile.read().split(', ')

with open(sys.argv[1], "r") as Sfile:
    lines = Sfile.read().splitlines()

S = ''
for line in lines:
    if line[0] != '>':
        S = S + line
S = S + chr(0)

isSorted = 1
for i in xrange(1, len(SA)):
    a = int(SA[i-1])
    b = int(SA[i])
    while a < len(S) and b < len(S):
        if S[a] < S[b]:
            break
        elif S[a] == S[b]:
            a = a + 1
            b = b + 1
        else:
            isSorted = 0
            break
    if isSorted == 0:
        break
if isSorted == 0:
    msg = 'Not sorted! Mistake on positions ' + str(i-1) + ' and ' + str(i)
    msg = msg + ' of SA.'
    print msg
else:
    print 'Sorted!'

end = time.clock()
print 'Time elapsed (sec): ', end - start
