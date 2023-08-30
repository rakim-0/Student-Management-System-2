#ifndef STUDENT
#define STUDENT

#define MAX_STRING_LEN 50

// Define a structure to hold student information   
struct Student 
{
    int id;
    char name[MAX_STRING_LEN];
    char hostel[MAX_STRING_LEN];
    char course[MAX_STRING_LEN];
    int roomNumber;
    char dob[13];
    int yearOfStudy;
};

#endif
