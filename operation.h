#ifndef OPERATION
#define OPERATION

struct operation
{
    char status; // 'r' - ready 'u' - running 'b' - blocked
    char role; // 'a' - admin 'm' - manager 'x' - members
    int opID;
    int stdID;
    int roomNo;
};
#endif
