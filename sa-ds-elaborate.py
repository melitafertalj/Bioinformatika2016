import sys
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
    outputFile.write('Beginning\n')
    outputFile.write('-------------------------------\n')
    outputFile.write('S: ' + str(S) + '\n')
    outputFile.write('SA: ' + str(SA) + '\n')
    outputFile.write('n: ' + str(n) + '\n') 
    outputFile.write('K: ' + str(K) + '\n') 
    
    #LS categorization
    t = [0] * n
    LSTypeChar(S, t, n)
    outputFile.write('t: ' + str(t) + '\n')
    
    #d-critical characters
    SA1 = SA #list of n ints, initially zeros
    n1 = dCriticalChars(S, t, n, SA1, 2) #d = 2
    SA1 = SA1[0:n1] #cut away unnecessary zeros

    outputFile.write('SA1: ' + str(SA1) + '\n')
    
    #bucket sorting
    s1 = SA[0:n1] #list of n1 ints, initially zeros
    
    outputFile.write('-------------------------------\n')

    bkt = [0] * (K+1)
    #sorting 4-character 2-critical substrings by type of 4th character
    bucketSortLS(SA1, s1, S, t, n, n1, 3)  # 3 = d + 2 - 1
    outputFile.write('LS pass: ' + str(s1) + '\n')
    
    #sorting 4-character 2-critical substring by 4th character
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 3)
    outputFile.write('Pass 1: ' + str(SA1) + '\n')
    
    #sorting ... by 3rd character
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 2)
    outputFile.write('Pass 2: ' + str(s1) + '\n')
    
    #sorting ... by 2nd character
    bucketSort(s1, SA1, S, t, n, n1, K, bkt, 1)
    outputFile.write('Pass 3: ' + str(SA1) + '\n')
    
    #sorting ... by 1st character
    bucketSort(SA1, s1, S, t, n, n1, K, bkt, 0)
    outputFile.write('Pass 4: ' + str(s1) + '\n')
    outputFile.write('-------------------------------\n')

    del bkt

    #naming
    #SA = [s1[0], -1, s1[1], -1, ..., s1[n1-1], -1, ?, -1, ?...]
    for i in xrange(n1-1, -1, -1):
        j = 2 * i
        SA[j] = s1[i]
        SA[j+1] = -1
    for i in xrange(2*(n1-1) + 3, n, 2):
        SA[i] = -1
    
    name = 0
    c = [-1, -1, -1, -1] #last encountered d-critical substring
    for i in xrange(n1):
        pos = SA[2*i]
        diff = 0
        #compare the current substring to c
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
            if (S[pos+3]*2 + t[pos+3]) != c[3]: #omega-weight
                diff = 1
        else:
            if c[3] != -1:
                diff = 1
        #if there is a difference, increase name and update c 
        if diff == 1:
            name += 1
            for h in xrange(3):
                if (pos+h) < n:
                    c[h] = S[pos+h]
                else:
                    c[h] = -1
            if (pos+3) < n:
                c[3] = S[pos+3]*2 + t[pos+3] #omega-weight
            else:
                c[3] = -1
        if (pos%2 == 0):
            pos -= 1
        #assign name. Position is strange but puts 0 (sentinel) on the end.
        SA[pos] = name - 1
    
    #copy names in s1
    i = n/2*2 - 1
    j = n - 1
    while (i >= 0 and j >= 0):
        if SA[i] != -1:
            SA[j] = SA[i]
            j -= 1
        i -= 2
    s1 = SA[-n1 :]
    
    #solving reduced problem
    outputFile.write('s1: ' + str(s1) + '\n')
    
    #recursion
    if name < n1:
        outputFile.write('\nRecursion:\n')
        SA_DS(s1, SA1, n1, name-1)
        outputFile.write('\nRecursion ended.\n\n')
    else:
        for i in xrange(n1):
            SA1[s1[i]] = i

    SA[0:n1] = s1
    SA[-n1:] = SA1
    
    outputFile.write('-------------------------------\n')
    outputFile.write('Inducing SA:\n')
    outputFile.write('SA1: ' + str(SA1) + '\n')
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
    outputFile.write('Step 1: ' + str(SA) + '\n')
    
    getBuckets(S, bkt, n, K, 0)
    for i in xrange(n):
        j = SA[i] - 1
        if j >= 0 and t[j] == 0:
            SA[bkt[S[j]]] = j
            bkt[S[j]] += 1
    outputFile.write('Step 2: ' + str(SA) + '\n')
    
    getBuckets(S, bkt, n, K, 1)
    for i in xrange(n - 1, -1, -1):
        j = SA[i] - 1
        if j >= 0 and t[j] == 1:
            bkt[S[j]] -= 1
            SA[bkt[S[j]]] = j
    outputFile.write('Step 3: ' + str(SA) + '\n')
    
    del bkt, t, SA1, s1


#main
start = time.clock()
K = 255

if len(sys.argv) != 3:
    print 'Pogresan broj argumenata!'
else:
    with open(sys.argv[1], "r") as inputFile:
        lines = inputFile.read().splitlines()

    S = []
    for line in lines:
        if line[0] != '>':
            S.extend(line)

    with open("EnteroBezKom.txt", "w") as ent:
        for line in S:
            ent.write(line)

    for i in xrange(len(S)):
        S[i] = ord(S[i])
    S.append(0)

    n = len(S)
    SA = [0] * n #result

    outputFile = open(sys.argv[2], "w")
    SA_DS(S, SA, n, K)
    outputFile.close()    
##    with open(sys.argv[2], "w") as outputFile:
##        outputFile.write(', '.join(str(x) for x in SA))
end = time.clock()
print 'Time elapsed (in seconds): ', end - start

