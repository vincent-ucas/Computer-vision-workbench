#if !defined(FeatherFilter_H)
#define FeatherFilter_H

#include "IImageFilter.h"

namespace HM_ImageFilter{

class FeatherFilter : public IImageFilter{

public:
	float Size;
	//FeatherFilter(): Size(0.5){};
	FeatherFilter(float in_size = 0.5){
		Size = in_size;
	}

	virtual Image process(Image imageIn)
	{
		 int r, g, b;
    	 int width = imageIn.getWidth();
    	 int height = imageIn.getHeight();
    	 int ratio = width >  height ?  height * 32768 / width : width * 32768 /  height;

         // Calculate center, min and max
         int cx = width >> 1;
         int cy = height >> 1;
         int max = cx * cx + cy * cy;
         int min = (int)(max * (1 - Size));
         int diff = max - min;

         for (int x = 0; x < width; x++) {
             for (int y = 0; y < height; y++) {
             	  r = imageIn.getRComponent(x, y);
                  g = imageIn.getGComponent(x, y);
                  b = imageIn.getBComponent(x, y);

                  // Calculate distance to center and adapt aspect ratio
                  int dx = cx - x;
                  int dy = cy - y;
                  if (width > height){
                     dx = (dx * ratio) >> 15;
                  }
                  else{
                     dy = (dy * ratio) >> 15;
                  }
                  int distSq = dx * dx + dy * dy;
            	  float v =  ((float)distSq / diff) * 255;
                  r = (int)(r + (v));
                  g = (int)(g + (v));
                  b = (int)(b + (v));
                  r = (byte)(r > 255 ? 255 : (r < 0 ? 0 : r));
                  g = (byte)(g > 255 ? 255 : (g < 0 ? 0 : g));
                  b = (byte)(b > 255 ? 255 : (b < 0 ? 0 : b));
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