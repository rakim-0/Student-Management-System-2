#ifndef KERNCALL
#define KERNCALL

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "student.h"
#include "log.c"

struct kerncall {
    char ch; // 'u' = updateFile 's' = searchFile;
    int id; // id to update/search
    int newRoomNo; // room number to update
};


void writeToPipe(int fd, char ch, int id, int newRoomNo)
{
    struct kerncall k = {0};
    k.ch = ch;
    k.id = id;
    k.newRoomNo = newRoomNo;
    write(fd, &k, sizeof(k));
}

struct Student searchFile(int id, int flag)
{
    FILE* db = fopen("disk", "r");
    if (db == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    struct Student temp;
    while(fread(&temp, 1, sizeof(struct Student), db) && sizeof(temp) == sizeof(struct Student))
    {
        if (temp.id == id)
        {
            if (flag == 0)
            {
                // printf("%d, %s, %s, %s, %d, %s, Year %d\n", temp.id, temp.name, 
                //     temp.hostel, temp.course, temp.roomNumber, temp.dob, temp.yearOfStudy);
            }
            fclose(db);
            return temp;
        }
    }
    
    // printf("NOT FOUND!!\n");
    fclose(db);
    temp.id = -1;
    return temp;
}

// Update the file.
void updateFile(int id, int newRoomNumber)
{
    /* 
     *
     *  Implement the logic for updating here.
     *  First search file, but instead of returning a number, get the file pointer.
     *  Use fseek to reposition the file pointer. The number of bytes will be similar.
     *  And write the new struct there.
     *
     */
    FILE* db = fopen("disk", "r+");
    if (db == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }

    struct Student temp;

    while(fread(&temp, 1, sizeof(struct Student), db) && sizeof(temp) == sizeof(struct Student))
    {
        if (temp.id == id)
        {
            temp.roomNumber = newRoomNumber;
            fseek(db, -sizeof(struct Student), SEEK_CUR);
            fwrite(&temp, 1, sizeof(struct Student), db);
            fclose(db);
            return;
        }
    }

    fclose(db);
    return;
}
#endif