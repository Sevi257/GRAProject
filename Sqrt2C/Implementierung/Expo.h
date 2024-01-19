//
// Created by falkb on 18.06.2023.
//

#ifndef TEAM105_EXPO_H
#define TEAM105_EXPO_H
#include "bignum.h"

int countBits(int number);
int karazuba(int bits, int x, int y);
void matrixmultiply(bignum ** a, bignum ** b);
void  expo(size_t exp, size_t gen, int print);
void testFunction(char* string);
#endif //TEAM105_EXPO_H