#include <stdio.h>
#include <string.h>

void firstCase(int argc, char const *argv[]);
void secondAndThirdCase(int argc, char const *argv[]);

int main(int argc, char const *argv[]) {

  printf("%d\n", argc);

  if(argc == 3) {
    firstCase(argc, argv);
  } else if (argc == 5 || argc == 6) {
    secondAndThirdCase(argc, argv);

    // handle with case of '\n'




      //unsigned char* u = (char*)&arr;
      //printf("%x %x\n", u[1],u[0]);





      if(strcmp(argv[3], "-win")) {

      } else if (strcmp(argv[3], "-unix")) {

      } else if (strcmp(argv[3], "-mac")) {

      }

      int a = fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
    if(argc == 6) {

    }

    fclose(file);
    fclose(newFile);
  }
  }

void firstCase(int argc, char const *argv[]) {
  FILE *file;
  file = fopen(argv[1], "rb");
  char arr[2];
  FILE *newFile;
  newFile = fopen(argv[2], "w");
  while (fread(&arr, sizeof(arr), 1, file)) {
    unsigned char* u = (char*)&arr;
    printf("%x\n", u[0]);
    printf("%x\n", u[1]);
    int a = fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
  }
  fclose(file);
  fclose(newFile);
}


void secondAndThirdCase(int argc, char const *argv[]) {
  // open the read file
  FILE *file;
  file = fopen(argv[1], "rb");
  // to get two byte by two byte from the file
  char arr[2];
  // open the write file
  FILE *newFile;
  newFile = fopen(argv[2], "w");
  // get the first two byte to know if it is big or little endian
  fread(&arr, sizeof(arr), 1, file);
  unsigned char* u = (char*)&arr;
  int isBigEndian = 1;
  if(u[0] == 0xff && u[1] == 0xfe) {
    isBigEndian = 0;
  }

  // read the file two byte by two byte
  while (fread(&arr, sizeof(arr), 1, file)) {
    unsigned char* u = (char*)&arr;
    
    // handle with case of '\r'
    if((isBigEndian && u[0] == 0x00 && u[1] == 0x0d) ||
    (!isBigEndian && u[0] == 0x0d && u[1] == 0x00)) {
      if (strcmp(argv[3], "-win")) {
        if (strcmp(argv[4], "-unix"))
          break;
    } else if (strcmp(argv[3], "-mac")) {
      if (strcmp(argv[4], "-unix")) {
        // if the destitation is unix then change the '\r' to '\n'
        if (isBigEndian) {
          u[0] = 0x00; u[1] = 0x0a;
        } else if(!isBigEndian) {
          u[0] = 0x0a; u[1] = 0x00;
        }
      } else if (strcmp(argv[4], "-win")) {
        // if the destitation is win then write the '\r' to the file
        // and then change the '\r' to '\n'
        fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
        if (isBigEndian) {
          u[0] = 0x00; u[1] = 0x0a;
        } else if(!isBigEndian) {
          u[0] = 0x0a; u[1] = 0x00;
        }
      }
    }
  }
}







  /* code */
  return 0;
}
