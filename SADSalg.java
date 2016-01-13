package bioinf.projekt;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class SADSalg {

	/**
	 *
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		String genom = null;
		int[] suffixArray = null;

		BufferedReader br = new BufferedReader(new FileReader("string.txt"));
		try {
			StringBuilder sb = new StringBuilder();
			String line = br.readLine();

			while (line != null) {
				sb.append(line);
				sb.append(System.lineSeparator());
				line = br.readLine();
			}
			genom = sb.toString();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			br.close();
		}

		suffixArray = sads(genom, suffixArray);

	}

	/**
	 *
	 * @param s1
	 * @param sa1
	 * @return
	 */
	public static int[] sads(String s, int[] sa) {

		String s1;
		boolean[] t;
		int[] p1 = null, sa1 = null;
		int[] b = new int[5];

		t = scanSLtype(s);
		p1 = findDCriticalSubstrings(t);
		p1 = sortDCriticals(p1, b);
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
		
		return null;
	}

	/**
	 *
	 * @param t
	 * @return
	 */
	private static int[] findDCriticalSubstrings(boolean[] t) {
		boolean[] dCritical = new boolean[t.length];
		List<Integer> dCrit = new ArrayList<Integer>();
		if (t[0] && !t[1]) {
			dCritical[1] = true;
		}
		for (int i = 2; i < t.length; i++) {
			if (t[i - 1] && !t[i] || dCritical[i - 2] && !(t[i] && !t[i + 1]) && !dCritical[i - 1]) {
				dCritical[i] = true;
				dCrit.add(i);
			}
		}
		// System.out.println(dCrit.toString());

		int[] p1 = new int[dCrit.size()];
		for (int i = 0; i < p1.length; i++) {
			p1[i] = dCrit.get(i).intValue();
		}

		System.out.println(p1.toString());
		return p1;



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
	private static boolean[] scanSLtype(String s) {

		char[] charArray = s.toCharArray();
		boolean[] t = new boolean[charArray.length - 2];
		t[charArray.length - 3] = false; // $ is always S-type

		for (int i = charArray.length - 4; i >= 0; i--) {
			if (charArray[i] < charArray[i + 1]) {
				t[i] = false; // S-type
			} else if (charArray[i] == charArray[i + 1]) {
				t[i] = t[i + 1];
			} else {
				t[i] = true; // L-type
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
