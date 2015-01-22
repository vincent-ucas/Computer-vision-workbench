
#if !defined(HslColorH)
#define HslColorH

#include <vector>

#include "IImageFilter.h"
#include "Image.h"

namespace HM_ImageFilter{
class HslColor
{

public:
	float h;
    float l;
    float s;

	HslColor():h(0), s(0), l(0){};
    HslColor(float _h, float _s, float _l)
    {
        h = _h;
        s = _s;
        l = _l;
    }

        
private:
	// HSL to RGB helper routine
	static double Hue_2_RGB(double v1, double v2, double vH)
    {
        if (vH < 0)
            vH += 1;
        if (vH > 1)
            vH -= 1;
        if ((6 * vH) < 1)
            return (v1 + (v2 - v1) * 6 * vH);
        if ((2 * vH) < 1)
            return v2;
        if ((3 * vH) < 2)
            return (v1 + (v2 - v1) * ((2.0 / 3) - vH) * 6);
        return v1;
    }

	static float HueToRgb(float t1, float t2, float h)
    {
        if (h < 0)
        {
            h++;
        }
        if (h > 1)
        {
            h--;
        }
        if ((6 * h) < 1)
        {
            return (t1 + (((t2 - t1) * 6) * h));
        }
        if ((2 * h) < 1)
        {
            return t2;
        }
        if ((3 * h) < 2)
        {
            return (t1 + (((t2 - t1) * (0.6666667f - h)) * 6));
        }
        return t1;
    }

public:
    static int HslToRgb(HslColor hsl, int &r, int &g, int &b)
    {
        if (hsl.h == 0)
        {
            // gray values
            r = g = b = (BYTE)(hsl.l * 255);
        }
        else
        {
            double v1, v2;
            double hue = (double)hsl.h / 360;

            v2 = (hsl.l < 0.5) ?
                (hsl.l * (1 + hsl.s)) :
                ((hsl.l + hsl.s) - (hsl.l * hsl.s));
            v1 = 2 * hsl.l - v2;

            r = (byte)(255 * Hue_2_RGB(v1, v2, hue + (1.0 / 3)));
            g = (byte)(255 * Hue_2_RGB(v1, v2, hue));
            b = (byte)(255 * Hue_2_RGB(v1, v2, hue - (1.0 / 3)));
        }
        return (255 << 24) + (r << 16) + (g << 8) + b;
    }

       

    HslColor Interpolate(HslColor c2, float amount)
    {
        return HslColor(h + ((c2.h - h) * amount), s + ((c2.s - s) * amount), l + ((c2.l - l) * amount));
    }

    static void RgbToHsl(int color, HslColor hsl)
    {
        RgbToHsl(0xff & (color >> 0x10), 0xff & (color >> 8), 0xff & color, hsl);
    }

    static HslColor RgbToHsl(int rr, int gg, int bb, HslColor &hsl)
    {
        double r = (rr / 255.0);
        double g = (gg / 255.0);
        double b = (bb / 255.0);

        double min = min(min(r, g), b);
        double max = max(max(r, g), b);
        double delta = max - min;

        // get luminance value
        hsl.l = (float)(max + min) / 2;

        if (delta == 0)
        {
            // gray color
            hsl.h = 0;
            hsl.s = 0.0f;
        }
        else
        {
            // get saturation value
            hsl.s = (float)((hsl.l < 0.5) ? (delta / (max + min)) : (delta / (2 - max - min)));

            // get hue value
            double del_r = (((max - r) / 6) + (delta / 2)) / delta;
            double del_g = (((max - g) / 6) + (delta / 2)) / delta;
            double del_b = (((max - b) / 6) + (delta / 2)) / delta;
            double hue;

            if (r == max)
                hue = del_b - del_g;
            else if (g == max)
                hue = (1.0 / 3) + del_r - del_b;
            else
                hue = (2.0 / 3) + del_g - del_r;

            // correct hue if needed
            if (hue < 0)
                hue += 1;
            if (hue > 1)
                hue -= 1;

            hsl.h = (int)(hue * 360);	
        }
		return hsl;
    }
    
};

}// namespace 
#endif
 
