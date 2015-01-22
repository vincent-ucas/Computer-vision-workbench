
#if !defined(SoftGlowFilter_H)
#define SoftGlowFilter_H

#include "IImageFilter.h"
#include "BrightContrastFilter.h"
#include "GaussianBlurFilter.h"

namespace HM_ImageFilter{

class SoftGlowFilter : public IImageFilter{

private:
	BrightContrastFilter contrastFx;
    
    GaussianBlurFilter gaussianBlurFx;

public:

	SoftGlowFilter(int nSigma, float nBrightness, float nContrast){
		contrastFx.BrightnessFactor = nBrightness;
        contrastFx.ContrastFactor = nContrast; 
        gaussianBlurFx.Sigma = nSigma; 
	};

	virtual Image process(Image imageIn)
	{
		Image clone = imageIn.clone();
		imageIn = gaussianBlurFx.process(imageIn);
        imageIn = contrastFx.process(imageIn);
         
		int old_r, old_g, old_b, r, g, b;
		for(int x = 0 ; x < (imageIn.getWidth() - 1) ; x++){
			for(int y = 0 ; y < (imageIn.getHeight() - 1) ; y++){
				   old_r = clone.getRComponent(x, y);
				   old_g = clone.getGComponent(x, y);
				   old_b = clone.getBComponent(x, y);

				   r = 255 - (255 - old_r)*(255 - imageIn.getRComponent(x, y))/255 ;
				   g = 255 - (255 - old_g)*(255 - imageIn.getGComponent(x, y))/255 ;
				   b = 255 - (255 - old_b)*(255 - imageIn.getBComponent(x, y))/255 ;
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