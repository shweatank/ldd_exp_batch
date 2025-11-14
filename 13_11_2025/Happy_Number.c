/*Problem Statement
------------------------------------------------------------------------------------
Write an algorithm to determine if a number n is happy.

A happy number is a number defined by the following process:

Starting with any positive integer, replace the number by the sum of the squares of its digits.
Repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle which does not include 1.
Those numbers for which this process ends in 1 are happy.
Return true if n is a happy number, and false if not.

Example 1:
Input: n = 19
Output: true
Explanation:
1^2 + 9^2 = 82
8^2 + 2^2 = 68
6^2 + 8^2 = 100
1^2 + 0^2 + 0^2 = 1

Example 2:
Input: n = 2
Output: false
Explanation: 2 -> 4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4 -> 16 -> 37 -> ........
-----------------------------------------------------------------------------------
*/

//Method1
int square(int n) {
    int sqr = 0;
    while(n) {
        int rem = n%10;
        sqr += rem*rem;
        n = n/10;
    }
    return sqr;
}
bool isHappy(int n) {
    int fast=n;
    int slow=n;
    while(1) {
        slow = square(slow);
        fast = square(square(fast));
        if(slow == 1 || fast == 1) return true;
        if(slow == fast) return false;
    }
    return false;
}

//Method2
int square(int n) {
    int sqr = 0;
    while(n) {
        int rem = n%10;
        sqr += rem*rem;
        n = n/10;
    }
    return sqr;
}
bool isHappy(int n) {
    int fast=n;
    int slow=n;
    do {
        slow = square(slow);
        fast = square(square(fast));
    } while(slow != fast);
    return (slow==1);
}

//Using hash table
int square(int n) {
    int sqr = 0;
    while(n) {
        int rem = n%10;
        sqr += rem*rem;
        n = n/10;
    }
    return sqr;
}
bool isHappy(int n) {
    int table[1000] = {0};
    while(1) {
        int sqr = square(n);
        n = sqr;
        if(sqr == 1) return true;
        if(table[sqr] == 1) return false;
        else table[sqr] = 1;
    }
    return false;
}
