
#if !defined(IImageFilter_H)
#define IImageFilter_H

//typedef BYTE UInt8; //BYTE此变量在IOS系统中会变成UInt8;
//typedef CImage UIImage;//CImage此变量在IOS系统中会变成UIImage;
#include "Image.h"
#include "math.h"
#include <vector>

#define   LIB_PI   3.14159265358979323846

namespace HM_ImageFilter{

class Point
{
public:
	   float X;
	   float Y;
	   Point(){
		   X = 0;
		   Y = 0;
	   }

	   Point(float x, float y){
		   X = x;
		   Y = y;
	   }
};


class Color{
public:
	int R;
	int G;
	int B;
	Color(){}

	Color(int r, int g, int b) : R(r), G(g), B(b){}

	Color(int rgb){
		R = ((rgb& 0x00FF0000) >> 16);
	    G = ((rgb& 0x0000FF00) >> 8);
		B = ( rgb& 0x000000FF);
	}

	static int rgb(int r, int g, int b){
		return (255 << 24) + (r << 16) + (g << 8) + b;
	}
};

class IImageFilter{
	
public:
    virtual Image process(Image image){
		return image;
	}

	//-------------------------------------------------------------------------------------
	// basic function
	//-------------------------------------------------------------------------------------
	// bound in [tLow, tHigh]
	template<class T> inline T FClamp (const T& t, const T& tLow, const T& tHigh)
	{
		if (t < tHigh)
		{
			return ((t > tLow) ? t : tLow) ;
		}
		return tHigh ;
	}
	inline int FClamp0255(const double d)
	{
		return (int)(FClamp(d, 0.0, 255.0) + 0.5) ;
	};

	virtual ~IImageFilter() {}
};

}// namespace HM
#endif
