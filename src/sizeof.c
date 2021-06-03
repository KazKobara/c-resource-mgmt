/*
 How to compile and run.
 $ gcc -g -o sizeof sizeof.c
 $ ./sizeof

 For MinGW
 $ x86_64-w64-mingw32-gcc -g -o sizeof_x86_64_mingw.exe -Wall -DMINGW -D_ISOC99_SOURCE sizeof.c
 $ i686-w64-mingw32-gcc -g -o sizeof_i686_mingw -Wall -DMINGW -DI686 -D_ISOC99_SOURCE  sizeof.c

 How to show the real type of the typedefed one, such as time_t.
 $ gdb ./sizeof
 (gdb) run
 (gdb) ptype time_t
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <time.h>

int32_t main(void)
{
    int32_t *ptr = (int32_t *)calloc(9, sizeof(int32_t));
    void *void_ptr = (int32_t *)calloc(11, sizeof(int32_t));
    const int32_t array[9];

    (void)printf("\n");
    (void)printf("         Type          :Size [octet]          MIN                  MAX Note\n");
    (void)printf("----------------------------------------------------------------------------\n");
    (void)printf("                   char: %2zu %21d %20d 1\n", sizeof(char),               CHAR_MIN,  CHAR_MAX);
    (void)printf("            signed char: %2zu %21d %20d\n", sizeof(char),              SCHAR_MIN, SCHAR_MAX);
    (void)printf("          unsigned char: %2zu %21d %20d\n", sizeof(unsigned char),             0, UCHAR_MAX);
    (void)printf("\n");
    (void)printf("     (signed) short int: %2zu %21d %20d\n", sizeof(short int),          SHRT_MIN, SHRT_MAX);
    (void)printf("     unsigned short int: %2zu %21d %20d\n", sizeof(unsigned short int),        0, USHRT_MAX);
    /* (-0x7fffffffL - 1), 0x7fffffffL */
    (void)printf("           (signed) int: %2zu %21ld %20ld\n", sizeof(int), (long int) INT_MIN, (long int) INT_MAX);
    (void)printf("           unsigned int: %2zu %21d %20u\n", sizeof(unsigned int), 0, (unsigned int) UINT_MAX);
    /* (-0x7fffffffffffffffLL - 1L), 0x7fffffffffffffffLL */
    (void)printf("      (signed) long int: %2zu %21lld %20lld\n", sizeof(long int), (long long int) LONG_MIN, (long long int) LONG_MAX);
    /* (0x7fffffffffffffffLL * 2UL + 1UL) */
    (void)printf("      unsigned long int: %2zu %21d %20llu\n", sizeof(unsigned long int), 0, (unsigned long long) ULONG_MAX);
    (void)printf(" (signed) long long int: %2zu %21lld %20lld\n", sizeof(long long int), LLONG_MIN, LLONG_MAX);
    (void)printf(" unsigned long long int: %2zu %21d %20llu\n", sizeof(unsigned long long int),  0, ULLONG_MAX);
    (void)printf("\n");
    #define LOCAL_DEC_DIG 12
#if !defined(MINGW)
    /* MIN and MAX of IEEE754-2008 binary16 are 2**−14 (subnormal 2**(-14-10)) and (2**10 − 1)*(2**−10)*(2**15), respectively. */
    (void)printf("(IEEE754-2008 binary16):(%2zu)%21.*f %20d   3,4\n", (size_t) 2, LOCAL_DEC_DIG, 0.00006103515625, 65504 );
    /* MIN and MAX of bfloat16 are 2**−126 (subnormal 2**(−126−7)) and (2**8 − 1)*(2**−7)*(2**127), respectively. */
    (void)printf("             (bfloat16):(%2zu)%21.*e %20.*e 2,3,4\n", (size_t) 2, LOCAL_DEC_DIG, 1.175494350822287507969e-38, LOCAL_DEC_DIG, 3.389531389251535475905e+38 );
#endif
    (void)printf("                  float: %2zu %21.*e %20.*e 2,3\n",   sizeof(float),       LOCAL_DEC_DIG, FLT_MIN,  LOCAL_DEC_DIG, FLT_MAX);
    (void)printf("                 double: %2zu %21.*e %20.*e 2,3\n",   sizeof(double),      LOCAL_DEC_DIG, DBL_MIN,  LOCAL_DEC_DIG, DBL_MAX);
    (void)printf("            long double: %2zu %21.*Le %20.*Le 2,3\n", sizeof(long double), LOCAL_DEC_DIG, LDBL_MIN, LOCAL_DEC_DIG, LDBL_MAX);
    (void)printf("\n");
#if defined(MINGW)
#if defined(I686)
    (void)printf("              pintptr_t: %2zu %21d %20d\n", sizeof(intptr_t), INTPTR_MIN, INTPTR_MAX);
    (void)printf("             upintptr_t: %2zu %21d %20u\n", sizeof(intptr_t),           0, UINTPTR_MAX);
#else  /* defined(I686) */
    (void)printf("              pintptr_t: %2zu %21lld %20lld\n", sizeof(intptr_t), INTPTR_MIN, INTPTR_MAX);
    (void)printf("             upintptr_t: %2zu %21d %20llu\n", sizeof(intptr_t),           0, UINTPTR_MAX);
#endif /* defined(I686) */
#else  /* defined(MINGW) */
    (void)printf("              pintptr_t: %2zu %21ld %20ld\n", sizeof(intptr_t), INTPTR_MIN, INTPTR_MAX);
    (void)printf("             upintptr_t: %2zu %21d %20lu\n", sizeof(intptr_t),           0, UINTPTR_MAX);
#endif /* defined(MINGW) */
    (void)printf("          pointer (ptr): %2zu\n", sizeof(ptr));
    (void)printf("          int32_t *ptr : %2zu\n", sizeof(*ptr));
    (void)printf("        void *void_ptr : %2zu\n", sizeof(*void_ptr));
    (void)printf("       int32_t array[9]: %2zu\n", sizeof(array));
    (void)printf("\n");
    (void)printf("       (signed) ssize_t: %2zu\n", sizeof(ssize_t));
    (void)printf("      (unsigned) size_t: %2zu\n", sizeof(size_t));
    (void)printf("                 time_t: %2zu\n", sizeof(time_t));
    (void)printf("\n");
    (void)printf("        struct timespec: %2zu\n", sizeof(struct timespec));
    (void)printf("\n");
    (void)printf("Note:\n");
    (void)printf(" 1. 'char' might be 'unsigned char' in some processing systems, though\n");
    (void)printf("    types without 'signed' are the same as 'signed' ones in 'int' types.\n");
    (void)printf(" 2. MIN and MAX here are rounded.\n");
    (void)printf(" 3. MIN here is the minimum positive normal value and the smallest value\n");
    (void)printf("    is given by -MAX.\n");
#if !defined(MINGW)
    (void)printf(" 4. Will be updated after implemented in C.\n");
#endif
    (void)printf("\n");

    free(ptr);
    ptr=NULL;
    free(void_ptr);
    void_ptr=NULL;

    return 0;
}