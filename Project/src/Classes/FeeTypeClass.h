#ifndef FeeTypeClass_H
#define FeeTypeClass_H


#include "SampleClass.h"
#include "freetype/fttypes.h"

class FeeTypeClass : public SampleClass
{
public:
	FeeTypeClass();
	~FeeTypeClass();
	void init();
    void main();
    
public:
	// h = 480 x w = 640
    int WIDTH = 60;
    int HEIGHT = 40;
    unsigned char image[40][60];
    
private:
    void draw_bitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
    void show_image(void);
    
};

#endif
