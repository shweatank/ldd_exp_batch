#ifndef OPS_H
#define OPS_H

enum operation {
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4
};

struct calc_data {
    int a;
    int b;
    enum operation op;
    int result;
};

#endif

