using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SA_DS
{
    class Program
    {
        static int d = 2;
        static void Main(string[] args)
        {
            
            var S = "mmiissiissiippii$";
            S = S.ToUpper();

            List<int> Sint = new List<int>();

            // transform string -> List<int>
            foreach(char c in S)
            {
                Sint.Add(val(c));
            }

            List<int> SA = SADS(Sint);
            Console.WriteLine("");
            
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
            // L/S list where S is true and L is false
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
            // - S[i] is d-critical only if it is a LMS or if  S[i-d] is d-critical and no char in between is critical
            // - S[i] is LMS if S[i] is S type (true) and S[i-1] is L type (false)
            // - first char is not critical, last char is critical
            // - if S[i] is critical, S[i-1] and S[i+1] are not
            // - d>=2, in this implementation d=2

            List<int> P1 = new List<int>();

            for (int i = t.Count-1; i > 0; i--)
            {
                if (t[i] == true||(P1[0]==i+d && t[i-1]!=true))
                // if S[i] is critical or the last added d-critical char was S[i+d] (we compare indices)
                {
                    P1.Insert(0, i);
                }
            }
            return P1;
        }
        private static List<int> CreateS1(List<int> S,List<int> P1, List<bool> t)
        {
            S.Add(36); S.Add(36); S.Add(36);
            t.Add(false); t.Add(false); t.Add(false);

            List<List<int>> valueLists = new List<List<int>>();

            // for each index in P1 we're creating an int list element_in_p1|list_of_elements_in_subarray|placeholder_for_bucket_number
            foreach(int p1element in P1)
            {
                valueLists.Add(new List<int>() {p1element});
                for (int i = 0; i < d + 2; i++)
                {
                    valueLists.Last().Add(2*val(S[p1element+i])+val(t[p1element + i]));
                }
                valueLists.Last().Add(0);
            }

            // this works only for d=2
            var result=valueLists.OrderBy(l => l[1]).ThenBy(l => l[2]).ThenBy(l => l[3]).ThenBy(l => l[4]);
            valueLists=result.ToList();

            for (int i = 0; i < P1.Count; i++)
            {
                P1[i] = valueLists[i][0];
            }

            int bucketNumber = 0;
            valueLists[0][d + 2+1] = bucketNumber++;

            for(int i = 1; i < valueLists.Count; i++)
            {
                if(substringsEqual(d, valueLists[i - 1], valueLists[i]))
                {
                    bucketNumber--;
                }

                valueLists[i][d + 2+1] = bucketNumber;
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
            // initialize SA (of the same length as S) with -1's
            List<int> SA0 = new List<int>();
            for (int i = 0; i < iS.Count; i++)
            {
                SA0.Add(-1);
            }

            // find the end of each bucket in SA for all the suffixes in S

            return new List<int>();
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
                if(list1[1+i]!= list2[1 + i])
                {
                    return false;
                }
            }
            return true;
        }
    }
}
// testing checking into a local branch