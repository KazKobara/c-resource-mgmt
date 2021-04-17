/*
 How to compile and run.
 $ gcc -o const_pointer const_pointer.c
 $ ./const_pointer
*/

#include <stdio.h>
#include <stdint.h>

/* Comment out to see compile errors. */
//#define SHOW_COMPILE_ERRORS

int32_t main(void)
{
    const int32_t constant = 23;
          int32_t variable = 23;
    const int32_t constant2 = 67;
          int32_t variable2 = 67;

    /*** Set ***/
    /* with a constant */
    const int32_t*      ptr_cvc = &constant;
    /* Some compilers discard 'const' of the input value,
       and this makes "const int32_t constant = 23;" replaceable!!
    */
          int32_t*const ptr_vcc = &constant;
    const int32_t*const ptr_ccc = &constant;
    /* with a variable */
    const int32_t*      ptr_cvv = &variable;
          int32_t*const ptr_vcv = &variable;
    const int32_t*const ptr_ccv = &variable;

    (void)printf("\nInitial addresses and values\n");
    (void)printf("&constant=%p, constant=%d\n", &constant, constant);
    (void)printf("ptr_cvc=%p, *ptr_cvc=%d\n", ptr_cvc, *ptr_cvc);
    (void)printf("ptr_vcc=%p, *ptr_vcc=%d\n", ptr_vcc, *ptr_vcc);
    (void)printf("ptr_ccc=%p, *ptr_ccc=%d\n", ptr_ccc, *ptr_ccc);
    (void)printf("ptr_cvv=%p, *ptr_cvv=%d\n", ptr_cvv, *ptr_cvv);
    (void)printf("ptr_vcv=%p, *ptr_vcv=%d\n", ptr_vcv, *ptr_vcv);
    (void)printf("ptr_ccv=%p, *ptr_ccv=%d\n", ptr_ccv, *ptr_ccv);

    /*** Replace value ***/
    *ptr_vcc = 45;  // "const int32_t constant = 23;" is replaced!!
    *ptr_vcv = 45;

    #ifdef SHOW_COMPILE_ERRORS
    /* The following causes compile errors. */
    *ptr_cvc = 45;
    *ptr_ccc = 45;

    *ptr_cvv = 45;
    *ptr_ccv = 45;
    #endif /* SHOW_COMPILE_ERRORS */

    (void)printf("\nAfter value replacement\n");
    (void)printf("&constant=%p, constant=%d\n", &constant, constant);
    (void)printf("ptr_cvc=%p, *ptr_cvc=%d\n", ptr_cvc, *ptr_cvc);
    (void)printf("ptr_vcc=%p, *ptr_vcc=%d\n", ptr_vcc, *ptr_vcc);
    (void)printf("ptr_ccc=%p, *ptr_ccc=%d\n", ptr_ccc, *ptr_ccc);
    (void)printf("ptr_cvv=%p, *ptr_cvv=%d\n", ptr_cvv, *ptr_cvv);
    (void)printf("ptr_vcv=%p, *ptr_vcv=%d\n", ptr_vcv, *ptr_vcv);
    (void)printf("ptr_ccv=%p, *ptr_ccv=%d\n", ptr_ccv, *ptr_ccv);

    /*** Replace address ***/
    ptr_cvc = &constant2;
    ptr_cvv = &variable2;

    #ifdef SHOW_COMPILE_ERRORS
    /* The following causes compile errors. */
    ptr_vcc = &constant2;
    ptr_ccc = &constant2;

    ptr_vcv = &variable2;
    ptr_ccv = &variable2;
    #endif /* SHOW_COMPILE_ERRORS */

    /* Final values */
    (void)printf("\nAfter address replacement\n");
    (void)printf("&constant=%p, constant=%d\n", &constant, constant);
    (void)printf("ptr_cvc=%p, *ptr_cvc=%d\n", ptr_cvc, *ptr_cvc);
    (void)printf("ptr_vcc=%p, *ptr_vcc=%d\n", ptr_vcc, *ptr_vcc);
    (void)printf("ptr_ccc=%p, *ptr_ccc=%d\n", ptr_ccc, *ptr_ccc);
    (void)printf("ptr_cvv=%p, *ptr_cvv=%d\n", ptr_cvv, *ptr_cvv);
    (void)printf("ptr_vcv=%p, *ptr_vcv=%d\n", ptr_vcv, *ptr_vcv);
    (void)printf("ptr_ccv=%p, *ptr_ccv=%d\n", ptr_ccv, *ptr_ccv);

    return 0;
}