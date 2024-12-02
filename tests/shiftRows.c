#include "../src/aes.h"
#include <stdio.h>

int main()
{
    u_int8_t state[16] = {      // stay, i + 4.  i + 4*2,   i + 4*3
        0x00, 0x01, 0x02, 0x03, // stay, 1 <--> 5, 2 <--> 10, 3 <--> 15
        0x04, 0x05, 0x06, 0x07, // stay, 5 <--> 9, 6 <--> 14, 7 <--> 3, 
        0x08, 0x09, 0x0a, 0x0b, // stay, 9 <--> 13, 10 <--> 2, 11 <--> 7
        0x0c, 0x0d, 0x0e, 0x0f  // stay, 13 <--> 1, 14 <--> 6, 15 <--> 11
    };
    u_int8_t desired_result[16] = {
        0x00, 0x05, 0x0a, 0x0f,
        0x04, 0x09, 0x0e, 0x03,
        0x08, 0x0d, 0x02, 0x07,
        0x0c, 0x01, 0x06, 0x0b
    };

    shiftRows(state, sizeof(state));

    for(int i = 0; i < sizeof(state); i++)
    {
        if (state[i] != desired_result[i])
        {
            printf("Error at index %d\n", i);
            printf("Expected: %x\n", desired_result[i]);
            printf("Got: %x\n", state[i]);
            return 1;
        }
    }

    printf("All tests passed!\n");

    return 0;
}