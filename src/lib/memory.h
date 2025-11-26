#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void *memcpy(void *destination, const void *source, size_t size);
void *memset(void *destination, uint8_t value, size_t size, size_t stride);

#endif