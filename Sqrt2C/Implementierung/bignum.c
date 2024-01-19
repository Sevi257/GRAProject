#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "bignum.h"
#include "Expo.h"

// initializes a new bignum with the given length
void initLong(bignum* num, size_t length) {

    // allocates the space needed and sets the length
    num->numbers = malloc(length * sizeof(long long));
    if(num->numbers == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    num->length = length;

}

// sets the numbers array of the given bignum to the value array and sets the length accordingly
void setLong(bignum* num, const long long * value, size_t length) {

    // allocates new space if needed and sets the length accordingly
    if (length > num->length) {
        free(num->numbers);
        num->numbers = malloc(length * sizeof(long long));
        if(num->numbers == NULL){printf("Not able");
            exit(1);}
    }
    num->length = length;

    // copies the values of the value array in the numbers array of the bignum
    for (size_t i = 0; i < length; i++) {
        num->numbers[i] = value[i];

    }
}

// sets an value array as the numbers array of a bignum
void set(bignum* num, long long * value, size_t length){

    if(num->numbers != NULL)
    {
        free(num->numbers);
    }
        num->numbers = value;
        num->length = length;
};

// prints a bignum
void printLong(bignum* num) {

    // prints the most significant array without leading zeros
    printf("%lld",num->numbers[num->length - 1]);

    // prints the other arrays, but also prints zeros if necessary
    for (size_t i = 1; i < num->length; ++i) {
        long long x = num->numbers[num->length - i - 1 ];
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
    }

}

// removes leading zeros
void removeLeadingZeros(bignum *num) {

    long i;

    // iterates over the array until the current array != 0
    // --> i = first index with array[ index ] != 0
    for (i = num->length; i > 0;) {
        if (num->numbers[--i] != 0)
            break;
    }

    // return if there are no leading zeros
    if(i == num->length -1)
        return;

    // allokates new space for the array with size = amount of arrays without leading zeros
    long long *arr = malloc((i+1) * sizeof(long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // copies the values without the leading zeros int arr and sets the bignum accordingly
    // TODO - evtl geht das auch ohne erstellen eines neuen Arrays oder mit SIMD
    int x = 0;
    for (; i >= 0; i--) {
        arr[i] = num->numbers[i];
        x++;
    }
    set(num,arr,x);
    //free(arr);
}

// adds two bignums and safes the result in the first one, the second one stays unchanged
void addLong(bignum *num1, bignum *num2){

    if(num1->length >= num2->length){

        // creats an array with length = length of num1 + 1 --> maximum length of the result
        long long * arr = malloc((num1->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }

        long long carry = 0;
        long long i = 0;

        // adds the two bignums array-wise with a carry and writes the resukt mod 1 billion into the result array
        // carry = 1 if result of the prvious addition was >= 1 billion else 0
        while (i < num2->length){
            long long x = num1->numbers[i] + num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }

        // if the length of the first numbers is greater then the length of the second numbers the remaining arrays of the first numbers are added to the result
        while (i < num1->length){
            long long x = num1->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }

        arr[i] = carry;

        // if the carry = 1 then the length need to be increased, otherwise the length of the longer numbers is enough
        set(num1,arr,num1->length+carry);
     //   free(arr);

    }else {
        // works the same as the other method, but num2 ist uses as num1 was used before

        long long * arr = malloc((num2->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
        long long carry = 0;
        size_t i = 0;
        while (i < num1->length){
            long long x = num1->numbers[i] + num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }
        while (i < num2->length){
            long long x = num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }
        arr[i] = carry;
        set(num1,arr,num2->length+carry);
     //   free(arr);
    }}

// multiplies two bignums and safes the result in the first one, the second one stays unchanged
void mulLong(bignum * num1, bignum *num2){

    // creates an array with the right length and sets all Elements to zero
    long long length = num1->length + num2->length +1;
    long long * arr = malloc((length) * sizeof (long long));
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
            long long x = num1->numbers[i] * num2->numbers[j];
            arr[i +j +1] += x / 1000000000;
            arr[i + j] += x % 1000000000;
        }
    }

   // writes the result int num1 and removes leading zeros
    set(num1,arr,length);
    removeLeadingZeros(num1);
  //  free(arr);

}



bignum* mulKarazuba(bignum * num1, bignum* num2) {
    //TODO -> Problem ist beim Shiften nach recht mit exp
    if(num1->length==1||num2->length==1) {
        //Resort to normal Multiplikation
        mulLong(num1,num2);
        printf("\n");
        printf("Found size1: ");
        printLong(num1);
        printf("\n");
        return num1;
    }
    else{
        printf("\n LOop");
    }
    printf("Hello\n");
    size_t len = 0;
    //Calculate the size and find the middle
    if(num1->length>num2->length) {
        len = num1->length;
    }
    else {
        len = num2->length;
    }
    size_t len2 = len>>1;
    //Create 4 new newLongs with half the numbers num1 low num1 high // num2 low num2 high
    bignum num1_low, num1_high, num2_low, num2_high;
    initLong(&num1_low,len2);
    initLong(&num2_low,len2);
    initLong(&num1_high,len2);
    initLong(&num2_high,len2);

    for(size_t i=0;i<len2;i++) {
        //make the two lower nums TODO -> Arraycopy
        num1_low.numbers[i] = num1->numbers[i];
        num2_low.numbers[i] = num2->numbers[i];
    }
    size_t count = 0;
    for(size_t i=len2;i<len;i++) {
        num1_high.numbers[count] = num1->numbers[i];
        num2_high.numbers[count] = num2->numbers[i];
        count++;
    }

    //Now we have all 4 arrays
    //Call recursively
    bignum z_0save, z_1save, z_2save, n1low_save;
    initLong(&z_0save,1);
    initLong(&z_1save,1);
    initLong(&z_2save,1);
    initLong(&n1low_save,1);
    setLong(&n1low_save, num1_low.numbers, num1_low.length);
    printf("Num1_low bv: ");
    printLong(&num1_low);
    printf("\nNum2_low bv ");
    printLong(&num2_low);
    bignum z_0 =  *(mulKarazuba(&num1_low,&num2_low));      //TODO Ergebnisse zwischenspeicher
    setLong(&z_0save, z_0.numbers, z_0.length);
    printf("\nz_0: ");
    printLong(&z_0);
    addLong(&n1low_save,&num1_high);
    addLong(&num2_low,&num2_high);
    printf("\nNum1_low af: ");
    printLong(&num1_low);
    printf("\nNum2_low af ");
    printLong(&num2_low);
    bignum z_1 =  *(mulKarazuba(&n1low_save, &num2_low));
    setLong(&z_1save, z_1.numbers, z_1.length);
    printf("z_1: ");
    printLong(&z_1);
    bignum z_2 = *mulKarazuba(&num1_high,&num2_high);
    setLong(&z_2save, z_2.numbers, z_2.length);
    printf("\nZ_2: ");
    printLong(&z_2);
    //long long exp = (long long) powl(10,len2*10*2);     //Overflow mit Mul10 shiften -> Exp ist normalerweise die Länge der Linksverschiebungen * 2
   // long long exp1Int = (long long) powl(10,len2*10);      //Overflow
    bignum  exp, exp1;
    initLong(&exp,1);
    initLong(&exp1,1);
    //Mit 10 initialisieren und dann len2*10*2 mal mul10
    exp.numbers[0] = 10;
    exp1.numbers[0] = 10;
    for(int i = 0;i<len2*9*2-1;i++) {
        mul10Long(&exp);
    }
    printf("\nExp: ");
    printLong(&exp);
    for(int i = 0;i<len2*9-1;i++) {
        mul10Long(&exp1);
    }
    printf("\n  EXP1: ");
    printLong(&exp1);
   // printf("\n, Exp: %lld , Exp1: %lld \n", exp, exp1Int);
   // bignum e; //exponent for z_2
   // initInt(&e,1);
   // e.numbers[0] = exp;
    //temp variable for z2 is needed
    bignum temp_z2;
    initLong(&temp_z2,z_2save.length);
    setLong(&temp_z2, z_2save.numbers, temp_z2.length);
    mulLong(&temp_z2,&exp);
    subLong(&z_1save,&z_2save);
    printf("\nZ nach sub1: ");
    printLong(&z_1save);
    printf("\n z_0 vor sub2: ");
    printLong(&z_0save);
    subLong(&z_1save,&z_0save);
    printf("\nZ nach sub2: ");
    printLong(&z_1save);
    printf("\nTemp_z2: ");
    printLong(&temp_z2);
    printf("\nZ!: ");
    printLong(&z_1save);
   // e.numbers[0] = exp1Int;
    mulLong(&z_1save,&exp1);  //Hier shiften stattdessen
    addLong(&z_1save,&temp_z2);
    addLong(&z_1save,&z_0save);
    printf("\nreturn: ");
    printLong(&z_1save);
    printf("\n");
    return &z_1save;

}


// compares two bignums
// return 1 if num1 > num2; -1 if num1 < num2; 0 if num1 = num2
int cmp(bignum* num1,bignum* num2){
    // in our implementation before evey call of cmpInt the leading zeros are removed, so it is not mandatory to do it here again

    // if one numbers is longer than the other, it is longer
    if(num1->length > num2->length)
        return 1;
    if(num2->length > num1->length)
        return -1;

    // compares all arays starting with the most significant one
    for(long long i = num1->length-1; i >=0;i--){
        if(num1->numbers[i] > num2->numbers[i]){
            return 1;
        }
        if(num2->numbers[i] > num1->numbers[i]) {
            return -1;
        }
    }
    // if num1 is neither larger nor smaller than num2, the numbers have to be equal
    return 0;
}

// subtracts num1 - num2; works only if num >= num2
void subLong(bignum * num1, bignum *num2){

    // creates an array with size = the length of num1 and fills it with zeros
    long long * arr = malloc((num1->length) * sizeof (long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    for(size_t i = 0; i < num1->length;i++)
        arr[i] = 0;

    // subtracts array wise
    long long i,j,k,diff;
    for (i = 0, j = 0, k = 0; i < num1->length && j < num2->length;k++) {
        diff = num1->numbers[i++] - (num2->numbers[j++]);
        arr[k] += diff % 1000000000;
        if (arr[k] < 0) {
            arr[k ] += 1000000000;
            arr[k +1] -= 1;
        }
    }

    // if num1 is longer then num2 the ------ arrays can be added to the result
    while (i < num1->length){
        arr[k++] += num1->numbers[i++];
    }
    // writes the result in num1 and removes leading zeros
    set(num1,arr,num1->length);
    // free(arr);
    removeLeadingZeros(num1);

}


void subLong3(bignum * num1, bignum *num2){
/*
    // loop unrolling
    long long i = 0;
    while ( i < num2->length / 12) {
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i+1] -= 1;
        }
        num1->numbers[i+1] = num1->numbers[i+1] - (num2->numbers[i+1]);
        if (num1->numbers[i+1] < 0) {
            num1->numbers[i+1] += 1000000000;
            num1->numbers[i+2] -= 1;
        }
        num1->numbers[i+2] = num1->numbers[i+2] - (num2->numbers[i+2]);
        if (num1->numbers[i+2] < 0) {
            num1->numbers[i+2] += 1000000000;
            num1->numbers[i+3] -= 1;
        }
        num1->numbers[i+3] = num1->numbers[i+3] - (num2->numbers[i+3]);
        if (num1->numbers[i+3] < 0) {
            num1->numbers[i+3] += 1000000000;
            num1->numbers[i+4] -= 1;
        }
        num1->numbers[i+4] = num1->numbers[i+4] - (num2->numbers[i+4]);
        if (num1->numbers[i+4] < 0) {
            num1->numbers[i+4] += 1000000000;
            num1->numbers[i+5] -= 1;
        }
        num1->numbers[i+5] = num1->numbers[i+5] - (num2->numbers[i+5]);
        if (num1->numbers[i+5] < 0) {
            num1->numbers[i+5] += 1000000000;
            num1->numbers[i+6] -= 1;
        }

        num1->numbers[i+6] = num1->numbers[i+6] - (num2->numbers[i+6]);
        if (num1->numbers[i+6] < 0) {
            num1->numbers[i+6] += 1000000000;
            num1->numbers[i+7] -= 1;
        }
        num1->numbers[i+7] = num1->numbers[i+7] - (num2->numbers[i+7]);
        if (num1->numbers[i+7] < 0) {
            num1->numbers[i+7] += 1000000000;
            num1->numbers[i+8] -= 1;
        }
        num1->numbers[i+8] = num1->numbers[i+8] - (num2->numbers[i+8]);
        if (num1->numbers[i+8] < 0) {
            num1->numbers[i+8] += 1000000000;
            num1->numbers[i+9] -= 1;
        }
        num1->numbers[i+9] = num1->numbers[i+9] - (num2->numbers[i+9]);
        if (num1->numbers[i+9] < 0) {
            num1->numbers[i+9] += 1000000000;
            num1->numbers[i+10] -= 1;
        }

        num1->numbers[i+10] = num1->numbers[i+10] - (num2->numbers[i+10]);
        if (num1->numbers[i+10] < 0) {
            num1->numbers[i+10] += 1000000000;
            num1->numbers[i+11] -= 1;
        }
        num1->numbers[i+11] = num1->numbers[i+11] - (num2->numbers[i+11]);
        if (num1->numbers[i+11] < 0) {
            num1->numbers[i+11] += 1000000000;
            num1->numbers[i+12] -= 1;
        }

        i += 12;
    }
    while ( i < num2->length / 4) {
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        num1->numbers[i + 1] = num1->numbers[i + 1] - (num2->numbers[i + 1]);
        if (num1->numbers[i + 1] < 0) {
            num1->numbers[i + 1] += 1000000000;
            num1->numbers[i + 2] -= 1;
        }
        num1->numbers[i + 2] = num1->numbers[i + 2] - (num2->numbers[i + 2]);
        if (num1->numbers[i + 2] < 0) {
            num1->numbers[i + 2] += 1000000000;
            num1->numbers[i + 3] -= 1;
        }
        num1->numbers[i + 3] = num1->numbers[i + 3] - (num2->numbers[i + 3]);
        if (num1->numbers[i + 3] < 0) {
            num1->numbers[i + 3] += 1000000000;
            num1->numbers[i + 4] -= 1;
        }
        num1->numbers[i + 4] = num1->numbers[i + 4] - (num2->numbers[i + 4]);
        if (num1->numbers[i + 4] < 0) {
            num1->numbers[i + 4] += 1000000000;
            num1->numbers[i + 5] -= 1;
        }
        i += 4;
    }
    */
    long long i = 0;
/*
    while ( i < (num2->length / 6)) {
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        num1->numbers[i + 1] = num1->numbers[i + 1] - (num2->numbers[i + 1]);
        if (num1->numbers[i + 1] < 0) {
            num1->numbers[i + 1] += 1000000000;
            num1->numbers[i + 2] -= 1;
        }
        num1->numbers[i + 2] = num1->numbers[i + 2] - (num2->numbers[i + 2]);
        if (num1->numbers[i + 2] < 0) {
            num1->numbers[i + 2] += 1000000000;
            num1->numbers[i + 3] -= 1;
        }
        num1->numbers[i + 3] = num1->numbers[i + 3] - (num2->numbers[i + 3]);
        if (num1->numbers[i + 3] < 0) {
            num1->numbers[i + 3] += 1000000000;
            num1->numbers[i + 4] -= 1;
        }
        num1->numbers[i + 4] = num1->numbers[i + 4] - (num2->numbers[i + 4]);
        if (num1->numbers[i + 4] < 0) {
            num1->numbers[i + 4] += 1000000000;
            num1->numbers[i + 5] -= 1;
        }
        num1->numbers[i + 5] = num1->numbers[i + 5] - (num2->numbers[i + 5]);
        if (num1->numbers[i + 5] < 0) {
            num1->numbers[i + 5] += 1000000000;
            num1->numbers[i + 6] -= 1;
        }
        i += 6;
    }
    */
    while ( i < num2->length) {
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        i++;
    }

    // Das Ergebnis wird an die Adresse von num1 geschrieben und es werden führende Nullen entfernt
    removeLeadingZeros(num1);

}
void subLongSIMD(bignum * num1, bignum *num2){

    long long i = 0;
    int carry = 0;
    while ( i < num2->length) {
        carry = __builtin_sub_overflow(num1->numbers[i], num2->numbers[i], &num1->numbers[i]);
        if (carry) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        i++;
    }
    removeLeadingZeros(num1);

}

void subLong5(bignum* num1, bignum* num2) {
    size_t i = 0;
    int carry;
    // Perform subtraction array-wise using SIMD instructions
    while (i < num2->length) {
        carry = __builtin_ssubll_overflow(num1->numbers[i], num2->numbers[i], &num1->numbers[i]);
        num1->numbers[i + 1] -= carry;
        i++;
    }

    removeLeadingZeros(num1);

}


// TODO --> Funktioniert im Moment nicht
// TODO --> nicht auf englisch kommentiert
void mul10Long(bignum *num){
    // Erstellt das Array mit richtiger Länge und nullt es
    long long length = num->length +1;
    long long * arr = malloc((length) * sizeof (long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    size_t i;
    for(i = 0; i < length;i++){
        arr[i] = 0;
    }

    //TODO - SIMD MÖGlICH
    for(i = 0; i < length-1;i++){
            long x = 10 * num->numbers[i];
            arr[i] += x % 1000000000;
            arr[i +1] = x / 1000000000;
    }
    if(arr[i] == 0){
        set(num,arr,length-1);
    }else
        set(num,arr,length);

  //  free(arr);

}

// divides num1 / num2 up to the given pre
char* divLong(bignum * num1, bignum *num2, long long pre){
    pre += 1;
    // creates a new bignum to shift the comma
    bignum ten;
    initLong(&ten,1);
    ten.numbers[0] = 10;
    char * srt = malloc( pre * sizeof (char ));
    if( srt == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // TODO --> Runden
    long long x = -1;
    long long i;
    for( i = 0; i < pre; i++){

        int8_t counter = 0;

        // Prüft wieoft num2 in num1 passt und zieht num2 sooft von num1 ab
        while (cmp(num1,num2) > 0){

            subLong3(num1,num2);
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
        if(cmp(num1,num2) == 0){
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
        mulLong(num1,&ten);

        // TODO --> Print entfernen
       /* if(i % 10000 == 0)
            printf("%lld \n",i); */
    }
    srt[x] = '\0';
    return srt;
}

// frees a bignum
void freeLong(bignum* num) {
    free(num->numbers);
    num->numbers = NULL;
    num->length = 0;
}

// compares two Strings
int compareSTRLong(char *a,char *b){
    int l = strlen(a);
    int i;
    for( i = 0 ; i < l;i++){
        if(a[i] != b[i]){
            printf(" \n Es sind nur %d von %d Nachkommastellen genau",i,l);

            return i;
        }
    }
    printf("\nAlle %d Nachkommastellen sind genau",i);
}


char* div16(char* str){
    int i = 0;
    int counter = 0;
    int length = strlen(str);
    if(length == 0)
        return "0";
    char* divStr = malloc(length * sizeof (char));
    long tmp = str[i++] - '0';
    while (str[i] != '\0'){
        tmp = tmp * 10 + (str[i++] - '0');

        while(tmp <= 16 && str[i] != '\0'){
            divStr[counter++] = '0';
            tmp = tmp * 10 + (str[i++] - '0');

        }
        int x = tmp / 16;

        if(x != 0)
        divStr[counter++] = x + '0';
        else
            divStr[counter++] = '0';
        tmp %= 16;

    }
    divStr[counter] = '\0';
    return divStr;

}
char* reverseChar(char* str){
    int length = strlen(str);
    for(int i = length / 2; i >= 0; --i){
        char tmp = str[i];
        str[i] = str[length - i -1];
        str[length - i -1] = tmp;
    }
    return str;
}


// Methode um auf eine newLong mod 16 anzuwenden

int mod16(char* str){
    int y = strlen(str);
    switch (y) {
        case 0:
            return 0;
        case 1:
            return str[y-1] - '0';
        case 2:
            return ((str[y-2] - '0') * 10 + (str[y-1] - '0')) % 16;
        case 3:
            return ((str[y-3] - '0') * 100 + (str[y-2] - '0') * 10 + (str[y-1] - '0')) % 16;
        default:
             return ((str[y-4] - '0') * 1000 + (str[y-3] - '0') * 100 +
            (str[y-2] - '0') * 10 + (str[y-1] - '0'))  % 16;
    }


}

// Methode um aus einem String mit int Werten einen String mit Hex Werten zu machen


char * intToHex(char * intStr) {

  //  printf("IntString = %s\n",intStr);


    // Variablendeklaration und Erstellen des neuen Strings
    int length = strlen(intStr);
    char* hexString = malloc((1+length) * sizeof (char));
    int counter = 0;
    int zeroCounter = 0;

    // Solange bis der alte String durchlaufen ist
    while(counter < length){

        // Rest der division durch 16 ist die Hex-Zahl
        int mod = mod16(intStr);

        // Der Counter wird verwendet, um führende Nullen zu entfernen
        zeroCounter++;
        if(mod != 0)
            zeroCounter = 0;

        // Hier wird der Nummer die Hex-Nummer zugeordnet
        if(mod < 10)
        hexString[counter++] = mod + '0';
        else
            switch (mod) {
                case 10:
                    hexString[counter++] = 'A';
                    break;
                case 11:
                    hexString[counter++] = 'B';
                    break;
                case 12:
                    hexString[counter++] = 'C';
                    break;
                case 13:
                    hexString[counter++] = 'D';
                    break;
                case 14:
                    hexString[counter++] = 'E';
                    break;
                case 15:
                    hexString[counter++] = 'F';
                    break;
                default:
                    printf("\n Konvertierung in Hex String fehlerhaft! \n");
            }

        // Zahl wird durch 16 geteilt, um im nächsten Schritt an die nächste Hex-Ziffer zu gelangen
        intStr = div16(intStr);
    }
    hexString[counter] = '\0';
  //  printf("HexString = %s \n",hexString);
    hexString = reverseChar(hexString);
 //   printf("HexString = %s \n",hexString);

    // Die führenden Nullen werden entfernt und der String wird Nullterminiert
    char* a = malloc(1+counter* sizeof (char ));
    int i = 0;
    int c = 0;
    int first = 0;
    while( i < counter ){
       // printf("\n%c\n",hexString[i]);
        if(first == 1){
            a[c++] = hexString[i];
        }else
            if(hexString[i] != '0'){
                first = 1;
                a[c++] = hexString[i];
            }

        i++;
    }
    a[counter] = '\0';
    //printf("HexString = %s \n",a);
    // Der String wird reversed und zurückgegeben
    return a;

}
/*
int main() {
    exp1(15,10000,3);
    //  27.6
    //  16.6
    return 0;
    // 15 50.000 ->
    // Version 1 = 9.9
    // Version 2 = 6 sek / 25086

     /*
    bignum d, n;
    initLong(&d,4);
    initInt(&n,4);
    d.numbers[0] = 2342;
    d.numbers[1] = 324247;
    d.numbers[2] = 67675;
    d.numbers[3] = 5448;
    n.numbers[2] = 2344328;
    n.numbers[0] = 4235423;
    n.numbers[1] = 243223;
    n.numbers[3] = 98983;
    printf("D: ");
    printLong(&d);
    printf("\n N: ");
    printLong(&n);
    printf("\n");
   int count = 0;
    while (count<1000) {
        printf("");
        count++;
    }
    bignum* res = mulKarazuba(&d,&n);
    printInt(res);

}
*/
// print = 1 printet das Ergebnsi, 0 vergleicht uns sonst wird es zurückgegeben
void exp1(size_t e, size_t g, int print){
    bignum matrix1,matrix2,matrix4,tmp1,tmp2,tmp3,tmp4;
    initLong(&matrix1, 1);
    initLong(&matrix2, 1);
    initLong(&matrix4, 1);
    initLong(&tmp1,1);
    initLong(&tmp2,1);
    initLong(&tmp3,1);
    initLong(&tmp4,1);

    matrix1.numbers[0] = 0;
    matrix2.numbers[0] = 1;
    matrix4.numbers[0] = 2;
    tmp1.numbers[0] = 0;
    tmp2.numbers[0] = 0;
    tmp3.numbers[0] = 0;
    tmp4.numbers[0] = 0;

    for(int i = 0; i < e; i++){

        setLong(&tmp1, matrix1.numbers, matrix1.length);
        mulLong(&tmp1,&matrix1);
        setLong(&tmp2, matrix2.numbers, matrix2.length);
        mulLong(&tmp2,&matrix2);
        addLong(&tmp1,&tmp2);

        setLong(&tmp2, matrix2.numbers, matrix2.length);
        mulLong(&tmp2,&matrix1);
        setLong(&tmp3, matrix2.numbers, matrix2.length);
        mulLong(&tmp3,&matrix4);
        addLong(&tmp2,&tmp3);

        setLong(&tmp4, tmp2.numbers, tmp2.length);
        addLong(&tmp4,&tmp2);
        addLong(&tmp4,&tmp1);

        setLong(&matrix1, tmp1.numbers, tmp1.length);
        setLong(&matrix2, tmp2.numbers, tmp2.length);
        setLong(&matrix4, tmp4.numbers, tmp4.length);


    }
    printf("\n");
    char* b = "";


    // printf(divLongInt(&matrix2,&matrix4,40));
    if(print == 1)
        printf("%s",divLong(&matrix2,&matrix4,(long long) g));
    else if(print == 2) {
        printf("%s", intToHex(divLong(&matrix2,&matrix4,(long long) g)));
    }
    else if(print == 3){
        testFunction(divLong(&matrix2,&matrix4,g));
    }
}
