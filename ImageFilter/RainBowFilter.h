#if !defined(RainBowFilter_H)
#define RainBowFilter_H

#include "ImageBlender.h"
#include "GradientFilter.h"


namespace HM_ImageFilter{

class RainBowFilter : public IImageFilter{
private:
	ImageBlender blender;
	GradientFilter gradientFx;
	
public:
	float gradAngleDegree; 
	bool IsDoubleRainbow;
	
	RainBowFilter(){
	    IsDoubleRainbow = true;
		gradAngleDegree = 40;

		blender.Mixture = 0.25f;
		blender.Mode = ::Additive;//LinearLight;
		   
        vector<int> rainbowColors = Gradient::RainBow().MapColors;
        if (IsDoubleRainbow)
        {
			rainbowColors.erase(rainbowColors.begin() + rainbowColors.size() - 1);//remove red
			vector<int> vi = Gradient::RainBow().MapColors;
			for(vector<int>::iterator it = vi.begin(); it!=vi.end(); it++){    
				rainbowColors.push_back(*it);
			}
        }  
		gradientFx.OriginAngleDegree = gradAngleDegree;
		Gradient gradient(rainbowColors);
		gradientFx.gradient = gradient;
	};

	virtual Image process(Image imageIn)
	{
       Image clone = gradientFx.process(imageIn.clone());
       return blender.Blend(imageIn, clone);
	}
};

}// namespace 
#endif