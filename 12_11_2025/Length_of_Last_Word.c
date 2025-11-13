/*Problem Statement
------------------------------------------------------------------------------------
Given a string s consisting of words and spaces, return the length of the last word in the string.
A word is a maximal substring consisting of non-space characters only.

Example 1:
Input: s = "Hello World"
Output: 5

Example 2:
Input: s = "   fly me   to   the moon  "
Output: 4

Example 3:
Input: s = "luffy is still joyboy"
Output: 6
-----------------------------------------------------------------------------------
*/

//Method1
int lengthOfLastWord(char* s) {
    int last = strlen(s) - 1;
    for( ; last>=0; last--) {
        if(s[last] == ' ') {
            if(s[last+1] == ' ' || s[last+1] == '\0') {
                s[last] = '\0';
            }
            else {
                break;
            }
        }
    }
    int len = strlen(s+last+1);
    return len;
}

//Method2
int lengthOfLastWord(char* s) {
    int last = strlen(s) - 1;
    int len = 0;

    //skip trailing spaces
    while(last>=0 && s[last] == ' ') {
        last--;
    }

    //count letters until space
    while(last>=0 && s[last] != ' ') {
        len++;
        last--;
    }

    return len;
}
