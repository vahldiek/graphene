#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if(system("echo hello from system")) {
        return 1;
    } else {
        return 0;
    }
}
