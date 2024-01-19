#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include<time.h>
#include "LongKomma.h"



void initLongKomma(newLongKomma* num, size_t length) {
    num->number = malloc(length * sizeof(long long));
    num->length = length;
    num->komma = length;
}

void setLongKomma(newLongKomma* num, const long long * value, long long length, long long komma) {
    if (length > num->length) {
        printf("\n In setLongnum: ");
        printLongKomma(num);
        num->number = realloc(num->number, length * sizeof(long long));
    }
    num->length = length;
    num->komma = komma;
    for (size_t i = 0; i < length; i++) {
        num->number[i] = value[i];
    }
}
void printLongKomma(newLongKomma* num) {
    long long komma = num->komma;
    if(komma == 0){
        komma--;
        printf("0,");}
    for (size_t i = 0; i <num->length ; ++i) {
        long long x = num->number[num->length - i -1 ];
        if(komma == 0)
            printf(",");
        if(x < 10){
            printf("0");
        }if(x < 100){
            printf("0");
        }if(x < 1000){
            printf("0");
        }if(x < 10000){
            printf("0");
        }if(x < 100000){
            printf("0");
        }if(x < 1000000){
            printf("0");
        }if(x < 10000000){
            printf("0");
        }
        if(x < 10000000){
            printf("0");
        }
        printf("%lld",x);
        komma--;
    }
}
void NullenEntfernen(newLongKomma *num) {
    long x = num->length -1;
    //printf("I = %d\n",num->length);
   // printf("I = %d\n",num->komma);

    while (num->komma > 0){
        if(num->number[x--] == 0){
            num->komma--;
            num->length--;
        }else
            break;
    }
    x = 0;
    while (num->length > 1){
        if(num->number[x++] != 0)
            break;
    }
    long y  = num->length -x +1;
    long long *arr = malloc(y * sizeof(long long));
    x--;
    for (long i = 0; i < y; i++) {
        arr[i] = num->number[x++];
    }

    setLongKomma(num,arr,y,num->komma);
    free(arr);

}


// Addiert zwei Zahlen und speichert das Ergebniss in die Adresse der ersten Zahl
// Beide Zahlern können verändert werden
void addLongKomma(newLongKomma *num1, newLongKomma *num2){
    long nachKomma1 = num1->length - num1->komma;
    long nachKomma2 = num2->length - num2->komma;


    // Die längere Anzahl an Vorkomma und Nachkommaziffern
    long vorKomma = (num1->komma > num2->komma) ? num1->komma +1: num2->komma+1;
    long nachKomma = (nachKomma1 > nachKomma2) ? nachKomma1 : nachKomma2;

    // Initialisiert den längstmöglichen Array
    long long * arr = malloc((nachKomma + vorKomma + 1) * sizeof (long long));
    long counter = 0;
    long counter1 = 0;
    long counter2 = 0;

    while (nachKomma1 > nachKomma2){
        arr[counter++] = num1->number[counter1++];
        nachKomma1--;
    }
    while (nachKomma2 > nachKomma1){
        arr[counter++] = num2->number[counter2++];
        nachKomma2--;
    }
    long carry = 0;
    while (nachKomma1 > 0){
        long long x = num1->number[counter1++] + num2->number[counter2++] + carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
        nachKomma1--;
    }

    // Nachkomma fertig
    while (num1->length > counter1 && num2->length > counter2){
        long long x = num1->number[counter1++] + num2->number[counter2++] + carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }
    while (num1->length > counter1){
        long long x = num1->number[counter1++] +  carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }
    while (num2->length > counter2){
        long long x = num2->number[counter2++] +  carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }

    arr[counter] = carry;
    setLongKomma(num1,arr,vorKomma + nachKomma,vorKomma);
    NullenEntfernen(num1);
    free(arr);










    }

void mulLongKomma(newLongKomma * num1, newLongKomma *num2){
    long nachKomma1 = num1->length - num1->komma;
    long nachKomma2 = num2->length - num2->komma;
    // Erstellt das Array mit richtiger Länge und nullt es
    long long length = num1->length + num2->length +1;
    long long * arr = malloc((length) * sizeof (long long));
    size_t i;
    for(i = 0; i < length;i++){
        arr[i] = 0;
    }

    size_t j;

    // Multiplikation wird Arrayweise ohne Beachtung des Carrys durchgeführt
    //TODO - SIMD MÖGlICH
    for(i = 0; i < num1->length;i++){
        for(j = 0; j < num2->length;j++){
            long long x = num1->number[i] * num2->number[j];
            arr[i +j +1] += x / 1000000000;
            arr[i + j] += x % 1000000000;
        }
    }

    // Carry wird ins nächste Array verschoben



    setLongKomma(num1,arr,length, (length -(nachKomma2 + nachKomma1 )));

    free(arr);
    NullenEntfernen(num1);

}

int cmpKomma(newLongKomma* num1,newLongKomma* num2){
    // Wenn eine Zahl ohne führende Nullen länger als eine andere ist, ist sie größer
    if(num1->length > num2->length)
        return 1;
    if(num2->length > num1->length)
        return -1;

    // Vergleich alle Tupel nacheinander, anfangend mit der linkesten Positiob
    for(size_t i = num1->length-1; i >=0;i--){

        if(num1->number[i] > num2->number[i]){
            return 1;
        }

        if(num2->number[i] > num1->number[i]) {
            return -1;
        }
    }

    // Wenn die Zahl weder größer noch kleiner ist, muss sie gleich der andern sein
    return 0;
}

void subLongKomma(newLongKomma * num1, newLongKomma *num2){

    long nachKomma1 = num1->length - num1->komma;
    long nachKomma2 = num2->length - num2->komma;

    // Die längere Anzahl an Vorkomma und Nachkommaziffern
    long vorKomma = (num1->komma > num2->komma) ? num1->komma : num2->komma;
    long nachKomma = (nachKomma1 > nachKomma2) ? nachKomma1 : nachKomma2;


    // Initialisiert den längstmöglichen Array
    long long * arr = malloc((nachKomma + vorKomma + 1) * sizeof (long long));
    long counter = 0;
    long counter1 = 0;
    long counter2 = 0;

    for(size_t i = 0; i < (nachKomma + vorKomma + 1);i++)
        arr[i] = 0;


    while (nachKomma1 > nachKomma2){
        arr[counter++] = num1->number[counter1++];
        nachKomma1--;
    }
    while (nachKomma2 > nachKomma1){
        long diff =  - (num2->number[counter2++]);
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter +1] -= 1;
        }
        counter++;
        nachKomma2--;
    }
    while (num2->length > counter2) {
        long diff = num1->number[counter1++] - (num2->number[counter2++]);
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter + 1] -= 1;
        }
        counter++;
    }
    while (num1->length > counter1){
        long diff = num1->number[counter1++];
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter + 1] -= 1;
        }
        counter++;
    }


    setLongKomma(num1,arr,vorKomma + nachKomma,vorKomma);
    free(arr);
    NullenEntfernen(num1);

}

void freeLongKomma(newLongKomma* num) {
    free(num->number);
    num->number = NULL;
    num->length = 0;
}
int compareSTRLongKomma(char *a,char *b){
    int l = strlen(a);
    int i;
    for( i = 0 ; i < l;i++){
        if(a[i] != b[i]){
            printf(" \nDie Genauigkeit ist = %d. Der Unterschied ist %d - %d",i,a[i],b[i]);

            return i;
        }
    }
    printf("\nAlle %d Stellen sind genau",i+1);
}

/*int main() {
    /*newLongKomma matrix1,matrix2,matrix4,tmp1,tmp2,tmp3,tmp4;
    initLong(&matrix1, 1);
    initLong(&matrix2, 1);
    initLong(&matrix4, 1);
    initLong(&tmp1,1);
    initLong(&tmp2,1);
    initLong(&tmp3,1);
    initInt(&tmp4,1);

    matrix1.numbers[0] = 0;
    matrix2.numbers[0] = 1;
    matrix4.numbers[0] = 2;
    tmp1.numbers[0] = 0;
    tmp2.numbers[0] = 0;
    tmp3.numbers[0] = 0;
    tmp4.numbers[0] = 0;

    for(int i = 0; i < 15; i++){

        setLong(&tmp1,matrix1.numbers,matrix1.length,matrix1.komma);
        mulLong(&tmp1,&matrix1);
        setLong(&tmp2,matrix2.numbers,matrix2.length,matrix2.komma);
        mulLong(&tmp2,&matrix2);
        addLong(&tmp1,&tmp2);

        setLong(&tmp2,matrix2.numbers, matrix2.length,matrix2.komma);
        mulLong(&tmp2,&matrix1);
        setLong(&tmp3,matrix2.numbers,matrix2.length,matrix2.komma);
        mulInt(&tmp3,&matrix4);
        addLong(&tmp2,&tmp3);

        setLong(&tmp4,tmp2.numbers,tmp2.length,tmp2.komma);
        addLong(&tmp4,&tmp2);
        addInt(&tmp4,&tmp1);

        setLong(&matrix1,tmp1.numbers,tmp1.length,tmp1.komma);
        setLong(&matrix2,tmp2.numbers,tmp2.length,tmp2.komma);
        setInt(&matrix4,tmp4.numbers,tmp4.length,tmp4.komma);

        printf("\n %d",i);
    }

    newLongKomma num1, num2;
    initLongKomma(&num1,4);
    num1.komma = 1;
    num1.numbers[0] = 0;
    num1.numbers[1] = 124;
    num1.numbers[2] = 0;
    num1.numbers[3] = 0;
    initLongKomma(&num2,2);
    num2.komma = 1;
    num2.numbers[0] = 123;
    num2.numbers[1] = 124;
    subLongKomma(&num1,&num2);
    printLongKomma(&num1);
}*/
