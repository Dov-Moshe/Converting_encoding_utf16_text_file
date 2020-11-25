#include <stdio.h>
#include <string.h>

void firstCase(int argc, char const *argv[], FILE *file, FILE *newFile);
void secondAndThirdCase(int argc, char const *argv[], FILE *file, FILE *newFile);
void caariageReturnCase(char const *argv[], FILE *file, FILE *newFile, int isBig, char* arr);
void lineFeedCase(char const *argv[], FILE *file, FILE *newFile, int isBig, char* arr);

/**
*
*
**/
int main(int argc, char const *argv[]) {
    printf("%d\n", argc);
    // open the read file
    FILE *file;
    file = fopen(argv[1], "rb");
    // open the write file
    FILE *newFile;
    newFile = fopen(argv[2], "w");
    // if 'argc' < 3 then there is one argument
    if(argc >= 3) {
        if(argc == 3) {
            // this is the simple case
            firstCase(argc, argv, file, newFile);
        } else if (argc == 5 || argc == 6) {
            // checking both the second and third cases
            secondAndThirdCase(argc, argv, file, newFile);
        }
    }
    fclose(file);
    fclose(newFile);
    return 0;
}

/**
*
*
**/
void firstCase(int argc, char const *argv[], FILE *file, FILE *newFile) {
    char arr[2];
    while (fread(&arr, sizeof(arr), 1, file)) {
        unsigned char* u = (char*)&arr;
        printf("%x\n", u[0]);
        printf("%x\n", u[1]);
        fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
    }
}



/**
*
*
**/
void secondAndThirdCase(int argc, char const *argv[], FILE *file, FILE *newFile) {
    // to get two byte by two byte from the file
    char arr[2];
    // get the first two byte to know if it is big or little endian
    // the needed is to recognize '\n' and '\r' when looking on the bytes
    fread(&arr, sizeof(arr), 1, file);
    unsigned char* u = (char*)&arr;
    int isBigEndian = 1;
    if(u[0] == 0xff && u[1] == 0xfe) {
        isBigEndian = 0;
    }
    // read the file two byte by two byte
    while (fread(&arr, sizeof(arr), 1, file)) {
        // if the source and dest are equal system then there is no point to check '\r' or '\n'
        if(!strcmp(argv[3], argv[4])) {
            // Because there is no distinction here between the both big and litte,
            // so the distinction wiil be inside of the function
            if((isBigEndian && u[0] == 0x00 && u[1] == 0x0d) ||
            (!isBigEndian && u[0] == 0x0d && u[1] == 0x00)) {
                // handle with case of '\r'
                caariageReturnCase(argv, file, newFile, isBigEndian, arr);
            } else if((isBigEndian && u[0] == 0x00 && u[1] == 0x0a) ||
            (!isBigEndian && u[0] == 0x0a && u[1] == 0x00)) {
                // handle with case of '\n'
                lineFeedCase(argv, file, newFile, isBigEndian, arr);
            }
        }
        // if it is needed to swap the endianness
        if(argc >= 5 && strcmp(argv[5], "–swap")) {
            u[0] = u[0] ^ u[1];
            u[1] = u[0] ^ u[1];
            u[0] = u[0] ^ u[1];
        }
        // write the bytes into the new file
        fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
    }
}


void caariageReturnCase(char const *argv[], FILE *file, FILE *newFile, int isBig, char* arr) {
    unsigned char* u = (char*)&arr;
    if (strcmp(argv[3], "-win")) {
        if (strcmp(argv[4], "-unix")) {
            // then jamp to next 2 byte
            fread(&arr, sizeof(arr), 1, file);
        }
    } else if (strcmp(argv[3], "-mac")) {
        if (strcmp(argv[4], "-unix")) {
            // if the destitation is unix then change the '\r' to '\n'
            if (isBig) {
                u[0] = 0x00; u[1] = 0x0a;
            } else if(!isBig) {
                u[0] = 0x0a; u[1] = 0x00;
            }
        } else if (strcmp(argv[4], "-win")) {
            // if the dest is win then write the '\r' to the file,
            // and then change the '\r' to '\n'
            // And before this' checking how to write the bytes
            if(argc >= 5 && strcmp(argv[5], "–swap")) {
                u[0] = u[0] ^ u[1];
                u[1] = u[0] ^ u[1];
                u[0] = u[0] ^ u[1];
            }
            fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
            // after writing '\r', changing the value in the array to '\n',
            // becaue it's needed to be added to the new file
            if (isBig) {
                u[0] = 0x00; u[1] = 0x0a;
            } else if(!isBig) {
                u[0] = 0x0a; u[1] = 0x00;
            }
        }
    }
}

void lineFeedCase(char const *argv[], FILE *file, FILE *newFile, int isBig, char* arr) {
    unsigned char* u = (char*)&arr;
    if (strcmp(argv[3], "-win")) {
        if (strcmp(argv[4], "-mac")) {
            // then jamp to next 2 byte, becaue there is on '\n' in mac
            fread(&arr, sizeof(arr), 1, file);
        } else if (argv[3], "-unix") {

        }
    }

}
