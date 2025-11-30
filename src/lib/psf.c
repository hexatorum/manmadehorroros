// #include <lib/psf.h>

// #include <limits.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include <stdlib.h>

// psf2_t *psf2_load(void *start, void *end)
// {
//     psf2_header_t *header = start;
    
//     if (header->magic != PSF2_MAGIC)
//     {
//         return NULL;
//     }

//     // calloc does not exist yet therefore this code does not work
//     psf2_t *font = calloc(1, sizeof(psf2_t));

//     font->header = header;
//     font->glyphs = start + sizeof(psf2_header_t);
    
//     bool is_unicode = !!(font->header->flags & PSF2_UNICODE);
//     if (!is_unicode)
//     {
//         font->unicode = NULL;
//         return font;
//     }

//     uint16_t *unicode = calloc(USHRT_MAX, sizeof(uint16_t));

//     uint8_t *table = font->glyphs + header->glyph_count * header->glyph_size;
//     uint16_t glyph = 0;

//     while (table < end)
//     {
//         uint16_t code = *table;

//         if (code == 0xFF)
//         {
//             glyph++;
//             table++;
//             continue;
//         }
//         else if (code & 0x80)
//         {
//             if ((code & 0x20) == 0)
//             {
//                 code = (table[0] & 0x1F << 6) + (table[1] & 0x3F);
//                 table++;
//             }
//             else if ((code & 0x10) == 0)
//             {
//                 code = (table[0] & 0x0F << 12) + (table[1] & 0x3F << 6) + (table[2] & 0x3F);
//                 table += 2;
//             }
//             else if ((code & 0x08) == 0)
//             {
//                 code = (table[0] & 0x07 << 18) +
//                     (table[1] & 0x3F << 12) +
//                     (table[2] & 0x3F << 6) +
//                     (table[3] & 0x3F);
//                 table += 3;
//             }
//             else
//                 code = 0;
//         }

//         unicode[code] = glyph;
//         table++;
//     }

//     font->unicode = unicode;

//     return font;
// }

// void psf2_free(psf2_t *font)
// {
//     free(font->unicode);
//     free(font);
// }