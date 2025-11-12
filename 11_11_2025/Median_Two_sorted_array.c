/*Problem Statement
------------------------------------------------------------------------------------
Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.

The overall run time complexity should be O(log (m+n)).

Example 1:

Input: nums1 = [1,3], nums2 = [2]
Output: 2.00000


Example 2:
Input: nums1 = [1,2], nums2 = [3,4]
Output: 2.50000


-----------------------------------------------------------------------------------
*/

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int merged[nums1Size + nums2Size];
    int index = 0;
    int idx1 = 0, idx2 = 0;
    while(idx1<nums1Size && idx2<nums2Size) {
        if(nums1[idx1] < nums2[idx2]) {
            merged[index++] = nums1[idx1++];
        }
        else {
            merged[index++] = nums2[idx2++];
        }
    }
    while(idx1<nums1Size) {
        merged[index++] = nums1[idx1++];
    }
    while(idx2<nums2Size) {
        merged[index++] = nums2[idx2++];
    }
    if(index%2) {
        return merged[index/2];
    }
    else {
        int sum = merged[(index/2)-1] + merged[index/2];
        return (double)sum/2;
    }
}
