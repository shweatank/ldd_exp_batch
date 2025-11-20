#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp;
    char write_buf[] = "Hello from high-level stdio!\n";
    char read_buf[100];
    size_t n;

    // open file
    fp = fopen("stdio_file.txt", "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    // write to file
    fwrite(write_buf, 1, strlen(write_buf), fp);
    fclose(fp);

    // reopen for reading
    fp = fopen("stdio_file.txt", "r");
    if (!fp) {
        perror("fopen read");
        return 1;
    }

    // read from file
    n = fread(read_buf, 1, sizeof(read_buf) - 1, fp);
    read_buf[n] = '\0';

    // print data
    printf("Read using stdio:\n%s", read_buf);

    fclose(fp);
    return 0;
}

