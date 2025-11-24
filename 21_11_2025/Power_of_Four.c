/* Problem Statement - Power of Four
----------------------------------------------------------------------------------------------------------
Given an integer n, return true if it is a power of four. Otherwise, return false.
An integer n is a power of four, if there exists an integer x such that n == 4x.

Example 1:
Input: n = 16
Output: true

Example 2:
Input: n = 5
Output: false
----------------------------------------------------------------------------------------------------------
*/

//Method 1 
bool isPowerOfFour(int n) {
    if(n<=0) return false;
    if((n & (n-1)) == 0) {
        if (n & 0x55555555)
            return true;
    }
    return false;
}

//Method 2
bool isPowerOfFour(int n) {
    return (n>0 && !(n & (n-1)) && (n & 0x55555555));
}
