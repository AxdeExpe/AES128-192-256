#include "../src/aes.h"

int main()
{
    if (numKeyWords(128) != 4)
        return 1;
    if (numKeyWords(192) != 6)
        return 1;
    if (numKeyWords(256) != 8)
        return 1;
    printf("Erfolgreich 1\n");
    return 0;
}