//
//  CameraController.h
//  CameraTest
//

#import <UIKit/UIKit.h>

#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

@interface CameraController : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureSession *captureSession;
    id <AVCaptureVideoDataOutputSampleBufferDelegate> forwardDelegate;
}

@property (nonatomic, readonly, retain) AVCaptureSession *captureSession;
@property (nonatomic, assign)id <AVCaptureVideoDataOutputSampleBufferDelegate> forwardDelegate;
-(void)beginCapture;

@end

