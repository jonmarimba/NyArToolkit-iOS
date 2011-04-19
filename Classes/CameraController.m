//
//  CameraController.m
//  CameraTest
//

#import <objc/runtime.h>
#import "CameraController.h"

@interface CameraController ()

@property (nonatomic, readwrite, retain) AVCaptureSession *captureSession;

@end

@implementation CameraController

-(void)beginCapture 
{
	NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	AVCaptureDevice *camera = nil;
	for (AVCaptureDevice *device in devices) 
	{
		if ([device position] == AVCaptureDevicePositionBack) 
		{
			camera = device;
		}
	}
	
	AVCaptureDeviceInput *captureInput = [AVCaptureDeviceInput 
										  deviceInputWithDevice:camera 
										  error:nil];
	AVCaptureVideoDataOutput *captureOutput = [[AVCaptureVideoDataOutput alloc] init];
	
	/*While a frame is processes in -captureOutput:didOutputSampleBuffer:fromConnection: delegate methods 
	 no other frames are added to the queue.  If you don't want this behaviour set the property to NO */
	captureOutput.alwaysDiscardsLateVideoFrames = YES; 
	
	/*Specify a minimum duration for each frame (play with this settings to avoid having too many frames waiting
	 in the queue because it can cause memory issues). It is similar to the inverse of the maximum framerate.
	 In this example we set a min frame duration of 1/10 seconds so a maximum framerate of 10fps. We say that
	 we are not able to process more than 30 frames per second as that is what we are using to animate.*/
	
	captureOutput.minFrameDuration = CMTimeMake(1, 30);
	
	// Create a serial queue to handle the processing of our frames
	dispatch_queue_t queue;
	//queue = dispatch_queue_create("cameraQueue", NULL);
	queue = dispatch_get_current_queue();
	[captureOutput setSampleBufferDelegate:self queue:queue];
	//dispatch_release(queue);
	
	// Set the video output to store frame in BGRA as that's going to be the fomat of our texture
	NSString* key = (NSString*)kCVPixelBufferPixelFormatTypeKey; 
	NSNumber* value = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA]; 
	NSDictionary* videoSettings = [NSDictionary dictionaryWithObject:value forKey:key]; 
	[captureOutput setVideoSettings:videoSettings]; 
	
	self.captureSession = [[[AVCaptureSession alloc] init] autorelease];
	
	[captureSession addInput:captureInput];
//	if ([camera supportsAVCaptureSessionPreset:AVCaptureSessionPresetLow])
//	{
//		[captureSession setSessionPreset:AVCaptureSessionPresetLow];
//	} 
//	else 
		if ([camera supportsAVCaptureSessionPreset:AVCaptureSessionPreset640x480])
	{
		[captureSession setSessionPreset:AVCaptureSessionPreset640x480];
	}

	
	[captureSession addOutput:captureOutput];
	
	[captureOutput release];
	
	[captureSession startRunning];	
}

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
	   fromConnection:(AVCaptureConnection *)connection 
{ 	
	if (forwardDelegate && [forwardDelegate respondsToSelector:_cmd]) 
	{
		[forwardDelegate captureOutput:captureOutput
				 didOutputSampleBuffer:sampleBuffer
						fromConnection:connection];
	}
} 

@synthesize forwardDelegate;
@synthesize captureSession;
- (void) dealloc
{
	forwardDelegate = nil;
	[captureSession release];
	[super dealloc];
}



@end
