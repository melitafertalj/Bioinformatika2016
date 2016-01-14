package bioinf.projekt;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SADSalg {

	/**
	 * main program. opens file, runs the algorithm then writes the result array
	 * to new file.
	 *
	 * @param args
	 *            input file, output file
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		long startTime = System.nanoTime();

		int K = 255, n = 0;
		String genom = null;
		int[] sa = null;
		char[] s = null;
		int[] sInt = null;

		BufferedReader br = new BufferedReader(new FileReader(args[0]));
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
			genom = genom.replace("\n", "").replace("\r", "");
			// System.out.println(genom.length());
			n = genom.length() + 1;
			sa = new int[n];
			s = genom.toCharArray();
			// System.out.println(Arrays.toString(s));
			sInt = new int[n];
			for (int i = 0; i < n - 1; i++) {
				sInt[i] = s[i];
			}
			sInt[n - 1] = 0;
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			br.close();
		}

		sa = sads(sInt, sa, n, K);

		String result = Arrays.toString(sa);
		// System.out.println(result);

		PrintWriter writer = new PrintWriter(args[1], "UTF-8");
		writer.println(result);
		writer.close();


		long estimatedTime = System.nanoTime() - startTime;
		System.out.println("Estimated time: " + estimatedTime / 1000000000.0 + "s");
	}

	/**
	 * The main body of the Nong, Zhang, Chan SA-DS algorithm
	 *
	 * @param k
	 *            number of different symbols
	 * @param n
	 *            size of string
	 * @param sInt
	 *            input array
	 * @param sa
	 *            output array
	 * @return output array
	 */
	public static int[] sads(int[] sInt, int[] sa, int n, int k) {

		// System.out.println("Beginning");
		// System.out.println("-------------------------");
		// System.out.println("S:" + Arrays.toString(sInt));

		// System.out.println("SA: " + Arrays.toString(sa));
		// System.out.println("n: " + n);
		// System.out.println("K: " + k);

		boolean[] t = scanSLtype(sInt);
		// System.out.println("t: " + Arrays.toString(t));

		sa = new int[sa.length];
		int[] sa1 = sa;
		sa1 = findDCriticalSubstrings(t); // d=2
		int n1 = sa1.length;

		// System.out.println("SA1: " + Arrays.toString(sa1));

		int[] s1 = new int[n1];

		int[] bucket = new int[k + 1];

		s1 = bucketSortLS(sa1, s1, sInt, t, n, n1, 3);
		// System.out.println("---------------------------------");
		// System.out.println("LS pass: " + Arrays.toString(s1));

		sa1 = bucketSort(s1, sa1, sInt, t, n, n1, k, bucket, 3);
		// System.out.println("Pass 1: " + Arrays.toString(sa1));

		s1 = bucketSort(sa1, s1, sInt, t, n, n1, k, bucket, 2);
		// System.out.println("Pass 2: " + Arrays.toString(s1));

		sa1 = bucketSort(s1, sa1, sInt, t, n, n1, k, bucket, 1);
		// System.out.println("Pass 3: " + Arrays.toString(sa1));

		s1 = bucketSort(sa1, s1, sInt, t, n, n1, k, bucket, 0);
		// System.out.println("Pass 4: " + Arrays.toString(s1));
		// System.out.println("----------------------------");

		// name d-critical array
		for (int i = n1 - 1; i >= 0; i--) {
			int j = 2 * i;
			sa[j] = s1[i];
			sa[j + 1] = -1;
		}
		for (int i = 2 * (n1 - 1) + 3; i < n; i += 2) {
			sa[i] = -1;
		}

		// System.out.println("sa: " + Arrays.toString(sa));
		int name = 0;
		int[] c = { -1, -1, -1, -1 }; // d = 2 -> s-crit substring length is 4

		for (int i = 0; i < n1; i++) {
			int pos = sa[2 * i];
			boolean diff = false;
			// compare substring to the substring in c array
			for (int h = 0; h < 3; h++) {
				if ((pos + h) < n) {
					if (sInt[pos + h] != c[h]) {
						diff = true;
						break;
					}
				} else {
					if (c[h] != -1) {
						diff = true;
						break;
					}
				}
			}
			// compare ...
			if ((pos + 3) < n) {
				int ls = 0;
				if (t[pos + 3]) {
					ls = 1;
				}
				if ((sInt[pos + 3] * 2 + ls) != c[3]) { // TODO
					diff = true;
				} else {
					if (c[3] == -1) {
						diff = true;
					}
				}
			}
			if (diff) {
				name++;
				for (int h = 0; h < 4; h++) {
					if (pos + h < n) {
						c[h] = sInt[pos + h];
					} else {
						c[h] = -1;
					}
				}
				if ((pos + 3) < n) { //if not $$$$ or xx$$
					int ls = 0;
					if (t[pos + 3]) {
						ls = 1;
					}
					c[3] = sInt[pos + 3] * 2 + ls;
				} else {
					c[3] = -1;
				}
			}
			if (pos % 2 == 0) {
				pos--;
			}
			sa[pos] = name - 1; // hax
		}


		// System.out.println("sa: " + Arrays.toString(sa));

		// copy names in new array (s1)
		int i = (n / 2) * 2 - 1;
		int j = n - 1;
		while (i >= 0 && j >= 0) {
			if (sa[i] != -1) {
				sa[j] = sa[i];
				j -= 1;
			}
			i -= 2;
		}
		s1 = Arrays.copyOfRange(sa, n - n1, n);
		// System.out.println("s1: " + Arrays.toString(s1));

		if (name < n1) {
			// System.out.println("Recursion:");
			sa1 = sads(s1, sa1, n1, name - 1);
			// System.out.println("Recursion end!");
		} else {
			for (i = 0; i < n1; i++) {
				sa1[s1[i]] = i;
			}
		}
		System.arraycopy(s1, 0, sa, 0, s1.length);
		System.arraycopy(sa1, 0, sa, sa.length - n1, sa1.length);

		// System.out.println("sa1: " + Arrays.toString(sa1));
		// System.out.println("Inducing SA:");

		s1 = findDCriticalSubstrings(t);
		System.arraycopy(s1, 0, sa, sa.length - n1, s1.length);
		int[] bkt = new int[k + 1];
		bkt = getBuckets(sInt, bkt, n, k, 1);

		for (i = 0; i < n1; i++) {
			if (sa1[i] >= 0) {
				sa1[i] = s1[sa1[i]];
			}
		}
		for (i = n1; i < n; i++) {
			sa[i] = -1;
		}
		System.arraycopy(sa1, 0, sa, 0, n1);
		for (i = n1 - 1; i > -1; i--) {
			j = sa[i];
			sa[i] = -1;
			if (j > 0 && t[j] && !t[j - 1]) {
				bkt[sInt[j]]--;
				sa[bkt[sInt[j]]] = j;
			}
		}
		// System.out.println("Step 1: " + Arrays.toString(sa));

		getBuckets(sInt, bkt, n, k, 0);
		for (i = 0; i < n; i++) {
			j = sa[i] - 1;
			if (j >= 0 && !t[j]) {
				sa[bkt[sInt[j]]] = j;
				bkt[sInt[j]]++;
			}
		}
		// System.out.println("Step 2: " + Arrays.toString(sa));

		getBuckets(sInt, bkt, n, k, 1);
		for (i = n - 1; i > -1; i--) {
			j = sa[i] - 1;
			if (j >= 0 && t[j]) {
				bkt[sInt[j]]--;
				sa[bkt[sInt[j]]] = j;
			}
		}
		// System.out.println("Step 3: " + Arrays.toString(sa));

		return sa;

	}

	/**
	 * Uses bucket sort to sort elements by first symbol.
	 *
	 * @param src
	 *            source array
	 * @param dest
	 *            destination array
	 * @param s
	 *            array of symbols
	 * @param t
	 *            array of LS types (1 for S, 0 for L)
	 * @param n
	 *            length of string
	 * @param n1
	 *            length of source array
	 * @param h
	 *            (d + 2) - 1
	 * @return destination array
	 */
	private static int[] bucketSortLS(int[] src, int[] dest, int[] s, boolean[] t, int n, int n1, int h) {
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

	/**
	 * Uses bucket sort to sort elements by the rest of the symbols
	 *
	 * @param src
	 *            source array
	 * @param dest
	 *            destination array
	 * @param s
	 *            array of symbols
	 * @param t
	 *            array of LS types (1 for S, 0 for L)
	 * @param n
	 *            length of string
	 * @param n1
	 *            length of source array
	 * @param h
	 *            (d + 2) - 1
	 * @return destination array
	 */
	private static int[] bucketSort(int[] src, int[] dest, int[] s, boolean[] t, int n, int n1, int k, int[] c, int d) {

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

	/**
	 * Returns bucket for induction.
	 *
	 * @param sInt
	 *            source array
	 * @param bkt
	 *            bucket array
	 * @param n
	 *            length of source
	 * @param k
	 *            number of symbols
	 * @param end
	 *            Step counter
	 * @return bucket array
	 */
	private static int[] getBuckets(int[] sInt, int[] bkt, int n, int k, int end) {
		int sum = 0;
		for (int i = 0; i < k + 1; i++) {
			bkt[i] = 0;
		}
		for (int i = 0; i < n; i++) {
			bkt[sInt[i]] += 1;
		}
		for (int i = 0; i < k + 1; i++) {
			sum += bkt[i];
			if (end == 1) {
				bkt[i] = sum;
			} else {
				bkt[i] = sum - bkt[i];
			}
		}
		return bkt;
	}

	/**
	 * Locates d-critical substrings and puts index of first symbol into sa1
	 * array
	 *
	 * @param t
	 *            LS type array
	 * @return sa1 array of d-critical substrings
	 */
	private static int[] findDCriticalSubstrings(boolean[] t) {

		boolean[] dCritical = new boolean[t.length];
		List<Integer> dCrit = new ArrayList<Integer>();
		if (!t[0] && t[1]) {
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


	/**
	 * Checks each character in String if it's L-type or S-type. L-type if the
	 * character is larger than the charatcer right of it or S-type otherwise.
	 * If it's the same, then just copy the type from right side neighbour.
	 *
	 * @param s
	 *            String to be checked
	 * @return array of L-type and S-type indicators
	 */
	private static boolean[] scanSLtype(int[] genomArray) {

		int n = genomArray.length;
		boolean[] t = new boolean[n];
		t[n - 1] = true;
		for (int i = n - 2; i > -1; i--) {
			if (genomArray[i] == 0) {
				t[i] = true;
			} else if (genomArray[i] < genomArray[i + 1]) {
				t[i] = true;
			} else if (genomArray[i] == genomArray[i + 1]) {
				t[i] = t[i + 1];
			}
		}
		return t;

	}

}
