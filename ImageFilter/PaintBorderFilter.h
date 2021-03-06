
#if !defined(PaintBorderFilter_H)
#define PaintBorderFilter_H

#include "IImageFilter.h"
#include "ImageBlender.h"

namespace HM_ImageFilter{

class PaintBorderFilter : public IImageFilter{

private:
		/// <summary>
        /// Should be in the range [0, 10].
        /// </summary>
        float Size;
        int R, G, B;

public:
        PaintBorderFilter(int color): Size(1.0f) /*color reference http://www.wescn.com/tool/color_3.html*/
        {
            R = (color & 0x00FF0000) >> 16;
            G = (color & 0x0000FF00) >> 8;
            B = (color & 0x000000FF);
        };

        PaintBorderFilter(int color, float size)
        {
            Size = size;
			R = (color & 0x00FF0000) >> 16;
            G = (color & 0x0000FF00) >> 8;
            B = (color & 0x000000FF);
        }

        //@Override
        virtual Image process(Image imageIn)
        {
            int r, g, b;
            int width = imageIn.getWidth();
            int height = imageIn.getHeight();
            int ratio = width > height ? height * 32768 / width : width * 32768 / height;

            // Calculate center, min and max
            int cx = width >> 1;
            int cy = height >> 1;
            int max = cx * cx + cy * cy;
            int min = (int)(max * (1 - Size));
            int diff = max - min;
            Image clone = imageIn.clone();
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    // Calculate distance to center and adapt aspect ratio
                    int dx = cx - x;
                    int dy = cy - y;
                    if (width > height)
                        dx = (dx * ratio) >> 15;
                    else
                        dy = (dy * ratio) >> 15;
                    int distSq = dx * dx + dy * dy;

                    r = (int)((((float)distSq / diff) * R));
                    g = (int)((((float)distSq / diff) * G));
                    b = (int)((((float)distSq / diff) * B));
                    r = (r > R ? R : (r < 0 ? 0 : r));
                    g = (g > G ? G : (g < 0 ? 0 : g));
                    b = (b > B ? B : (b < 0 ? 0 : b));
                    imageIn.setPixelColor(x, y, r, g, b);
                }
            }
            ImageBlender blender;
            blender.Mode = BlendMode::Additive;
		    imageIn = blender.Blend(clone, imageIn);
#ifndef WIN32 //only for apple ios
			imageIn.copyPixelsFromBuffer();
#endif
            return clone;
        }
};

}// namespace
#endif