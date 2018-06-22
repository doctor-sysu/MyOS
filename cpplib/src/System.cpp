#include <System.hpp>

void SysC(int Funmem){
    asm volatile(
    "int 0x80\n"
    ::"a"(Funmem)
    );
}

void printf(char* str)
{
    asm volatile(
    "mov eax, 5\n"
    "int 0x80\n"
    ::"b"(str)
    );
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *d;
    const char *s;

    if (dest > (src+ count) || (dest < src))
    {
        d = static_cast<char*>(dest);
        s = static_cast<const char*>(src);
        while (count--)
            *d++ = *s++;
    }
    else /* overlap */
    {
        d = (char *)(dest + count - 1); /* offset of pointer is from 0 */
        s = (char *)(src + count -1);
        while (count --)
            *d-- = *s--;
    }

    return dest;
}