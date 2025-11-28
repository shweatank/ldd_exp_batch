/* Problem Statement - Complement of Base 10 integer
----------------------------------------------------------------------------------------------------------
The complement of an integer is the integer you get when you flip all the 0's to 1's and all the 1's to 0's in its binary representation.

For example, The integer 5 is "101" in binary and its complement is "010" which is the integer 2.
Given an integer n, return its complement.

Example 1:
Input: n = 5
Output: 2
Explanation: 5 is "101" in binary, with complement "010" in binary, which is 2 in base-10.
----------------------------------------------------------------------------------------------------------
*/

//Method 1
int bitwiseComplement(int n) {
    int comp = 0;
    int i = 0;
    if(n==0) return 1;
    while(n && (i<32)) {
        if(!(n&1)) {
            comp |= (1<<i);
        }
        i++;
        n >>= 1;
    }
    return comp;
}

//Method 2
int bitwiseComplement(int n) {
    if(n==0) return 1;
    int mask = 0;
    int temp = n;
    while(temp) {
        mask = (mask << 1) | 1;
        temp >>= 1;
    }
    return mask ^ n;
}
