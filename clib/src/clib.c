#include "clib.h"

int strcmp(const char *str1, const char *str2)
{
    /*不可用while(*str1++==*str2++)来比较，当不相等时仍会执行一次++，
    return返回的比较值实际上是下一个字符。应将++放到循环体中进行。*/
    while (*str1 == *str2)
    {
        if (*str1 == '\0')
            return 0;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int strncmp(const char *str1, const char *str2, int n)
{
    while (*str1 && *str2 && (*str1 == *str2) && (n--))
    {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

size_t strlen(const char *s)
{
    const char *sc = s;

    for (; *sc != '\0'; ++sc);
    return sc - s;
}

// size_t gets_s(char* input, size_t len) {
// 	char* replace = input;
// 	for (; len != 0; len--) {
// 		*replace = getchar();
// 		replace++;
// 	}
// 	return len;
// }
