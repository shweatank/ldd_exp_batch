/*Problem Statement
------------------------------------------------------------------------------------
Given an m x n integer matrix matrix, if an element is 0, set its entire row and column to 0's.
You must do it in place.

Example 1:
Input: matrix = [[1,1,1],[1,0,1],[1,1,1]]
Output: [[1,0,1],[0,0,0],[1,0,1]]

Example 2:
Input: matrix = [[0,1,2,0],[3,4,5,2],[1,3,1,5]]
Output: [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
 
-----------------------------------------------------------------------------------
*/

/*method 1*/
void setZeroes(int** matrix, int matrixSize, int* matrixColSize) {
    int colSize = *matrixColSize;
    int total = matrixSize * colSize;
    int arr[total][2];
    int idx = 0;
    for(int i=0; i<matrixSize; i++) {
        for(int j=0; j<colSize; j++) {
            if(matrix[i][j] == 0) {
                arr[idx][0] = i;
                arr[idx][1] = j;
                idx++;
            }
        }
    }

    for(int i=0; i<idx; i++) {
        int row = arr[i][0];
        int col = arr[i][1];

        for(int j=0; j<colSize; j++) {
            matrix[row][j] = 0;
        }
        for(int j=0; j<matrixSize; j++) {
            matrix[j][col] = 0;
        }
    }
    return;
}


