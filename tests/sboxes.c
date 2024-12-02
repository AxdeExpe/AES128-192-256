#include "../src/aes.h"
#include <stdio.h>

int main() {
    if (getSBoxValue(0x00) != 0x63)
        return 1;
    if (getSBoxValue(0x01) != 0x7c)
        return 1;
    if (getSBoxValue(0x49) != 0x3b)
        return 1;
    if (getSBoxInvert(0x55) != 0xed)
        return 1;
    if (getSBoxInvert(0x00) != 0x52)
        return 1;
    if (getSBoxInvert(0xda) != 0x7a)
        return 1;
    printf("All tests passed\n");
    return 0;
}