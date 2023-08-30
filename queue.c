#include <stdio.h>
#include <stdlib.h>

#include "operation.h"

struct qnode {
    struct operation key;
    struct qnode* next;
};

struct Queue {
    struct qnode *front, *rear;
};

struct qnode* newQnode(struct operation k)
{
    struct qnode* temp = (struct qnode*)malloc(sizeof(struct qnode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

struct Queue* createqueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(struct Queue* q, struct operation k)
{
    struct qnode* temp = newQnode(k);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
    }

    q->rear->next = temp;
    q->rear = temp;
}

struct operation dequeue(struct Queue* q)
{
    struct operation key = {0};
    // key.opID = '9';

    if (q->front == NULL)
        return key;

    struct qnode* temp = q->front;
    key = temp->key;

    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    //free(temp);
    return key;
}

void enqueueAdmin(struct Queue* q, struct operation key)
{
    struct qnode* temp = newQnode(key);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    struct qnode* tempq = q->front->next;

    if (tempq == NULL)
    {
        q->front->next = temp;
        return;
    }

    temp->next = q->front->next;
    q->front->next = temp;
    return;
}

void enqueueManager(struct Queue* q, struct operation key)
{
    struct qnode* temp = newQnode(key);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    struct qnode* tempq = q->front;

    int n = 1;

    while (tempq != NULL)
    {
        if (n == 3 && tempq->next != NULL)
        {
            temp->next = tempq->next;
            tempq->next = temp;
            return;

        }
        else if (tempq->next == NULL)
        {
            tempq->next = temp;
            return;
        }
        if (tempq->key.role != 'a')
        {
            n++;
        }
        tempq = tempq->next;
        
    }
    return;
}


void enqueueMember(struct Queue* q, struct operation key)
{
    enqueue(q, key);
}

// int main()
// {
//     struct Queue* q = createqueue();
//     struct operation op = {0};
//     op.opID = '1';
//     enqueue(q, op);
//     op.opID = '2';
//     enqueue(q, op);
//     op.opID = '4';
//     enqueueAdmin(q, op);
//     op.opID = '5';
//     enqueue(q, op);
//     op.opID = '6';
//     enqueue(q, op);
//     op.opID = '3';
//     enqueueManager(q, op);



//     int n = 10;
//     while(n--)
//     {
//         if (q->front != NULL)
//             printf("%d. Front: %c\n", 10-n,q->front->key.opID);
//         dequeue(q);
//     }
//     return 0;
// }

