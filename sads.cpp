#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
#include <memory.h>
#include <time.h>

#define TESTPRINT 0

clock_t myStart, myFinish;
double  myDuration;

// globals, prototypes
extern int dst; // distance - global defined in is.cpp
void trackMemory(int mem); // mem tracker
// print int array
void printIntArr(char *msg, int *a, int n);
// print char array, in given format fmt
void prinCharArr(char *msg, unsigned char *a, int n, char *fmt);

#define s(i) (cs==sizeof(int)?((int*)s)[i]:((unsigned char *)s)[i])
#define s1(i) (cs==sizeof(int)?((int*)s1)[i]:((unsigned char *)s1)[i])
#define s2(i) (cs==sizeof(int)?((int*)s1)[i]:((unsigned char *)s2)[i])
#define tget(i) (t[i] == 1 ? t[i] : 0)
#define tset(i, b) t[(i)]=(b)

void myTimerStart() {
  myStart = clock();
}

void myTimerFinish(char *str) {
  myFinish = clock();
  myDuration = (double)(myFinish - myStart) / CLOCKS_PER_SEC;
  fprintf(stderr, "\n%s: %.2lf seconds", str, myDuration);
}


// store to p1 the pointers for all the critical characters in s[0..n-1]
int mycriticalChars(unsigned char *s, unsigned char *t, int *p1, int n, int cs) {
  int i, j;
  int d;
  d = dst; // set distance

  for (i = 1, j = 0; i <= n; i += d, j++) {
    //s[i-2] is type-L and s[i-1] is type-S
    if (i>1 && tget(i - 2) == 0 && tget(i - 1) == 1) i--;
    else
      //s[i] is type-L and s[i+1] is type-S
    if (tget(i) == 0 && tget(i + 1) == 1) i++;
    if (p1 != NULL) p1[j] = i;
  }
  return j;
}

// compute the start and end of each bucket, and init SA
void mygetBuckets(unsigned char *s, int *bkt, int n, int K, int cs, bool end) {
  int i, sum = 0;
  for (i = 0; i <= K; i++) bkt[i] = 0; // clear all buckets
  for (i = 0; i<n; i++) bkt[s(i)]++; // compute the size of each bucket
  for (i = 0; i <= K; i++) { sum += bkt[i]; bkt[i] = end ? sum : sum - bkt[i]; }
}

//######################################################################

//check if there are d-critical chars in subarry t[from, to]
int hasDcrit(unsigned char *t, int from, int to)
{
  int i;
  for (i = from; i <= to; i++) {
    if (t[i] == 1) return 1;
  }
  return 0;
}

// sort a[0..n-1] to b[0..n-1] according to the LS-types of characters from s
// ### excluded arguments: s, K, cs 
static void myradixPassLS(int *a, int *b, unsigned char *t, int n1, int n, int d)
{ // count occurrences
  int i, j;

  int lIndex = 0;
  int rIndex = n1 - 1;

  for (i = 0; i < n1; i++) {
    j = a[i] + d;

    if (j>n - 1) j = n - 1;
    if (t[j]) b[rIndex--] = a[i]; // type-S
    else b[lIndex++] = a[i]; // type-L

  }
}

// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from s
// ### excluded arguments: t i cs
static void myradixPass(int *a, int *b, unsigned char *s, int n1, int K, int n, int d, int cs)
{ // count occurrences
  int i, j, sum, ch;
  int K2 = K + 2;
  int* c = new int[K2]; trackMemory(K2*sizeof(int)); // counter array
  for (i = 0; i < K2; i++) c[i] = 0; // reset counters
  for (i = 0; i < n1; i++) {
    j = a[i] + d;
    ch = (j >= n - 1) ? 0 : s(j) + 1;
    c[ch]++; // count occurences
  }
  for (i = 0, sum = 0; i < K2; i++) { // exclusive prefix sums
    int t = c[i];  c[i] = sum;  sum += t;
  }
  for (i = 0; i < n1; i++) {
    j = a[i] + d;
    ch = (j >= n - 1) ? 0 : s(j) + 1;
    b[c[ch]++] = a[i]; // sort non-sentinel characters
  }

  delete[] c;
}

// test if two string are equal within given length n
// strings start at s+ix
// depending of cs, s is array of chars or int
int stringsEqual(unsigned char *s, int i1, int i2, int n, int cs) {
  int i;
  for (i = 0; i < n; i++) {
    if (s[i1+i] != s[i2+i]) return 0;
  }
  return 1;
}

// copies a1 to a2
void copyArr(int *a1, int *a2, int n) {
  int i;
  for (i = 0; i < n; ++i)
    a2[i] = a1[i];
}

// find index of value in arr with length n
int srchArr(int *a, int val, int n) {
  int i;
  for (i = 0; i < n; ++i)
    if (a[i] == val) return i;
  return -1;
}

// generates array P1 based on array t and using local array LMS
// returns n1;
int genP1(unsigned char *t, int *SA, int n)
{
  int i, n1;
  // control array
  // according to the rules a) to e)
  unsigned char *LMS = new unsigned char[n + 1]; trackMemory((n + 1)*sizeof(char));
  LMS[0] = 0;
  for (i = n - 1; i >= 1; i--) {
    LMS[i] = (t[i] == 1 && t[i - 1] == 0) ? 1 : 0;
  }
  //prinCharArr("LMS", LMS, n, "%3d");

  for (i = dst + 1; i < n; i++) {
    if (LMS[i - dst - 1] == 1 || LMS[i - dst + 1] == 1) {
      LMS[i - dst] = 0;
    }
    else if (LMS[i - dst] != 1) {
      LMS[i - dst] = (i - dst > 0 && LMS[i + 1] != 1 && !hasDcrit(LMS, i - dst + 1, i - 1)) ? 1 : 0;
    }
  }
  prinCharArr("LMS", LMS, n, "%3d");

  // construct SA=P1 based on LMS array
  n1 = 0;
  for (i = 1; i < n; i++) {
    if (LMS[i] == 1) SA[n1++] = i;
  }
  delete[] LMS;
  return n1;
}

// my version of main algorithm
void mySA_DS(unsigned char *s, int *SA, int n, int K, int m, int level) {
  static double redu_ratio = 0;
  static long sum_n = 0, sum_n1 = 0;
  int cs = (level == 0) ? sizeof(char) : sizeof(int);

  fprintf(stderr, "\nLevel: %d", level);
  
  int i, j, n1;
  unsigned char *t = new unsigned char[n + 1]; trackMemory((n + 1)*sizeof(char)); // LS-type array in bits

  // stage 1: reduce the problem by at least 1/2

  myTimerStart();

  // Classify the type of each character
  t[n - 2] = 0;
  t[n - 1] = 1;
  for (i = n - 3; i >= 0; i--) {
    //  tset(i, (chr(i) < chr(i + 1) || (chr(i) == chr(i + 1) && tget(i + 1) == 1)) ? 1 : 0); 
    t[i] = (s(i) < s(i + 1) || (s(i) == s(i + 1) && t[i + 1] == 1)) ? 1 : 0;
  }

#if TESTPRINT
  printIntArr("Index", NULL, n);
  prinCharArr("S", s, n, "%3c");
  prinCharArr("t", t, n, "%3d");
#endif

  //n1 = mycriticalChars(s, t, SA, n, cs);
  n1 = genP1(t, SA, n); // P1 is at myStart of SA

  printIntArr("P1", SA, n1);

  int *oldP1 = new int[n1]; trackMemory(n1*sizeof(int));
  copyArr(SA, oldP1, n1);

  // lsb radix sort critical 5-tuplets
  // SA is sorted into new array b and back
  int *a = SA; // a is same as SA
  int *b = new int[n1]; trackMemory(n1*sizeof(int)); // temp for sort

  // my version is simpler but not efficient
  myradixPassLS(a, b, t, n1, n, 4);
  myradixPass(b, a, s, n1, K, n, 4, cs);

  // normal bucket sort, one character per item
  myradixPass(a, b, s, n1, K, n, 3, cs);
  myradixPass(b, a, s, n1, K, n, 2, cs);
  myradixPass(a, b, s, n1, K, n, 1, cs);
  myradixPass(b, a, s, n1, K, n, 0, cs);

  // printIntArr(" a", a, n1); // a equals sorted SA
  delete[] b;

  myTimerFinish("Time for sorting all the d-critical substrings");

  // SA[0, n1-1] stores the sorted d-critical substrings of s now
  printIntArr("SA", SA, n1);

  myTimerStart();

  // my giving new values to SA, then sorting 
  int length; // length of substring comparison
  int *newP1 = new int[n1]; trackMemory(n1*sizeof(int));
  int currval = 0; // starting new P1 values
  int name = 1; // no of different names, at least one for $, eg newP1[0] = 0;

  newP1[0] = 0;
  for (i = 1; i < n1; ++i) {  // for all substrings starting with d-char
    length = dst + 2; 
    if (SA[i] + length > n) length = n - SA[i]; // not to exceed string s
    // if (stringsEqual(s + SA[i], s + SA[i - 1], length, cs)) { // duplicates
    if (stringsEqual(s, SA[i], SA[i - 1], length, cs)) { // duplicates
        newP1[i] = currval;
    }
    else { // unique
      newP1[i] = ++currval;
      ++name;
    }
  }
  //printIntArr("oldP1", oldP1, n1);
  //printIntArr("newP1", newP1, n1);
  
  // originani algoritam ima pogrešku da ostavlja jedan -1 između SA i s1
  SA[n1] = -1; // GAP
  int oldIndex; // index of SA[i] in oldP1
  for (i = 0; i < n1; ++i) {
    oldIndex = srchArr(oldP1, SA[i], n1);
    //SA[n1 + oldIndex + 1] = newP1[i]; // there is a GAP !!!
    //SA[m - (n1 - oldIndex) - 1] = newP1[i]; // no GAP  
    SA[m - (n1 - oldIndex)] = newP1[i]; // same GAP
  }
  ++name; // for GAP

  delete[] oldP1;
  delete[] newP1;

  myTimerFinish("Time for naming");

  // s1 is now stored in SA[m-n1, m-1]
  int *SA1 = SA;
  int *s1 = SA + m - n1; // &SA[m - n1]

  printIntArr("SA", SA, m);
  printIntArr("S1", s1, n1); //   printIntArr("S1", &SA[m - n1], n1);
  printIntArr("SA1", SA1, n1);

  // stage 2: solve the reduced problem

  fprintf(stderr, "\nReduction ratio: %.2lf", (double)n1 / n);
  redu_ratio += (double)n1 / n;
  sum_n1 += n1; sum_n += n;
  // recurse if names are not yet unique
  if (name<n1) {
    mySA_DS((unsigned char*)s1, SA1, n1, name - 1, m - n1, level + 1);
  }
  else { // generate the suffix array of s1 directly
    for (i = 0; i<n1; i++) SA1[s1[i]] = i;
    printIntArr("SA1", SA1, n1);

    cerr << endl << "Recursion ends";
    fprintf(stderr, "\nMean reduction ratio over iterations: %.2lf", redu_ratio / (level + 1));
    fprintf(stderr, "\nMean reduction ratio over characters: %.2lf", (double)sum_n1 / sum_n);
  }

  fprintf(stderr, "\nLevel: %d", level);

  // stage 3: induce the result for the original problem
  
  myTimerStart();

  // compute suffix array for critical chars in s
  //mycriticalChars(s, t, s1, n, cs); // get p1
  genP1(t, s1, n); // n1 remains the same

  int *bkt = new int[K + 1]; trackMemory((K + 1)*sizeof(int)); // bucket counters

  // put all left-most S characters into their buckets
  mygetBuckets(s, bkt, n, K, cs, true); // find ends of buckets
  for (i = 0; i<n1; i++) SA1[i] = s1[SA1[i]]; // get index in s1
  for (i = n1; i<n; i++) SA[i] = -1; // init SA[n1..n-1]
  for (i = n1 - 1; i>0; i--) {
    j = SA[i]; SA[i] = -1;
    if (j>0 && tget(j) && !tget(j - 1)) SA[--bkt[s(j)]] = j;
  }

  printIntArr("Step 1:", SA1, n);

  // compute SAl
  mygetBuckets(s, bkt, n, K, cs, false); // find starts of buckets
  if (level == 0)
    bkt[0]++; // handle the virtual sentinel
  for (i = 0; i<n; i++) {
    j = SA[i] - 1;
    if (j >= 0 && !tget(j)) SA[bkt[s(j)]++] = j;
  }

  printIntArr("Step 2:", SA1, n);

  // compute SAs
  mygetBuckets(s, bkt, n, K, cs, true); // find ends of buckets
  for (i = n - 1; i>0; i--) {
    j = SA[i] - 1;
    if (j >= 0 && tget(j)) SA[--bkt[s(j)]] = j;
  }
 
  printIntArr("Step 3:", SA1, n);

  myTimerFinish("Time for sorting all the suffixes");

  delete[] bkt;
  delete[] t;

}
