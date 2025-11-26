#ifndef PSF_H
#define PSF_H

#define PSF1_MAGIC 0x0436
#define PSF1_EXTENDED 0x01
#define PSF1_UNICODE 0x02

#define PSF2_MAGIC 0x864AB572
#define PSF2_UNICODE 0x00000001

#include <stdint.h>

typedef struct
{
    uint16_t magic;
    uint8_t flags;
    uint8_t height;
} psf1_header_t;

typedef struct
{
    uint32_t magic;
    uint32_t version;
    uint32_t size;
    uint32_t flags;
    uint32_t glyph_count;
    uint32_t glyph_size;
    uint32_t height;
    uint32_t width;
} psf2_header_t;

typedef struct
{
    psf2_header_t *header;
    uint16_t *unicode;
    uint8_t *glyphs;
} psf2_t;

psf2_t *psf2_load(void *start, void *end);
void psf2_free(psf2_t *font);

#endif