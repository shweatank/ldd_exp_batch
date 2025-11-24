/* Problem Statement - Missing number
----------------------------------------------------------------------------------------------------------
Given an array nums containing n distinct numbers in the range [0, n], return the only number in the range that is missing from the array.

Example 1:
Input: nums = [3,0,1]
Output: 2
Explanation:
n = 3 since there are 3 numbers, so all numbers are in the range [0,3]. 2 is the missing number in the range since it does not appear in nums.
----------------------------------------------------------------------------------------------------------
*/

char* addBinary(char* a, char* b) {

    int carry = 0;
    int i = strlen(a);
    int j = strlen(b);
    int res_size = i>j?i:j;
    char *res = malloc(res_size+2);
    res[res_size+1] = '\0';
    int index = res_size;
    i--;
    j--;
    while(i>=0 || j>=0 || carry) {
        int sum = carry;
        if(i>=0) sum += a[i] - '0';
        if(j>=0) sum += b[j] - '0';

        res[index] = (sum % 2) + '0';
        carry = sum/2;

        i--; j--; index--; 
    }

    return res+index+1;

