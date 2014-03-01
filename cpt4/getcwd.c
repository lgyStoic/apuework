#include "apue.h"
#include <unistd.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    char *fullpath;
    char *temppath;
    int size;
    DIR *dp;
    char *ptr;
    char *rootpath;
    struct dirent *dirp;
    struct stat statbuf;

    if (argc != 2)
    {
        err_ret("agrc error");
    }
    
    rootpath = argv[1];
    fullpath = path_alloc(&size);
    temppath = path_alloc(&size);
    strcpy(fullpath, rootpath);
    dp = opendir(fullpath);
    dirp = readdir(dp);
    while (dirp != NULL)
    {
        strcpy(temppath, fullpath);
        ptr = temppath + strlen(temppath);
        *ptr++ = '/';
        *ptr = '0';
        if (strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            dirp = readdir(dp);
            continue;
        }
        strcpy(ptr, dirp->d_name);
        if (stat(temppath, &statbuf) < 0)
        {
            break;
        }
        else
        {

            if (!S_ISDIR(statbuf.st_mode))
            {
                dirp = readdir(dp);
            }
            else{
                ptr = fullpath + strlen(fullpath);
                *ptr++ = '/';
                *ptr = '0';
                strcpy(ptr, dirp->d_name);
                dp = opendir(fullpath);
                dirp = readdir(dp);
            }
        }
        memset(temppath, '0', sizeof(temppath));
    }
    if (chdir(fullpath) < 0)
    {
        err_sys("chdir error");
    }
    if (getcwd(fullpath, size) == NULL)
    {
        err_sys("getcwd error");
    }
    printf("%s\n", fullpath);
    exit(0);
}
