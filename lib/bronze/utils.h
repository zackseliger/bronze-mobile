#ifndef UTILS_H
#define UTILS_H

#include <string.h>

void LOG(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

// to compare 2 characters, used in sound.h, image.h
struct char_cmp {
    bool operator () (const char *a,const char *b) const {
        return strcmp(a,b)<0;
    }
};

#endif // UTILS_H
