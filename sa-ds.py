#getting the L and S types for each character
def LSTypeChar(S, t, n):
    for x in xrange(n-1, -1, -1):
        if S[x] == '$':
            t[x] = 1
        elif S[x] < S[x+1]:
            t[x] = 1
        elif S[x] == S[x+1]:
            t[x] = t[x+1]
    return

#getting the list of d-critical characters
def dCriticalChars(S, t, n, p1, d):
    i = -1
    j = 0
    while (i < len(S) - 1):
        isLMS = 0
        for h in xrange(2, (d + 1) + 1):
            if (t[i + h - 1] == 0) and (t[i + h] == 1):
                isLMS = 1
                break
        if (j == 0) and (isLMS == 0):
            i = i + d
            continue
        if isLMS == 1:
            i = i + h
        else:
            i = i + d
        p1.append(i)
        j = j + 1
    return j

#bucket sort
#L/S bucket
def bucketSortLS(src, dest, S, t, n, n1, h):
    start = 0
    end = n1 - 1
    for i in xrange(0, n1):
        j = src[i] + h
        if (j > n-1):
            j = n - 1
        if (t[j] == 1):
            dest[end] = src[i]
            end = end - 1
        else:
            dest[start] = src[i]
            start = start + 1
    return

#sort by d-critical substrings
def bucketSort(src, dest, S, t, n, n1, K, c, d):
    v_sum = 0
    for i in xrange(K+1):
        c[i] = 0
    for i in xrange(n1):
        j = src[i] + d
        if (j > n-1):
            j = n - 1
        c[ord(S[j])] += 1
    for i in xrange(K+1):
        v_len = c[i]
        c[i] = v_sum
        v_sum = v_sum + v_len
    for i in xrange(n1):
        j = src[i] + d
        if (j > n-1):
            j = n - 1
        dest[c[ord(S[j])]] = src[i]
        c[ord(S[j])] += 1
    return

#main
S = ('m', 'm', 'i', 'i', 's', 's', 'i', 'i', 's', 's', 'i', 'i', 'p', 'p', 'i',
     'i', '$')
t = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
SA = [] #result
SA1 = []
s1 = []
d = 2
n = len(S)
K = 255
bkt = []
for i in xrange(K+1):
    bkt.append(0)

LSTypeChar(S, t, n)
n1 = dCriticalChars(S, t, n, SA1, d)

print S
print t
print SA1

s1 = list(SA1)
bucketSortLS(s1, SA1, S, t, n, n1, 3)  # 3 = d + 2 - 1
print SA1
bucketSort(SA1, s1, S, t, n, n1, K, bkt, 3)
print 'Pass 1: ', s1
bucketSort(s1, SA1, S, t, n, n1, K, bkt, 2)
print 'Pass 2: ', SA1
bucketSort(SA1, s1, S, t, n, n1, K, bkt, 1)
print 'Pass 3: ', s1
bucketSort(s1, SA1, S, t, n, n1, K, bkt, 0)
print 'Pass 4: ', SA1
del bkt

SA = [None] * n
for i in xrange(n1-1, -1, -1):
    j = 2 * i
    SA[j] = SA1[i]
    SA[j+1] = -1
for i in xrange(2*(n1-1)+3, n, 2):
    SA[i] = -1

print 'SA: ', SA
print 'SA1: ', SA1

name = 0
c = [-1, -1, -1, -1, -1]
for i in xrange(n1):
    pos = SA[2*i]
    diff = 0
    for h in xrange(4):
        if ord(S[pos+h]) != c[h]:
            diff = 1
            break
    if (ord(S[pos+4])*2 + t[pos+4]) != c[4]:
        diff = 1
    if diff == 1:
        name += 1
        for h in xrange(4):
            if (pos+h) < n:
                c[h] = ord(S[pos+h])
            else:
                c[h] = -1
        if (pos+h) < n:
            ch[h] = ord(S[pos+h])*2 + t[pos+h]
        else:
            c[h] = -1
    if (pos%2 == 0):
        pos -= 1
    SA[pos] = name - 1
print 'SA: ', SA
