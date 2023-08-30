#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include <stdlib.h>
//#include "main.c"
#include "log.c"
#include "student.h"
#include "kerncall.c"

typedef struct Node
{
    struct Student student;    
    struct Node* next;
}node;

node* newNode(struct Student newStudent);
void addNode(node** head, struct Student std);
void removeHeadNode(node** head, int update);
void search(node** head, int id);
void update(node** head, int id, int hostelRoomNumber);
void printList(node* head);
void freeList(node** head);
void deleteNode(node** head, int id);

// int main()
// {
//     // node* head = malloc(sizeof(node));
//     // head->student = std;
//     // head->next = NULL;
//     node *head = NULL;
//     for (int i = 1; i <= 5; i++)
//     {
//         struct Student std = generateStudent(i);
//         addNode(&head, std);
//     }
//     deleteNode(&head, 3);
//     printList(head);
//     //search(head, 15);
// }

void search(node** head, int id)
{
    writeOne();
    node* temp = *head;
    int f = 0;
    while (temp != NULL)
    {
        if (temp->student.id == id)
        {
            f = 1;
            break;
        } 
        temp = temp->next;
    }

    if (f == 1)
    {
        // struct Student tempStd = temp->student;
        // printf("%d, %s, %s, %s, %d, %s, Year %d\n", tempStd.id, tempStd.name, 
        //         tempStd.hostel, tempStd.course, tempStd.roomNumber, tempStd.dob, tempStd.yearOfStudy);
    }
    else
    {
        writeTwo();
        struct Student tempStd = searchFile(id, 0);
        addNode(head, tempStd);
    }
    return;
}


void addNode(node** head, struct Student std)
{
    node* newNode = malloc(sizeof(node));
    newNode->next = NULL;
    newNode->student = std;

    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    node* temp = *head;
    int count = 1;
    while(temp->next != NULL)
    {
        count++;
        temp = temp->next;
    }

    //printf("Count: %d\n", count);

    if (count >= 5)
        removeHeadNode(head, 1);
    temp->next = newNode;
}

void removeHeadNode(node** head, int update)
{
    struct Student std = (*head)->student;
    *head = (*head)->next;
    if (update == 1)
    {
        updateFile(std.id, std.roomNumber);
    }  
}

void printList(node* head)
{
    while(head != NULL)
    {
        printf("%d ", head->student.id);
        head = head->next;
    }
    printf("\n");
}



void update(node** head, int id, int hostelRoomNumber)
{
    node* temp = *head;
    int f = 0;
    writeOne();
    while (temp != NULL)
    {
        if (temp->student.id == id)
        {
            f = 1;
            break;
        } 
        temp = temp->next;
    }

    if (f == 1)
    {
        temp->student.roomNumber = hostelRoomNumber;
    }
    else
    {
        writeTwo();
        struct Student tempStd = searchFile(id, 1);
        tempStd.roomNumber = hostelRoomNumber;
        addNode(head, tempStd);
    }
    return;
}

void deleteNode(node** head, int id)
{
    node* prev = *head;
    if (prev->student.id == id)
    {
        *head = (*head)->next;
        free(prev);
    }
    else
    {
        node* temp = prev->next;
        while (prev != NULL)
        {
            temp = prev->next;
            if (temp != NULL)
            {
                if (temp->student.id == id)
                {
                    prev->next = temp->next;
                }
            }
            prev = prev->next;
        }
    }
}
#endif