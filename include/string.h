#ifndef __INCLUDE_STRING_H_
#define __INCLUDE_STRING_H_
#include <types.h>
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(uint8_t *dest, uint8_t value, uint32_t len);
int strcmp(const uint8_t *str1, const uint8_t *str2);
uint8_t *strcpy(uint8_t *dest, const uint8_t *src);
uint8_t *strcat(uint8_t *dest, const uint8_t *src);
int strlen(const uint8_t *src);
#endif