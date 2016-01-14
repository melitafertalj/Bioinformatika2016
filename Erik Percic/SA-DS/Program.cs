using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace SA_DS
{
    class Program
    {
        static int d = 2;
        static void Main(string[] args)
        {
            for (int i = 1; i <= 12; i++)
            {
                StringBuilder s = new StringBuilder();
                string line;

                System.IO.StreamReader file =
                    new System.IO.StreamReader(args[0]);
                while ((line = file.ReadLine()) != null)
                {
                    if (line[0]!='>') {
                        s.Append(line);
                    }
                }
                file.Close();

                string S = s.ToString() + "$";

                S = S.ToUpper();

                List<int> Sint = new List<int>();

                // transform string -> List<int>
                foreach (char c in S)
                {
                    Sint.Add(val(c));
                }

                List<int> SA = SADS(Sint);

                // result output saved as output(name_of_input).txt
                using (StreamWriter writer = new StreamWriter("output("+args[0]+").txt"))
                {
                    foreach (int j in SA)
                    {
                        writer.WriteLine(j);
                    }
                }
                return;
            }
            
        }
        private static List<int> SADS(List<int> S)
        {
            // 1. Scan S once to classify all the characters as L - or S - type into t;
            List<bool> t = CreateLSList(S);

            // 2. Scan t once to find all the d-critical substrings in S into P1
            List<int> P1 = CreateP1(t, d);

            // 3. Bucket sort all the d-critical substrings using P1
            // 4. Name each d-critical substring in S by its bucket index to get a new shortened string S1;
            List<int> S1 = CreateS1(S, P1, t);

            List<int> SA1 = new List<int>();
            // if number of buckets == number of elements
            if (S1.Count != (S1.Max() + 1))
            {
                // fire a recursive call
                SA1 = SADS(S1);
            }
            else
            {
                // directly compute SA1 from S1
                SA1 = S1;
            }

            // induce SA from SA1<--OSTALO JE SAMO OVO ZA NAPRAVITI
            List<int> SA = InduceSA0(S,SA1,P1,t);

            return SA;
        }
        private static List<bool> CreateLSList(List<int> S)
        {
            // creates a L/S list where S is true and L is false
            List<bool> t = new List<bool>();

            for (int i = S.Count - 1; i >= 0; i--)
            {
                if (i == S.Count - 1)
                {
                    t.Insert(0, true); //insert S for last charachter
                }
                else
                {
                    if (S[i] > S[i + 1])
                    {
                        t.Insert(0, false); //insert L for S[i] > S[i + 1]
                    }
                    else if (S[i] < S[i + 1])
                    {
                        t.Insert(0, true); //insert S for S[i] < S[i + 1]
                    }
                    else
                    {
                        t.Insert(0, t[0]); //copy value if S[i] == S[i + 1]
                    }
                }
            }

            return t;
        }
        private static List<int> CreateP1(List<bool> t, int d)
        {
            // creates P1 array of pointers to d-critical chars
            // RULES:
            // - S[i] is d-critical only if it is a LMS [S[i] is S type (true) and S[i-1] is L type (false)] 
            // or if  S[i-d] is d-critical and no char in between is critical
            // - first char is not critical, last char is critical
            // - if S[i] is critical, S[i-1] and S[i+1] are not
            // - d>=2, in this implementation d=2

            List<int> P1 = new List<int>();

            for (int i = 1; i < t.Count; i++)
            {
                if (t[i] == true && t[i-1]==false)
                // if S[i] is d-critical
                {
                    P1.Add(i);
                }else if (P1.Count != 0)
                {
                    // S[i-d] is a d-critical char and S[i+1] is not LMS
                    if (P1.Last() == i - d && !(t[i]==false && t[i+1]==true))
                    {
                        P1.Add(i);
                    }
                }
            }
            return P1;
        }
        private static List<int> CreateS1(List<int> S,List<int> P1, List<bool> t)
        {
            S.Add(36); S.Add(36); S.Add(36);
            t.Add(false); t.Add(false); t.Add(false);

            List<List<int>> valueLists = new List<List<int>>();

            // for each index in P1 we're creating an int list {element_in_p1|list_of_elements_in_subarray|placeholder_for_bucket_number}
            foreach(int p1element in P1)
            {
                valueLists.Add(new List<int>() {p1element});
                for (int i = 0; i < d + 2; i++)
                {
                    // 2*value(element in S) + value(element in t)
                    valueLists.Last().Add(2*val(S[p1element+i])+val(t[p1element + i]));
                }
                valueLists.Last().Add(0);
            }

            // bucket sorting by each element of thesubstring (d+2=4)
            var result=valueLists.OrderBy(l => l[1]).ThenBy(l => l[2]).ThenBy(l => l[3]).ThenBy(l => l[4]);
            valueLists=result.ToList();

            for (int i = 0; i < P1.Count; i++)
            {
                P1[i] = valueLists[i][0];
            }

            int bucketNumber = 0;
            valueLists[0][d+2+1] = bucketNumber++;

            for(int i = 1; i < valueLists.Count; i++)
            {
                if(substringsEqual(d, valueLists[i - 1], valueLists[i]))
                {
                    bucketNumber--;
                }

                valueLists[i][d+2+1] = bucketNumber;
                bucketNumber++;
            }

            var result2 = valueLists.OrderBy(e => e[0]);
            valueLists = result2.ToList();

            List<int> S1 = new List<int>();
            foreach(List<int> l in valueLists)
            {
                S1.Add(l[d+2+1]);
            }

            return S1;
        }
        private static List<int> InduceSA0(List<int> iS,List<int> SA1,List<int> iP1, List<bool> it)
        {
            iP1.Sort();

            // transform SA1 into a list of buckets
            int[] sortedArrayOfUniqueElements = makeArrayOfUniqueElements(iS);
            for(int i = 0; i < iS.Count; i++)
            {
                // replace the item with a bucket number
                for(int j = 0; j < sortedArrayOfUniqueElements.Count(); j++)
                {
                    if (sortedArrayOfUniqueElements[j] == iS[i])
                    {
                        iS[i] = j;
                        break;
                    }
                }
            }

            // remove the three helping $ at the end of S
            iS.RemoveRange(iS.Count-3,3);

            // calculate sizes of buckets
            int[] bucketSizes = new int[differentElements(iS)];
            Array.Clear(bucketSizes,0,bucketSizes.Length);

            foreach(int x in iS)
            {
                bucketSizes[x]++;
            }

            List<int> headPositions;

            // Setup SA as a list of buckets
            List<List<int>> SA = new List<List<int>>();

            // Alg. for Inducing SA from SA1 in SA-DS
            // Initialize each item of SA as -1
            foreach (int x in bucketSizes)
            {
                List<int> newBucket = new List<int>();
                for(int i = 0; i < x; i++)
                {
                    newBucket.Add(-1);
                }
                SA.Add(newBucket);
            }
            // 1st step
            headPositions = setHead(SA, "end");
            for(int i = SA1.Count - 1; i >= 0; i--)
            {
                int SA1Element = SA1[i];
                int p1Element = iP1[SA1Element];

                // if suf(S; P1[SA1[i]]) is a LMS suffix
                if(p1Element>0 && it[p1Element] == true && it[p1Element-1] == false)
                {
                    //put P1[SA1[i]] to the current end of the bucket for suf(S; P1[SA1[i]]) in SA
                    SA[iS[p1Element]][headPositions[iS[p1Element]]]= p1Element;
                    headPositions[iS[p1Element]]--;
                }
            }

            // 2nd step
            headPositions = setHead(SA, "start");
            for(int i = 0; i < SA.Count; i++)
            {
                for (int j = 0; j < SA[i].Count; j++)
                {
                    int saElement = SA[i][j];
                    // if S[SA[i]−1] is L-type
                    if (saElement > 0 && it[saElement-1]==false)
                    {
                        SA[iS[saElement - 1]][headPositions[iS[saElement - 1]]] = saElement - 1;
                        headPositions[iS[saElement - 1]]++;
                    }
                }
            }

            // 3rd step
            headPositions = setHead(SA, "end");
            for (int i=SA.Count-1;i>=0;i--)
            {
                for(int j = SA[i].Count - 1; j >= 0; j--)
                {
                    int saElement = SA[i][j];
                    // if S[SA[i]−1] is S-type
                    if (saElement > 0 && it[saElement - 1] == true)
                    {
                        SA[iS[saElement - 1]][headPositions[iS[saElement - 1]]] = saElement - 1;
                        headPositions[iS[saElement - 1]]--;
                    }
                }
            }

            // prepare SA for returning (from a list of buckets to a list of elements)
            List<int> retSA=new List<int>();
            foreach(List<int> l in SA)
            {
                foreach (int i in l)
                {
                    retSA.Add(i);
                }
            } 

            return retSA;
        }

        private static int[] makeArrayOfUniqueElements(List<int> iS)
        {
            // creates a sorted Array of unique elements found in iS
            List<int> seenElements = new List<int>();
            foreach(int i in iS)
            {
                if (!seenElements.Contains(i))
                {
                    seenElements.Add(i);
                }
            }
            seenElements.Sort();
            return seenElements.ToArray();
        }

        private static int differentElements(List<int> iS)
        {
            // calculates the number of different elements in a list
            int counter = 0;
            List<int> seenElements = new List<int>();

            foreach(int x in iS)
            {
                if (!seenElements.Contains(x))
                {
                    counter++;
                    seenElements.Add(x);
                }
            }
            return counter;
        }
        
        private static List<int> setHead(List<List<int>>SA, string where)
        {
            // for a input list of lists and instruction where
            // returns a list of header locations

            List<int> answer = new List<int>();

            if (where == "start")
            {
                foreach(List<int> l in SA)
                {
                    answer.Add(0);
                }
            }
            else
            {
                foreach (List<int> l in SA)
                {
                    answer.Add(l.Count-1);
                }
            }
            return answer;
        }

        private static int val(char c)
        {
            int i = c;
            return i;
        }
        private static int val (int i)
        {
            return i;
        }
        private static int val(bool b)
        {
            if (b)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        private static bool substringsEqual(int d,List<int> list1, List<int> list2)
            // returns true if the substrings of S (d+2 elements) are equal
        {
            for(int i = 0; i < d + 2; i++)
            {
                if(list1[1+i]!= list2[1+i])
                {
                    return false;
                }
            }
            return true;
        }
    }
}
