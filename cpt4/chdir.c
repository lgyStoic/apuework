#include <apue.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    int filedeco;
    struct stat* statbuf;
    if (stat(".", statbuf) < 0)
    {
        err_sys("stat error");
    }
    else{
        printf("%ld\n", (long)statbuf->st_ino);
    }
    if (chdir("/test") < 0)
    {
        err_sys("chdir error");
    }
    else
    {
        printf("change dir ok\n");
    }
    if (stat(".", &statbuf) < 0)
    {
        err_sys("stat error");
    }
    else{
        printf("%ld\n", (long)statbuf.st_ino);
    }
    exit(0);
}