#if !defined(HslModifyFilter_H)
#define HslModifyFilter_H

#include "IImageFilter.h"
#include "HslColor.h"

namespace HM_ImageFilter{

class HslModifyFilter : public IImageFilter{
private:
	 float HueFactor;//É«µ÷
    
public:

	/// <summary>
    /// initial value setting reference to http://blog.csdn.net/yacper/article/details/4743014
    /// </summary>
    /// <param name="HueFactor"></param>
	HslModifyFilter(float hueFactor)
    {
        HueFactor = max(0, min(359, hueFactor));
    };

	virtual Image process(Image imageIn)
	{
		int r, g, b;
		HslColor hsl(HueFactor, 0, 0);
          
		for (int x = 0; x < imageIn.getWidth(); x++) {
			for (int y = 0; y < imageIn.getHeight(); y++) {
				r = imageIn.getRComponent(x, y);
				g = imageIn.getGComponent(x, y);
				b = imageIn.getBComponent(x, y);

				hsl = HslColor::RgbToHsl(r, g, b, hsl);
				hsl.h = HueFactor;
				int color = HslColor::HslToRgb(hsl, r, g, b);
				imageIn.setPixelColor(x, y, color);
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