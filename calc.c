#include <stdio.h>
#include <string.h>

struct calculator_device {
    int is_open;
    int result;
};

struct calculator_device calc_dev = {0, 0};

int my_open(void);
int my_write(const char *expression);
int my_read(int *out);
int my_close(void);

struct file_operations {
    int (*open)(void);
    int (*write)(const char *);
    int (*read)(int *);
    int (*close)(void);
};

struct file_operations fops = {
    .open  = my_open,
    .write = my_write,
    .read  = my_read,
    .close = my_close
};

int my_open(void)
{
    if (calc_dev.is_open) {
        printf("Calculator already open!\n");
        return -1;
    }
    calc_dev.is_open = 1;
    printf("Calculator opened\n");
    return 0;
}

int my_write(const char *expression)
{
    if (!calc_dev.is_open) {
        printf("Calculator not open!\n");
        return -1;
    }

    int a, b;
    char op;

    if (sscanf(expression, "%d %c %d", &a, &op, &b) != 3) {
        printf("Invalid expression!\n");
        return -1;
    }

    switch (op) {
        case '+': calc_dev.result = a + b; break;
        case '-': calc_dev.result = a - b; break;
        case '*': calc_dev.result = a * b; break;
        case '/':
            if (b == 0) {
                printf("Divide by zero error!\n");
                return -1;
            }
            calc_dev.result = a / b;
            break;
        default:
            printf("Invalid operator!\n");
            return -1;
    }

    printf("Write: %s --> OK\n", expression);
    return 0;
}

int my_read(int *out)
{
    if (!calc_dev.is_open) {
        printf("Calculator not open!\n");
        return -1;
    }

    *out = calc_dev.result;
    printf("Read: %d\n", calc_dev.result);
    return 0;
}

int my_close(void)
{
    if (!calc_dev.is_open) {
        printf("Calculator already closed!\n");
        return -1;
    }
    calc_dev.is_open = 0;
    printf("Calculator closed\n");
    return 0;
}

int main(void)
{
    char input[50];
    int result;

    fops.open();

    printf("Enter expression (example: 10 + 20): ");
    fgets(input, sizeof(input), stdin);

    fops.write(input);
    fops.read(&result);

    printf("Final Result = %d\n", result);

    fops.close();
    return 0;
}

