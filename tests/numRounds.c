#include "../src/aes.h"

int main()
{
    if (numRounds(128) != 10)
        return 1;
    if (numRounds(192) != 12)
        return 1;
    if (numRounds(256) != 14)
        return 1;
    printf("Erfolgreich\n");
    return 0;
}