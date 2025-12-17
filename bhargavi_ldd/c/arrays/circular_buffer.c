#include <stdio.h>
#include <stdbool.h>

#define SIZE 5   // buffer capacity

typedef struct {
    int buffer[SIZE];
    int head;   // index for reading
    int tail;   // index for writing
    int count;  // number of elements
} CircularBuffer;

// Initialize buffer
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Check if buffer is full
bool isFull(CircularBuffer *cb) {
    return cb->count == SIZE;
}

// Check if buffer is empty
bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

// Add element to buffer
bool enqueue(CircularBuffer *cb, int value) {
    if (isFull(cb)) {
        printf("Buffer is full!\n");
        return false;
    }
    cb->buffer[cb->tail] = value;
    cb->tail = (cb->tail + 1) % SIZE;  // wrap around
    cb->count++;
    return true;
}

// Remove element from buffer
bool dequeue(CircularBuffer *cb, int *value) {
    if (isEmpty(cb)) {
        printf("Buffer is empty!\n");
        return false;
    }
    *value = cb->buffer[cb->head];
    cb->head = (cb->head + 1) % SIZE;  // wrap around
    cb->count--;
    return true;
}

// Print buffer contents
void printBuffer(CircularBuffer *cb) {
    printf("Buffer: ");
    for (int i = 0; i < cb->count; i++) {
        int index = (cb->head + i) % SIZE;
        printf("%d ", cb->buffer[index]);
    }
    printf("\n");
}

int main() {
    CircularBuffer cb;
    initBuffer(&cb);

    enqueue(&cb, 10);
    enqueue(&cb, 20);
    enqueue(&cb, 30);
    enqueue(&cb, 40);
    printBuffer(&cb);

    int val;
    dequeue(&cb, &val);
    printf("Dequeued: %d\n", val);
    printBuffer(&cb);

    enqueue(&cb, 50);
    enqueue(&cb, 60);  // wraps around
    printBuffer(&cb);

    return 0;
}

