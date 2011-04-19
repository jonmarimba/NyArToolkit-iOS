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

#import <Foundation/Foundation.h>

@interface NyARToolkitWrapper : NSObject {
	CGContextRef cgctx;
	int ctxWidth;
	int ctxHeight;
	CGContextRef texCtx;
	
	bool wasInit;
}

@property (nonatomic,readonly) CGContextRef cgctx;
@property (nonatomic,readonly) CGContextRef texCtx;
@property (nonatomic,readonly) int ctxWidth;
@property (nonatomic,readonly) int ctxHeight;
@property (nonatomic,assign) bool wasInit;

unsigned char* load_file_image(const char* i_filename);
int _wrapper_main(int argc,char* argv[]);

CGContextRef CreateARGBBitmapContextWithWidthAndHeight(int pixelsWide, int pixelsHigh);
CGContextRef CreateARGBBitmapContext (CGImageRef inImage);

-(void)doMain;
-(void)initNyARTwithWidth:(int)width andHeight:(int)height;
-(bool)detectMarker:(float[])resultMat;
-(void)setNyARTWidth:(int)width andHeight:(int)height;
-(void)setNyARTBuffer:(Byte*)buf;
-(void)detectMarkerWithIamge:(CGImageRef)inImage intoMatrix:(float[])m;
-(void)getProjectionMatrix:(float[])m;

@end
