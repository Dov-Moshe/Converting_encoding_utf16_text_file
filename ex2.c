#include <stdio.h>
#include <string.h>

void firstCase(int argc, char const *argv[], FILE* file, FILE* newFile);
void secondAndThirdCase(int argc, char const* argv[], FILE* file, FILE* newFile);
void carriageReturnCase(int argc, char const* argv[], FILE** file, FILE** newFile, int isBig, char* arr);
void lineFeedCase(int argc, char const* argv[], FILE** file, FILE** newFile, int isBig, char* arr);

/**
*
*
**/
int main(int argc, char const *argv[]) {
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
            printf("here1\n");
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
void firstCase(int argc, char const *argv[], FILE* file, FILE* newFile) {
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
void secondAndThirdCase(int argc, char const* argv[], FILE* file, FILE* newFile) {
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
    // if it is needed to swap the endianness
    if(argc >= 6 && strcmp(argv[5], "-swap") == 0) {
        u[0] = u[0] ^ u[1];
        u[1] = u[0] ^ u[1];
        u[0] = u[0] ^ u[1];
    }
    // write the bytes into the new file
    fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);


    int i = 1;

    // read the file two byte by two byte
    while (fread(&arr, sizeof(arr), 1, file)) {

        if(isBigEndian == 0 && u[0] == 0x0d && u[1] == 0x00) {
            printf("num: %d\n", i);
            i++;
        }

        //printf("%02hhX %02hhX\n", u[1], u[0]);

        // if the source and dest are equal system then there is no point to check '\r' or '\n'
        if(strcmp(argv[3], argv[4]) !=0) {
            // Because there is no distinction here between the both big and litte,
            // so the distinction will be inside of the function
            if((isBigEndian == 1 && u[0] == 0x00 && u[1] == 0x0d) ||
            (isBigEndian == 0 && u[0] == 0x0d && u[1] == 0x00)) {
                // handle with case of '\r'
                carriageReturnCase(argc, argv, &file, &newFile, isBigEndian, arr);
            } else if((isBigEndian == 1 && u[0] == 0x00 && u[1] == 0x0a) ||
            (isBigEndian == 0 && u[0] == 0x0a && u[1] == 0x00)) {
                printf("here2\n");
                // handle with case of '\n'
                lineFeedCase(argc, argv, &file, &newFile, isBigEndian, arr);
            }
        }
        // if it is needed to swap the endianness
        if(argc >= 6 && strcmp(argv[5], "-swap") == 0) {
            u[0] = u[0] ^ u[1];
            u[1] = u[0] ^ u[1];
            u[0] = u[0] ^ u[1];
        }
        printf("%s", arr);
        // write the bytes into the new file
        fwrite(&arr , sizeof(arr[0]) , sizeof(arr)/sizeof(arr[0]), newFile);
    }
}


void carriageReturnCase(int argc, char const* argv[], FILE** file, FILE** newFile, int isBig, char* arr) {
    printf("here3\n");
    unsigned char* u = (char*)&arr;
    if (strcmp(argv[3], "-win") == 0) {
        printf("here4\n");
        if (strcmp(argv[4], "-unix") == 0) {
            // then jump to next 2 byte
            fread(&arr , 2, 1, *file);
        }
    } else if (strcmp(argv[3], "-mac") == 0) {
        if (strcmp(argv[4], "-unix") == 0) {
            // if the destination is unix then change the '\r' to '\n'
            if (isBig) {
                u[0] = 0x00; u[1] = 0x0a;
            } else if(!isBig) {
                u[0] = 0x0a; u[1] = 0x00;
            }
        } else if (strcmp(argv[4], "-win") == 0) {
            // if the dest is win then write the '\r' to the file,
            // and then change the '\r' to '\n'
            // And before this' checking how to write the bytes
            if(argc >= 6 && strcmp(argv[5], "-swap") == 0) {
                u[0] = u[0] ^ u[1];
                u[1] = u[0] ^ u[1];
                u[0] = u[0] ^ u[1];
            }
            fwrite(&arr , sizeof(arr[0]) , sizeof(*arr)/sizeof(arr[0]), *newFile);
            // after writing '\r', changing the value in the array to '\n',
            // because it's needed to be added to the new file
            if (isBig) {
                u[0] = 0x00; u[1] = 0x0a;
            } else if(!isBig) {
                u[0] = 0x0a; u[1] = 0x00;
            }
        }
    }
}

void lineFeedCase(int argc, char const* argv[], FILE** file, FILE** newFile, int isBig, char* arr) {
    unsigned char* u = (char*)&arr;
    char tempArr[2];
    if (strcmp(argv[3], "-win") == 0) {
        if (strcmp(argv[4], "-mac") == 0) {
            // then jump to next 2 byte, because there is on '\n' in mac
            printf("here5\n");
            fread(&tempArr, 2, 1, *file);
            //printf("%x %x\n",u[0], u[1]);
        }
        arr[0] = tempArr[0];
        arr[1] = tempArr[1];
        //printf("%s\n", arr);
    } else if(strcmp(argv[3], "-unix") == 0) {
        if(strcmp(argv[4], "-mac") == 0) {
            // if the destination is mac then change the '\n' to '\r'
            if (isBig == 1) {
                arr[0] = 0x00; arr[1] = 0x0d;
            } else if(isBig == 0) {
                arr[0] = 0x0d; arr[1] = 0x00;
            }
        } else if(strcmp(argv[4], "-win") == 0) {
            char temp[2];
            // if the dest is win then write the '\r' before '\n' to the file,
            // so writing '\r' to the file and continue
            if (isBig == 1 && argc >= 6 && strcmp(argv[5], "-swap") == 0) {
                temp[0] = 0x0d;
                temp[1] = 0x00;
                fwrite(&temp , sizeof(temp[0]) , sizeof(temp)/sizeof(temp[0]), *newFile);

            } else if (isBig == 0 && argc >= 6 && strcmp(argv[5], "-swap") == 0) {
                temp[0] = 0x00;
                temp[1] = 0x0d;
                fwrite(&temp , sizeof(temp[0]) , sizeof(temp)/sizeof(temp[0]), *newFile);

            } else if (isBig == 1 && argc >= 6 && strcmp(argv[5], "-swap") != 0) {
                printf("here6\n");

                temp[0] = 0x00;
                temp[1] = 0x0d;
                fwrite(&temp , sizeof(temp[0]) , sizeof(temp)/sizeof(temp[0]), *newFile);

            } else if (isBig == 0 && argc >= 6 && strcmp(argv[5], "-swap") != 0) {
                temp[0] = 0x0d;
                temp[1] = 0x00;
                fwrite(&temp , sizeof(temp[0]) , sizeof(temp)/sizeof(temp[0]), *newFile);
            }
        }
    }

}
