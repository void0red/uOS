#include <string.h>
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    while (len-- != 0)
    {
        *dest++ = *src++;
    }
}
void memset(uint8_t *dest, uint8_t value, uint32_t len)
{
    while (len--)
    {
        *dest++ = value;
    }
}
int strcmp(const uint8_t *str1, const uint8_t *str2)
{
    while (*str1 != '\0' && *str1++ == *str2++)
        ;
    return *str1 - *str2;
}
uint8_t *strcpy(uint8_t *dest, const uint8_t *src)
{
    uint8_t *ret = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return ret;
}
uint8_t *strcat(uint8_t *dest, const uint8_t *src)
{
    //drop
    return NULL;
}
int strlen(const uint8_t *src)
{
    int ret = 0;
    while (*src++ != '\0')
    {
        ret++;
    }
    return ret;
}