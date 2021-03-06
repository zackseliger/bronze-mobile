//
//  ViewController.m
//  idle
//
//  Created by Zack Seliger on 7/29/20.
//  Copyright © 2020 Zack Seliger. All rights reserved.
//

#import "ViewController.h"
#include "game.h"
#include "application.h"
#include "event.h"

@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end

@implementation ViewController

// destructor
- (void)dealloc {
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

// setup
- (void)setupGL {
    [EAGLContext setCurrentContext:self.context];
    getCurrentApplication()->context->init();
    getCurrentApplication()->init();
}

// on load
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // get openGL context
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
    // setup listener to look for orientation change
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter]
       addObserver:self selector:@selector(orientationChanged:)
       name:UIDeviceOrientationDidChangeNotification
       object:[UIDevice currentDevice]];
}

// orientation change
- (void)orientationChanged:(NSNotification *)note {
  emitEvent(WindowResize, new WindowEvent([[self view] bounds].size.width, [[self view] bounds].size.height));
}

// update
- (void)update {
  getCurrentApplication()->update();
}

// render
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
  getCurrentApplication()->render();
}

// touch events
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch* touch in touches) {
      emitEvent(TouchStart, new TouchEvent((int)(size_t)touch.estimationUpdateIndex, [touch locationInView:self.view].x, [touch locationInView:self.view].y));
    }
}
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch* touch in touches) {
      emitEvent(TouchEnd, new TouchEvent((int)(size_t)touch.estimationUpdateIndex, [touch locationInView:self.view].x, [touch locationInView:self.view].y));
    }
}
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch* touch in touches) {
      emitEvent(TouchMove, new TouchEvent((int)(size_t)touch.estimationUpdateIndex, [touch locationInView:self.view].x, [touch locationInView:self.view].y));
    }
}
-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch* touch in touches) {
      emitEvent(TouchEnd, new TouchEvent((int)(size_t)touch.estimationUpdateIndex, [touch locationInView:self.view].x, [touch locationInView:self.view].y));
    }
}

@end
