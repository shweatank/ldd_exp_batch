/* Problem Statement - Binary Number with Alternating Bits
----------------------------------------------------------------------------------------------------------
Given a positive integer, check whether it has alternating bits: namely, if two adjacent bits will always have different values.

Example 1:
Input: n = 5
Output: true
Explanation: The binary representation of 5 is: 101

Example 2:
Input: n = 7
Output: false
Explanation: The binary representation of 7 is: 111.

----------------------------------------------------------------------------------------------------------
*/

//Method 1 
bool hasAlternatingBits(int n) {
    int start = n & 1;
    while(n) {
        int bit1 = n & 1;
        int bit2 = (n>>1) & 1;
        if(start) {
            if(bit1==0 || bit2==1) return false;
        }
        else {
            if(bit1==1 || bit2==0) return false;
        }
        n >>=2;
    }
    return true;
}

//Method 2
bool isPowerOfFour(int n) {
    return (n>0 && !(n & (n-1)) && (n & 0x55555555));
}
