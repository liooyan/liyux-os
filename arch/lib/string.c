#include "string.h"

void strcopy(char *source, char *target) {
    while (*source) {
        *target = *source;
        source++;
        target++;
    }
    *target = 0;

}