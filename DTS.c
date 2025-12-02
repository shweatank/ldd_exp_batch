#include <stdio.h>

int main() {
    FILE *file;
    char buff[100];

    file = fopen("DTS1.txt", "r");

    if (file == NULL) {
        perror("Error: Could not open DTS1.txt");
        return -1;
    }

    while (fgets(buff, sizeof(buff), file) != NULL) {
        printf("%s", buff);
    }

    fclose(file);

    return 0;

}