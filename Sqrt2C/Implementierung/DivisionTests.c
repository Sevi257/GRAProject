#include <stdio.h>
#include <math.h>
#include "bignum.h"
#include "LongKomma.h"
void newtonLong(bignum * N, bignum* D) {
    //D ist 70 und Divisor N ist 29 und Quotient
    //andersrum
    //Divisor ist D_prime
    //Quotient ist N_prime
    bignum divisor;
    initLong(&divisor,N->length);
    divisor.numbers = N->numbers;
    bignum quotient;
    initLong(&quotient, D->length);
    quotient.numbers = D->numbers;
    //k = Anzahl der Bits
    printf("Divisor: ");
    printLong(&divisor);
    printf("\nQuotient: ");
    printLong(&quotient);
    printf("\n");
    unsigned long long k = divisor.length * sizeof (long long) + quotient.length*sizeof (long long);
    bignum two;
    bignum twoMul;
    initLong(&two,1);
    initLong(&twoMul,1);
    twoMul.numbers[0] = 2;
    two.numbers[0] = 1;
    for(long long i = 0;i<k;i++) {
        mulLong(&two,&twoMul);
    }
    printf("Two: ");
    printLong(&two);
    printf("\n");
    bignum X;
    initLong(&X,1);
    X.numbers[0] = 14;
    //subLong(&X,N);
    printf("X zuvor: ");
    printLong(&X);
    printf("\n");
    bignum X_prime;
    initLong(&X_prime,1);
    X_prime.numbers[0] = 0;
    bignum xx;
    initLong(&xx,1);
    xx.numbers[0] = 0;
    int count = 0;
    while (count<10) {
        count++;
        bignum  t1; //Tempvariable for X
        initLong(&t1,X.length);
        setLong(&t1, X.numbers, X.length);
       // t1.numbers = X.numbers;
        bignum t2; //Tempvariable for two
        initLong(&t2,two.length);
        setLong(&t2, two.numbers, two.length);
       // t2.numbers = two.numbers;
        //(x * (pow2 - x * divisor)
        /*printf("X: ");
        printLong(&X);
        printf("\n");
        mulInt(&t1,&divisor);
        printf("T2 (TWO): ");
        printLong(&t2);
        printf("\nT1 (X): ");
        printInt(&t1);
        printf("\n");*/
        //Hier kommt SIGTRAP
        subLong(&t2,&t1);
        mulLong(&X,&t2);
        //nach rechts shiften
        for(long long i=0;i<k;i++){
            divLong(&X,&twoMul,X.length);
        }
        printf("X: ");
        printLong(&X);
        printf("\n");
        /*if(cmp(&X,&X_prime)==0 || cmpInt(&X,&xx)==0) {
            break;
        }
        X_prime.numbers = xx.numbers;
        xx.numbers = X.numbers;*/
    }
    mulLong(&quotient,&X);
    for(long long i = 0;i<k;i++) {
        divLong(&quotient,&twoMul,quotient.length);
    }
    printf("Quotient: ");
    printLong(&quotient);
    printf("\n");

}
void calculateMandE(double D, double *M, int *e) {
    *M = frexp(D, e);  // Extract M and e from D

    *M *= 2;  // Adjust M to satisfy 1 ≤ M < 2
    *e -= 1;  // Adjust e to reflect the multiplication by 2
    printf("%f\n", *M);
    printf("%d\n",*e);
}

double computeResult(double D, double N, int P) {
    double M_D, M_N;
    int e_D, e_N;
    calculateMandE(D, &M_D, &e_D);
    calculateMandE(N, &M_N, &e_N);

    double D_prime = D / pow(2, e_D + 1);
    //Equivalent to shifting right e_D + 1 times
    double N_prime = N / pow(2, e_D + 1);     //Normalize N and D to be between 0.5 and 1.0 for easier calculation

    double X = 48./17 - 32./17 * D_prime;
    printf("N': %f\n", N_prime);
    printf("D': %f\n", D_prime);
    int iterations = ceil(log2(P + 1) / log2(17)); //Amount of Iterations to be P-precise

    for (int i = 0; i < iterations; i++) {
        X = X* (2-D_prime*X);
    }
    //0.014285714285714285

    printf("X: %f\n", X);
    return N_prime * X;
}
/*double newtonRaphsonDivision(double D, double N, int P) {
    // Calculate initial estimate X0
    double X0 = 48.0 / 17.0 - (32.0 / 17.0) * D;

    // Compute successive estimates using Newton-Raphson method
    for (int i = 0; i < P; i++) {
        X0 = X0 * (2 - D * X0);
    }

    // Compute quotient Q = N * Xs
    double Q = N * X0;
    return Q;
}*/
double goldschmidtDivision(double N, double D, int maxIterations) {
    // Scale dividend and divisor
    while (D >= 1) {
        N /= 2;
        D /= 2;
    }

    double F = 2 - D;  // Initial multiplication factor

    // Initialize scaled dividend and divisor
    double dividend = N;
    double divisor = D;

    for (int i = 0; i < maxIterations; i++) {
        dividend *= F;
        divisor *= F;

        F = 2 - divisor;

        // Check if divisor is sufficiently close to 1
        if (fabs(1 - divisor) < 1e-9) {
            break;
        }
    }

    return dividend;
}
void newtonKomma(newLongKomma* D, newLongKomma* N) {
    //das Komma so verschieben, dass es bei der Länge ist und dann noch solange durch 2 dividieren bis es kleiner als 1 ist
    //bei beiden das Komma verschieben
    newLongKomma D_prime, N_prime, one,X, two;
    initLongKomma(&two,1);
    initLongKomma(&X,2);
    initLongKomma(&one,1);
    initLongKomma(&D_prime,D->length);
    initLongKomma(&N_prime,N->length);
    two.number[0] = 2;
    two.komma = 1;
    one.number[0] = 1;
    one.komma = 1;
    D_prime.number = D->number;
    N_prime.number = N->number;
    D_prime.komma = 0;
    N_prime.komma = 0;
    X.number[0] = 4;
    X.number[1] = 1;
    X.komma = 1;
    //Jetzt noch durch 2 teilen bis kleiner 1
    printf("\nD_prime: ");
    printLongKomma(&D_prime);
    printf("\nN_prime: ");
    printLongKomma(&N_prime);
    //Tempvar for D_prime

    for(int i = 0;i<10;i++) {
        newLongKomma temp;
        initLongKomma(&temp,D_prime.length);
        temp.number = D_prime.number;
        temp.komma = D_prime.komma;
        initLongKomma(&two,1);
        two.number[0] = 2;
        two.komma = 1;
        mulLongKomma(&temp,&X); //X*(2-D_prime*X)
        printf("\nD_prime: ");
        printLongKomma(&temp);
        subLongKomma(&two,&temp);
        printf("\n Two after sub: ");
        printLongKomma(&two);
        mulLongKomma(&X,&two);
        printf("\nX: ");
        printLongKomma(&X);
    }
    mulLongKomma(&N_prime,&X);
    printf("\n Result: ");
    printLongKomma(&N_prime);

}

/*int main(){
    bignum N;
    bignum D;
    initLong(&N,1);
    initInt(&D,1);

    N.numbers[0] = 70;
    D.numbers[0] = 29;
    printf("Hello");
    newtonLong(&N,&D);
    printf("Goldschmidt: %f\n", goldschmidtDivision(29,70,100));
    printf("Newton: %f\n", computeResult(70,29,3));
    newLongKomma N, D;
    initLongKomma(&D,1);
    D.komma = 1;
    D.numbers[0] = 70000000;
    initLongKomma(&N,1);
    N.komma = 1;
    N.numbers[0] = 29000000;
    printLongKomma(&D);
    printf("\n");
    printLongKomma(&N);
    newtonKomma(&D,&N);
}*/
