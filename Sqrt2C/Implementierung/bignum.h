//
// Created by Fabian Strobl on 27.06.23.
//

#ifndef UNTITLED1_NEWLONG_H
#define UNTITLED1_NEWLONG_H
#include "stdlib.h"

typedef struct {
    long long * numbers;
    size_t length;
} bignum;

void initLong(bignum* num, size_t length);
void setLong(bignum* num, const long long * value, size_t length);
void printLong(bignum* num);
void removeLeadingZeros(bignum *num);
void addLong(bignum *num1, bignum *num2);
void mulLong(bignum * num1, bignum *num2);
int cmp(bignum* num1,bignum* num2);
void subLong(bignum * num1, bignum *num2);
void mul10Long(bignum *num);
char* divLong(bignum * num1, bignum *num2, long long );
void freeLong(bignum* num);
int compareSTRLong(char *a,char *b);
void exp1(size_t e, size_t g, int print);
char * intToHex(char * intStr);
#endif //UNTITLED1_NEWLONG_H

// Konvention zum speichern der Zahlen
// Die Zahl 5 123456789 987654321 wird in 3 Arrays gespeichert
// [2] = 5; [1] = 123456789 ; [0] = 987654321
// -->  [5] [123456789] [987654321]
// Länge = die Anzahl der Arrays
// --> Zahlen zischen 0 und 100.000.000 haben lenght = 1
// -> wirkliche Länge = lengt-1 * 8 + Zifferanzahl in num[0]