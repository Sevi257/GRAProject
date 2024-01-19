cmake_minimum_required(VERSION 3.25)
project(team105)

set(CMAKE_CXX_STANDARD 14)

include_directories(Implementierung)

add_executable(team105
        Implementierung/Expo.h Implementierung/expo.c Implementierung/DivisionTests.c Implementierung/main.c Implementierung/LongKomma.c Implementierung/LongKomma.h Implementierung/intArr.c Implementierung/intArr.h Implementierung/bignum.c Implementierung/bignum.h)
