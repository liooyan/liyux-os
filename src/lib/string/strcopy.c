//
// Created by root on 10/31/22.
//
void strcopy(char *source, char *target) {
    while (*source) {
        *target = *source;
        source++;
        target++;
    }
    *target = 0;

}
