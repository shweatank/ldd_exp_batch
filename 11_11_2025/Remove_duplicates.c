/*Problem Statement
------------------------------------------------------------------------------------
Given an integer array nums sorted in non-decreasing order, remove the duplicates in-place such that each unique element appears only once. The relative order of the elements should be kept the same.

Consider the number of unique elements in nums to be k​​​​​​​​​​​​​​. After removing duplicates, return the number of unique elements k.

Example 1:
Input: nums = [1,1,2]
Output: 2, nums = [1,2,_]

Example 2:
Input: nums = [0,0,1,1,1,2,2,3,3,4]
Output: 5, nums = [0,1,2,3,4,_,_,_,_,_]
-----------------------------------------------------------------------------------
*/
int removeDuplicates(int* nums, int numsSize) {
    if(numsSize == 0) return 0;

    int index = 1;
    for(int i=1; i<numsSize; i++) {
        if (nums[i] != nums[i-1]) {
            nums[index] = nums[i];
            index++;
        }
    }
    return index;
}
