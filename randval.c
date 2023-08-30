#include <stdio.h>

unsigned int randval()
{
    unsigned int randval;
    FILE *f;

    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);

    return randval;

    return 0;
}
