#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* cheats[7] = {"All Championships", "All Bonus Championships", "Double Power", "Swap FWD and RWD", "Invincible Cars", "All Cutscenes", "All Trans World Cup Events"};
const char* platforms[4] = {"PC", "PS2", "PSP", "XBOX"};

int isplatform(char platform[]) {
 for(size_t i = 0; i < strlen(platform); i++) platform[i] = toupper(platform[i]);
 for(int i = 0; i < 4; i++) {
  if(!strcmp(platform, platforms[i])) return 1;
 }
 return 0;
}

int32_t calcseed(const int32_t input, const int32_t mult, const int32_t array[]) {
 int32_t seed = 0x01;
 if(input != 0) {
  seed = array[1];
  for(int32_t i = 0; i < input - 1; i++) seed = ((array[2] + seed * array[1] * mult) & 0xFFFFFFFF) % array[3];
 }
 return seed;
}

uint64_t pwr(const uint64_t num, const int e) {
 uint64_t n = 1;
 for(int i = 0; i < e; i++) n *= num;
 return n;
}

char* gencodeps2(const int32_t accesscode, const int32_t cheatid) {
 uint64_t val1 = 1;
 uint64_t val2 = (accesscode + 19) * (cheatid + 17);
 uint64_t loopctr = 10007;
 while(loopctr != 0) {
  if(loopctr & 1) val1 = (((((((val1 * val2) % 0xFFFFFFFB) + ((((val1 >> 0x20) * val2) % 0xFFFFFFFB) * 0x40)) % 0xFFFFFFFB) + (((val1 * (val2 >> 0x20)) % 0xFFFFFFFB) * 0x40)) % 0xFFFFFFFB) + ((((val1 >> 0x20) * (val2 >> 0x20)) % 0xFFFFFFFB) * 0x80)) % 0xFFFFFFFB;
  val2 = (((((((val2 * val2) % 0xFFFFFFFB) + ((((val2 >> 0x20) * val2) % 0xFFFFFFFB) * 0x40)) % 0xFFFFFFFB) + (((val2 * (val2 >> 0x20)) % 0xFFFFFFFB) * 0x40)) % 0xFFFFFFFB) + ((((val2 >> 0x20) * (val2 >> 0x20)) % 0xFFFFFFFB) * 0x80)) % 0xFFFFFFFB;
  loopctr >>= 1;
 }
 val1 = (val1 ^ 0xFA47D181) % 0x309F1020EF;
 const uint64_t seed = (val1 ^ ((val1 ^ (val1 >> 0x08) ^ (val1 >> 0x10) ^ (val1 >> 0x18) ^ (val1 >> 0x20) ^ (val1 >> 0x28) ^ (val1 >> 0x30) ^ (val1 >> 0x38)) << 0x21)) & 0x1FFFFFFFFFF;
 static char code[9];
 for(int i = 0; i < 8; i++) code[i] = 0x41 + ((seed / pwr(0x1A, i)) % 0x1A);
 code[8] = '\0';
 return code;
}

char* gencode(const char platform[], const int32_t accesscode, const int32_t cheatid) {
 if(!strcmp(platform, "PS2")) return gencodeps2(accesscode, cheatid);
 int32_t array[4];
 if(!strcmp(platform, "PC")) memcpy(array, (int32_t[4]) {79838997, 107, 453889, 456959}, 4 * sizeof(int32_t));
 else if(!strcmp(platform, "PSP")) memcpy(array, (int32_t[4]) {31873457, 211, 453889, 456959}, 4 * sizeof(int32_t));
 else if(!strcmp(platform, "XBOX")) memcpy(array, (int32_t[4]) {93199323, 179, 453889, 456959}, 4 * sizeof(int32_t));
 const int32_t seed1 = calcseed((cheatid % 0x64) ^ (accesscode % 0x64) ^ (array[0] % 0x64), (array[0] / 0x2710) % 0x64, array);
 const int32_t seed2 = calcseed((cheatid % 0x64) ^ ((array[0] / 0x64) % 0x64) ^ ((accesscode / 0x64) % 0x64), (array[0] / 0xF4240) % 0x64, array);
 static char code[9];
 code[0] = 0x41 + (seed1 % 0x1A);
 code[1] = 0x41 + ((seed1 / 0x1A) % 0x1A);
 code[2] = 0x41 + ((seed1 / 0x2A4) % 0x1A);
 code[3] = 0x41 + ((seed1 / 0x44A8) % 0x1A);
 code[4] = 0x41 + (seed2 % 0x1A);
 code[5] = 0x41 + ((seed2 / 0x1A) % 0x1A);
 code[6] = 0x41 + ((seed2 / 0x2A4) % 0x1A);
 code[7] = 0x41 + ((seed2 / 0x44A8) % 0x1A);
 code[8] = '\0';
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
 else if(accesscode < 0 || accesscode > 9999) {
  printf("Valid access codes are in range 0 - 9999\n");
  return 1;
 }
 printf("TOCA Race Driver 2 Cheats\n");
 printf("Platform: %s\n", platform);
 printf("Access code: %i\n", accesscode);
 int n = !strcmp(platform, "PSP") ? 7 : 6;
 for(int i = 0; i < n; i++) printf("%s: %s\n", cheats[i], gencode(platform, accesscode, i));
 return 0;
}
