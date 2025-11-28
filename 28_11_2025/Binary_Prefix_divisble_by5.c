/* Problem Statement - Binary prefix devisible by 5
----------------------------------------------------------------------------------------------------------
You are given a binary array nums (0-indexed).
We define xi as the number whose binary representation is the subarray nums[0..i] (from most-significant-bit to least-significant-bit).
For example, if nums = [1,0,1], then x0 = 1, x1 = 2, and x2 = 5.
Return an array of booleans answer where answer[i] is true if xi is divisible by 5.

Example 1:
Input: nums = [0,1,1]
Output: [true,false,false]
Explanation: The input numbers in binary are 0, 01, 011; which are 0, 1, and 3 in base-10.
----------------------------------------------------------------------------------------------------------
*/

bool* prefixesDivBy5(int* nums, int numsSize, int* returnSize) {
    bool *result = calloc(numsSize, sizeof(bool));
    *returnSize = numsSize;
    int current = 0;

    for (int i = 0; i < numsSize; i++) {
        current = (current * 2 + nums[i]) % 5;
        result[i] = (current == 0);
    }

    return result;
}
