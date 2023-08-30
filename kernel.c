#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

#include "kerncall.c"
#include "student.h"
#include "operation.h"
#include "request.h"

struct Student searchFile(int id, int flag);
void updateFile(int id, int newRoomNumber);

int main()
{
    int pid = fork();
    if (pid < 0)
    {
        perror("Fork couldn't be created: ");
    }
    else if (pid == 0)
    {
        //char *argv[] = {"generate_students", "", "\0"};
        execl("./generate_students", "generate_students", NULL);
        exit(0);
    }

    pid_t wpid;
    int status = 0;

    while ((wpid = wait(&status)) > 0);

    /*
        For two way communication between request generator and kernel, there
        needs to be two pipes: reqToKern[] and kernToReq[]
        req to Kern will be for writing by kern and reading by req
        kern to req will be for writing by req and reading by kern 
    */

    int reqToKern[2];
    int kernToReq[2];

    pipe(reqToKern);
    pipe(kernToReq);
    
    char reqToKernStr[] = {reqToKern[0], reqToKern[1], '\0'};
    char kernToReqStr[] = {kernToReq[0], kernToReq[1], '\0'};

    int pidAdmin = fork();
    if (pid < 0)
    {
        perror("Fork couldn't be created: ");
    }
    else if (pidAdmin == 0)
    {
        // printf("admin\n");
        execl("./request_generator_admin", "request_generator_admin", reqToKernStr, kernToReqStr, NULL);
    }

    int pidManager = fork();
    if (pid < 0)
    {
        perror("Fork couldn't be created: ");
    }
    else if (pidManager == 0)
    {
        execl("./request_generator_manager", "request_generator_manager", reqToKernStr, kernToReqStr, NULL);
    }

    int pidMember = fork();
    if (pid < 0)
    {
        perror("Fork couldn't be created: ");
    }
    else if (pidMember == 0)
    {
        execl("./request_generator_member", "request_generator_member", reqToKernStr, kernToReqStr, NULL);
    }

    int flag = 0;
    read(reqToKern[0], &flag, sizeof(flag));

    /*
     if the request generator has asked for a connection with server, then make a new pipe and 
     return the values else return 0 if pipes failed to get created.
    */

    int reqToServe[2];

    if (flag == 1)
    {
        int pp = pipe(reqToServe);
        if (pp == -1)
        {
            int temp = 0; // indicating that pipe couldn't be created
            write(kernToReq[1], &temp, sizeof(temp));
        }

        else 
        {
            write(kernToReq[1], &reqToServe[0], sizeof(reqToServe[0]));
            write(kernToReq[1], &reqToServe[1], sizeof(reqToServe[1]));
            int pid = fork();
            if (pid == 0)
            {
                execl("./server", "server", reqToKernStr, kernToReqStr, NULL);
            }
        }
    }
    // pid_t wpid;
    // int status = 0;

    while ((wpid = wait(&status)) > 0); // wait for all the child programs to exit;

    // printf("Exiting Kernel!!\n");
}