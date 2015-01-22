
#if !defined(SceneFilter_H)
#define SceneFilter_H

#include "IImageFilter.h"
#include "GradientFilter.h"
#include "SaturationModifyFilter.h"
#include "ImageBlender.h"

namespace HM_ImageFilter{

class SceneFilter : public IImageFilter{

private:
	 GradientFilter gradientFx;
     SaturationModifyFilter saturationFx;

public:

	 SceneFilter(float angle, Gradient gradient)
     {
		 gradientFx.gradient = gradient;
         gradientFx.OriginAngleDegree = angle;

         saturationFx.SaturationFactor = -0.6f;
     };

	 virtual Image process(Image imageIn)
	 {
		 Image clone = imageIn.clone();
         imageIn = gradientFx.process(imageIn);
         ImageBlender blender;
         blender.Mode = BlendMode::Subractive;
         imageIn = saturationFx.process(blender.Blend(clone, imageIn));

#ifndef WIN32 //only for apple ios
		 imageIn.copyPixelsFromBuffer();
#endif
	     return imageIn;
        //return imageIn;// saturationFx.process(imageIn);
	 }
};

}// namespace 
#endif