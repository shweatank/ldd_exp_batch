/* Problem Statement - Prime Number of Set Bits in Binary Representation
----------------------------------------------------------------------------------------------------------
Given two integers left and right, return the count of numbers in the inclusive range [left, right] having a prime number of set bits in their binary representation.
Recall that the number of set bits an integer has is the number of 1's present when written in binary.
For example, 21 written in binary is 10101, which has 3 set bits.

Example 1:
Input: left = 6, right = 10
Output: 4


----------------------------------------------------------------------------------------------------------
*/

//Method 1 
int countPrimeSetBits(int left, int right) {
    int prime_count = 0;
    for (int i=left; i<= right; i++) {
        int num = i;
        int count = 0;
        while(num) {
            count++;
            num = num & (num-1);
        }
        int flag = 0;
        for(int i=2; i<count; i++) {
            if(count%i == 0) flag = 1;
        }
        if(!flag && count > 1) prime_count++;
    }
    return prime_count;
}

//Method 2
int countPrimeSetBits(int left, int right) {
    int prime[33] = {0};
    int prime_in_32[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    for(int i=0; i<11; i++) {
        prime[prime_in_32[i]] = 1;
    }
    
    int count = 0;
    for(int i=left; i<=right; i++) {
        int num = i;
        int bit_count = 0;
        while(num) {
            bit_count++;
            num = num & (num-1);
        }
        if(prime[bit_count]) count++;
    }

    return count;
}


