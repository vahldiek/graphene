#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
    struct dirent * dirent;

    DIR * dir = opendir(".");

    while ((dirent = readdir(dir)))
        printf("found %s\n", dirent->d_name);

    closedir(dir);

    return 0;
}
