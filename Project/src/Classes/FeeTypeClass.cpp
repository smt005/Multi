#include "FeeTypeClass.h"
#include <cmath>
#include<fstream>
#include<iostream>

#include "ft2build.h"
#include FT_FREETYPE_H

FeeTypeClass::FeeTypeClass()
{
	
}

FeeTypeClass::~FeeTypeClass()
{
	
}

void FeeTypeClass::init()
{
	
}

void FeeTypeClass::main()
{
    FT_Library    library;
    FT_Face       face;
    
    FT_GlyphSlot  slot;
    FT_Matrix     matrix;                 /* transformation matrix */
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;
    
    char*         filename = 0;
    char*         text = "ttf";
    
    double        angle;
    int           target_height;
    int           n, num_chars;
    
#ifdef BUILD_WIN
	filename = "X:/Projects/CMake/Multiplatform/Project/res/Font/luximr.ttf";
#elif BUILD_WIN2
	filename = "/Users/akoptyaev/Documents/projects/make/Multiplatform/Project/res/a.ttf";
#endif

	showText("FreeType START");
	showText(text);
	showText(filename);
    
    num_chars = 10;
    angle = (25.0 / 360) * 3.14159 * 2 * 0.00001;      /* use 25 degrees     */
    target_height = HEIGHT;
    
    error = FT_Init_FreeType(&library);              /* initialize library */
													/* error handling omitted */
    if (error)
    {
		stopFunction("error FT_Init_FreeType");
        return;
    }
    
    error = FT_New_Face(library, filename, 0, &face);/* create face object */
    /* error handling omitted */
    if (error)
    {
		stopFunction("error FT_New_Face");
        return;
    }
    /* use 50pt at 100dpi */
    error = FT_Set_Char_Size(face, 50 * 64, 0,
                             100, 0);                /* set character size */
    /* error handling omitted */
    if (error)
    {
		stopFunction("error FT_Set_Char_Size");
        return;
    }
    
    slot = face->glyph;
    
    /* set up matrix */
    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
    
    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (300,200) relative to the upper left corner  */
    pen.x = 300 * 64;
    pen.y = (target_height - 200) * 64;
    
    for (n = 0; n < num_chars; n++)
    {
        /* set transformation */
        FT_Set_Transform(face, &matrix, &pen);
        
        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
        if (error)
            continue;                 /* ignore errors */
        
        /* now, draw to our target surface (convert position) */
        draw_bitmap(&slot->bitmap,
                    slot->bitmap_left,
                    target_height - slot->bitmap_top);
        
        /* increment pen position */
        pen.x += slot->advance.x / 1000;
        pen.y += slot->advance.y / 1000;
    }
    
    show_image();
    
    FT_Done_Face(face);
    FT_Done_FreeType(library);

	showText("FreeType END");
}

//#pragma mark private

void FeeTypeClass::draw_bitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;
    
    for (i = x, p = 0; i < x_max; i++, p++)
    {
        for (j = y, q = 0; j < y_max; j++, q++)
        {
            if (i < 0 || j < 0 ||
                i >= WIDTH || j >= HEIGHT)
                continue;
            
            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void FeeTypeClass::show_image(void)
{
    std::ofstream _WRITE_LOG("Log.txt", std::ios::app);
    int index = 0;
    
    int  i, j;
    
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            //_WRITE_LOG << (int)(image[index]);
        }

		putchar(image[i][j] == 0 ? ' ' : image[i][j] < 128 ? '+' : '*');
        putchar('\n');
    }
    
    _WRITE_LOG << std::endl;
}
