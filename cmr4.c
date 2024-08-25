#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const char* cheats[7] = {"Group B Mode/Cars", "All Cars", "All Tracks", "Expert Mode", "All Parts", "All Tests", "Mirror Mode"};

int32_t calcseed(const int32_t input) {
 int32_t seed = 0x01;
 if(input != 0) {
  seed = 0x181AB;
  for(int32_t i = 0; i < input - 1; i++) seed = (0x181AB * seed) % 0x5243;
 }
 return seed;
}

int32_t calcfeedback(const int32_t input[]) {
 int32_t feedback = 0;
 for(int i = 0; i < 5; i++) feedback += input[i];
 return feedback;
}

char* gencode(const int32_t accesscode, const int32_t cheatid) {
 const int32_t array[10] = {491, 563, 613, 661, 733, 797, 857, 919, 983, 1039};
 const int32_t cheatidmagic = 0x13CB5B * cheatid % 0x26DD;
 const int32_t accesscodemagic = (accesscode % 0x3E8) ^ (0x20B9 * cheatidmagic);
 const int32_t seed1 = calcseed(accesscodemagic % 0x853);
 const int32_t seed2 = calcseed(((accesscodemagic ^ 0x114CF1) * ((0x41B * cheatidmagic) ^ ((accesscode / 0x3E8) % 0x3E8))) % 0x8CB);
 const int32_t buffer[6] = {0x41 + (seed1 % 0x1A),
                            0x41 + ((seed1 / 0x2A4) % 0x1A),
                            0x41 + ((seed1 / 0x1A) % 0x1A),
                            0x41 + ((seed2 / 0x1A) % 0x1A),
                            0x41 + ((seed2 / 0x2A4) % 0x1A),
                            0x41 + (seed2 % 0x1A)};
 const int32_t bufmidxor = calcfeedback(buffer);
 int32_t feedback1 = (buffer[0] << 0x18) + (buffer[1] << 0x10) + (buffer[2] << 0x08) + buffer[3];
 int32_t feedback2 = (buffer[4] << 0x18) + (buffer[5] << 0x10) + ((bufmidxor + ((cheatidmagic ^ 0x197ABD9) % (seed1 & 0xFFFFFFFF))) << 0x08) + bufmidxor + ((cheatidmagic ^ 0x13478FDD) % (seed2 & 0xFFFFFFFF));
 for(int i = 0; i < 80; i++) {
  int32_t oldfeedback2 = feedback2;
  feedback2 = feedback1 ^ array[i % 10];
  feedback1 = oldfeedback2 ^ feedback1;
 }
 static char code[7];
 code[0] = 0x41 + (((feedback2 >> 0x18) & 0xFF) % 0x1A);
 code[1] = 0x41 + (((feedback2 >> 0x10) & 0xFF) % 0x1A);
 code[2] = 0x41 + (((feedback1 >> 0x18) & 0xFF) % 0x1A);
 code[3] = 0x41 + (((feedback1 >> 0x10) & 0xFF) % 0x1A);
 code[4] = 0x41 + (((feedback1 >> 0x08) & 0xFF) % 0x1A);
 code[5] = 0x41 + ((feedback1 & 0xFF) % 0x1A);
 code[6] = '\0';
 return code;
}

int main(int argc, char* argv[]) {
 if(argc != 2) {
  printf("Invalid command\n");
  return 1;
 }
 const int32_t accesscode = strtol(argv[1], NULL, 10);
 if(accesscode < 0 || accesscode > 99999) {
  printf("Valid access codes are in range 0 - 99999\n");
  return 1;
 }
 printf("Colin McRae Rally 4 Cheats\n");
 printf("Access code: %i\n", accesscode);
 for(int i = 0; i < 7; i++) printf("%s: %s\n", cheats[i], gencode(accesscode, i));
 return 0;
}