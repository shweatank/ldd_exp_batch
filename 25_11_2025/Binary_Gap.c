/* Problem Statement - Binary gap
----------------------------------------------------------------------------------------------------------
Given a positive integer n, find and return the longest distance between any two adjacent 1's in the binary representation of n. If there are no two adjacent 1's, return 0.

Two 1's are adjacent if there are only 0's separating them (possibly no 0's). The distance between two 1's is the absolute difference between their bit positions. For example, the two 1's in "1001" have a distance of 3.

Example 1:
Input: n = 22
Output: 2
Explanation: 22 in binary is "10110".

Input: n = 8
Output: 0
Explanation: 8 in binary is "1000".
----------------------------------------------------------------------------------------------------------
*/

int binaryGap(int n) {
    int last = -1;
    int count_max = 0;

    for(int i=0; i<32; i++) {
        if((n >> i) & 1) {
            if(last != -1) {
                int count = i - last;
                if(count > count_max) {
                    count_max = count;
                }
            }
            last = i;
        }
    }
    return count_max;
}
