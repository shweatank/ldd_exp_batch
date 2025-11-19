/* Problem Statement
----------------------------------------------------------------------------------------------------------
Given a signed 32-bit integer x, return x with digits reversed.
If reversing x causes the value go outside signed 32-bit integer range then return 0

----------------------------------------------------------------------------------------------------------
*/
int reverse(int x){
    int rev = 0;
    int count = 0;
    while(x) {
        int rem = x%10;
        if((rev > INT_MAX/10) || (rev==INT_MAX/10 && rem > 7)) return 0;
        if((rev < INT_MIN/10) || (rev==INT_MIN/10 && rem < -8)) return 0;      
        rev = (rev * 10) + rem;
        x /= 10;
        count++;
    }
    return rev;
}
