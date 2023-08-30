#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "log.c"
#include "student.h"
#include "main_memory.c"
#include "operation.h"
#include "request.h"
#include "queue.c"
#include "kerncall.c"

// Functional Prototypes
struct request server(int number, char op_id, int st_id, int newRoomNumber);
void registerStudent();
void registration(struct Student student);
struct Student generateStudent(int id);
char* getRandomString(char names[][MAX_STRING_LEN], int range);
char* generateFullName(char first_names[20][MAX_STRING_LEN], char last_names[20][MAX_STRING_LEN]);
void loadNamesFromFile(char* filename, char array[][MAX_STRING_LEN], int numNames);
double timeInSeconds(void) ;
double GetTime();
unsigned int randval();

void delete(int id, node* head);
void printFile(char* filename);

int main(int argc, char* argv[])
{
    int reqToKern[2];
    reqToKern[0] = argv[1][0];
    reqToKern[1] = argv[1][1];
    
    /*
     
    int kernToReq[2];
    kernToReq[0] = argv[2][0];
    kernToReq[1] = argv[2][1];
    */
    // printf("serverRead: %d\n", reqToServe[0]);
    // printf("Write: %d\n", reqToServe[1]);

    int sum = 0;

    struct operation op;
    int count = 1;

    struct Queue* q = createqueue();
    double sumOfTurnaroundTime = 0;

    double totalTurnaroundAdmin = 0;
    double totalTurnaroundManager = 0;
    double totalTurnaroundMember = 0;

    FILE* reqFile = fopen("requests.txt", "w");
    FILE* reqFiledb = fopen("requests", "w");

    while((read(reqToKern[0], &op, sizeof(op))))
    {
        if (op.opID == -1)
        {
            count--;
            sum += op.opID;
            if (sum == -3)
            {
                break;
            }
        }

        else 
        {
            struct request req;
            req.request_sequence_no = count;
            req.submission_time = timeInSeconds();

            switch (op.role)
            {
                case 'x':
                {
                    enqueueMember(q, op);
                    break;
                }
                
                case 'm':
                {
                    enqueueManager(q, op);
                    break;
                }

                case 'a':
                {
                    enqueueAdmin(q, op);
                    break;
                }
                default:
                    break;
            }

            op = dequeue(q);
            server(count, op.opID+'0', op.stdID, op.roomNo);

            req.completion_time = timeInSeconds();
            req.total_waiting_time = req.completion_time-req.submission_time;
            sumOfTurnaroundTime += req.total_waiting_time;
            if (op.role == 'x')
            {
                totalTurnaroundMember += req.total_waiting_time;
            }
            if (op.role == 'm')
            {
                totalTurnaroundManager += req.total_waiting_time;
            }
            if (op.role == 'a')
            {
                totalTurnaroundAdmin += req.total_waiting_time;
            }

            fprintf(reqFile, "ID                :   %d\n", count);
            fprintf(reqFile, "submission_time   :   %f\n", req.submission_time);
            fprintf(reqFile, "completion_time   :   %f\n", req.completion_time);
            fprintf(reqFile, "waiting_time      :   %f\n", req.total_waiting_time);
            fprintf(reqFile, "\n");

            fwrite(&req, 1, sizeof(req), reqFiledb);
            fflush(reqFile);
            fflush(reqFiledb);
            count++;
        }
    }

    double meanTurnaroundTime = sumOfTurnaroundTime/count;
    printf("Throughput: %f\n", count/(sumOfTurnaroundTime));
    printf("Mean Response Time: %f\n", meanTurnaroundTime);
    /*
    printf("ADMIN:\n");
    printf("Throughput: %f\n", 12/(totalTurnaroundAdmin));
    printf("Mean Response Time: %f\n\n", totalTurnaroundAdmin/12);

    printf("MANAGER:\n");
    printf("Throughput: %f\n", 15/(totalTurnaroundManager));
    printf("Mean Response Time: %f\n\n", totalTurnaroundManager/15);
    
    printf("MEMBER:\n");
    printf("Throughput: %f\n", 35/(totalTurnaroundMember));
    printf("Mean Response Time: %f\n\n", totalTurnaroundMember/35);
    */
    fclose(reqFile);
    fclose(reqFiledb);
}

struct request server(int number, char op_id, int st_id, int newRoomNumber)
{
    node *head = NULL;
    struct request req;
    req.request_sequence_no = number;
    req.submission_time = time(NULL);
    switch(op_id)
    {
        case '0':
        {
            registerStudent();
            break;
        }
            

        case '1':
        {
            //scanf("%d", &st_id);
            search(&head, st_id);
            break;
        }


        case '2':
        {
            //scanf("%d", &st_id);
            //scanf("%d\n", &newRoomNumber);
            update(&head, st_id, newRoomNumber);
            break;
        }
            
            
        case'3':
        {
            //scanf("%d", &st_id);
            delete(st_id, head);
            //printFile("disk.tmp");
            break;
        }

        case '4':
        {
            printFile("disk");
            break;
        }

    }

    while (head != NULL)
    {
        removeHeadNode(&head, 1);
    }

    req.completion_time = time(NULL);
    return req;
}

double GetTime() 
{
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}

double timeInSeconds(void) 
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000.0)/1000.0;
}


// Register a user and update the file accordingly
// First get the last roll no. used and then use generate_student(id) to write it to the file.
// Use printFile to see if the file is updated.
void registerStudent()
{
    writeTwo();
    FILE* db = fopen("disk", "r+");
    if (db == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    struct Student temp;
    int id = 0;

    while(fread(&temp, 1, sizeof(struct Student), db) && sizeof(temp) == sizeof(struct Student))
    {
        id = temp.id;
    }

    struct Student s;
    id++;
    s = generateStudent(id);
    fwrite(&s, 1, sizeof(struct Student), db);
    fclose(db);
}

// Delete a line from the file 

void delete(int id, node* head)
{
    /* 
     *
     *  Implement the logic for deleting here.
     *
     *  First search file, but instead of returning a number, get the line number.
     *  Now make a new copy of the file and copy all the lines except that one.
     *  Delete the original file and rename the current file to disk.
     *
     *  refer https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-8.php
     *
     */
    FILE* db = fopen("disk", "r");
    FILE* dbTemp = fopen("disk.tmp", "w");
    writeTwo();

    if (db == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    if (dbTemp == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }

    struct Student temp;
    while(fread(&temp, 1, sizeof(struct Student), db) && sizeof(temp) == sizeof(struct Student))
    {
        if (temp.id != id)
        {
            fwrite(&temp, 1, sizeof(struct Student), dbTemp);
        }
    }

    fclose(db);
    fclose(dbTemp);
    remove("disk");
    rename("disk.tmp", "disk");
    
    if (head != NULL)
    {
        if (head->student.id != id)
        {
            removeHeadNode(&head, 1);
        }
        else
        {
            removeHeadNode(&head, 0);
        }
    }
    return;
}

void printFile(char* filename)
{
    FILE* db = fopen(filename, "r");
    if (db == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    struct Student temp;
    while(fread(&temp, 1, sizeof(struct Student), db) && sizeof(temp) == sizeof(struct Student))
    {
        printf("%d: %s\n", temp.id, temp.name);
    }
    fclose(db);
}

unsigned int randval()
{
    unsigned int randval;
    FILE *f;
    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);
    return randval;
}

void loadNamesFromFile(char* filename, char array[][MAX_STRING_LEN], int numNames)
{
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < 20; i++)
    {
        char ch = fgetc(file);
        int j = 0;
        while (ch != EOF)
        {
            //printf("%c", ch);
            if (ch == '\n')
            {
                array[i][j] = '\0';
                break;
            }
            array[i][j] = ch;
            ch = fgetc(file);
            j++;
        }
        // fgets(array[i], MAX_NAME_LEN, file);
    }
    for (int i = 0; i < numNames-1; i++)
    {
        array[i][strlen(array[i]) - 1] = '\0';
    }
    fclose(file);
}

char* generateFullName(char first_names[20][MAX_STRING_LEN], char last_names[20][MAX_STRING_LEN])
{
    srand(time(NULL));
    int i = randval()%20;
    int j = randval()%20;
    char *name = malloc(sizeof(char)*105);
    sprintf(name, "%s %s", first_names[i], last_names[j]);
    //printf("%s", name);
    return name;
}

char* getRandomString(char names[][MAX_STRING_LEN], int range)
{
    srand(time(NULL));
    int num = randval() % range;
    return names[num];
}

struct Student generateStudent(int id) 
{  
    struct Student student;

    char firstNames[20][MAX_STRING_LEN];
    char lastNames[20][MAX_STRING_LEN];
    char hostelNames[5][MAX_STRING_LEN];
    char courses[5][MAX_STRING_LEN];

    loadNamesFromFile("names/firstname.txt", firstNames, 20);
    loadNamesFromFile("names/lastname.txt", lastNames, 20);
    loadNamesFromFile("names/hostelnames.txt", hostelNames, 5);
    loadNamesFromFile("names/coursenames.txt", courses, 5);
 
    srand(time(NULL));
    student.id = id;
    strcpy(student.name, generateFullName(firstNames, lastNames));
    strcpy(student.hostel, getRandomString(hostelNames, 5));
    strcpy(student.course, getRandomString(courses, 5));
    student.roomNumber = randval() % 400 + 101;
    sprintf(student.dob, "%d/%d/%d", randval() % 28 + 1, randval() % 12 + 1, randval() % 16 + 1985);
    student.yearOfStudy = randval() % 5 + 1;

    return student;
}

void registration(struct Student student) 
{
    FILE *file = fopen("disk", "a");
    if (file == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    
    fwrite(&student, 1, sizeof(struct Student), file);
    fclose(file);
}

#endif
