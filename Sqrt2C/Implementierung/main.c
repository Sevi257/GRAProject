#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include "getopt.h"
#include "errno.h"
#include "Expo.h"
#include "bignum.h"
#include "LongKomma.h"
#include "intArr.h"
#include <time.h>
#include "float.h"

//Divide and conquer for Karazuba -> Take half the arrays and recursively go down until you only have normal arrays

void help(){
    printf("This program calculates sqrt(2) and depending on the command-line argument prints the numbers out as "
           "Decimal or Hexadezimal \n you have the following options: -V (with the optional argument of the Implementation you want "
           "to choose) There are 4 Options:\n"
           "0) Exponentiation by squaring coupled with long-array datastructures \n "
           "1) Self-developed algorithm for exponentiation with long-array datastructures \n"
           "2) Self-developed algorithm for exponentiation with int-array datastructures \n"
           "The Option -B measures the time it takes to produce a result with the optional argument of repetitions of the function \n"
           "The Option -d returns the sqrt(2) in decimal Radix it has priority over -h with returns the numbers as hexadecimal represenatiation \n"
           "both options take an argument which is the amount of places you want to have printed out\n"
           "The Option -t takes a string as an optional argument and tests how many decimal places correspond to the squareroot of two (up to 10Million).\n"
           "If no argument is provided it compares the calculate result to the correct string and doesn't return the string. \n"
           "-h without an argument displays this message \n"
           "and --help also displays this message \n"
           "Last but not least please consider that large precision (>100000) can take some time to compute \n");
}
void error_exit(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}
size_t parseLongFromCommandline(char* opt_arg) {
    size_t arg = 0;
    errno = 0;
    char *endptr;
    arg = (int) strtol(opt_arg, &endptr, 10);
    if((errno == ERANGE && (arg == __LONG_MAX__ || arg == (-(__LONG_MAX__) -1L)))|| (errno != 0 && arg == 0)) {
        // arg is valid
        error_exit("The Value provided for the amount of repetitions of the function was not provided correctly!\n");
    }
    else if(endptr == opt_arg) {
        error_exit("No digits were found! \n");
    }
    else if(*endptr != '\0') {
        error_exit("Please refrain from appending non-integers to your numbers!\n");
    }
    return arg;
}

int main(int argc, char** argv) {
    int opt;
    char* testString;
    size_t implementation = 0;  // Set default implementation
    bool measure = false;
    size_t repetition = 1;
    bool has_h = false;
    bool has_d = false;
    size_t h_arg = -1;
    size_t d_arg = -1;  // for decimal places
    errno = 0;
    bool test = false;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'H'},
            {0, 0, 0, 0}
    };
    int optionId  = 0;
    while ((opt = getopt_long(argc, argv, "V:B::d:h::t", long_options, &optionId)) != -1) {
        optionId  = 0;
        switch (opt) {
            case 'H':
                help();
                return EXIT_SUCCESS;
            case 'V':
                if (optarg != NULL) {
                    implementation = parseLongFromCommandline(optarg);
                }
                break;
            case 'B':
                if (optarg != NULL) {
                    repetition = parseLongFromCommandline(optarg);
                }
                measure = true;
                break;
            case 'd':
                if (optarg != NULL) {
                    d_arg = parseLongFromCommandline(optarg);
                    if(d_arg > 100000) {
                        printf("This might take some time to compute do not abort if you want the result ... \n");
                    }
                    has_d = true;
                }
                break;
            case 'h':
                if (optarg == NULL) {
                    help();
                    exit(EXIT_SUCCESS);
                } else {
                    h_arg = parseLongFromCommandline(optarg);
                    if(h_arg > 50000) {
                        printf("This might take some time to compute do not abort if you want the result ... \n");
                    }
                    has_h = true;
                }
                break;
            case 't':
                if(optarg!=NULL) {
                    test = true;
                    testString = optarg;
                }
                break;
            case '?':
                // Handle unrecognized options or missing arguments
                error_exit("Unrecognized option or missing arguments");
        }
    }



    if(!has_h && !has_d) {          //check if a precision is specified
        if(test) {
            testFunction(testString);
            return EXIT_SUCCESS;
        }
        error_exit("No precision specified! Please provide either -d or -h!");
    }
    size_t gen;

    if(has_d) {         //Find the precision
        gen = d_arg;

    } else{
        gen = h_arg;
    }


    int print;      //Find the right return Option
    if(has_d) {
        print = 1;
    }
    else if (test) {
        print = 3;
    }
    else if(has_h) {
        print = 2;
    }
    size_t wert = ceil(1.31*(double) gen);
    if(wert== 0 && gen>0) {
        error_exit("Sorry but the maximum of possible accuracy has been reached! \n");
    }
    //TODO -> Dezimal und Hexadezimal
    if(implementation == 0) {
        //Hauptimplementierung mit schneller Exponentiation
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                expo(wert,gen,print);       //Exponentiation by squaring Algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            expo(wert,gen,print);
        }
    } else if (implementation == 1) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                size_t exponentiations = ceil(log((double) gen)/ log(4)); //calculate the amount of iterations on the loop
                exp1(exponentiations,gen,print);        //self developed algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            size_t exponentiations = ceil(log((double) gen)/ log(4)); //calculate the amount of iterations on the loop
            exp1(exponentiations,gen,print);
        }
    }
    else if (implementation == 2) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                size_t exponentiations = ceil(log((double) gen)/ log(4)); //calculate the amount of iterations on the loop
                exp1Int(exponentiations,gen,print);        //self developed algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            size_t exponentiations = ceil(log((double) gen)/ log(4)); //calculate the amount of iterations on the loop
            exp1(exponentiations,gen,print);
        }
    }
    //Print: Legende -> 1 => Ausgabe der Zahl als Dezimalzahl
    //                  2 => Ausgabe der Zahl als Hexadezimalzahl
    //                  3 => Vergleich der Zahl mit Sqrt(2) bis zu 10 Millionen Stellen
    //TODO
    //Longkomma kommt noch
    //intArr kommt auch noch
    //Division NewtonRaphson vielleicht auch noch || Karatsuba
    return 0;
}

