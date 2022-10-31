//
// Created by root on 10/31/22.
//
int streq(const char *source, const char *target) {
    while (*source && *target) {
        if (*target != *source) {
            return 0;
        }
        source++;
        target++;
    }
    if (!*source && !*target) {
        return 1;
    } else {
        return 0;
    }

}