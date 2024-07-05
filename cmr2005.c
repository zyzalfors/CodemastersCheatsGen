#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* cheats[10] = {"All Tracks", "4WD Cars", "2WD Cars", "Super 2WD Cars", "RWD Cars",
                          "4x4 Cars", "Classic Cars", "Special Cars", "Group B Cars", "Mirror Mode"};
const char* platforms[4] = {"PC", "PS2", "PSP", "XBOX"};

int isplatform(char platform[]) {
 for(size_t i = 0; i < strlen(platform); i++) platform[i] = toupper(platform[i]);
 for(int i = 0; i < 4; i++) {
  if(!strcmp(platform, platforms[i])) return 1;
 }
 return 0;
}

int32_t calcseed(const int32_t input) {
 int32_t seed = 0x01;
 if(input != 0) {
  seed = 0xF82D;
  for(int32_t i = 0; i < input - 1; i++) seed = (0xF82D * seed) % 0x5243;
 }
 return seed;
}

int32_t calcfeedback(const int32_t input[]) {
 int32_t feedback = 0;
 for(int i = 0; i < 5; i++) feedback += input[i] ^ 0x13C501;
 return feedback;
}

char* gencode(const char platform[], const int32_t accesscode, const int32_t cheatid) {
 int32_t array1[10], array2[10];
 if(!strcmp(platform, "PC")) {
  memcpy(array1, (int32_t[10]) {331, 1259, 241, 109, 521, 853, 71, 719, 941, 269}, 10 * sizeof(int32_t));
  memcpy(array2, (int32_t[10]) {72481, 180307, 130241, 392827, 421019, 949147, 32801, 1296649, 91249, 639679}, 10 * sizeof(int32_t));
 }
 else if(!strcmp(platform, "PS2") || !strcmp(platform, "XBOX")) {
  memcpy(array1, (int32_t[10]) {859, 773, 151, 47, 487, 211, 617, 131, 947, 313}, 10 * sizeof(int32_t));
  memcpy(array2, (int32_t[10]) {69119, 67783, 70271, 77929, 238099, 148151, 472751, 818963, 1195489, 839381}, 10 * sizeof(int32_t));
 }
 else if(!strcmp(platform, "PSP")) {
  memcpy(array1, (int32_t[10]) {743, 1663, 227, 991, 443, 89, 571, 199, 1373, 601}, 10 * sizeof(int32_t));
  memcpy(array2, (int32_t[10]) {35491, 783019, 1116491, 591319, 194591, 37369, 822839, 86083, 354661, 99809}, 10 * sizeof(int32_t));
 }
 const int32_t cheatidmagic = 0x13CB5B * cheatid % 0x26DD;
 const int32_t accesscodemagic = (accesscode % 0x3E8) ^ (0x20B9 * cheatidmagic);
 const int32_t seed1 = calcseed(accesscodemagic % 0x9AD);
 const int32_t seed2 = calcseed(((accesscodemagic ^ 0x114CF1) * ((0x41B * cheatidmagic) ^ ((accesscode / 0x3E8) % 0x3E8))) % 0x91D);
 const int32_t buffer[6] = {0x41 + (seed1 % 0x1A),
                            0x41 + ((seed1 / 0x2A4) % 0x1A),
                            0x41 + ((seed1 / 0x1A) % 0x1A),
                            0x41 + ((seed2 / 0x1A) % 0x1A),
                            0x41 + ((seed2 / 0x2A4) % 0x1A),
                            0x41 + (seed2 % 0x1A)};
 const int32_t bufmidxor = calcfeedback(buffer);
 int32_t feedback1 = (buffer[0] << 0x18) + (buffer[1] << 0x10) + (buffer[2] << 0x08) + buffer[3];
 int32_t feedback2 = (buffer[4] << 0x18) + (buffer[5] << 0x10) + ((bufmidxor + ((cheatidmagic ^ 0x197ABD9) % (seed1 & 0xFFFFFFFF))) << 0x08) + bufmidxor + ((cheatidmagic ^ 0x13478FDD) % (seed2 & 0xFFFFFFFF));
 for(int i = 0; i < 42; i++) {
  int32_t oldfeedback2 = feedback2;
  feedback2 = feedback1 ^ array1[i % 10];
  feedback1 = oldfeedback2 ^ feedback1;
 }
 for(int i = 0; i < 277; i++) {
  int32_t oldfeedback2 = feedback2;
  feedback2 = feedback1 ^ array2[i % 10];
  feedback1 = feedback1 ^ oldfeedback2;
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
 if(argc != 3) {
  printf("Invalid command\n");
  return 1;
 }
 char* platform = NULL;
 int32_t accesscode = 0;
 for(int i = 1; i < 3; i++) {
  if(isplatform(argv[i])) platform = argv[i];
  else accesscode = strtol(argv[i], NULL, 10);
 }
 if(!platform) {
  printf("Valid platforms are PC, PS2, PSP, XBOX\n");
  return 1;
 }
 else if(accesscode < 0 || accesscode > 99999) {
  printf("Valid access codes are in range 0 - 99999\n");
  return 1;
 }
 printf("Colin McRae Rally 2005 Cheats\n");
 printf("Platform: %s\n", platform);
 printf("Access code: %i\n", accesscode);
 for(int i = 0; i < 10; i++) printf("%s: %s\n", cheats[i], gencode(platform, accesscode, i));
 return 0;
}