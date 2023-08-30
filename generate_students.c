#ifndef GENERATE
#define GENERATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>

#include "student.h"

// Function to load names from a file into an array


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

double GetTime() 
{
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}

void Spin(int howlong) 
{
    double t = GetTime();
    while ((GetTime() - t) < (double) howlong); // do nothing in loop
}


void generate()
{
    FILE *file = fopen("disk", "a");
    if (file == NULL) 
    {
        perror("Error opening database file");
        exit(0);
    }
    fseek(file, -sizeof(struct Student), SEEK_CUR);
    struct Student temp;
    fread(&temp, 1, sizeof(struct Student), file);
    if (temp.id >= 100)
    {
        // printf("Registration complete for 100 students.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < 100; i++) 
    {
        struct Student student = generateStudent(i+1);
        //Spin(1);
        registration(student);
    }

    // printf("Registration complete for 100 students.\n");
    fclose(file);
}

int main()
{
    generate();
}
#endif