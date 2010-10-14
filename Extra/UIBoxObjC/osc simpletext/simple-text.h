#ifndef simple_text_h
#define simple_text_h

#include <stdint.h>

typedef struct OpaqueSTTypeFace *STTypeFace;

extern STTypeFace ST_load_type_face(
    const char   *path,
    unsigned int  pixel_size);

extern void ST_free_type_face(STTypeFace type_face);

typedef enum STAlignment
{
    ST_ALIGN_LEFT,
    ST_ALIGN_CENTER,
    ST_ALIGN_RIGHT
}
STAlignment;

extern void ST_render_text(
    STTypeFace    type_face,
    STAlignment   alignment,
    unsigned int  bitmap_width,
    unsigned int  bitmap_height,
    uint8_t      *bitmap,
    const char   *utf8);

#endif
