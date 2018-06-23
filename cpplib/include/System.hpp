#pragma once

#define __SIZE_TYPE  long unsigned int
typedef __SIZE_TYPE__  size_t;


void SysC(int);

void * memcpy(void *dest, const void *src, unsigned int count);

void printf(char*);