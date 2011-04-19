/*
Copyright (c) 2009, Roy S.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Roy S. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Roy S. ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Roy S. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#import "NyARToolkitWrapper.h"
#define CODE_FILE "data/patt.hiro"
#define DATA_FILE "data/320x240ABGR.raw"
#define CAMERA_FILE "data/camera_para.dat"

#include "NyAR_core.h"

#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;
using namespace NyARToolkitCPP;

NyARCode* code=NULL;
NyARRgbRaster_BGRA* ra;
NyARSingleDetectMarker* ar;
NyARParam ap;

void arglCameraFrustumRH(const NyARParam& cparam, const double focalmin, const double focalmax, double m_projection[16]);

void toCameraViewRH(NyARTransMatResult mat, float ogl_result[]);

@implementation NyARToolkitWrapper
@synthesize cgctx,ctxWidth,ctxHeight,texCtx,wasInit;

-(void)doMain {
	_wrapper_main(0, NULL);
}

unsigned char* load_file_image(const char* i_filename)
{
	ifstream inf;
	unsigned char* result=NULL;
	try {
		inf.open(i_filename,ios::binary|ios::in);
		int size=320*240*4;
		result=new unsigned char[size];
		inf.read((char*)result,size);
		if(size!=inf.gcount()){
			throw exception();
		}
		inf.close();
	} catch(exception e){
		if(result!=NULL){
			delete result;
		}
		throw;
	}
	return result;
}

-(void)initNyARTwithWidth:(int)width andHeight:(int)height {
	ap.setEndian(TNyAREndian_LITTLE);
	
	NSString* str = [[NSBundle mainBundle] pathForResource:@"camera_para" ofType:@"dat"];
	//NSLog(str);
	char cstr[512] = {0};
	[str getCString:cstr maxLength:512 encoding:NSASCIIStringEncoding];
	
	ap.loadARParamFromFile(cstr);
	ap.changeScreenSize(width, height);
	
//	unsigned char* buf;
	
	code= new NyARCode(16, 16);
	
	str = [[NSBundle mainBundle] pathForResource:@"patt" ofType:@"hiro"];
	//NSLog(str);
	[str getCString:cstr maxLength:512 encoding:NSASCIIStringEncoding];
	
	code->loadARPattFromFile(cstr);
	
	
	ra = new NyARRgbRaster_BGRA(width, height);

	
	ar = new NyARSingleDetectMarker(ap, code, 80.0);
	code=NULL;
	ar->setContinueMode(false);
	
	wasInit = true;
}

-(void)setNyARTBuffer:(Byte*)buf {
	ra->setBuffer(buf);
}

-(void)setNyARTWidth:(int)width andHeight:(int)height {
	ap.changeScreenSize(width,height);
	if(ra) delete ra;
	ra = new NyARRgbRaster_BGRA(width,height);
}

-(bool)detectMarker:(float[])resultMat {
	NyARTransMatResult result_mat;	
	if(ar->detectMarkerLite(*ra, 100)) {
		ar->getTransmationMatrix(result_mat);
		toCameraViewRH(result_mat, resultMat);
		return true;
	} else {
		return false;
	}

}

-(void)detectMarkerWithIamge:(CGImageRef)inImage intoMatrix:(float[])m
{
    size_t w = CGImageGetWidth(inImage);
    size_t h = CGImageGetHeight(inImage);

    if (cgctx == NULL) 
    { 
        cgctx = CreateARGBBitmapContext(inImage);
		void *data = CGBitmapContextGetData (cgctx);
		ra->setBuffer((NyAR_BYTE_t*)data);
		
		ctxWidth = w;
		ctxHeight = h;
		
		texCtx = CreateARGBBitmapContextWithWidthAndHeight(512, 512);
    }
	
	// Get image width, height. We'll use the entire image.
    CGRect rect = {{0,0},{w,h}}; 
	
	//	NSLog(@"depth: %@",CGImageGetBitsPerPixel(inImage));
	//	NSLog(@"bytes per row: %@",CGImagebytesPerRow(inImage));
	
    // Draw the image to the bitmap context. Once we draw, the memory 
    // allocated for the context for rendering will then contain the 
    // raw image data in the specified color space.
    CGContextDrawImage(cgctx, rect, inImage); 
	
	CGContextDrawImage(texCtx, rect, inImage);
	
	[self detectMarker:m];
}

CGContextRef CreateARGBBitmapContext (CGImageRef inImage)
{
	
	// Get image width, height. We'll use the entire image.
    size_t pixelsWide = CGImageGetWidth(inImage);
    size_t pixelsHigh = CGImageGetHeight(inImage);

	return CreateARGBBitmapContextWithWidthAndHeight(pixelsWide,pixelsHigh);
}

CGContextRef CreateARGBBitmapContextWithWidthAndHeight(int pixelsWide, int pixelsHigh) {
    // Declare the number of bytes per row. Each pixel in the bitmap in this
    // example is represented by 4 bytes; 8 bits each of red, green, blue, and
    // alpha.

	CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
	
    int bitmapBytesPerRow   = 4 * pixelsWide; //pixelsWide; //(pixelsWide * 4);
    int bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
	
    // Use the generic RGB color space.
    colorSpace = CGColorSpaceCreateDeviceRGB();
    if (colorSpace == NULL)
    {
        fprintf(stderr, "Error allocating color space\n");
        return NULL;
    }
	
    // Allocate memory for image data. This is the destination in memory
    // where any drawing to the bitmap context will be rendered.
    bitmapData = malloc( bitmapByteCount );
    if (bitmapData == NULL) 
    {
        fprintf (stderr, "Memory not allocated!");
        CGColorSpaceRelease( colorSpace );
        return NULL;
    }
	
    // Create the bitmap context. We want pre-multiplied ARGB, 8-bits 
    // per component. Regardless of what the source image format is 
    // (CMYK, Grayscale, and so on) it will be converted over to the format
    // specified here by CGBitmapContextCreate.
    context = CGBitmapContextCreate (bitmapData,
									 pixelsWide,
									 pixelsHigh,
									 8,      // bits per component
									 bitmapBytesPerRow,
									 colorSpace,
									 kCGImageAlphaPremultipliedLast);
    if (context == NULL)
    {
        free (bitmapData);
        fprintf (stderr, "Context not created!");
    }
	
    // Make sure and release colorspace before returning
    CGColorSpaceRelease( colorSpace );
	
    return context;
}


int _wrapper_main(int argc,char* argv[])
{
	{
		NyARParam ap;
		ap.setEndian(TNyAREndian_LITTLE);
		
		NSString* str = [[NSBundle mainBundle] pathForResource:@"camera_para" ofType:@"dat"];
		NSLog(@"%@",str);
		char cstr[512] = {0};
		[str getCString:cstr maxLength:512 encoding:NSASCIIStringEncoding];
		
		ap.loadARParamFromFile(cstr);
		ap.changeScreenSize(320, 240);
		
		NyARCode* code=NULL;
		unsigned char* buf;
		NyARRgbRaster_BGRA* ra;
		NyARSingleDetectMarker* ar;
		
		
		code= new NyARCode(16, 16);

		str = [[NSBundle mainBundle] pathForResource:@"patt" ofType:@"hiro"];
		NSLog(@"%@",str);
		[str getCString:cstr maxLength:512 encoding:NSASCIIStringEncoding];
		
		code->loadARPattFromFile(cstr);

		str = [[NSBundle mainBundle] pathForResource:@"320x240ABGR" ofType:@"raw"];
		NSLog(@"%@",str);
		[str getCString:cstr maxLength:512 encoding:NSASCIIStringEncoding];
		
		buf=load_file_image(cstr);
		
		ra = new NyARRgbRaster_BGRA(320, 240);
		ra->setBuffer(buf);
		
		ar = new NyARSingleDetectMarker(ap, code, 80.0);
		code=NULL;
		ar->setContinueMode(false);
		
		NyARTransMatResult result_mat;
		
		ar->detectMarkerLite(*ra, 100);
		ar->getTransmationMatrix(result_mat);
		printf("%s",
			   "performance test of NyARToolkit.\n"
			   "This test measures processing time of marker detection and transform matrix calculation 1000 times.\n");
		printf("Marker confidence\n cf=%f,direction=%d\n",ar->getConfidence(),ar->getDirection());
		printf("Transform Matrix\n");
		printf(
			   "% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
			   "% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
			   "% 4.8f,% 4.8f,% 4.8f,% 4.8f\n",
			   result_mat.m00,result_mat.m01,result_mat.m02,result_mat.m03,
			   result_mat.m10,result_mat.m11,result_mat.m12,result_mat.m13,
			   result_mat.m20,result_mat.m21,result_mat.m22,result_mat.m23);
		
		//DWORD st=GetTickCount();
		NSDate* start = [NSDate date];
		
		// É}Å[ÉJÅ[Çåüèo1000âÒï™ÇÃèàóùéûä‘Çåvë™
		for (int i = 0; i < 100; i++) {
			// ïœä∑çsóÒÇéÊìæ
			ar->detectMarkerLite(*ra, 100);
			ar->getTransmationMatrix(result_mat);
		}

		//printf("done.\ntotal=%u[ms]\n",GetTickCount()-st);
		printf("done.\ntotal=%.3f[s]\n",-1.0 * [start timeIntervalSinceDate:[NSDate date]]);
		delete ra;
		delete ar;
		delete buf;
	}
	return 0;
}

void toCameraViewRH(NyARTransMatResult mat, float ogl_result[]) {
	float view_scale_factor = 0.025;

	ogl_result[0 + 0 * 4] = -mat.m10; //Rightx
	ogl_result[0 + 1 * 4] = -mat.m11; //Righty
	ogl_result[0 + 2 * 4] = -mat.m12; //Rightz
	ogl_result[0 + 3 * 4] = -mat.m13; //-Eye*right
	ogl_result[1 + 0 * 4] = -mat.m00; //Upx
	ogl_result[1 + 1 * 4] = -mat.m01; //Upy
	ogl_result[1 + 2 * 4] = -mat.m02; //Upz
	ogl_result[1 + 3 * 4] = -mat.m03; //-Eye*up
	ogl_result[2 + 0 * 4] = -mat.m20; //Lookx
	ogl_result[2 + 1 * 4] = -mat.m21; //Looky
	ogl_result[2 + 2 * 4] = -mat.m22; //Lookz
	ogl_result[2 + 3 * 4] = -mat.m23; //-Eye*look
	ogl_result[3 + 0 * 4] = 0.0;
	ogl_result[3 + 1 * 4] = 0.0;
	ogl_result[3 + 2 * 4] = 0.0;
	ogl_result[3 + 3 * 4] = 1.0;
	if (view_scale_factor != 0.0) {
		ogl_result[12] *= view_scale_factor;
		ogl_result[13] *= view_scale_factor;
		ogl_result[14] *= view_scale_factor;
	}
	return;
}

static double norm( double a, double b, double c )
{
    return( sqrt( a*a + b*b + c*c ) );
}

static double dot( double a1, double a2, double a3,
				  double b1, double b2, double b3 )
{
    return( a1 * b1 + a2 * b2 + a3 * b3 );
}

int  arParamDecompMat( double source[3][4], double cpara[3][4], double trans[3][4] )
{
    int       r, c;
    double    Cpara[3][4];
    double    rem1, rem2, rem3;
	
    if( source[2][3] >= 0 ) {
        for( r = 0; r < 3; r++ ){
            for( c = 0; c < 4; c++ ){
                Cpara[r][c] = source[r][c];
            }
        }
    }
    else {
        for( r = 0; r < 3; r++ ){
            for( c = 0; c < 4; c++ ){
                Cpara[r][c] = -(source[r][c]);
            }
        }
    }
	
    for( r = 0; r < 3; r++ ){
        for( c = 0; c < 4; c++ ){
            cpara[r][c] = 0.0;
        }
    }
    cpara[2][2] = norm( Cpara[2][0], Cpara[2][1], Cpara[2][2] );
    trans[2][0] = Cpara[2][0] / cpara[2][2];
    trans[2][1] = Cpara[2][1] / cpara[2][2];
    trans[2][2] = Cpara[2][2] / cpara[2][2];
    trans[2][3] = Cpara[2][3] / cpara[2][2];
	
    cpara[1][2] = dot( trans[2][0], trans[2][1], trans[2][2],
					  Cpara[1][0], Cpara[1][1], Cpara[1][2] );
    rem1 = Cpara[1][0] - cpara[1][2] * trans[2][0];
    rem2 = Cpara[1][1] - cpara[1][2] * trans[2][1];
    rem3 = Cpara[1][2] - cpara[1][2] * trans[2][2];
    cpara[1][1] = norm( rem1, rem2, rem3 );
    trans[1][0] = rem1 / cpara[1][1];
    trans[1][1] = rem2 / cpara[1][1];
    trans[1][2] = rem3 / cpara[1][1];
	
    cpara[0][2] = dot( trans[2][0], trans[2][1], trans[2][2],
					  Cpara[0][0], Cpara[0][1], Cpara[0][2] );
    cpara[0][1] = dot( trans[1][0], trans[1][1], trans[1][2],
					  Cpara[0][0], Cpara[0][1], Cpara[0][2] );
    rem1 = Cpara[0][0] - cpara[0][1]*trans[1][0] - cpara[0][2]*trans[2][0];
    rem2 = Cpara[0][1] - cpara[0][1]*trans[1][1] - cpara[0][2]*trans[2][1];
    rem3 = Cpara[0][2] - cpara[0][1]*trans[1][2] - cpara[0][2]*trans[2][2];
    cpara[0][0] = norm( rem1, rem2, rem3 );
    trans[0][0] = rem1 / cpara[0][0];
    trans[0][1] = rem2 / cpara[0][0];
    trans[0][2] = rem3 / cpara[0][0];
	
    trans[1][3] = (Cpara[1][3] - cpara[1][2]*trans[2][3]) / cpara[1][1];
    trans[0][3] = (Cpara[0][3] - cpara[0][1]*trans[1][3]
				   - cpara[0][2]*trans[2][3]) / cpara[0][0];
	
    for( r = 0; r < 3; r++ ){
        for( c = 0; c < 3; c++ ){
            cpara[r][c] /= cpara[2][2];
        }
    }
	
    return 0;
}

-(void)getProjectionMatrix:(float[])m {
	double mat[16] = {0.0};
	arglCameraFrustumRH(ap, 1.0, 100.0, mat);
	for (int i=0; i<16; m[i] = (float)mat[i],i++);
}

void arglCameraFrustumRH(const NyARParam& cparam, const double focalmin, const double focalmax, double m_projection[16])
{
	double   icpara[3][4];
    double   trans[3][4];
    double   p[3][3], q[4][4];
	int      width, height;
    int      i, j;
	
	width  = cparam.getScreenSize()->w;
    height = cparam.getScreenSize()->h;
	
	double cparam_m[3][4] = {0.0};
	const NyARPerspectiveProjectionMatrix* proj = cparam.getPerspectiveProjectionMatrix();
	cparam_m[0][0] = proj->m00;
	cparam_m[0][1] = proj->m01;
	cparam_m[0][2] = proj->m02;
	cparam_m[0][3] = proj->m03;
	cparam_m[1][0] = proj->m10;
	cparam_m[1][1] = proj->m11;
	cparam_m[1][2] = proj->m12;
	cparam_m[1][3] = proj->m13;
	cparam_m[2][0] = proj->m20;
	cparam_m[2][1] = proj->m21;
	cparam_m[2][2] = proj->m22;
	cparam_m[2][3] = proj->m23;
	
	if (arParamDecompMat(cparam_m, icpara, trans) < 0) {
        printf("arglCameraFrustum(): arParamDecompMat() indicated parameter error.\n"); 
        return;
    }
	for (i = 0; i < 4; i++) {
        icpara[1][i] = (height - 1)*(icpara[2][i]) - icpara[1][i];
    }
	
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            p[i][j] = icpara[i][j] / icpara[2][2];
        }
    }
    q[0][0] = (2.0 * p[0][0] / (width - 1));
    q[0][1] = (2.0 * p[0][1] / (width - 1));
    q[0][2] = -((2.0 * p[0][2] / (width - 1))  - 1.0);
    q[0][3] = 0.0;
	
    q[1][0] = 0.0;
    q[1][1] = -(2.0 * p[1][1] / (height - 1));
    q[1][2] = -((2.0 * p[1][2] / (height - 1)) - 1.0);
    q[1][3] = 0.0;
	
    q[2][0] = 0.0;
    q[2][1] = 0.0;
    q[2][2] = (focalmax + focalmin)/(focalmin - focalmax);
    q[2][3] = 2.0 * focalmax * focalmin / (focalmin - focalmax);
	
    q[3][0] = 0.0;
    q[3][1] = 0.0;
    q[3][2] = -1.0;
    q[3][3] = 0.0;
	
    for (i = 0; i < 4; i++) { // Row.
		// First 3 columns of the current row.
        for (j = 0; j < 3; j++) { // Column.
            m_projection[i + j*4] = q[i][0] * trans[0][j] +
			q[i][1] * trans[1][j] +
			q[i][2] * trans[2][j];
        }
		// Fourth column of the current row.
        m_projection[i + 3*4] = q[i][0] * trans[0][3] +
		q[i][1] * trans[1][3] +
		q[i][2] * trans[2][3] +
		q[i][3];
    }   
}

- (void) dealloc
{
    CGContextRelease(cgctx); 

	[super dealloc];
}


@end
