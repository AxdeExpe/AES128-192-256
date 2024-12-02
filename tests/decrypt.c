#include "../src/aes.h"
#include <stdio.h>
#include <string.h>

int main() {
  u_int8_t input[] = {0xc7, 0xd1, 0x24, 0x19, 0x48, 0x9e, 0x3b, 0x62,
                               0x33, 0xa2, 0xc5, 0xa7, 0xf4, 0x56, 0x31, 0x72};
  
  u_int8_t block[16];
  memcpy(block, input, 16);

  u_int8_t roundkeys[176] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63,
      0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x9b, 0x98, 0x98, 0xc9,
      0xf9, 0xfb, 0xfb, 0xaa, 0x9b, 0x98, 0x98, 0xc9, 0xf9, 0xfb, 0xfb, 0xaa,
      0x90, 0x97, 0x34, 0x50, 0x69, 0x6c, 0xcf, 0xfa, 0xf2, 0xf4, 0x57, 0x33,
      0x0b, 0x0f, 0xac, 0x99, 0xee, 0x06, 0xda, 0x7b, 0x87, 0x6a, 0x15, 0x81,
      0x75, 0x9e, 0x42, 0xb2, 0x7e, 0x91, 0xee, 0x2b, 0x7f, 0x2e, 0x2b, 0x88,
      0xf8, 0x44, 0x3e, 0x09, 0x8d, 0xda, 0x7c, 0xbb, 0xf3, 0x4b, 0x92, 0x90,
      0xec, 0x61, 0x4b, 0x85, 0x14, 0x25, 0x75, 0x8c, 0x99, 0xff, 0x09, 0x37,
      0x6a, 0xb4, 0x9b, 0xa7, 0x21, 0x75, 0x17, 0x87, 0x35, 0x50, 0x62, 0x0b,
      0xac, 0xaf, 0x6b, 0x3c, 0xc6, 0x1b, 0xf0, 0x9b, 0x0e, 0xf9, 0x03, 0x33,
      0x3b, 0xa9, 0x61, 0x38, 0x97, 0x06, 0x0a, 0x04, 0x51, 0x1d, 0xfa, 0x9f,
      0xb1, 0xd4, 0xd8, 0xe2, 0x8a, 0x7d, 0xb9, 0xda, 0x1d, 0x7b, 0xb3, 0xde,
      0x4c, 0x66, 0x49, 0x41, 0xb4, 0xef, 0x5b, 0xcb, 0x3e, 0x92, 0xe2, 0x11,
      0x23, 0xe9, 0x51, 0xcf, 0x6f, 0x8f, 0x18, 0x8e};

  u_int8_t desired_result[16] = {0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07,
                        0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f};

  decrypt(block, sizeof(block), roundkeys, 10);

  for (int i = 0; i < 16; i++) {
    if (block[i] != desired_result[i]) {
      printf("Error at index %d\n", i);
      printf("Expected: %x\n", desired_result[i]);
      printf("Got: %x\n", block[i]);
      return 1;
    }
  }

  printf("Decrypt test passed\n");

  return 0;
}