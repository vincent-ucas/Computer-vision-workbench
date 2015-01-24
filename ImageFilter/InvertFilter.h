
#if !defined(INVERTFILTER_H)
#define INVERTFILTER_H

#include "IImageFilter.h"


namespace HM_ImageFilter{

class InvertFilter : public IImageFilter{

public:

	InvertFilter(){};

	virtual Image process(Image imageIn)
	{
		  int r, g, b;
		  for(int x = 0 ; x < imageIn.getWidth() ; x++){
			  for(int y = 0 ; y < imageIn.getHeight(); y++){
				   r = 255 - imageIn.getRComponent(x, y);
				   g = 255 - imageIn.getGComponent(x, y);
				   b = 255 - imageIn.getBComponent(x, y);
				   imageIn.setPixelColor(x, y, r, g, b);
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