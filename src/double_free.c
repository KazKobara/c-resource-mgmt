#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * @retval  0 success
 * @retval -1 failure
 */
int32_t main(void)
{
    u_int32_t *ptr = NULL, *ptr2 = NULL;

    ptr = (u_int32_t *)calloc(10,sizeof(u_int32_t));
    if (NULL == ((void *)ptr)){
        perror("perror calloc()");
        return -1;
    }
    ptr2 = ptr;  // Do not duplicate a pointer to be freed
    ptr[0] = 1;  // Dummy

    free(ptr);
    ptr=NULL; // Set NULL after free()

    /* Check the behavior of double free */
    free(ptr2);  // Double free
    ptr2=NULL; // Set NULL after free()

    (void)printf("Done\n");
    return 0;
}
