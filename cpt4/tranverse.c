#include <apue.h>
#include <dirent.h>
#include <limits.h>

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(char*, Myfunc*);
static int dopath(Myfunc*);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

char* pall_alloc(int*);

int main(int argc, char* argv[])
{
    int ret;
    if (argc != 2)
    {
        err_quit("usage: ftw <starting-pathname>");
    }

    ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    printf("regular file %ld\n", nreg);
    printf("dir file %ld\n", ndir);
    printf("block special %ld\n", nblk);
    printf("char special %ld\n", nchr);
    printf("symbolic links %ld\n", nslink);
    printf("sockets %ld\n", nsock);

    exit(0);
}

static char *fullpath;

static int
myftw(char* pathname, Myfunc* func)
{
    int len;
    fullpath = path_alloc(&len);

    strncpy(fullpath, pathname, len);
    
    fullpath[len - 1] = 0;

    return (dopath(func));    
}

#define FTW_F 1 /*file other than directory*/
#define FTW_D 2 /*directory*/
#define FTW_DNR 3 /*directory that can't be read*/
#define FTW_NS 4 /*file we can't stat*/

static int
dopath(Myfunc* func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;
    char *ptr;

    if(lstat(fullpath, &statbuf) < 0)
        return (func(fullpath, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        return (func(fullpath, &statbuf, FTW_F));
    }

    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return (ret);

    ptr = fullpath + strlen(fullpath);

    *ptr++ = '/';
    *ptr = 0;

    if((dp = opendir(fullpath)) == NULL)
        return(func(fullpath, &statbuf, FTW_DNR));

    while((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(ptr, dirp->d_name);

        if((ret = dopath(func)) != 0)
            break;
    }
    ptr[-1] = 0;

    if (closedir(dp) < 0)
    {
        err_ret("cannot close dirctory %s", fullpath);
    }
    return(ret);
}

static int
myfunc (const char *pathname, const struct stat *buf, int mode)
{
    switch(mode)
    {
        case FTW_F:
            switch(buf -> st_mode & S_IFMT)
            {
                case S_IFREG:   nreg++;    break;
                case S_IFBLK:   nblk++;    break;
                case S_IFCHR:   nchr++;    break;
                case S_IFIFO:   nfifo++;    break;
                case S_IFLNK:   nslink++;   break;
                case S_IFSOCK:   nsock++;   break;
                case S_IFDIR:
                    err_dump("for S_IFDIR path is %s", pathname);
                    break;  
            }

        case FTW_D:
            ndir++;
            break;

        case FTW_DNR:
            err_ret("cannnot read directory");
            break;

        case FTW_NS:
            err_ret("stat error");
            break;

        default:
            err_dump("unknow mode for %d for pathname %s", mode, pathname);
    }
    return (0);
}

char* path_alloc(int* size)
{
    char *p = NULL;
    if(!size) return NULL;
    p = malloc(256);
    if(p)
    *size = 256;
    else
    *size = 0;
    return p;
}