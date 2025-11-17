#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    else if (pid == 0) {
        char expression[100];
        double a, b, result;
        char op;

        close(pipefd[1]);

        read(pipefd[0], expression, sizeof(expression));
        printf("Child received: %s\n", expression);

        if (sscanf(expression, "%lf %c %lf", &a, &op, &b) == 3) {
            switch (op) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b != 0)
                        result = a / b;
                    else {
                        printf("Error: Division by zero\n");
                        close(pipefd[0]);
                        exit(0);
                    }
                    break;
                default:
                    printf("Error: Invalid operator\n");
                    close(pipefd[0]);
                    exit(0);
            }

            printf("Result = %.2f\n", result);
        } else {
            printf("Error: Invalid expression format\n");
        }

        close(pipefd[0]);
        exit(0);
    }

    else {
        char expression[100];

        close(pipefd[0]);
        printf("Enter expression (e.g., 3 + 4): ");
        fgets(expression, sizeof(expression), stdin);

        write(pipefd[1], expression, strlen(expression) + 1);

        close(pipefd[1]);

        wait(NULL);
        printf("Parent: Child process completed.\n");
    }

    return 0;
}
