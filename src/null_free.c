#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int32_t main(void)
{
    int32_t *ptr = (int32_t *)calloc(10,sizeof(int32_t));
    // int32_t *ptr = (int32_t *)aligned_alloc(sizeof(int32_t),10*sizeof(int32_t));
    if (NULL == ptr){
        perror("perror calloc()");
        return -1;
    }
    ptr[0] = 1;  // Dummy

    free(ptr);
    ptr=NULL; // Set NULL after free()

    /* Check the behavior of double free. */
    free(ptr);
    ptr=NULL; // Set NULL after free()
    (void)printf("Done\n");

    return 0;
}
