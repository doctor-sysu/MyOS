#include <System.hpp>
#include <stdint.h>

void SysC(int Funmem) {
    asm volatile(
    "int 0x80\n"
    ::"a"(Funmem)
    );
}

void printf(const char *str)
{
    asm(
    "mov eax, 5\n"
    "int 0x80\n"
    :
    : "d"(str)
    );
}

void *memcpy(void *dest, const void *src, size_t count) {
    if (dest == src)
        return nullptr;
    char *d;
    const char *s;

    if (dest > (src + count) || (dest < src)) {
        d = static_cast<char *>(dest);
        s = static_cast<const char *>(src);
        while (count--)
            *d++ = *s++;
    } else /* overlap */
    {
        d = (char *) (dest + count - 1); /* offset of pointer is from 0 */
        s = (char *) (src + count - 1);
        while (count--)
            *d-- = *s--;
    }

    return dest;
}

void newProcess(char *name) {
    uint32_t a = 1;
    asm volatile(
    "mov eax, 6\n"
    "int 0x80\n"
    ::"b"(name)
    :"eax"
    );
}