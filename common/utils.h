#ifndef UTILS_H
#define UTILS_H
#include <map>

void LOG(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

// for use in sound.cpp and image.cpp
struct char_cmp {
    bool operator () (const char *a,const char *b) const {
        return strcmp(a,b)<0;
    }
};
typedef std::map<const char *, int, char_cmp> strmap;

#endif // UTILS_H
