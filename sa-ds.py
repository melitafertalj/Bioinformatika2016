import time

#getting the L and S types for each character
def LSTypeChar(S, t, n):
    for x in xrange(n-1, -1, -1):
        if S[x] == 0:
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
    while (i < n-1):
        isLMS = 0
        for h in xrange(2, (d+1) + 1):
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
        p1[j] = i
        j = j + 1
    return j

#L/S bucket sort
def bucketSortLS(src, dest, S, t, n, n1, h):
    #print 'Src: ', src
    #print 'Dest: ', dest
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

#d-critical substrings bucket sort
def bucketSort(src, dest, S, t, n, n1, K, c, d):
    v_sum = 0
    for i in xrange(K+1):
        c[i] = 0
    for i in xrange(n1):
        j = src[i] + d
        if (j > n-1):
            j = n - 1
        c[S[j]] += 1
    for i in xrange(K+1):
        v_len = c[i]
        c[i] = v_sum
        v_sum = v_sum + v_len
    for i in xrange(n1):
        j = src[i] + d
        if (j > n-1):
            j = n - 1
        dest[c[S[j]]] = src[i]
        c[S[j]] += 1
    return

#compute start/end of buckets
def getBuckets(S, bkt, n, K, end):
    v_sum = 0
    for i in xrange(K+1):
        bkt[i] = 0
    for i in xrange(n):
        bkt[S[i]] += 1
    for i in xrange(K+1):
        v_sum += bkt[i]
        if end == 1:
            bkt[i] = v_sum
        else:
            bkt[i] = v_sum - bkt[i]


#recursive function - core of SA_DS algorithm
def SA_DS(S, SA, n, K):
    #print 'Beginning'
    #print '-------------------------------'
    #print 'S: ', S
    #print 'SA: ', SA
    #print 'n: ', n
    #print 'K: ', K
    
    #LS categorization
    t = [0] * n
    LSTypeChar(S, t, n)
    #print 't: ', t
    
    #d-critical characters
    SA1 = SA
    n1 = dCriticalChars(S, t, n, SA1, 2) #d = 2
    SA1 = SA1[0:n1]

    #print 'SA1: ', SA1
    #print 'D-critical characters found.'
    
    #bucket sorting
    s1 = SA[n-n1 :]
    #print 's1: ', s1
    #print '-------------------------------'
    bkt = [0] * (K+1)
    bucketSortLS(SA1, s1, S, t, n, n1, 3)  # 3 = d + 2 - 1
    #print 'LS pass: ', s1
    #print 'Bucket sort LS done.'
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 3)
    #print 'Pass 1: ', SA1
    #print 'Bucket sort pass 1 done.'
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 2)
    #print 'Pass 2: ', s1
    #print 'Bucket sort pass 2 done.'
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 1)
    #print 'Pass 3: ', SA1
    #print 'Bucket sort pass 3 done.'
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 0)
    #print 'Pass 4: ', s1
    #print 'Bucket sort pass 4 done.'
    #print '-------------------------------'
    del bkt

    SA[0:n1] = SA1
    SA[-n1:] = s1
    
    #naming
    for i in xrange(n1-1, -1, -1):
        j = 2 * i
        SA[j] = s1[i]
        SA[j+1] = -1
    for i in xrange(2*(n1-1) + 3, n, 2):
        SA[i] = -1
    
    name = 0
    c = [-1, -1, -1, -1]
    for i in xrange(n1):
        pos = SA[2*i]
        diff = 0
        for h in xrange(3):
            if pos+h < n:
                if S[pos+h] != c[h]:
                    diff = 1
                    break
            else:
                if c[h] != -1:
                    diff = 1
                    break
        if pos+3 < n:
            if (S[pos+3]*2 + t[pos+3]) != c[3]:
                diff = 1
        else:
            if c[3] != -1:
                diff = 1
        if diff == 1:
            name += 1
            for h in xrange(3):
                if (pos+h) < n:
                    c[h] = S[pos+h]
                else:
                    c[h] = -1
            if (pos+3) < n:
                c[3] = S[pos+3]*2 + t[pos+3]
            else:
                c[3] = -1
        if (pos%2 == 0):
            pos -= 1
        SA[pos] = name - 1
    
    #filling in the gaps in SA
    i = n/2*2 - 1
    j = n - 1
    while (i >= 0 and j >= 0):
        if SA[i] != -1:
            SA[j] = SA[i]
            j -= 1
        i -= 2
    s1 = SA[n-n1 :]
    
    #solving reduced problem
    #print 's1: ', s1, '\n'
    
    #recursion
    if name < n1:
        #print '\nRecursion:\n'
        SA_DS(s1, SA1, n1, name-1)
        #print '\nRecursion ended.\n\n'
    else:
        for i in xrange(n1):
            SA1[s1[i]] = i

    SA[0:n1] = s1
    SA[-n1:] = SA1
    
    #print 'SA1: ', SA1
    #print 'Inducing SA:'
    dCriticalChars(S, t, n, s1, 2)
    SA[-n1:] = s1
    bkt = [0] * (K+1)
    getBuckets(S, bkt, n, K, 1)
    for i in xrange(n1):
        SA1[i] = s1[SA1[i]]
    SA[0:n1] = SA1
    for i in xrange(n1, n):
        SA[i] = -1
    for i in xrange(n1-1, -1, -1):
        j = SA[i]
        SA[i] = -1
        if j > 0 and t[j] == 1 and t[j-1] == 0:
            bkt[S[j]] -= 1
            SA[bkt[S[j]]] = j
    #print 'Step 1: ', SA
    #print 'Step 1 done.'
    getBuckets(S, bkt, n, K, 0)
    for i in xrange(n):
        j = SA[i] - 1
        if j >= 0 and t[j] == 0:
            SA[bkt[S[j]]] = j
            bkt[S[j]] += 1
    #print 'Step 2: ', SA
    #print 'Step 2 done.'
    getBuckets(S, bkt, n, K, 1)
    for i in xrange(n - 1, -1, -1):
        j = SA[i] - 1
        if j >= 0 and t[j] == 1:
            bkt[S[j]] -= 1
            SA[bkt[S[j]]] = j
    #print 'Step 3: ', SA
    #print 'Step 3 done.'
    
    del bkt, t, SA1, s1


#main
K = 255

with open("Tests/haemo.txt", "r") as inputFile:
    lines = inputFile.read().splitlines()

S = []
for line in lines:
    if line[0] != '>':
        S.extend(line)
for i in xrange(len(S)):
    S[i] = ord(S[i])
S.append(0)

n = len(S)
SA = [0] * n #result

start = time.clock()
SA_DS(S, SA, n, K)
end = time.clock()
print 'Time elapsed (in seconds): ', end - start

with open("output.txt", "w") as outputFile:
    outputFile.write(', '.join(str(x) for x in SA))

strS = ''.join(str(x) for x in S)
isSorted = 'Sorted!'
for i in xrange(1, len(SA)):
    if S[SA[i-1] :] > S[SA[i] :]:
        isSorted = 'Not sorted! ' + str(i) + '. suffix is smaller than ' + str(i+1) + '.'
        break
print isSorted
