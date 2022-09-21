#include "string.h"

void strcopy(char *source, char *target) {
    while (*source) {
        *target = *source;
        source++;
        target++;
    }
    *target = 0;

}
int streq(const char *source,const char *target){
    while (*source &&  *target) {
        if( *target != *source){
            return 0;
        }
        source++;
        target++;
    }
    return 1;

}