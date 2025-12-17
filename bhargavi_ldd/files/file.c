#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp;
    char data[] = "hello is written by using the fwrite\n";
    char buffer[100];

    // 1. Open file in write mode
    fp = fopen("file.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // 2. Write data to file
    fwrite(data, sizeof(char), strlen(data), fp);

    // 3. Close file
    fclose(fp);
    printf("Data written successfully!\n");

    // ------------------------------
    // 4. Open file again in read mode
    fp = fopen("file.txt", "r");
    if (fp == NULL) {
        printf("Error opening file for reading!\n");
        return 1;
    }

    // 5. Read data from file
    fread(buffer, sizeof(char), sizeof(buffer), fp);

    // 6. Close file
    fclose(fp);

    // Print the read data
    printf("Data read from file:\n%s\n", buffer);

    return 0;
}


