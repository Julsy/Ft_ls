#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    DIR             *mydir;
    struct dirent   *myfile;
    struct stat     mystat;
    char buf[512];

    if (argc > 0)
    {
        mydir = opendir(argv[1]);
        while((myfile = readdir(mydir)) != NULL)
        {
            sprintf(buf, "%s/%s", argv[1], myfile->d_name);
            stat(buf, &mystat);
            printf("%lld",mystat.st_size);
            printf(" %s\n", myfile->d_name);
        }
        closedir(mydir);
    }
}

// int main(int argc, char* argv[])
// {
//     DIR *mydir;
//     struct dirent *myfile;
//     struct stat mystat;

//     if (argc)
//     {
//         mydir = opendir(argv[1]);
//         while((myfile = readdir(mydir)) != NULL)
//         {
//             stat(myfile->d_name, &mystat);    
//             printf("%lld",mystat.st_size);
//             printf(" %s\n", myfile->d_name);
//         }
//         closedir(mydir);
//     }
// }
