/* Problem Statement - Add binary
----------------------------------------------------------------------------------------------------------
Given two binary strings a and b, return their sum as a binary string.

Example 1:

Input: a = "11", b = "1"
Output: "100"

Example 2:
Input: a = "1010", b = "1011"
Output: "10101"

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

