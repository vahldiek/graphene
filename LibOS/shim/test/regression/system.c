#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int ret = system("echo hello from system");
    if(ret == -1 && ret == 127) {
        return 1;
    } else {
        return 0;
    }
}
