#include "apue.h"
#ifdef linux
#include <sys/sysmacros.h>
#endif

int main(int argc, char* argv[])
{
    struct stat statbuf;
    int i;
    for (i = 1; i < argc; ++i)
    {
        if(stat(argv[i], &statbuf) < 0)
        {
            err_ret("stat error %s", argv[i]);
            continue;
        }
        else
        {
            printf("dev = %d/%d", major(statbuf.st_dev), minor(statbuf.st_dev));
            if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode))
            {
                printf("dev = %d/%d", major(statbuf.st_rdev),minor(statbuf.st_rdev));
            }       
        }
        printf("\n"); 
    }
    exit(0);
}