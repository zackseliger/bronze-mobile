#include "utils.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef __ANDROID__
#include <android/log.h>
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,"io.deemo",__VA_ARGS__)
#endif

void LOG(const char *fmt, ...) {
  va_list arg;
  char buffer[256];

  /* Write the error message */
  va_start(arg, fmt);
  vsprintf(buffer, fmt, arg);
  va_end(arg);

  #ifdef __ANDROID__
  ALOG("%s", buffer);
  #elif __APPLE__
  printf("%s\n", buffer);
  #endif
}
