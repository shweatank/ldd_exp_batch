/* Problem Statement - Major Element II
----------------------------------------------------------------------------------------------------------
Given an integer array of size n, find all elements that appear more than ⌊ n/3 ⌋ times.

Example 1:
Input: nums = [3,2,3]
Output: [3]

Example 2:
Input: nums = [1]
Output: [1]

Example 3:
Input: nums = [1,2]
Output: [1,2]
----------------------------------------------------------------------------------------------------------
*/

int* majorityElement(int* nums, int numsSize, int* returnSize) {
    int cand1 = 0, cand2 = 0;
    int count1 = 0, count2 = 0;

    for (int i=0; i<numsSize; i++) {
        if (nums[i] == cand1) count1++;
        else if(nums[i] == cand2) count2++;
        else if (count1 == 0) {
            count1 = 1;
            cand1 = nums[i];
        }
        else if (count2 == 0) {
            count2 = 1;
            cand2 = nums[i];
        }
        else {
            count1--;
            count2--;
        }
    }

    int *arr = malloc(sizeof(int) * 2);
    count1 = count2 = 0;
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] == cand1) count1++;
        else if (nums[i] == cand2) count2++;
    }

    *returnSize = 0;
    if (count1 > numsSize / 3) {
        arr[*returnSize] = cand1;
        (*returnSize)++;
    }
    if (count2 > numsSize / 3) {
        arr[*returnSize] = cand2;
        (*returnSize)++;
    }
    
    return arr;
}
