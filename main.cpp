#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define DEBUGLEVEL 1
#include <time.h>
#if !defined( unix )
#include <io.h>
#include <fcntl.h>
#endif

// comment to disable verifying the result SA
#define _verify_sa

// activate test print
#define TESTPRINT 0

// distance, default 2, change enable to test variances
int dst = 2; 
int curmem = 0; // memory usage
int maxmem = 0;

// print int array
void printIntArr(char *msg, int *a, int n) {
  int i;
#if TESTPRINT
  fprintf(stderr, "\n%7s", msg);
  if (a == NULL)
  for (i = 0; i < n; ++i) fprintf(stderr, "%3d", i);
  else
  for (i = 0; i < n; ++i) fprintf(stderr, "%3d", a[i]);
#endif
}

// print char array, in given format fmt
void prinCharArr(char *msg, unsigned char *a, int n, char *fmt) {
  int i;
#if TESTPRINT
  fprintf(stderr, "\n%7s", msg);
  if (a == NULL)
  for (i = 0; i < n; ++i)
    fprintf(stderr, "%3d", i);
  else
  for (i = 0; i < n; ++i)
    fprintf(stderr, fmt, a[i]); // a[i] > 0 ? a[i] : '$');
#endif
}

void trackMemory(int mem) {
  curmem += mem;
  if (curmem > maxmem) maxmem = curmem;
}

// prototypes
//void SA_DS(unsigned char *s, int *SA, int n, int K, int m, int level);
void mySA_DS(unsigned char *s, int *SA, int n, int K, int m, int level);

// output values:
//  1: s1<s2
//  0: s1=s2
// -1: s1>s2
int sless(unsigned char *s1, unsigned char *s2, unsigned int n) {
  for (unsigned int i = 0; i<n; i++) {
    if (s1[i] < s2[i]) return 1;
    if (s1[i] > s2[i]) return -1;
  }
  return 0;
}

// test if SA is sorted for the input string s
bool isSorted(unsigned int *SA, unsigned char *s, unsigned int n) {
  for (unsigned int i = 0; i < n - 1; i++) {
    unsigned int d = SA[i]<SA[i + 1] ? n - SA[i + 1] : n - SA[i];
    int rel = sless(s + SA[i], s + SA[i + 1], d);
    if (rel == -1 || rel == 0 && SA[i + 1]>SA[i])
      return false;
  }
  return true;
}

int main(int argc, char **argv) {
  int d;

  if (argc == 1) { // usage
    fprintf(stderr, "\nuse: program input [output [distance]]\n"); 
    return 1;
  }

  fprintf(stderr, "\nComputing suffix array by SA-DS on "); 
  if (argc > 1) { // input file
    freopen(argv[1], "rb", stdin);
    fprintf(stderr, "%s", argv[1]);
  }
  else // stdin by default
    fprintf(stderr, "stdin");
  fprintf(stderr, " to ");

  if (argc > 2) { // output file
    freopen(argv[2], "wb", stdout);
    fprintf(stderr, "%s", argv[2]);
  }
  else // stdout by default
    fprintf(stderr, "stdout");
  
  if (argc > 3) { // distance
    d = atoi (argv[3]);
    if (d < 2) 
      fprintf(stderr, "\nDistance should be > 1");
    dst = d;
  }
  fprintf(stderr, "\nd = %d", dst); 

  fprintf(stderr, "\n");
#if !defined( unix )
  setmode(fileno(stdin), O_BINARY);
  setmode(fileno(stdout), O_BINARY);
#endif

  // Allocate 5(n+1) bytes memory for input string and output suffix array
  fseek(stdin, 0, SEEK_END);
  int n = ftell(stdin);
  if (n <= 0) {
    fprintf(stderr, "Empty file, nothing to sort, exit!");
    return 0;
  }

  n++; // count for the virtual sentinel
  fprintf(stderr, "Allocating input and output space: %d bytes = %.4lf MB", 5 * n, (double)5 * n / 1024 / 1024); // 5 =  sizeof(char) + sizeof(int)
  unsigned char *s_ch = new unsigned char[n]; trackMemory(n*sizeof(char));
  int *SA = new int[n]; trackMemory(n*sizeof(int));
  if (s_ch == NULL || SA == NULL) {
    delete[] s_ch; delete[] SA;
    fprintf(stderr, "\nInsufficient memory, exit!");
    return 0;
  }

  // read the string into buffer.
  fprintf(stderr, "\nReading input string...");
  fseek(stdin, 0, SEEK_SET);
  fread((unsigned char *)s_ch, 1, n - 1, stdin);

  s_ch[n - 1] = 0; // append the virtual sentinel

  clock_t start, finish;
  double  duration;
  start = clock();

#if 0
  SA_DS(s_ch, SA, n, 255, n, 0);
#else
  mySA_DS(s_ch, SA, n, 255, n, 0);
#endif

  finish = clock();
  duration = (double)(finish - start) / CLOCKS_PER_SEC;

  // final result
  // maxmem = 5 * (n - 1); // original 
  // size of input, total mem consumed, total time
  fprintf(stderr, "\nFile: %s, Size: %d bytes, Memory: %.4lf kB / %.4lf MB, Time: %5.4f seconds\n", argv[1], n - 1, (double)maxmem / 1024, (double)maxmem / 1024 / 1024, duration);


#ifdef _verify_sa
  fprintf(stderr, "\nVerifying the suffix array...");
  fprintf(stderr, "\nSorted: %d", (int)isSorted((unsigned int *)SA, (unsigned char *)s_ch, (unsigned int)n));
#endif

#if 1
  fprintf(stderr, "\nOutputing the suffix array...");
  for (int i = 1; i < n; i++) {
    fwrite((unsigned char *)SA + i, 1, sizeof(int), stdout);
  }
#endif

  delete[] SA;
  delete[] s_ch;
  return 0;
}
