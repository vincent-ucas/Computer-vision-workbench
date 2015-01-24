
#if !defined(NoiseFilter_H)
#define NoiseFilter_H

#include "IImageFilter.h"


namespace HM_ImageFilter{

class NoiseFilter : public IImageFilter{
public:
	float Intensity;
	 
    static int getRandomInt(int a, int b) {  
    	int min = min(a, b);    
    	int max = max(a, b);
        return rand()%((b)-(a)) + (a);
    }  

	NoiseFilter() : Intensity(0.2f){};

	virtual Image process(Image imageIn)
	{
		int r, g, b;
        int num = (int)(Intensity * 32768);
        for (int x = 0; x < imageIn.getWidth(); x++) {
            for (int y = 0; y < imageIn.getHeight(); y++) {
                 r = imageIn.getRComponent(x, y);
                 g = imageIn.getGComponent(x, y);
                 b = imageIn.getBComponent(x, y);
                 if (num != 0){
                    int rr = getRandomInt(-255, 0xff) * num;
                    int gg = getRandomInt(-255, 0xff) * num;
                    int bb = getRandomInt(-255, 0xff) * num;
                    int rrr = r + (rr >> 15);
                    int ggg = g + (gg >> 15);
                    int bbb = b + (bb >> 15);
                    r = SAFECOLOR(rrr);
                    g = SAFECOLOR(ggg);
                    b = SAFECOLOR(bbb);
                 }              
                 imageIn.setPixelColor(x,y,r,g,b);
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