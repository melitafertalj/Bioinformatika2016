package bioinf.projekt;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SADSalg {

	/**
	 *
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		int K = 255, n = 0;
		String genom = null;
		int[] sa = null;
		char[] s = null;

		BufferedReader br = new BufferedReader(new FileReader("string.txt"));
		try {
			StringBuilder sb = new StringBuilder();
			String line = br.readLine();

			while (line != null) {
				if (line.startsWith(">")) {
					continue;
				}
				sb.append(line);
				sb.append(System.lineSeparator());
				line = br.readLine();
			}
			genom = sb.toString();
			genom = genom.substring(0, genom.length() - 2);
			n = genom.length();
			sa = new int[n];
			s = genom.toCharArray();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			br.close();
		}

		sa = sads(s, sa, n, K);

		// TODO write to file

	}

	/**
	 *
	 * @param k
	 * @param n
	 * @param s1
	 * @param sa1
	 * @return
	 */
	public static int[] sads(char[] s, int[] sa, int n, int k) {

		System.out.println("Beginning");
		System.out.println("-------------------------");
		System.out.println("S:" + Arrays.toString(s));
		System.out.println("SA: " + Arrays.toString(sa));
		System.out.println("n: " + n);
		System.out.println("K: " + k);

		int[] b = new int[5];


		boolean[] t = scanSLtype(s);
		System.out.println("t: " + Arrays.toString(t));

		// int[] sa1 = sa;
		int[] sa1 = findDCriticalSubstrings(t); // d=2
		int n1 = sa1.length;

		System.out.println("SA1: " + Arrays.toString(sa1));

		int[] s1 = new int[n - n1 - 1];

		int[] bucket = new int[k + 1];

		s1 = bucketSortLS(sa1, s1, s, t, n, n1, 3);
		System.out.println("---------------------------------");
		System.out.println("LS pass: " + Arrays.toString(s1));

		sa1 = bucketSort(s1, sa1, s, t, n, n1, k, bucket, 3);
		System.out.println("Pass 1: " + Arrays.toString(sa1));
		System.out.println("Bucket sort pass 1 done");

		s1 = bucketSort(sa1, s1, s, t, n, n1, k, bucket, 2);
		System.out.println("Pass 2: " + Arrays.toString(s1));
		System.out.println("Bucket sort pass 2 done");

		sa1 = bucketSort(s1, sa1, s, t, n, n1, k, bucket, 1);
		System.out.println("Pass 3: " + Arrays.toString(sa1));
		System.out.println("Bucket sort pass 3 done");

		s1 = bucketSort(sa1, s1, s, t, n, n1, k, bucket, 0);
		System.out.println("Pass 4: " + Arrays.toString(s1));
		System.out.println("Bucket sort pass 4 done");
		System.out.println("----------------------------");

		// fill SA array with last two sorted d-crit arrays
		System.arraycopy(sa1, 0, sa, 0, sa1.length);
		System.arraycopy(s1, 0, sa, (sa.length - s1.length), s1.length);

		// s1 = nameDCriticals(b);
		// for(int c: s1.toCharArray()) {
		// if(isNumberofBuckets(c)) {
		// computeSA1();
		// }
		// else {
		// sads(s1, sa1);
		// }
		// }
		// sa = induce(sa1);
		return sa;

	}

	private static int[] bucketSortLS(int[] src, int[] dest, char[] s, boolean[] t, int n, int n1, int h) {
		int start = 0;
		int end = n1 - 1;
		for (int i = 0; i < n1; i++) {
			int j = src[i] + h;
			if (j > n - 1) {
				j = n - 1;
			}
			if (t[j]) {
				dest[end] = src[i];
				end--;
			} else {
				dest[start] = src[i];
				start++;
			}
		}
		return dest;
	}

	private static int[] bucketSort(int[] src, int[] dest, char[] s, boolean[] t, int n, int n1, int k, int[] c, int d) {

		int sum = 0;

		for (int i = 0; i < k + 1; i++) {
			c[i] = 0;
		}

		for (int i = 0; i < n1; i++) {
			int j = src[i] + d;
			if (j > n - 1) {
				j = n - 1;
			}
			c[s[j]]++;
		}
		for (int i = 0; i < (k + 1); i++) {
			int length = c[i];
			c[i] = sum;
			sum = sum + length;
		}
		for (int i = 0; i < n1; i++) {
			int j = src[i] + d;
			if (j > n - 1) {
				j = n - 1;
			}
			dest[c[s[j]]] = src[i];
			c[s[j]]++;
		}

		return dest;
	}

	private static boolean isNumberofBuckets(int c) {
		// TODO Auto-generated method stub
		return false;
	}

	private static String nameDCriticals(int[] b) {
		// TODO Auto-generated method stub
		return null;
	}

	private static int[] sortDCriticals(int[] p1, int[] b) {
		int[] temp = p1;
		for(int i = 0; i < 4; i++) {


		}
		return null;
	}

	/**
	 * d = 2
	 *
	 * @param t
	 * @param j
	 * @return
	 */
	private static int[] findDCriticalSubstrings(boolean[] t) {
		// int i = -1, j = 0;
		//
		// List<Integer> p1List = new ArrayList<Integer>();
		//
		// while(i < n-1) {
		// boolean isLMS = false;
		// for (int k = 2; k <= d+1; k++) {
		// if(t[i + k -1] && t[i + k]){
		// isLMS = true;
		// break;
		// }
		// }
		// if(j == 0 && isLMS) {
		// i = i + d;
		// continue;
		// }
		// isLMS ? i = i + k : i + d;
		//
		//
		// [j] = i;
		// j = j + 1;
		// }

		boolean[] dCritical = new boolean[t.length];
		List<Integer> dCrit = new ArrayList<Integer>();
		if (t[0] && !t[1]) {
			dCritical[1] = true;
			dCrit.add(1);
		}
		for (int i = 2; i < t.length - 1; i++) {
			// if t[i] = LMS character OR
			if (!t[i - 1] && t[i] || dCritical[i - 2] && !(!t[i] && t[i + 1]) && !dCritical[i - 1]) {
				dCritical[i] = true;
				dCrit.add(i);
			}
		}
		dCrit.add(t.length - 1);

		int[] sa1 = new int[dCrit.size()];
		for (int i = 0; i < sa1.length; i++) {
			sa1[i] = dCrit.get(i).intValue();
		}
		return sa1;
	}

	// private static boolean[] findLMS(boolean[] t) {
	//
	// boolean[] LMS = new boolean[t.length];
	// for(int i = 1; i < t.length; i++) {
	// if(t[i-1] && !t[i]) { //if (i-1 = L) && (i = S)
	// LMS[i] = true;
	// }
	// }
	//
	// return LMS;
	// }

	private static int[] induce(int[] sa1) {
		// TODO Auto-generated method stub
		return null;
	}

	private static void computeSA1() {
		// TODO Auto-generated method stub

	}

	/**
	 * Checks each character in String if it's L-type or S-type. L-type if the
	 * character is larger than the charatcer right of it or S-type otherwise.
	 * If it's the same, then just copy the type from right side neighbour.
	 *
	 * @param s
	 *            String to be checked
	 * @return array of L-type and S-type indicators
	 */
	private static boolean[] scanSLtype(char[] genomArray) {
		// TODO možda zamijeniti boolean vrijednosti
		boolean[] t = new boolean[genomArray.length];
		t[genomArray.length - 1] = true; // $ is always S-type

		for (int i = genomArray.length - 2; i >= 0; i--) {
			if (genomArray[i] < genomArray[i + 1]) {
				t[i] = true; // S-type
			} else if (genomArray[i] == genomArray[i + 1]) {
				t[i] = t[i + 1];
			} else {
				t[i] = false; // L-type
			}
		}
		// printBooleanArray(t);

		return t;
	}

	private static void printBooleanArray(boolean[] t) {
		for (int i = 0; i < t.length; i++) {
			if (t[i]) {
				System.out.print("1");
			} else {
				System.out.print("0");
			}
		}
		System.out.println();

	}

}
