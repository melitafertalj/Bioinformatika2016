#getting the L and S types for each character
def LSTypeChar(S, t, n):
    for x in xrange(n-1, -1, -1):
        if ord(S[x]) == 0:
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

#recursive function - core of SA_DS algorithm
def SA_DS(S, SA, n, K):
    d = 2
    print 'Pocetno stanje'
    print '-------------------------------'
    print 'S: ', S
    print 'SA: ', SA
    print 'n: ', n
    print 'K: ', K
    
    #LS categorization
    t = [0] * n
    LSTypeChar(S, t, n)
    print 't: ', t
    
    #d-critical characters
    SA1 = SA
    n1 = dCriticalChars(S, t, n, SA1, d)
    SA1 = SA1[0:n1]

    print 'SA1: ', SA1
    print '-------------------------------'

    #bucket sorting
    s1 = SA[n-n1 :]
    print 's1: ', s1
    print '-------------------------------'
    bkt = [0] * (K+1)
    bucketSortLS(SA1, s1, S, t, n, n1, 3)  # 3 = d + 2 - 1
    print 'LS pass: ', s1
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 3)
    print 'Pass 1: ', SA1
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 2)
    print 'Pass 2: ', s1
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 1)
    print 'Pass 3: ', SA1
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 0)
    print 'Pass 4: ', s1
    print '-------------------------------'
    del bkt

    SA[0:n1] = s1
    SA[-n1:] = SA1
    #print 'SA: ', SA

    #naming
    for i in xrange(n1-1, -1, -1):
        j = 2 * i
        SA[j] = SA1[i]
        SA[j+1] = -1
    for i in xrange(2*(n1-1) + 3, n, 2):
        SA[i] = -1

    #print 'SA: ', SA
    #print 'SA1: ', SA1

    name = 0
    c = [-1, -1, -1, -1]
    for i in xrange(n1):
        pos = SA[2*i]
        diff = 0
        #print 'I: ', i
        #print 'C: ', c
        #print 'S: ', S[pos : pos+4]
        for h in xrange(3):
            if pos+h < n:
                if ord(S[pos+h]) != c[h]:
                    #print 'Razlika1: ', ord(S[pos+h]), ' != ', c[h]
                    diff = 1
                    break
            else:
                if c[h] != -1:
                    diff = 1
                    break
        if pos+3 < n:
            if (ord(S[pos+3])*2 + t[pos+3]) != c[3]:
                #print 'Razlika2: ', (ord(S[pos+3])*2 + t[pos+3]), ' != ', c[3]
                diff = 1
        else:
            if c[3] != -1:
                diff = 1
        if diff == 1:
            name += 1
            for h in xrange(3):
                if (pos+h) < n:
                    c[h] = ord(S[pos+h])
                else:
                    c[h] = -1
            if (pos+3) < n:
                c[3] = ord(S[pos+3])*2 + t[pos+3]
            else:
                c[3] = -1
        if (pos%2 == 0):
            pos -= 1
        SA[pos] = name - 1
    #print 'SA: ', SA

    #filling in the gaps in SA
    i = n/2*2 - 1
    j = n - 1
    while (i >= 0 & i >= 0):
        if SA[i] != -1:
            SA[j] = SA[i]
            j -= 1
        i -= 2
    s1 = SA[n-n1 :]
    
    #solving reduced problem
    #print 'SA: ', SA
    print 's1: ', s1
    #print 'SA1: ', SA1

    print'\n'
    for i in xrange(n1):
        s1[i] = chr(s1[i])
    if name < n1:
        SA_DS(s1, SA1, n1, name-1)
    
    


#main
K = 255
S = ('m', 'm', 'i', 'i', 's', 's', 'i', 'i', 's', 's', 'i', 'i', 'p', 'p', 'i',
     'i', chr(0))
n = len(S)
SA = [0] * n #result

SA_DS(S, SA, n, K)
