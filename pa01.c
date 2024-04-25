/*============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Jilben Brito
| Language: c
|
| To Compile: javac pa01.java
| gcc -o pa01 pa01.c
| g++ -o pa01 pa01.cpp
| go build pa01.go
|
| To Execute: java -> java pa01 kX.txt pX.txt
| or c++ -> ./pa01 kX.txt pX.txt
| or c -> ./pa01 kX.txt pX.txt
| or go -> ./pa01 kX.txt pX.txt
| or python -> python3 pa01.py kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
|
| Class: CIS3360 - Security in Computing - Fall 2023
| Instructor: McAlpin
| Due Date: 2/25/24
+===========================================================================*/

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Defines
#define MAXSIZE 10000

//Function Prototypes
void fillKeyArr(FILE *key, int kLen, int kArr[][kLen]);
void printKeyArr(int kLen, int kArr[][kLen]);
void fillPlainText(FILE* pt, char ptArr[]);
void printArr(char arr[], int idx);
int idxPtArr(char ptArr[]);
void padPtArr(char ptArr[], int idx, int kLen);
int charToInt(char ch);
char intToChar(int val);
void ctEncrypt(int idx, char ptArr[], char ctArr[], int kLen, int kArr[][kLen]);
void matrixMultiply(int kLen, int kArr[][kLen], char subArr[], char tempArr[]);
void fillCtArr(char ctArr[], int idx, char tempArr[], int kLen);

int main(int argc, char **argv){

    FILE *key, *pt;
    char ptArr[MAXSIZE];
    int kLen;

    key = fopen(argv[1], "r");

    pt = fopen(argv[2], "r");

    fscanf(key, "%d ", &kLen);

    int kArr[kLen][kLen];

    fillKeyArr(key, kLen, kArr);

    printKeyArr(kLen, kArr);

    fillPlainText(pt, ptArr);

    int idx = idxPtArr(ptArr);

    if(idx%kLen != 0){
        padPtArr(ptArr, idx, kLen);
        idx = idxPtArr(ptArr);
    }
    printf("\nPlaintext:\n");
    printArr(ptArr, idx);
    printf("\n");

    char ctArr[idx];

    ctEncrypt(idx, ptArr, ctArr, kLen, kArr);

    printf("\nCiphertext:\n");
    printArr(ctArr, idx);

    fclose(key);
    fclose(pt);

}

void fillKeyArr(FILE *key, int kLen, int kArr[][kLen]){

    for(int i = 0; i < kLen; i++){
        for(int j = 0; j < kLen; j++){
            fscanf(key, " %d", &kArr[i][j]);
        }
    }
}

void printKeyArr(int kLen, int kArr[][kLen]){

    printf("\nKey matrix:\n");
    for(int i = 0; i < kLen; i++){
        for(int j = 0; j < kLen; j++){
            printf("\t%d ", kArr[i][j]);
        }
        printf("\n");
    }
}

void fillPlainText(FILE* pt, char ptArr[]){

    int ptIdx = 0;
    int ch;

    while((ch = fgetc(pt)) != EOF){ 
        if(isalpha(ch)){
            ptArr[ptIdx] = tolower(ch);
            ptIdx++;
        }
    }

}

void printArr(char arr[], int idx){

    for(int i = 0; i < idx; i++){
        if(i%80 != 0 || i == 0){
            if(isalpha(arr[i])){
                printf("%c", arr[i]);
            }
            else{
                break;
            }
        }
        else{
            printf("\n");
            if(isalpha(arr[i]))
                printf("%c", arr[i]);
        }
    }

}

int idxPtArr(char ptArr[]){

        int idx = 0;

        for(int i = 0; i < MAXSIZE; i++){
        if(isalpha(ptArr[i])){
            idx++;
        }
        else{
            break;
        }
    }
    return idx;
}

void padPtArr(char ptArr[], int idx, int kLen){

        int pad = kLen - (idx%kLen);

        for(int i = 0; i < pad; i++){
            ptArr[idx + i] = 'x';
        }
}

int charToInt(char ch){
    return ch - 'a';
}

char intToChar(int val){
    return val + 'a';
}

void ctEncrypt(int idx, char ptArr[], char ctArr[], int kLen, int kArr[][kLen]){

    char subArr[kLen];
    char tempArr[kLen];
    int fill = 0;
    int tempIdx = 0;

        for(int i = 0; i < idx/kLen; i++){
            for(int j = 0; j < kLen; j++){
                subArr[j] = ptArr[fill];
                fill++;
            }
            matrixMultiply(kLen, kArr, subArr, tempArr);
            fillCtArr(ctArr, tempIdx, tempArr, kLen);
            tempIdx += kLen;
        }

}

void matrixMultiply(int kLen, int kArr[][kLen], char subArr[], char tempArr[]){

        for(int i = 0; i < kLen; i++){
            int ch = 0;
            for(int j = 0; j < kLen; j++){
                ch += (kArr[i][j]%26 * charToInt(subArr[j])%26);
            }
            tempArr[i] = intToChar(ch%26);
        }

}

void fillCtArr(char ctArr[], int idx, char tempArr[], int kLen){

    int x = idx;
    for(int i = 0; i < kLen; i++){
        ctArr[x] = tempArr[i];
        x++;
    }

}

/*=============================================================================
| I Jilben Brito(5512370) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/