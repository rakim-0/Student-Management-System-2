#include <stdio.h>
#include <stdlib.h>

#include "request.h"


int main()
{
    FILE* reqFiledb = fopen("requests", "r");
    struct request req;
    while(fread(&req, 1, sizeof(struct request), reqFiledb) && sizeof(req) == sizeof(struct request))
    {
        // printf("ID                :   %d\n", req.request_sequence_no);
        // printf("submission_time   :   %d\n", req.submission_time);
        // printf("completion_time   :   %d\n", req.completion_time);
        // printf("waiting_time      :   %d\n", req.total_waiting_time);
    }
    fclose(reqFiledb);
}