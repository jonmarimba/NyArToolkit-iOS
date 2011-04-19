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

#import "NYArToolkitAppDelegate.h"
#import "teapot.h"

#import <QuartzCore/QuartzCore.h>
#import "CameraController.h"
#import "GraphicsView.h"

#define kTeapotScale                6.0
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

@implementation NYArToolKitAppDelegate

@synthesize window;

const GLfloat spriteTexcoords[] = {0.65625f, 0.6171875f,
	                               0.65625f, 0.3828125f,   
	                               0.34375f, 0.6171875f,   
								   0.34375f, 0.3828125f};

const GLfloat spriteVertices[] =  {0,0,0,   
								   1,0,0,   
								   0,1,0,   
	                               1,1,0};

const GLfloat colorArray[] = {1,0,0,1,   
	                          0,1,0,1,   
							  0,0,1,1,   
							  1,0,1,1};

const GLfloat axisColorArray[] = {1,0,0,1, 
	                              1,0,0,1,   
	                              0,1,0,1, 
	                              0,1,0,1,   
	                              0,0,1,1, 
	                              0,0,1,1};

const GLfloat axisVertices[] = {0,0,0, 
	                            1,0,0,   
	                            0,0,0, 
	                            0,1,0,   
	                            0,0,0, 
	                            0,0,1};

const GLfloat            lightAmbient[] = {0.2, 0.2, 0.2, 1.0};
const GLfloat            lightDiffuse[] = {1.0, 0.6, 0.0, 1.0};
const GLfloat            matAmbient[] = {0.6, 0.6, 0.6, 1.0};
const GLfloat            matDiffuse[] = {1.0, 1.0, 1.0, 1.0};    
const GLfloat            matSpecular[] = {1.0, 1.0, 1.0, 1.0};
const GLfloat            lightShininess = 100.0;
const GLfloat            lightPosition[] = {-1.55, -1.5, 1.5, 1.0}; 

- (void)drawView:(GraphicsView*)view;
{

    //Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrthof(0, 1, 0, 1, -1000, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// Sets up pointers and enables states needed for using vertex arrays and textures
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, spriteVertices);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, spriteTexcoords);	
	
	glBindTexture(GL_TEXTURE_2D, spriteTexture);
	glEnable(GL_TEXTURE_2D);
		
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(ogl_projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ogl_camera_matrix);
	
	
	//Draw axes....
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, axisColorArray);
    glVertexPointer(3 ,GL_FLOAT, 0, axisVertices);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnable(GL_LIGHTING);
	//-----
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPushMatrix();	
	
    glScalef(kTeapotScale, kTeapotScale, kTeapotScale);    
    {
        static GLfloat spinZ = 0.0;
        glRotatef(spinZ, 0.0, 0.0, 1.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        spinZ += 10.0;
    }
	
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);    
    glVertexPointer(3 ,GL_FLOAT, 0, teapot_vertices);
    glNormalPointer(GL_FLOAT, 0, teapot_normals);
    glEnable(GL_NORMALIZE);
	
	int	start = 0, i = 0;
   	// draw the teapot
	while(i < num_teapot_indices) {
		if(teapot_indices[i] == -1) {
			glDrawElements(GL_TRIANGLE_STRIP, i - start, GL_UNSIGNED_SHORT, &teapot_indices[start]);
			start = i + 1;
		}
		i++;
	}
	if(start < num_teapot_indices)
		glDrawElements(GL_TRIANGLE_STRIP, i - start - 1, GL_UNSIGNED_SHORT, &teapot_indices[start]);
	
	
	glPopMatrix();
}

-(void)setupView:(GraphicsView*)view
{
	NSLog(@"[%@ %@]", self, NSStringFromSelector(_cmd));

	GLfloat		zNear = -1000,
	zFar = 1000,
	fieldOfView = 60.0;
    GLfloat                    size;
    //Configure OpenGL lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
	
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
	
    //Set the OpenGL projection matrix
    glMatrixMode(GL_PROJECTION);
	
    size = zNear * tanf(DEGREES_TO_RADIANS(fieldOfView) / 2.0);
    CGRect rect = view.bounds;
    glFrustumf(-size, size, -size / (rect.size.width / rect.size.height), size / (rect.size.width / rect.size.height), zNear, zFar);
    glViewport(0, 0, rect.size.width, rect.size.height);
		
    
    //Make the OpenGL modelview matrix the default
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//2D texture setup
	//----------------
	glGenTextures(1, &spriteTexture);
	glBindTexture(GL_TEXTURE_2D, spriteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);	
	
	// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// Enable use of the texture
	glEnable(GL_TEXTURE_2D);	
}

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
	   fromConnection:(AVCaptureConnection *)connection 
{ 	
	NSAssert([NSThread isMainThread], @"This won't work in the background as written.");
	[glView startAnimation];

	CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
	CVPixelBufferLockBaseAddress(imageBuffer,0); 
	uint8_t *pixelData /*tmp*/ = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
	
	size_t videoWidth = CVPixelBufferGetWidth(imageBuffer); 
    size_t videoHeight = CVPixelBufferGetHeight(imageBuffer);  
    size_t xOffset = (size_t) floorf((float)((2048 - videoWidth)) * .5);
    size_t yOffset = (size_t) floorf((float)((2048 - videoHeight)) * .5);
	
	glBindTexture(GL_TEXTURE_2D, spriteTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, videoWidth, videoHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixelData);
	
	if(![nyartwrapper wasInit]) {
		[nyartwrapper initNyARTwithWidth:videoWidth andHeight:videoHeight];
	}
	[nyartwrapper getProjectionMatrix:ogl_projection_matrix];
	[nyartwrapper setNyARTBuffer:pixelData];
	[nyartwrapper detectMarker:ogl_camera_matrix];
	
	CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	NSLog(@"[%@ %@]", self, NSStringFromSelector(_cmd));

	ctad = [[CameraController alloc] init];
	
    CGRect rect = [[UIScreen mainScreen] bounds];
    
    //Create a full-screen window
    window = [[UIWindow alloc] initWithFrame:rect];
    
    //Create the OpenGL ES view and add it to the window
    glView = [[GraphicsView alloc] initWithFrame:rect];
	
    glView.delegate = self;
    glView.animationFrameInterval = 2;
	
	[window addSubview:glView];
    
    //Show the window
    [window makeKeyAndVisible];
	
	nyartwrapper = [[NyARToolkitWrapper alloc] init];
	[ctad setForwardDelegate:self];
	
	[ctad beginCapture];
}

- (void)dealloc {
	[glView stopAnimation];
	[glView release];
    [window release];
	[nyartwrapper release];
    [super dealloc];
}


@end
