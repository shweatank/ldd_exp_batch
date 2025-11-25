/* Problem Statement - Flipping an image
----------------------------------------------------------------------------------------------------------
Given an n x n binary matrix image, flip the image horizontally, then invert it, and return the resulting image.
To flip an image horizontally means that each row of the image is reversed.

For example, flipping [1,1,0] horizontally results in [0,1,1].
To invert an image means that each 0 is replaced by 1, and each 1 is replaced by 0.
For example, inverting [0,1,1] results in [1,0,0].

Example 1:
Input: image = [[1,1,0],[1,0,1],[0,0,0]]
Output: [[1,0,0],[0,1,0],[1,1,1]]
Explanation: First reverse each row: [[0,1,1],[1,0,1],[0,0,0]].
Then, invert the image: [[1,0,0],[0,1,0],[1,1,1]]
----------------------------------------------------------------------------------------------------------
*/

//Method 1 
int** flipAndInvertImage(int** image, int imageSize, int* imageColSize, int* returnSize, int** returnColumnSizes) {
    for(int i=0; i<imageSize; i++) {
        int j = 0;
        int k = *imageColSize - 1;
        while(j<=k) {
            if(j==k) {
                image[i][j] = image[i][j] ^ 1;
            }
            else {
                int temp = image[i][j] ^ 1;
                image[i][j] = image[i][k] ^ 1;
                image[i][k] = temp;
            }
            j++;
            k--;
        }
    }
 
    *returnSize = imageSize;
    *returnColumnSizes = imageColSize;
    return image;
}

