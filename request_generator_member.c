#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "operation.h"
#include "randval.c"

int main(int argc, char* argv[])
{
    int reqToKern[2]; //request to kern
    reqToKern[0] = argv[1][0];
    reqToKern[1] = argv[1][1];

    // int kernToReq[2]; // kern to request
    // kernToReq[0] = argv[2][0];
    // kernToReq[1] = argv[2][1];
    
    // request to establish a connection with server

    // int buffer = 1;

    // write(reqToKern[1], &buffer, sizeof(buffer));
    // read(kernToReq[0], &buffer , sizeof(int));

    // int reqToServe[2];
    // if (buffer != 0)
    // {
    //     reqToServe[0] = buffer;
    //     read(kernToReq[0], &reqToServe[1], sizeof(reqToServe[1]));
    // }

    // printf("Read: %d\n", reqToServe[0]);
    // printf("Write: %d\n", reqToServe[1]);

    struct operation op;
    for (int i = 0; i < 35; i++)
    {
        sleep(1);

        op.opID = 0;
        op.stdID = 0;
        op.roomNo = 0;

        op.role = 'x';
        op.opID = randval()%4;

        if (op.opID != 0)
        {
            op.stdID = randval()%100;
        }
        if (op.opID == 2)
        {
            op.roomNo = randval() % 400 + 101;
        }
        // printf("%d. reqgen: %c\n", i+1, op.role);
        write(reqToKern[1], &op, sizeof(op));
    }

    op.opID = -1;
    write(reqToKern[1], &op, sizeof(op));
    exit(0);
}