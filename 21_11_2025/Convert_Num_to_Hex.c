/* Problem Statement - Convert number to hexaadecimal
----------------------------------------------------------------------------------------------------------
Given a 32-bit integer num, return a string representing its hexadecimal representation. For negative integers, two’s complement method is used.
All the letters in the answer string should be lowercase characters, and there should not be any leading zeros in the answer except for the zero itself.
Note: You are not allowed to use any built-in library method to directly solve this problem.

Example 1:
Input: num = 26
Output: "1a"

Example 2:
Input: num = -1
Output: "ffffffff"
----------------------------------------------------------------------------------------------------------
*/

/char* toHex(int num) {
    char symbol[16] = "0123456789abcdef";

    if(num == 0) {
        char *hex = malloc(2);
        hex[0] = '0';
        hex[1] = '\0';
        return hex;
    }

    char *hex = malloc(9);
    hex[8] = '\0';
    int i=7;

    while(num && (i>=0)) {
        int res = 0x0f & num;
        hex[i] = symbol[res];
        num >>= 4;
        i--;
    }

    strcpy(hex, hex+i+1);
    return hex;
}
