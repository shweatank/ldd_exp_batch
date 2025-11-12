/*Problem Statement
------------------------------------------------------------------------------------
Given a sorted array of distinct integers and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.

You must write an algorithm with O(log n) runtime complexity.

Example 1:
Input: nums = [1,3,5,6], target = 5
Output: 2

Example 2:
Input: nums = [1,3,5,6], target = 2
Output: 1

Example 3:
Input: nums = [1,3,5,6], target = 7
Output: 4
-----------------------------------------------------------------------------------
*/

/*
Time Complexity O(n)
*/
int searchInsert(int* nums, int numsSize, int target) {
    if(numsSize == 0) return 0;

    for(int i=0; i<numsSize; i++) {
        if(target <= nums[i] ) return i;
    }

    return numsSize;
}

/*
Time Complexity O(logn)
*/
int searchInsert(int* nums, int numsSize, int target) {
    if(numsSize == 0) return 0;

    int start = 0, end = numsSize-1;
    while(start <= end) {
        int mid = start + (end - start)/2;
        if(nums[mid] > target) end = mid-1;
        else if (nums[mid] < target) start = mid+1;
        else
            return mid;
    }
    return start;
}

