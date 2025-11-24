/* Problem Statement - Counting Bits
----------------------------------------------------------------------------------------------------------
Given an integer n, return an array ans of length n + 1 such that for each i (0 <= i <= n), ans[i] is the number of 1's in the binary representation of i.

Example 1:
Input: n = 2
Output: [0,1,1]

Example 2:
Input: n = 5
Output: [0,1,1,2,1,2]
----------------------------------------------------------------------------------------------------------
*/

//Method 1 O(nlogn)
int* countBits(int n, int* returnSize) {
    int *arr = malloc((n+1) * sizeof(int));
    *returnSize = n+1;
    for(int i=0; i<=n ;i++) {
        int count = 0;
        int num = i;
        while(num) {
            count++;
            num = num & (num-1);
        }
        arr[i] = count;
    }
    return arr;
}

//Method 2 O(n)
int* countBits(int n, int* returnSize) {
    int *arr = malloc((n+1) * sizeof(int));
    *returnSize = n+1;
    arr[0] = 0;
    for(int i=1; i<=n ;i++) {
        arr[i] = arr[i>>1] + (i&1);
    }
    return arr;
}
