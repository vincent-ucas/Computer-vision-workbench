#if !defined(SharpFilter_H)
#define SharpFilter_H

#include "IImageFilter.h"


namespace HM_ImageFilter{

class SharpFilter : public IImageFilter{

private:
	int _step;
public:

	SharpFilter():_step(1) {};

	SharpFilter(int step):_step(step) {};

	virtual Image process(Image imageIn)
	{
		int height =imageIn.getHeight();
		int width =imageIn.getWidth();
		Image clone = imageIn.clone();
		imageIn.clearImage(0xffffff);
        
        //拉普拉斯模板
        int Laplacian[9] ={ -1, -1, -1, -1, 8 +_step, -1, -1, -1, -1 };
        for (int x = 1; x < width - 1; x++)
		{
            for (int y = 1; y < height - 1; y++)
            {
                int r = 0, g = 0, b = 0;
                int Index = 0;
                for (int col = -1; col <= 1; col++)
				{
                    for (int row = -1; row <= 1; row++)
                    {
						int	rr = clone.getRComponent(x + row, y + col);
						int	gg = clone.getGComponent(x + row, y + col);
						int	bb = clone.getBComponent(x + row, y + col);

                        r += rr * Laplacian[Index];
                        g += gg * Laplacian[Index];
                        b += bb * Laplacian[Index];
                        Index++;
                    }
				}
				imageIn.setPixelColor(x-1, y-1, SAFECOLOR(r), SAFECOLOR(g), SAFECOLOR(b));
            }
	    }
#ifndef WIN32 //only for apple ios
		imageIn.copyPixelsFromBuffer();
#endif
		return imageIn;
	}
};

}// namespace 
#endif