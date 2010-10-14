#include <ctype.h>

#include "simple-text.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

static int st_freetype_initialized = 0;
static FT_Library st_library;

typedef struct OpaqueSTTypeFace
{
    FT_Face face;
}
OpaqueSTTypeFace;

static int st_init_freetype(void)
{
    FT_Error error;
    
    if (!st_freetype_initialized)
    {
        error = FT_Init_FreeType(&st_library);
        if (error != 0)
        {
            return 0;
        }
        else
        {
            st_freetype_initialized = 1;
        }
    }
    
    return 1;
}

static STTypeFace st_alloc_type_face(void)
{
    return malloc(sizeof(OpaqueSTTypeFace));
}

static int st_open_face(
    const char *path,
    STTypeFace type_face)
{
    FT_Open_Args args;
    args.flags = FT_OPEN_PATHNAME;
    char pathBuffer[PATH_MAX];
    strcpy(pathBuffer, path);
    args.pathname = pathBuffer;
    
    FT_Error error = FT_Open_Face(
        st_library,
        &args,
        0,
        &(type_face->face));
    if (error != 0)
    {
        return 0;
    }
    
    return 1;
}

STTypeFace ST_load_type_face(
    const char   *path,
    unsigned int  pixel_size)
{
    if (!st_init_freetype())
    {
        return NULL;
    }
    
    STTypeFace type_face = st_alloc_type_face();
    if (type_face == NULL)
    {
        return NULL;
    }
    
    if (!st_open_face(path, type_face))
    {
        free(type_face);
        return NULL;
    }
    
    FT_Error error = FT_Set_Pixel_Sizes(
        type_face->face, 0, pixel_size);
    if (error != 0)
    {
        ST_free_type_face(type_face);
        return NULL;
    }
    
    return type_face;
}

void ST_free_type_face(STTypeFace type_face)
{
    FT_Done_Face(type_face->face);
    free(type_face);
}

static void st_chars_to_glyph_indices(
    STTypeFace      type_face,
    size_t          char_count,
    const uint32_t *text,
    FT_UInt        *glyph_indices)
{
    unsigned int i;
    for (i = 0; i < char_count; ++i)
    {
        glyph_indices[i] = FT_Get_Char_Index(
            type_face->face,
            text[i]);
    }
}

static void st_load_glyphs(
    STTypeFace     type_face,
    size_t         glyph_count,
    const FT_UInt *glyph_indices,
    FT_Glyph      *glyphs)
{
    unsigned int i;
    for (i = 0; i < glyph_count; ++i)
    {
        FT_Error error = FT_Load_Glyph(
            type_face->face,
            glyph_indices[i],
            FT_LOAD_DEFAULT);
        if (error != 0)
        {
            continue;
        }
        FT_Get_Glyph(type_face->face->glyph, &glyphs[i]);
    }
}

static void st_unload_glyphs(
    size_t    glyph_count,
    FT_Glyph *glyphs)
{
    unsigned int i;
    for (i = 0; i < glyph_count; ++i)
    {
        FT_Done_Glyph(glyphs[i]);
    }
}

static void st_lay_out_horizontally(
    STTypeFace      type_face,
    size_t          glyph_count,
    const FT_UInt  *glyph_indices,
    const FT_Glyph *glyphs,
    FT_Vector      *positions)
{
    int pen_x = 0;
    int use_kerning = FT_HAS_KERNING(type_face->face);
    
    unsigned int i;
    for (i = 0; i < glyph_count; ++i)
    {
        positions[i].x = pen_x;
        positions[i].y = 0;
        
        if (use_kerning && i > 0)
        {
            FT_Vector delta; 
            FT_Get_Kerning(type_face->face,
                           glyph_indices[i - 1],
                           glyph_indices[i],
                           FT_KERNING_DEFAULT,
                           &delta);
            pen_x += delta.x >> 6;
        }
            
        pen_x += glyphs[i]->advance.x >> 16;
    }
    
    positions[glyph_count].x = pen_x;
    positions[glyph_count].y = 0;
}

static void st_wrap_lines(
    STTypeFace      type_face,
    int             width,
    size_t          char_count,
    const uint32_t *text,
    FT_Vector      *positions,
    STAlignment     alignment)
{
    unsigned int line_y = 0;

    unsigned int line_start = 0;
    while (line_start < char_count)
    {
        unsigned int character = line_start;
        unsigned int last_break = line_start;
        int start_position = positions[line_start].x;
        
        int found_end_of_line = 0;
        while (!found_end_of_line)
        {
            if (character == char_count)
            {
                last_break = character - 1;
                found_end_of_line = 1;
            }
            else if ((positions[character].x - start_position) >= width)
            {
                last_break = character;
                found_end_of_line = 1;
            }
            else
            {
                if (isspace(text[character]))
                {
                    last_break = character;
                }
                
                if (text[character] == '\n')
                {
                    ++character;
                    found_end_of_line = 1;
                }
            }
            
            ++character;
        }
        
        int offset = 0;
        if (alignment != ST_ALIGN_LEFT)
        {
            offset = (width - (positions[last_break + 1].x - start_position));
            if (alignment == ST_ALIGN_CENTER)
            {
                offset /= 2;
            }
        }
        
        unsigned int i;
        for (i = line_start; i <= last_break; ++i)
        {
            positions[i].x -= start_position - offset;
            positions[i].y = line_y;
        }
        
        line_y -= type_face->face->size->metrics.height >> 6;
        line_start = last_break + 1;
    }
}

static void st_draw_bitmap(
    FT_BitmapGlyph  glyph,
    uint8_t        *bitmap,
    int             bitmap_width,
    int             bitmap_height,
    int             pen_x,
    int             pen_y)
{
    int x, y;
    for (y = 0; y < glyph->bitmap.rows; ++y)
    {
        int dest_y = pen_y + glyph->top - glyph->bitmap.rows + y;
        if (dest_y < 0)
        {
            continue;
        }
        else if (dest_y >= bitmap_height)
        {
            break;
        }
                
        for (x = 0; x < glyph->bitmap.width; ++x)
        {
            int dest_x = pen_x + x + glyph->left;
            
            if (dest_x < 0)
            {
                continue;
            }
            else if (dest_x >= bitmap_width)
            {
                break;
            }
            
            int src_x = x;
            int src_y = glyph->bitmap.rows - 1 - y;
        
            bitmap[dest_x + bitmap_width * dest_y] +=
                glyph->bitmap.buffer[src_x + glyph->bitmap.pitch * src_y];
        }
    }
}

static const char *st_unpack_utf8_char(
    const char *i,
    uint32_t   *o)
{
    if ((i[0] & 0x80) == 0x00)
    {
        *o = i[0] & 0x7F;
        return i + 1;
    }
    else if ((i[0] & 0xE0) == 0xC0)
    {
        if ((i[1] & 0xC0) != 0x80)
        {
            *o = '?';
            return i + 1;
        }
        
        *o = ((i[0] & 0x1F) << 6) |
             ((i[1] & 0x3F) << 0);
        return i + 2;
    }
    else if ((i[0] & 0xF0) == 0xE0)
    {
        if ((i[1] & 0xC0) != 0x80 ||
            (i[2] & 0xC0) != 0x80)
        {
            *o = '?';
            return i + 1;
        }
        
        *o = ((i[0] & 0x0F) << 12) |
             ((i[1] & 0x3F) <<  6) |
             ((i[2] & 0x3F) <<  0);
        return i + 3;
    }
    else if ((i[0] & 0xF8) == 0xF0)
    {
        if ((i[1] & 0xC0) != 0x80 ||
            (i[2] & 0xC0) != 0x80 ||
            (i[3] * 0xC0) != 0x80)
        {
            *o = '?';
            return i + 1;
        }
        
        *o = ((i[0] & 0x07) << 18) |
             ((i[1] & 0x3F) << 12) |
             ((i[2] & 0x3F) <<  6) |
             ((i[3] & 0x3F) <<  0);
        return i + 4;
    }
    else
    {
        *o = '?';
        return i + 1;
    }
}

static size_t st_utf8_to_ucs4(
    const char  *utf8,
    uint32_t   **out_ucs4)
{
    size_t byte_count = strlen(utf8);
    // TODO potentially 4x too much memory!
    uint32_t *ucs4 = malloc(byte_count * sizeof(uint32_t));
    
    const char *i = utf8;
    uint32_t *o = ucs4;
    while (*i != '\0')
    {
        i = st_unpack_utf8_char(i, o++);
    }
    
    *out_ucs4 = ucs4;
    return o - ucs4;
}

void ST_render_text(
    STTypeFace    type_face,
    STAlignment   alignment,
    unsigned int  bitmap_width,
    unsigned int  bitmap_height,
    uint8_t      *bitmap,
    const char   *utf8)
{
    FT_Error error;
    
    uint32_t *ucs4;
    size_t char_count = st_utf8_to_ucs4(utf8, &ucs4);
    
    // TODO use alloca() if character count is small?
    // TODO better glyph management?
    FT_UInt   *glyph_indices =
        malloc(char_count * sizeof(FT_UInt));
    FT_Glyph  *glyphs =
        malloc(char_count * sizeof(FT_Glyph));
    FT_Vector *positions =
        malloc((char_count + 1) * sizeof(FT_Vector));
    
    st_chars_to_glyph_indices(
        type_face,
        char_count,
        ucs4,
        glyph_indices);
    st_load_glyphs(
        type_face,
        char_count,
        glyph_indices,
        glyphs);
    st_lay_out_horizontally(
        type_face,
        char_count,
        glyph_indices,
        glyphs,
        positions);
    st_wrap_lines(
        type_face,
        bitmap_width,
        char_count,
        ucs4,
        positions,
        alignment);
    
    int vertical_offset = bitmap_height -
        (type_face->face->size->metrics.height >> 6) -
        (type_face->face->size->metrics.descender >> 6);
    
    unsigned int i;
    for (i = 0; i < char_count; ++i)
    {
        if (ucs4[i] == '\n')
        {
            continue;
        }
        
        FT_Glyph bitmap_glyph = glyphs[i];
        error = FT_Glyph_To_Bitmap(
            &bitmap_glyph, ft_render_mode_normal, NULL, 0);
        if (error != 0)
        {
            continue;
        }
        
        st_draw_bitmap(
            (FT_BitmapGlyph)bitmap_glyph,
            bitmap,
            bitmap_width,
            bitmap_height,
            positions[i].x,
            positions[i].y + vertical_offset);
    }
    
    st_unload_glyphs(char_count, glyphs);
    
    free(positions);
    free(glyphs);
    free(glyph_indices);
    free(ucs4);
}
