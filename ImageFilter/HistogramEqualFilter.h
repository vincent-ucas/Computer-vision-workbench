
#if !defined(HistogramEqualFilter_H)
#define HistogramEqualFilter_H

#include <vector>
#include "IImageFilter.h"
#include "HslColor.h"

namespace HM_ImageFilter{

class HistogramEqualFilter : public IImageFilter{
public:
	float ContrastIntensity ;

	HistogramEqualFilter():ContrastIntensity(1.0){};

	HistogramEqualFilter(float in_contrast){
		ContrastIntensity = in_contrast;
	};
	
	/************************************************************************/
	/*    more details can be found:
	/*		http://en.wikipedia.org/wiki/Histogram_equalization             */
	/************************************************************************/
	virtual Image process(Image imageIn)
	{
		int r, g, b;
		//HslColor hsl (0, 0, 0);
		int _array[256] = {0};
		std::vector<HslColor> hslArray(imageIn.getHeight()*imageIn.getWidth());
		std::vector<int> idxArray(imageIn.getHeight()*imageIn.getWidth());
		int contrast = (int) (ContrastIntensity * 255);
		int pos = 0;
		for (int x = 0; x < imageIn.getWidth(); x++) {
			for (int y = 0; y < imageIn.getHeight(); y++) {
				r = imageIn.getRComponent(x, y);
				g = imageIn.getGComponent(x, y);
				b = imageIn.getBComponent(x, y);

				HslColor::RgbToHsl(r, g, b, hslArray[pos]);

				int index = hslArray[pos].s * 255;
				_array[index]++;
				idxArray[pos] = index;
				pos++;
			}
		}
		for (int i = 1; i < 0x100; i++){
			_array[i] += _array[i - 1];
		}
		int _arrayMin = _array[0];
		for (int i = 0; i < 0x100; i++){
			_array[i] = (float)(_array[i]-_arrayMin) / (imageIn.getHeight()*imageIn.getWidth() - _arrayMin) * 255;
			_array[i] = ((contrast * _array[i]) >> 8) + (((0xff - contrast) * i) >> 8);
		}
		pos = 0;
		for (int x = 0; x < imageIn.getWidth(); x++) {
			for (int y = 0; y < imageIn.getHeight(); y++) {
				r = imageIn.getRComponent(x, y);
				g = imageIn.getGComponent(x, y);
				b = imageIn.getBComponent(x, y);
				if (idxArray[pos] != 0)
				{
 					int num = _array[idxArray[pos]];
					hslArray[pos].s = hslArray[pos].s*num/idxArray[pos];
					HslColor::HslToRgb(hslArray[pos], r, g, b);

					r = (r > 0xff) ? ((byte) 0xff) : ((r < 0) ? ((byte) 0) : ((byte) r));
					g = (g > 0xff) ? ((byte) 0xff) : ((g < 0) ? ((byte) 0) : ((byte) g));
					b = (b > 0xff) ? ((byte) 0xff) : ((b < 0) ? ((byte) 0) : ((byte) b));
				}    
				imageIn.setPixelColor(x,y,r,g,b);
				pos++;
			}
		}
#ifndef WIN32 //only for apple ios
		imageIn.copyPixelsFromBuffer();
#endif
        return imageIn;
	}
};

}
#endif