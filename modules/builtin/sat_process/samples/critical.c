#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv [])
{
    printf ("Critical process running...\n");

    sleep (30);

    return 0;
}