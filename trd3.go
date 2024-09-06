package main
import "math/big"
import "encoding/binary"
import "fmt"
import "os"
import "strings"
import "strconv"

var cheats = [12]string{"All Championships", "All Bonus Championships", "Boost For All Cars", "Turbo Boost", "Toy Cars", "Slot Racer",
                        "Invincible Cars", "All Cutscenes", "Unused", "Honda 2006", "Honda", "No Streamed Car Sound"}

var pspcheats = [4]string{"All Championships", "All Bonus Championships", "All Cutscenes", "Invincible cars"}

var platforms = [4]string{"PC", "PS2", "PSP", "XBOX"}

func isplatform(argv string) bool {
 for i := 0; i < 4; i++ {
  if argv == platforms[i] {
   return true
  }
 }
 return false
}

func calcseed(input uint32, mult uint32, arr [4]uint32) uint32 {
 var seed uint32 = 0x01
 if input != 0 {
  seed = arr[1]
  var i uint32 = 0
  for i < input - 1 {
   seed = ((arr[2] + seed * arr[1] * mult) & 0xFFFFFFFF) % arr[3]
   i++
  }
 }
 return seed
}

func gencodepsp(accesscode uint32, cheatid uint32) string {
 arr := [4]uint32{31873457, 211, 453889, 456959}
 var seed1 uint32 = calcseed((cheatid % 0x64) ^ (accesscode % 0x64) ^ (arr[0] % 0x64), (arr[0] / 0x2710) % 0x64, arr)
 var seed2 uint32 = calcseed((cheatid % 0x64) ^ ((arr[0] / 0x64) % 0x64) ^ ((accesscode / 0x64) % 0x64), (arr[0] / 0xF4240) % 0x64, arr)
 code := make([]byte, 8)
 code[0] = byte(0x41 + (seed1 % 0x1A))
 code[1] = byte(0x41 + ((seed1 / 0x1A) % 0x1A))
 code[2] = byte(0x41 + ((seed1 / 0x2A4) % 0x1A))
 code[3] = byte(0x41 + ((seed1 / 0x44A8) % 0x1A))
 code[4] = byte(0x41 + (seed2 % 0x1A))
 code[5] = byte(0x41 + ((seed2 / 0x1A) % 0x1A))
 code[6] = byte(0x41 + ((seed2 / 0x2A4) % 0x1A))
 code[7] = byte(0x41 + ((seed2 / 0x44A8) % 0x1A))
 return string(code)
}

func gencode(platform string, accesscode uint32, cheatid uint32) string {
 if platform == "PSP" {
  return gencodepsp(accesscode, cheatid + 9)
 }
 var arr [4]big.Int
 if platform == "PC" {
  n0, _ := new(big.Int).SetString("21203499539617337777", 10)
  n1, _ := new(big.Int).SetString("26952959852310653999", 10)
  n2, _ := new(big.Int).SetString("20342932087051438433", 10)
  n3, _ := new(big.Int).SetString("23791686600665980187", 10)
  arr = [4]big.Int{*n0, *n1, *n2, *n3}
 } else if platform == "PS2" {
  n0, _ := new(big.Int).SetString("2076591965032980673", 10)
  n1, _ := new(big.Int).SetString("19114270758136415677", 10)
  n2, _ := new(big.Int).SetString("6121936729286270513", 10)
  n3, _ := new(big.Int).SetString("21291305872647903137", 10)
  arr = [4]big.Int{*n0, *n1, *n2, *n3}
 } else if platform == "XBOX" {
  n0, _ := new(big.Int).SetString("120062904899007353", 10)
  n1, _ := new(big.Int).SetString("20175801555923186401", 10)
  n2, _ := new(big.Int).SetString("7674794942240248673", 10)
  n3, _ := new(big.Int).SetString("30355041441880756343", 10)
  arr = [4]big.Int{*n0, *n1, *n2, *n3}
 }
 xor, _ := new(big.Int).SetString("14703658657559957748", 10)
 var salt, exp, mod, packed, encrypted, b big.Int
 if cheatid == 9 || cheatid == 10 {
  salt.And(&arr[3], big.NewInt(0xFFFF))
  exp = arr[2]
  mod = arr[3]
 } else {
  salt.And(&arr[1], big.NewInt(0xFFFF))
  exp = arr[0]
  mod = arr[1]
 }
 data := make([]byte, 8)
 buff := make([]byte, 2)
 data[0] = 0
 data[1] = byte(cheatid)
 binary.LittleEndian.PutUint16(buff, uint16(accesscode))
 data[2] = buff[0]
 data[3] = buff[1]
 binary.LittleEndian.PutUint16(buff, uint16(salt.Int64()))
 data[4] = buff[0]
 data[5] = buff[1]
 data[6] = 0
 data[7] = 0
 packed.Xor(big.NewInt(int64(binary.LittleEndian.Uint64(data))), xor)
 encrypted.Exp(&packed, &exp, &mod)
 code := make([]byte, 13)
 i := 0
 for encrypted.Cmp(big.NewInt(0)) > 0 {
  b.And(&encrypted, big.NewInt(0x1F))
  ch := byte(b.Int64()) + '0'
  if ch >= ':' {
   ch = byte(b.Int64()) + '7'
  }
  if ch >= 'I' {
   ch += 1
  }
  if ch >= 'O' {
   ch += 1
  }
  if ch >= 'S' {
   ch += 1
  }
  if ch >= 'Z' {
   ch += 1
  }
  code[i] = ch
  i++
  encrypted.Rsh(&encrypted, 5)
 }
 return string(code)
}

func main() {
 argv := os.Args
 if len(argv) != 3 {
  fmt.Println("Invalid command")
  return
 }
 platform := ""
 var accesscode uint32 = 0
 for i := 1; i < 3; i++ {
  argv[i] = strings.ToUpper(argv[i])
  if isplatform(argv[i]) {
   platform = argv[i]
  } else {
   n, err := strconv.ParseUint(argv[i], 10, 32)
   if err == nil {
    accesscode = uint32(n)
   }
  }
 }
 if platform == "" {
  fmt.Println("Valid platforms are PC, PS2, PSP, XBOX")
  return
 } else if accesscode < 0 || accesscode > 9999 {
  fmt.Println("Valid access codes are in range 0 - 9999")
  return
 }
 fmt.Println("TOCA Race Driver 3 Cheats")
 fmt.Printf("Platform: %s\n", platform)
 fmt.Printf("Access code: %d\n", accesscode)
 if platform != "PSP" {
  for i := 0; i < 12; i++ {
   fmt.Printf("%s: %s\n", cheats[i], gencode(platform, accesscode, uint32(i)))
  }
 } else {
  for i := 0; i < 4; i++ {
   fmt.Printf("%s: %s\n", pspcheats[i], gencode(platform, accesscode, uint32(i)))
  }
 }
}
