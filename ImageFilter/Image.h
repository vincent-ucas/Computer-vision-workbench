

#if !defined(Image_H)
#define Image_H

#include "../stdafx.h"
//typedef CImage UIImage;
//typedef CString string;

#define SAFECOLOR(color) min(255, max(0, color))

namespace HM_ImageFilter{

class Image 
{
private:
	int width, height;

public:
	 
#ifdef WIN32
	//format of image (jpg/png)
    CString formatName;
	 // RGB Array Color
	int* colorArray;
#else //only for apple ios
	//format of image (jpg/png)
	NSString formatName;
	 // RGB Array Color
	UInt8* colorArray;
	UInt8* destImagecolorArray;
#endif

public:
    
#ifdef WIN32
	//original bitmap image
    CImage* image;
    CImage* destImage;
#else //only for apple ios
	//original bitmap image
	CGImageRef image;
	CFDataRef   m_DataRef;
	CFDataRef   m_OutDataRef;
    CGImageRef destImage;
#endif    
   
	Image(){}; 

#ifdef WIN32
	//dimensions of image 
    Image(CImage *img){   
        image =  img;
        formatName = "jpg";
        width = img->GetWidth();
        height = img->GetHeight();
		CImage *dest(img);
	    destImage = dest;
		updateColorArray();
    };
#else //only for apple ios
	//dimensions of image 
	Image(CGImageRef img){                
        image = img;
		formatName = "jpg";
	    m_DataRef = CGDataProviderCopyData(CGImageGetDataProvider(img));  
		m_OutDataRef = CGDataProviderCopyData(CGImageGetDataProvider(img));  
		colorArray = (UInt8 *) CFDataGetBytePtr(m_DataRef);  
		destImagecolorArray = (UInt8 *) CFDataGetBytePtr(m_OutDataRef);  
	
		height = CGImageGetHeight(inImage);
		width = CGImageGetWidth(inImage);
    };
#endif   

    Image clone(){
    	return Image(image);
    }
    
    /**
     * Method to reset the image to a solid color
     * @param color - color to rest the entire image to
     */
    void clearImage(int color){
    	for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
            	setPixelColor(x, y, color);
            }
        }
    }
    
#ifdef WIN32
    /**
     * Set color array for image - called on initialisation
     * by constructor
     * 
     * @param bitmap
     */
    void updateColorArray(){
        colorArray = new int[width * height];
        int r, g, b;
        int index = 0;
		BYTE* rgb; 
		for(int y = 0 ; y < (image->GetHeight() - 1); y++){
		    for(int x = 0 ; x < (image->GetWidth() - 1); x++){
				  int index = y * width + x;
				   rgb = (BYTE*) image->GetPixelAddress(x, y);
				   r = SAFECOLOR(rgb[2]);
				   g = SAFECOLOR(rgb[1]);
				   b = SAFECOLOR(rgb[0]);
				   int rgbcolor = (r << 16) | (g << 8) | b;
				   colorArray[index] = (r << 16) | (g << 8) | b;   
				   index++;	  
			  }
		 }
    }
#endif    
    
    /**
     * Method to set the color of a specific pixel
     * 
     * @param x
     * @param y
     * @param color
     */
    void setPixelColor(int x, int y, int color){
		colorArray[((y * width+x))] = color;
#ifdef WIN32
		BYTE r  = (0xFF0000 &color) >> 16;
		BYTE g  = (0x00FF00 &color) >> 8;
		BYTE b  = 0x0000FF &color;
		destImage->SetPixelRGB(x, y, r, g, b);
#else //only for apple ios
		destImagecolorArray[((y * width+x))] = color;
#endif
    }
    

    /**
     * Get the color for a specified pixel
     * 
     * @param x
     * @param y
     * @return color
     */
    int getPixelColor(int x, int y){
        return colorArray[y*width+x];
    }
    
    /**
     * Set the color of a specified pixel from an RGB combo
     * 
     * @param x
     * @param y
     * @param c0
     * @param c1
     * @param c2
     */
    void setPixelColor(int x, int y, int r, int g, int b){
		colorArray[((y * width+x))] = (255 << 24) + (r << 16) + (g << 8) + b;
#ifdef WIN32
		destImage->SetPixelRGB(x, y, r, g, b);
#else //only for apple ios
		destImagecolorArray[((y * width+x))] = (255 << 24) + (r << 16) + (g << 8) + b;
#endif
    }

    
    void copyPixelsFromBuffer() { //��colorArray����ָ���е����ݰ󶨵�destImage
#ifdef WIN32
	    int index = 0;
		for(int x = 0 ; x < (image->GetWidth() - 1); x++){
			for(int y = 0 ; y < (image->GetHeight() - 1); y++){
				   index = y * width + x;
				   setPixelColor(x, y, colorArray[index]);
				   index++;	  
			  }
		 }
#else //only for apple ios
	    CGContextRef ctx = CGBitmapContextCreate(destImagecolorArray,  							
			                                     CGImageGetWidth(image),
												 CGImageGetHeight(image),
												 CGImageGetBitsPerComponent(image),	
												 CGImageGetBytesPerRow(image),
												 CGImageGetColorSpace(image),
												 CGImageGetBitmapInfo(image) 
												 ); 
		destImage = CGBitmapContextCreateImage(ctx);  
		//CGContextRelease(ctx);
		//UIImage *finalImage = [UIImage imageWithCGImage:destImage];
		//CGImageRelease(destImage);
		//CFRelease(m_DataRef);
        //CFRelease(m_OutDataRef);
#endif
    }
    
    /**
     * Method to get the RED color for the specified 
     * pixel 
     * @param x
     * @param y
     * @return color of R
     */
    int getRComponent(int x, int y){
         return (colorArray[((y*width+x))]& 0xFF0000) >> 16;
    }

    
    /**
     * Method to get the GREEN color for the specified 
     * pixel 
     * @param x
     * @param y
     * @return color of G
     */
    int getGComponent(int x, int y){
         return (colorArray[((y*width+x))]& 0x00FF00) >> 8;
    }


    /**
     * Method to get the BLUE color for the specified 
     * pixel 
     * @param x
     * @param y
     * @return color of B
     */
    int getBComponent(int x, int y){
         return (colorArray[((y*width+x))] & 0x0000FF);
    }

    
 
    /**
     * @return the image
     */
    CImage* getImage() {
        //return image;
    	return destImage;
    }


    /**
     * @param image the image to set
     */
    void setImage(CImage *img) {
        image = img;
    }


    /**
     * @return the formatName
     */
    CString getFormatName() {
        return formatName;
    }


    /**
     * @param formatName the formatName to set
     */
    void setFormatName(CString formatName) {
        formatName = formatName;
    }


    /**
     * @return the width
     */
    int getWidth() {
        return width;
    }


    /**
     * @param width the width to set
     */
    void setWidth(int width) {
       width = width;
    }


    /**
     * @return the height
     */
    int getHeight() {
        return height;
    }


    /**
     * @param height the height to set
     */
    void setHeight(int height) {
        height = height;
    }


    /**
     * @return the colorArray
     */
    int* getColorArray() {
        return colorArray;
    }


    /**
     * @param colorArray the colorArray to set
     */
    void setColorArray(int colors[]) {
        colorArray = colors;
    }

	void Destroy()
	{
		delete colorArray;
#ifdef WIN32
		image->Destroy();
		destImage->Destroy();
#else
		delete destImagecolorArray;
		CGImageRelease(image);
		CGImageRelease(destImage);
		CFRelease(m_DataRef);
		CFRelease(m_OutDataRef);
#endif
	}


	//����ͼƬ
#ifdef WIN32	
	static Image LoadImage(std::string imagePath){
		CImage *cimage = new CImage;
		CString filePath((CString)imagePath.c_str());
		HRESULT hresult = cimage->Load(filePath);
		if(cimage->IsNull()){
		   std::cout<<"Cannot open file.."<<std::endl;
		   return 0;
		}
		Image image(cimage);
		return image;
	}
#else
	static Image LoadImage(NSString imagePath){
		UIImage *image = [UIImage imageNamed:@imagePath];
		CGImage image = image.CGImage;
		printDateTime();
		Image image(cimage);
		return image;
	}
#endif

	//	Save images in the save path..
#ifdef WIN32
	static void SaveImage(Image image, std::string savePath)
	{
		// output the processed images	
		CString outfilePath((CString)savePath.c_str());
		HRESULT hresult = image.destImage->Save(outfilePath);
		if(FAILED(hresult)){
			std::cout<<"Sorry.. File saving failed"<<std::endl;
			return ;
		}
		image.image->Destroy();
		image.Destroy();
		//printDateTime();
		//string s = "";char* p = (char*)s.c_str();cin>>p;
	}
#endif

};

}// namespace HM
#endif