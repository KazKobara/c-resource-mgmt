/*
 How to compile and run.
 $ gcc -o sizeof sizeof.c
 $ ./sizeof
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t main(void)
{
    int32_t *ptr = (int32_t *)calloc(9, sizeof(int32_t));
    void *void_ptr = (int32_t *)calloc(11, sizeof(int32_t));
    const int32_t array[9];

    struct struct1 {
        int16_t val3;
        int32_t val1;
        char    val2;
    };

    struct struct2 {
        char    val2;
        int16_t val3;
        int32_t val1;
    };

    const struct struct1 i16i32c;
    const struct struct2 ci16i32;

    (void)printf("\n");
    (void)printf("Type             : Size in Octets \n");
    (void)printf("----------------------------------\n");
    (void)printf("char             : %lu\n", sizeof(char));
    (void)printf("short int        : %lu\n", sizeof(short int));
    (void)printf("int              : %lu\n", sizeof(int));
    (void)printf("long int         : %lu\n", sizeof(long int));
    (void)printf("long long int    : %lu\n", sizeof(long long int));
    (void)printf("float            : %lu\n", sizeof(float));
    (void)printf("double           : %lu\n", sizeof(double));
    (void)printf("long double      : %lu\n", sizeof(long double));
    (void)printf("\n");
    (void)printf("int32_t array[9] : %lu\n", sizeof(array));
    (void)printf("pointer (ptr)    : %lu\n", sizeof(ptr));
    (void)printf("int32_t *ptr     : %lu\n", sizeof(*ptr));
    (void)printf("void *void_ptr   : %lu\n", sizeof(*void_ptr));
    (void)printf("\n");
    (void)printf("struct ci16i32   : %lu\n", sizeof(ci16i32));
    (void)printf("struct i16i32c   : %lu\n", sizeof(i16i32c));
    (void)printf("\n");

    free(ptr);
    ptr=NULL;
    free(void_ptr);
    void_ptr=NULL;

    return 0;
}