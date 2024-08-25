#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const char* cheats[10] = {"Baja Buggy", "Jet Fighter", "Hovercraft", "Battle Tank", "Radio Control Car",
                          "All Cars", "All Tracks", "All Parts", "All Difficulties", "Ford Super Focus"};

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
 code[0] = 0x5A - (seed1 % 0x1A);
 code[1] = 0x5A - ((seed1 / 0x2A4) % 0x1A);
 code[2] = 0x5A - ((seed1 / 0x1A) % 0x1A);
 code[3] = 0x5A - ((seed2 / 0x1A) % 0x1A);
 code[4] = 0x5A - ((seed2 / 0x2A4) % 0x1A);
 code[5] = 0x5A - (seed2 % 0x1A);
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
 printf("Colin McRae Rally 3 Cheats\n");
 printf("Access code: %i\n", accesscode);
 for(int i = 0; i < 10; i++) printf("%s: %s\n", cheats[i], gencode(accesscode, i));
 return 0;
}