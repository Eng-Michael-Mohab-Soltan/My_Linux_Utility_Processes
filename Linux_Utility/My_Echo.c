#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{
    int len;
    int ret; 

    for (int i = 1; i < argc; i++)
    {
        if (i != 1)
        {
            ret = write(1, " ", 1);
        }

        len = strlen(argv[i]);
        ret = write(1, argv[i], len);

        if (ret == -1)
        {
            write(1, "write function failed!!!!\n", 26);
            return 0;
        }
    }

    write(1, "\n", 1);

    return 0;
}
