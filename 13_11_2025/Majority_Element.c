/*Problem Statement
------------------------------------------------------------------------------------
Given an array nums of size n, return the majority element.

The majority element is the element that appears more than ⌊n / 2⌋ times. You may assume that the majority element always exists in the array.

Example 1:
Input: nums = [3,2,3]
Output: 3

Example 2:
Input: nums = [2,2,1,1,1,2,2]
Output: 2
-----------------------------------------------------------------------------------
*/

/*method 1 works for smaller length array*/
int majorityElement(int* nums, int numsSize) {
    int count = 0;
    int element = 0;
    for(int i=0; i<numsSize; i++) {
        int cur_count = 0;
        for(int j=0; j<numsSize; j++) {
            if(nums[i] == nums[j]) cur_count++;
        }
        if(cur_count > (int)(numsSize/2)) return nums[i];
        if(cur_count > count) {
            count = cur_count;
            element = nums[i];
        }
    }
    if(count > (int)(numsSize/2)) return element;
    return 0;
}

/*Method 2: Sort Works for smaller array*/
int majorityElement(int* nums, int numsSize) {
    for(int i=0; i<numsSize-1; i++) {
        for(int j=0; j<numsSize-i-1; j++) {
            if(nums[j] > nums[j+1]) {
                int temp = nums[j];
                nums[j] = nums[j+1];
                nums[j+1] = temp;
            }
        }
    }
    return nums[numsSize/2];
}

/*Optimized: Boyer–Moore Voting Algorithm
Boyer–Moore majority vote finds the element that appears more than half the time in a list, very efficiently, using only a single counter and a candidate, even if the data is huge or comes as a stream.*/
int majorityElement(int* nums, int numsSize) {
    int count = 0;
    int element = 0;
    for(int i=0; i<numsSize; i++) {
        if(count == 0) element = nums[i];
        count += (nums[i] == element) ? 1 : -1;
    }
    return element;
}
