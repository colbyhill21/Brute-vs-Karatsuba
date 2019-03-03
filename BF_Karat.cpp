/*
 * Project X
 * Colby Hill
 */

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace std;

void makeEqualLength(string &str1, string &str2) //pads the beginning of the longer string with zeroes.
{
    int len1 = str1.size();
    int len2 = str2.size();
    if (len1 < len2)
    {
        for (int i = 0 ; i < len2 - len1 ; i++)
            str1 = '0' + str1;
    }
    else if (len1 > len2)
    {
        for (int i = 0 ; i < len1 - len2 ; i++)
            str2 = '0' + str2;
    }
}
void makeVectorLengthEqual(vector<int> &v1, vector<int> &v2)  //pads the beginning of the longer string with zeroes.
{
  int len1 = v1.size();
  int len2 = v2.size();
  if (len1 < len2)
  {
      std::reverse(v1.begin(),v1.end());
      for (int i = 0 ; i < (len2 - len1) ; i++)
          v1.push_back(0);
      std::reverse(v1.begin(),v1.end());
  }
  else if (len1 > len2)
  {
      std::reverse(v2.begin(),v2.end());
      for (int i = 0 ; i < len1 - len2 ; i++)
          v2.push_back(0);
      std::reverse(v2.begin(),v2.end());
  }
}
vector<int> removeVectorLeadingZeroes(vector<int> &v) //removes leading zeroes from a vector, makes ouput "look nicer" - it's more readable.
{
  bool hitNum = false;
  vector<int> v2;
  for(int i = 0; i< v.size(); i++)
  {
      if(!hitNum)
      {
          if(v[i] != 0)
          {
              v2.push_back(v[i]);
              hitNum = true;
          }
      }
      else
      {
          v2.push_back(v[i]);
      }
  }
  if(!hitNum) // if all of the vector was zeroes return a vector with 0
  {
      vector<int> retv;
      retv.push_back(0);
      return retv;
  }
  return v2;
}
vector<int> bruteForceMult(vector<int> v1, vector<int> v2)
{
    makeVectorLengthEqual(v1,v2);
    int d = v1.size();
    vector<int> ret;
    vector<int> ans;
    int carry = 0;

    for(int i = 0; i < 2*d; i++)
    {
        ret.push_back(0); //initialize length *2 to zeroes.
    }
    for(int i = 0; i < d; i++) //while i is less than size
    {
        for(int j = 0; j < d; j++) //while j is less than size
        {
            ret[i+j] += v1[i] * v2[j]; //this does allow positions of a vector to exceed a single digit( > 9) but I deal with this later.
        }
    }
    for(int i = (ret.size()-1); i >= 0; i-- ) //this is the portion of the function that deals with the positions that exceeded a single digit.
    {
        int temp = (ret[i]+carry);
        if(temp > 9)
        {
            ans.push_back(temp%10); //set to remainder and let carry keep it going.
            carry = temp/10; //intger division.
        }
        else //if the value at this position + carry is still a single digit we can just add this to the list.
        {
            ans.push_back(temp);
            carry = 0;
        }
    }
    ans.push_back(carry); //adding carry to the end of the list, e.g. carry[list]. if carry is 0 it doesn't matter because it will be removed by the removeVectorLeadingZeroes function
    std::reverse(ans.begin(),ans.end()); //reverses the list to be in normal direction
    ans = removeVectorLeadingZeroes(ans); //removes leading zeroes to make the output more readable
    if(ans.size() >= 2) //takes care of a problem I had relating waiting to deal with the multiplication until later causing a problem.
        ans.pop_back();
    return ans;
}
vector<int> vectorAdd(vector<int> v1, vector<int> v2) //adds the values of two vectors together and returns the result
{
    makeVectorLengthEqual(v1,v2);
    vector<int> ret;
    int carry = 0;
    int last = 0;
    for(int i = 0; i < (2*v1.size()); i++)
    {
        ret.push_back(0); //initialize length *2 to zeroes.
    }
    for(int i = (v1.size()-1); i >= 0; i-- )
    {
        int temp = (v1[i]+v2[i]+carry);
        if(temp > 9)
        {
            last = ((ret.size()-1) - i - (v1.size()));
            ret[last] = ((temp)%10); //set to remainder and let carry keep it going.
            carry = temp/10; //intger division.
        }
        else
        {
            //cout << "adding: " << ret[i] << "\n";
            last = ((ret.size()-1) - i - (v1.size()));
            ret[last] = temp;
            carry = 0;
        }
    }
    if(carry != 0)
    {
        //cout << "adding " << carry << "to position: " << last << "\n";
        ret[last+1] = carry;
    }
    std::reverse(ret.begin(),ret.end());
    ret = removeVectorLeadingZeroes(ret);
    return ret;
}
vector<int> vectorSubtract(vector<int> v1, vector<int> v2) //subtracts the values of the second vector from the first and returns the result. e.g. result = v1 - v2
{
    makeVectorLengthEqual(v1,v2);
    vector<int> ret;
    for(int i = 0; i < (v1.size()); i++)
    {
        ret.push_back(0); //initialize length *2 to zeroes.
    }
    for(int i = (v1.size()-1); i >= 0; i-- )
    {
        int temp = (v1[i] - v2[i]);
        if( temp >= 0)
        {
            ret[i] = temp;
        }
        else
        {
            v1[i-1] = (v1[i-1] -1);
            v1[i] = (v1[i] + 10);
            ret[i] = (v1[i] - v2[i]);
        }
    }
    ret = removeVectorLeadingZeroes(ret);
    return ret;
}
vector<int> karatsubaMult(vector<int> a, vector<int> b)
{
    makeVectorLengthEqual(a,b);
    if(a.size() < 2 && b.size() < 2)
    {
        vector<int> ret = bruteForceMult(a,b);
        return ret;
    }
    else
    {
        int m = a.size()/2;
        int runover = a.size()%2;
        /* this is something I had to add in to ensure that if an odd number of digits occurred that a0 and b0 recieved m number of digits.
         *I'm sure that there is a better way to do this although I couldn't think of any... It works though...
         */
        vector<int> a0;
        for(int i = m+runover; i < a.size(); i++)
        {
            a0.push_back(a[i]);
        }
        vector<int> a1;
        for(int i = 0; i < m+runover; i++)
        {
            a1.push_back(a[i]);
        }
        vector<int> b0;
        for(int i = m+runover; i < b.size(); i++)
        {
            b0.push_back(b[i]);
        }
        vector<int> b1;
        for(int i = 0; i < m+runover; i++)
        {
            b1.push_back(b[i]);
        }
        vector<int> c2 = karatsubaMult(a1,b1);
        vector<int> c0 = karatsubaMult(a0,b0);
        vector<int> c1 = karatsubaMult(vectorAdd(a0,a1),vectorAdd(b0,b1));
        c1 = vectorSubtract(c1,c2);
        c1 = vectorSubtract(c1,c0); //broke this up into multiple steps so that it was a bit easier to read
        for(int i = 0; i < 2*m; i++) //10^(2m)*c2
        {
            c2.push_back(0);
        }
        for(int i = 0; i < m; i++) //10^(m)*c1
        {
            c1.push_back(0);
        }
        return (vectorAdd(c2,vectorAdd(c1,c0)));
    }
}
int main(int argc, char** argv)
{
    vector<int> v1, v2;
    string intA = "", intB = "";
	  cin >> intA;
    cin >> intB;
    makeEqualLength(intA,intB); //ensures both A and B are equal length by padding the longer with zeroes
    for(int i = 0; i < intA.length(); i++) // turns input1 into an int vector
    {
        char a = intA[i];
        int ia = a - '0';
        v1.push_back(ia);
    }
    for(int i = 0; i < intA.length(); i++) // turns input2 into an int vector
    {
        char b = intB[i];
        int ib = b - '0';
        v2.push_back(ib);
    }
    cout << "Brute Force: ";
    vector<int> bfres = bruteForceMult(v1,v2);
    for(int i = 0; i < bfres.size(); i++) //prints brute force result
    {
        cout << bfres[i];
    }
    cout << "\n";
    cout << "Karatsuba:   ";
    vector<int> kres = karatsubaMult(v1,v2); //prints Karatsuba result
    for(int i = 0; i < kres.size(); i++)
    {
        cout << kres[i];
    }
    cout << "\n";
    return 0;
}
