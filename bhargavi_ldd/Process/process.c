#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h> // For signals

// Message structure for the message queue
struct message {
    long mtype; // Message type
    int data;   // Data to be sent
};

// Global variables for signal handling
volatile sig_atomic_t parent_received_result = 0;
volatile sig_atomic_t child_received_data = 0;

// Signal handler for the parent process
void parent_signal_handler(int signum) {
    if (signum == SIGUSR1) {
        parent_received_result = 1;
    }
}

// Signal handler for the child process
void child_signal_handler(int signum) {
    if (signum == SIGUSR1) {
        child_received_data = 1;
    }
}

int main() {
    key_t key;
    int msgid;
    pid_t pid;

    // Generate a unique key for the message queue
    if ((key = ftok("msgqueue_file", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        // Set up signal handler for the child
        struct sigaction sa_child;
        sa_child.sa_handler = child_signal_handler;
        sigemptyset(&sa_child.sa_mask);
        sa_child.sa_flags = 0;
        if (sigaction(SIGUSR1, &sa_child, NULL) == -1) {
            perror("sigaction child");
            exit(EXIT_FAILURE);
        }

        printf("Child: Waiting for data from parent...\n");
        // Wait for a signal from the parent indicating data is ready
        while (!child_received_data) {
            pause(); // Suspend until a signal is received
        }
        child_received_data = 0; // Reset flag

        // Receive data from the message queue
        struct message msg_rcv;
        if (msgrcv(msgid, &msg_rcv, sizeof(struct message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv child");
            exit(EXIT_FAILURE);
        }
        printf("Child: Received data from parent: %d\n", msg_rcv.data);

        // Perform calculation
        int result = msg_rcv.data * 2; // Example calculation

        // Send the result back to the parent
        struct message msg_snd_back;
        msg_snd_back.mtype = 2; // Different message type for child to parent
        msg_snd_back.data = result;
        if (msgsnd(msgid, &msg_snd_back, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd child");
            exit(EXIT_FAILURE);
        }
        printf("Child: Sent result %d back to parent.\n", result);

        // Send a signal to the parent to notify about the result
        kill(getppid(), SIGUSR1);
        printf("Child: Sent signal to parent.\n");

    } else { // Parent process
        // Set up signal handler for the parent
        struct sigaction sa_parent;
        sa_parent.sa_handler = parent_signal_handler;
        sigemptyset(&sa_parent.sa_mask);
        sa_parent.sa_flags = 0;
        if (sigaction(SIGUSR1, &sa_parent, NULL) == -1) {
            perror("sigaction parent");
            exit(EXIT_FAILURE);
        }

        // Prepare data to send
        struct message msg_snd;
        msg_snd.mtype = 1; // Message type for parent to child
        msg_snd.data = 10; // Example data

        printf("Parent: Sending data %d to child...\n", msg_snd.data);
        if (msgsnd(msgid, &msg_snd, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd parent");
            exit(EXIT_FAILURE);
        }
        printf("Parent: Data sent. Signaling child...\n");

        // Send a signal to the child to trigger data processing
        kill(pid, SIGUSR1);

        printf("Parent: Waiting for result from child...\n");
        // Wait for a signal from the child indicating the result is ready
        while (!parent_received_result) {
            pause(); // Suspend until a signal is received
        }
        parent_received_result = 0; // Reset flag

        // Receive the result from the message queue
        struct message msg_rcv_result;
        if (msgrcv(msgid, &msg_rcv_result, sizeof(struct message) - sizeof(long), 2, 0) == -1) {
            perror("msgrcv parent");
            exit(EXIT_FAILURE);
        }
        printf("Parent: Received result from child: %d\n", msg_rcv_result.data);

        // Clean up the message queue
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }
        printf("Parent: Message queue removed.\n");
    }

    return 0;
}
