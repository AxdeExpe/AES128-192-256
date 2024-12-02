#include "../src/aes.h"
#include <stdio.h>

int main()
{
    if (rc(1) != 0x01) {
        printf("Expected: 0x01, Returned: 0x%02x\n", rc(1));
    }
    if (rc(2) != 0x02) {
        printf("Expected: 0x02, Returned: 0x%02x\n", rc(2));
        return 1;
    }
    if (rc(3) != 0x04) {
        printf("Expected: 0x04, Returned: 0x%02x\n", rc(3));
        return 1;
    }
    if (rc(4) != 0x08) {
        printf("Expected: 0x08, Returned: 0x%02x\n", rc(4));
        return 1;
    }
    if (rc(5) != 0x10) {
        printf("Expected: 0x10, Returned: 0x%02x\n", rc(5));
        return 1;
    }
    if (rc(6) != 0x20) {
        printf("Expected: 0x20, Returned: 0x%02x\n", rc(6));
        return 1;
    }
    if (rc(7) != 0x40) {
        printf("Expected: 0x40, Returned: 0x%02x\n", rc(7));
        return 1;
    }
    if (rc(8) != 0x80) {
        printf("Expected: 0x80, Returned: 0x%02x\n", rc(8));
        return 1;
    }
    if (rc(9) != 0x1b) {
        printf("Expected: 0x1b, Returned: 0x%02x\n", rc(9));
        return 1;
    }
    if (rc(10) != 0x36) {
        printf("Expected: 0x36, Returned: 0x%02x\n", rc(10));
        return 1;
    }
    printf("All tests passed!\n");
    return 0;
}