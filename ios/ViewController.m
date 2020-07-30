//
//  ViewController.m
//  idle
//
//  Created by Zack Seliger on 7/29/20.
//  Copyright © 2020 Zack Seliger. All rights reserved.
//

#import "ViewController.h"
#include "game.h"

@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)dealloc {
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)setupGL {
    [EAGLContext setCurrentContext:self.context];
    //TODO SETUP HERE
    
    glSetup([[self view] bounds].size.width, [[self view] bounds].size.height);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    //TODO DRAW HERE
    
    glRender();
}

@end
