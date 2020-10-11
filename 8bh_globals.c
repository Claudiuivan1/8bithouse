#include <stdio.h>
#include <string.h>

// Used to set/unset specific bit in a bitmap
/* Arguments:
 * - array -> pointer of the bitmap
 * - n -> n-th place of the bitmap
 * - value -> 0 or 1
 */
int SET(unsigned char *array, int n, int value) {
    if(n < 8) {
        if(value)
            *array |= (1<<(7-n));
        else
            *array &= ~(1<<(7-n));
        return 1;
    }
    return 0;
}
