#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv [])
{
    if (argc != 2)
    {
        return -1;
    }

    printf ("Hello World: %s\n", argv [1]);

    sleep (10);

    return 0;
}