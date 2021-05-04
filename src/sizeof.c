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

    (void)printf("\n");
    (void)printf("Type             : Size in Octets \n");
    (void)printf("----------------------------------\n");
    (void)printf("char             : %zu\n", sizeof(char));
    (void)printf("short int        : %zu\n", sizeof(short int));
    (void)printf("int              : %zu\n", sizeof(int));
    (void)printf("long int         : %zu\n", sizeof(long int));
    (void)printf("long long int    : %zu\n", sizeof(long long int));
    (void)printf("float            : %zu\n", sizeof(float));
    (void)printf("double           : %zu\n", sizeof(double));
    (void)printf("long double      : %zu\n", sizeof(long double));
    (void)printf("\n");
    (void)printf("int32_t array[9] : %zu\n", sizeof(array));
    (void)printf("pointer (ptr)    : %zu\n", sizeof(ptr));
    (void)printf("int32_t *ptr     : %zu\n", sizeof(*ptr));
    (void)printf("void *void_ptr   : %zu\n", sizeof(*void_ptr));
    (void)printf("\n");

    free(ptr);
    ptr=NULL;
    free(void_ptr);
    void_ptr=NULL;

    return 0;
}