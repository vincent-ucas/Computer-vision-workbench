#if !defined(OldPhotoFilter_H)
#define OldPhotoFilter_H

#include "GaussianBlurFilter.h"
#include "NoiseFilter.h"
#include "VignetteFilter.h"
#include "GradientMapFilter.h"

namespace HM_ImageFilter{

class OldPhotoFilter : public IImageFilter{

private:
	GaussianBlurFilter blurFx;
	NoiseFilter noiseFx;
	VignetteFilter vignetteFx;
	GradientMapFilter gradientFx;

public:
	
	OldPhotoFilter(){
		blurFx.Sigma = 0.1f;		  
		noiseFx.Intensity = 0.03f;		
		vignetteFx.Size = 0.8f;		
    	gradientFx.ContrastFactor = 0.3f;
	};


	virtual Image process(Image imageIn)
	{
		imageIn = noiseFx.process(blurFx.process(imageIn));
    	imageIn = gradientFx.process(imageIn);
    	return vignetteFx.process(imageIn);
	}
};

}// namespace 
#endif