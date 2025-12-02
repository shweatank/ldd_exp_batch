/* Problem Statement - Bulls and Cows
----------------------------------------------------------------------------------------------------------
You are playing the Bulls and Cows game with your friend.
You write down a secret number and ask your friend to guess what the number is. When your friend makes a guess, you provide a hint with the following info:
The number of "bulls", which are digits in the guess that are in the correct position.
The number of "cows", which are digits in the guess that are in your secret number but are located in the wrong position. Specifically, the non-bull digits in the guess that could be rearranged such that they become bulls.
Given the secret number secret and your friend's guess guess, return the hint for your friend's guess.
The hint should be formatted as "xAyB", where x is the number of bulls and y is the number of cows. Note that both secret and guess may contain duplicate digits.

Example 1:
Input: secret = "1807", guess = "7810"
Output: "1A3B"
Explanation: Bulls are connected with a '|' and cows are underlined:
"1807"
  |
"7810"
----------------------------------------------------------------------------------------------------------
*/

char* getHint(char* secret, char* guess) {
    int bull_count = 0;
    int digit[10] = {0};
    int cow_count = 0;

    int i=0;
    while(secret[i] && guess[i]) {
        if(guess[i] == secret[i]) bull_count++;
        else digit[secret[i] - '0']++;
        i++;
    }

    i=0;
    while(secret[i] && guess[i]) {
        if(guess[i] != secret[i]) {
            if(digit[guess[i]-'0']) {
                cow_count++;
                digit[guess[i]-'0']--;
            }
        }
        i++;
    }

    char *result = malloc(12);
    sprintf(result,"%dA%dB",bull_count,cow_count);
    return result;
}
