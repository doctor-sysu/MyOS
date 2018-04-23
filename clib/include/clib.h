#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int strcmp(const char *str1, const char *str2);

int strncmp(const char *str1, const char *str2, int n);

size_t strlen(const char *s);
//size_t gets_s(char* input, size_t len);



#ifdef __cplusplus
}
#endif
