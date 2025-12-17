#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

// Structure for sending a message (parent → child)
struct calc_msg {
    long msg_type;
    int num1;
    int num2;
    char op;
};

// Structure for receiving result (child → parent)
struct result_msg {
    long msg_type;
    float result;
};

int main() {
    key_t key;
    int msgid;

    // Generate a unique key
    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // ---------- Parent Process ----------
        struct calc_msg message;
        struct result_msg result;

        message.msg_type = 1; // message type for child
        result.msg_type = 2;  // result message type

        printf("Enter first number: ");
        scanf("%d", &message.num1);
        printf("Enter operator (+, -, *, /): ");
        scanf(" %c", &message.op);
        printf("Enter second number: ");
        scanf("%d", &message.num2);

        // Send data to child
        if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        // Receive result from child
        if (msgrcv(msgid, &result, sizeof(result) - sizeof(long), 2, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Result: %.2f\n", result.result);

        // Destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);
    }
    else if (pid == 0) {
        // ---------- Child Process ----------
        struct calc_msg message;
        struct result_msg result;

        // Receive message from parent
        if (msgrcv(msgid, &message, sizeof(message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        // Perform calculation
        switch (message.op) {
            case '+': result.result = message.num1 + message.num2; break;
            case '-': result.result = message.num1 - message.num2; break;
            case '*': result.result = message.num1 * message.num2; break;
            case '/': 
                if (message.num2 == 0) {
                    printf("Division by zero error!\n");
                    result.result = 0;
                } else {
                    result.result = (float)message.num1 / message.num2;
                }
                break;
            default:
                printf("Invalid operator!\n");
                result.result = 0;
        }

        result.msg_type = 2;

        // Send result back to parent
        if (msgsnd(msgid, &result, sizeof(result) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        exit(0);
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}

