
// Created by Fabian Strobl on 22.06.23.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "intArr.h"
#include "Expo.h"



// initializes a new bignum with the given length
void initInt(intArr * num, size_t length) {

    // allocates the space needed and sets the length
    num->number = malloc(length * sizeof(long long));
    if( num->number == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    num->length = length;

}

// sets the numbers array of the given bignum to the value array and sets the length accordingly
void setInt(intArr* num, const int * value, size_t length) {

    // allocates new space if needed and sets the length accordingly
    if (length > num->length) {
        free(num->number);
        num->number = malloc(length * sizeof(long long));
        if(num->number==NULL){printf("Not able");
            exit(1);}
    }
    num->length = length;

    // copies the values of the value array in the numbers array of the bignum
    for (size_t i = 0; i < length; i++) {
        num->number[i] = value[i];

    }
}

// sets a value array as the numbers array of a bignum
void setShort(intArr* num, int * value, size_t length){

    if(num->number != NULL)
    {
        free(num->number);
    }
    num->number = value;
    num->length = length;
};

// prints a bignum
void printInt(intArr* num) {

    // prints the most significant array without leading zeros
    printf("%d",num->number[num->length - 1]);

    // prints the other arrays, but also prints zeros if necessary
    for (size_t i = 1; i < num->length; ++i) {
        int x = num->number[num->length - i -1 ];
        if(x < 10){
            printf("0");
        }if(x < 100){
            printf("0");
        }if(x < 1000){
            printf("0");
        }
        printf("%d",x);
    }

}
// 1 000 000 000
//        10 000
// removes leading zeros

void removeLeadingZerosInt(intArr *num) {

    long long i;

    // iterates over the array until the current array != 0
    // --> i = first index with array[ index ] != 0
    for (i = num->length; i > 0;) {
        if (num->number[--i] != 0)
            break;
    }

    // return if there are no leading zeros
    if(i == num->length -1)
        return;

    // allokates new space for the array with size = amount of arrays without leading zeros
    int *arr = malloc((i+1) * sizeof(long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // copies the values without the leading zeros int arr and sets the bignum accordingly
    // TODO - evtl geht das auch ohne erstellen eines neuen Arrays oder mit SIMD
    int x = 0;
    for (; i >= 0; i--) {
        arr[i] = num->number[i];
        x++;
    }
    setShort(num, arr, x);
    //free(arr);
}

// adds two bignums and safes the result in the first one, the second one stays unchanged
void addInt(intArr *num1, intArr *num2){

    if(num1->length >= num2->length){

        // creats an array with length = length of num1 + 1 --> maximum length of the result
        int * arr = malloc((num1->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }

        int carry = 0;
        int i = 0;

        // adds the two bignums array-wise with a carry and writes the resukt mod 1 billion into the result array
        // carry = 1 if result of the prvious addition was >= 1 billion else 0
        while (i < num2->length){
            int x = num1->number[i] + num2->number[i] + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }

        // if the length of the first numbers is greater then the length of the second numbers the remaining arrays of the first numbers are added to the result
        while (i < num1->length){
            int x = num1->number[i]  + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }

        arr[i] = carry;

        // if the carry = 1 then the length need to be increased, otherwise the length of the longer numbers is enough
        setShort(num1, arr, num1->length + carry);
        //   free(arr);

    }else {
        // works the same as the other method, but num2 ist uses as num1 was used before

        int * arr = malloc((num2->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
        int carry = 0;
        size_t i = 0;
        while (i < num1->length){
            int x = num1->number[i] + num2->number[i] + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }
        while (i < num2->length){
            int x = num2->number[i]  + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }
        arr[i] = carry;
        setShort(num1, arr, num2->length + carry);
        //   free(arr);
    }
}

// multiplies two bignums and safes the result in the first one, the second one stays unchanged
void mulInt(intArr * num1, intArr *num2){

    // creates an array with the right length and sets all Elements to zero
    size_t length = num1->length + num2->length +1;
    int * arr = malloc((length) * sizeof (long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    long long i, j;
    for(i = 0; i < length;i++){
        arr[i] = 0;
    }

    // multiplies the numbers array-wise
    // writes the result mod 1 billion in the reult array add the result / 1 billion into to the next array
    //TODO - SIMD MÖGlICH
    for(i = 0; i < num1->length;i++){
        for(j = 0; j < num2->length;j++){
            int x = num1->number[i] * num2->number[j];
            arr[i +j +1] += x / 10000;
            arr[i + j] += x % 10000;
        }
    }

    // writes the result int num1 and removes leading zeros
    setShort(num1, arr, length);
    removeLeadingZerosInt(num1);
    //  free(arr);

}

void subInt3(intArr * num1, intArr *num2){

    // creates an array with size = the length of num1 and fills it with zeros


    // subtracts array wise
    long long i = 0;
    while ( i < num2->length / 6) {
        num1->number[i] = num1->number[i] - (num2->number[i]);
        if (num1->number[i] < 0) {
            num1->number[i] += 10000;
            num1->number[i+1] -= 1;
        }
        num1->number[i+1] = num1->number[i+1] - (num2->number[i+1]);
        if (num1->number[i+1] < 0) {
            num1->number[i+1] += 10000;
            num1->number[i+2] -= 1;
        }
        num1->number[i+2] = num1->number[i+2] - (num2->number[i+2]);
        if (num1->number[i+2] < 0) {
            num1->number[i+2] += 10000;
            num1->number[i+3] -= 1;
        }
        num1->number[i+3] = num1->number[i+3] - (num2->number[i+3]);
        if (num1->number[i+3] < 0) {
            num1->number[i+3] += 10000;
            num1->number[i+4] -= 1;
        }
        num1->number[i+4] = num1->number[i+4] - (num2->number[i+4]);
        if (num1->number[i+4] < 0) {
            num1->number[i+4] += 10000;
            num1->number[i+5] -= 1;
        }
        num1->number[i+5] = num1->number[i+5] - (num2->number[i+5]);
        if (num1->number[i+5] < 0) {
            num1->number[i+5] += 10000;
            num1->number[i+6] -= 1;
        }
        i += 6;
    }

    while ( i < num2->length) {
        num1->number[i] = num1->number[i] - (num2->number[i]);
        if (num1->number[i] < 0) {
            num1->number[i] += 10000;
            num1->number[i+1] -= 1;
        }
        i++;
    }

    // Das Ergebnis wird an die Adresse von num1 geschrieben und es werden führende Nullen entfernt
    removeLeadingZerosInt(num1);

}



int cmpInt(intArr* num1, intArr* num2){
/*    printf("CMP Anfang: \n");
    printLong(num1);
    printf(" - ");
    printInt(num2);
    printf("\n Länge 1 = %d; Länge 2 = %d\n",num1->length,num2->length);
    */
    // in our implementation before evey call of cmpInt the leading zeros are removed, so it is not mandatory to do it here again

    // if one numbers is longer than the other, it is longer
    if(num1->length > num2->length)
        return 1;
    if(num2->length > num1->length)
        return -1;
    //  printf("keine ist Länger \n");
    // compares all arays starting with the most significant one
    for(long long i = num1->length-1; i >=0;i--){
        //      printf("I = %d\n",i);
        if(num1->number[i] > num2->number[i]){
            return 1;
        }
        if(num2->number[i] > num1->number[i]) {
            return -1;
        }
    }
    //  printf("akfkjaklasjdasjdaskdjasdas");
    // if num1 is neither larger nor smaller than num2, the numbers have to be equal
    return 0;
}

char* divLongInt(intArr * num1, intArr *num2, long long pre){
    pre += 1;
    // creates a new bignum to shift the comma
    intArr ten;
    initInt(&ten, 1);
    ten.number[0] = 10;
    char * srt = malloc( pre * sizeof (char ));
    if( srt == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // TODO --> Runden
    long long x = -1;
    long long i;
    for( i = 0; i < pre; i++){

        int counter = 0;

        // Prüft wieoft num2 in num1 passt und zieht num2 sooft von num1 ab
        while (cmpInt(num1, num2) > 0){


            subInt3(num1, num2);
            counter++;

            // 3 -->  41 - 42 sek

        }
        /*       printf("\n");
               printLong(num1);
               printf(" - ");
               printInt(num2);
               printf("\n");
               */
        // Wenn die Zahlen gleich sind, wird abgebrochen
        if(cmpInt(num1, num2) == 0){
            //   printf("%d",counter);
            srt[x++] = (counter+1) + '0';
            break;
        }

        // Fügt ein, wie oft num2 in num1 gepasst hat
        if(x < 0){
            x++;
        }else {
            srt[x++] = counter + '0';
        }
        // Verschiebt das imaginäre Komma bei num1 um 1 nach rechts
        // -> wie bei num2 das Komma um 1 nachlinks verschieben
        mulInt(num1, &ten);

        // TODO --> Print entfernen
        /* if(i % 10000 == 0)
             printf("%lld \n",i); */
    }
    srt[x] = '\0';
    return srt;
}
#include <immintrin.h>


void exp1Int(size_t e, size_t g, int print){
    intArr matrix1,matrix2,matrix4,tmp1,tmp2,tmp3,tmp4;
    initInt(&matrix1, 1);
    initInt(&matrix2, 1);
    initInt(&matrix4, 1);
    initInt(&tmp1, 1);
    initInt(&tmp2, 1);
    initInt(&tmp3, 1);
    initInt(&tmp4, 1);

    matrix1.number[0] = 0;
    matrix2.number[0] = 1;
    matrix4.number[0] = 2;
    tmp1.number[0] = 0;
    tmp2.number[0] = 0;
    tmp3.number[0] = 0;
    tmp4.number[0] = 0;

    for(int i = 0; i < e; i++){

        setInt(&tmp1, matrix1.number, matrix1.length);
        mulInt(&tmp1, &matrix1);
        setInt(&tmp2, matrix2.number, matrix2.length);
        mulInt(&tmp2, &matrix2);
        addInt(&tmp1, &tmp2);

        setInt(&tmp2, matrix2.number, matrix2.length);
        mulInt(&tmp2, &matrix1);
        setInt(&tmp3, matrix2.number, matrix2.length);
        mulInt(&tmp3, &matrix4);
        addInt(&tmp2, &tmp3);

        setInt(&tmp4, tmp2.number, tmp2.length);
        addInt(&tmp4, &tmp2);
        addInt(&tmp4, &tmp1);

        setInt(&matrix1, tmp1.number, tmp1.length);
        setInt(&matrix2, tmp2.number, tmp2.length);
        setInt(&matrix4, tmp4.number, tmp4.length);


    }
    if(print == 1)
        printf("%s",divLongInt(&matrix2,&matrix4,(long long) g));
    else if(print == 2) {
        printf("%s", intToHex(divLongInt(&matrix2,&matrix4,(long long) g)));
    }
    else if(print == 3){
        testFunction(divLongInt(&matrix2,&matrix4,(long long) g));
    }
}

/*
int main(){
    exp1Int(7, 10);
    return 0;
}
*/