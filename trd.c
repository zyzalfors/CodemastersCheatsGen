#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const char* cheats[9] = {"All Cars", "All Tracks", "Realistic Handling",
                         "Realistic Damage", "All Championships", "All Pro Challenges",
                         "Different Handling", "Invincible Cars", "Unlock Credits"};

int32_t calcseed(const int32_t input) {
 int32_t seed = 0x01;
 if(input != 0) {
  seed = 0x39;
  for(int32_t i = 0; i < input - 1; i++) seed = (0x39 * seed) % 0x44A5;
 }
 return seed;
}

char* gencode(const int32_t accesscode, const int32_t cheatid) {
 const int32_t seed1 = calcseed((cheatid % 0x64) ^ (accesscode % 0x64));
 const int32_t seed2 = calcseed((cheatid % 0x64) ^ ((accesscode / 0x64) % 0x64));
 static char code[7];
 code[0] = 0x41 + (seed1 % 0x1A);
 code[1] = 0x41 + ((seed1 / 0x2A4) % 0x1A);
 code[2] = 0x41 + ((seed1 / 0x1A) % 0x1A);
 code[3] = 0x41 + ((seed2 / 0x1A) % 0x1A);
 code[4] = 0x41 + ((seed2 / 0x2A4) % 0x1A);
 code[5] = 0x41 + (seed2 % 0x1A);
 code[6] = '\0';
 return code;
}

int main(int argc, char* argv[]) {
 if(argc != 2) {
  printf("Invalid command\n");
  return 1;
 }
 const int32_t accesscode = strtol(argv[1], NULL, 10);
 if(accesscode < 0 || accesscode > 9999) {
  printf("Valid access codes are in range 0 - 9999\n");
  return 1;
 }
 printf("TOCA Race Driver Cheats\n");
 printf("Access code: %i\n", accesscode);
 for(int i = 0; i < 9; i++) printf("%s: %s\n", cheats[i], gencode(accesscode, i));
 return 0;
}