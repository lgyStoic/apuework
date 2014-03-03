#include "apue.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define BUFSIZE 4096
#define RWRWRW S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
int createAholeFIle();
int createNoHoleFile();

char *fileWithHole = "file.hole";
char *fileWithNoHole = "file.nohole"; 
int main(void)
{
    int creatFileNo,writeFileNo;
    char buf[BUFSIZE];
    int n;
    creatFileNo = createAholeFIle(); //create a file and write something
    writeFileNo = createNoHoleFile(); //create a file  
    // close(creatFileNo);
    // if ((creatFileNo = open(fileWithHole, O_RDONLY)) < 0)
    // {
    //     err_ret("open error");
    // }
    fdatasync(creatFileNo);
    if(lseek(creatFileNo, 0, SEEK_SET) < 0)
        err_ret("lseek error");
    while ((n = read(creatFileNo, buf, BUFSIZE)) > 0)
    {
        printf("-------\n");
        write(writeFileNo, buf, n);
    }
    printf("%d\n", n);
    close(writeFileNo);
    close(creatFileNo);
    exit (0);
}

int createAholeFIle(void)
{
    char *buf1 = "abcdef";
    char *buf2 = "ABCDEF";
    int fileno;
    if ((fileno = creat(fileWithHole, RWRWRW)) < 0)
    {
        err_ret("1creat error");
    }
    if (write(fileno, buf1, strlen(buf1)) != strlen(buf1))
    {
        err_ret("write error");
    }
    if (lseek(fileno, 16384, SEEK_SET) == -1)
    {
        err_ret("lseek error");
    }
    if (write(fileno, buf2, strlen(buf2)) != strlen(buf2))
    {
        err_ret("write error");
    }
    return fileno;
}

int createNoHoleFile(void)
{
    int fileno;
    if((fileno = creat(fileWithNoHole, RWRWRW)) < 0)
        err_ret("2creat error");
    return fileno;
}