#include <lib/memory.h>

#include <stdint.h>

// overlap checking commented out because for some reason it also fails when the memory regions are
// touching
// libc optimizes this function by copying larger words but we're not doing that because we don't
// do premature optimization
// also for fuck's sake why does destination go first i knew i would confuse the order
void *memcpy(void *destination, const void *source, size_t size)
{
    // if (!(destination + size < source || source + size < destination)) return NULL;

    size_t copied = 0;

    uint8_t *d = destination;
    const uint8_t *s = source;

    while (copied != size)
    {
        d[copied] = s[copied];
        copied++;
    }

    return destination;
}

// added additional stride argument for our purposes
void *memset(void *destination, uint8_t value, size_t size, size_t stride)
{
    size_t set = 0;

    uint8_t *d = destination;

    while (set < size)
    {
        d[set] = value;
        set += sizeof(uint8_t) + stride;
    }

    return destination;
}
