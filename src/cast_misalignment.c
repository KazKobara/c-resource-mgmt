#include <assert.h>
#include <stdlib.h>

int32_t main (){
    char c_dummy;
    // alignas(int) char c = 'x';  // C11
    char c = 'x';
    /* might mis-aligned on some implementations */
    /* cf. https://wiki.sei.cmu.edu/confluence/display/c/EXP36-C.+Do+not+cast+pointers+into+more+strictly+aligned+pointer+types
*/
    int *ip = (int *)&c;
    char *cp = (char *)ip;
    /* Will fail on the implementations */
    assert(cp == &c);
    /* return 134 if asserted */
}