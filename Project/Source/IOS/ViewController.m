//
//  ViewController.m
//  ProjectIOS2
//
//  Created by Alexander Koptyaev on 25/01/2018.
//  Copyright © 2018 Alexander Koptyaev. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"
#import "GLView.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <QuartzCore/QuartzCore.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 1. Create an Xcode Project
    
    // 2. Create a Context
    EAGLContext *context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    [EAGLContext setCurrentContext:context];
    
    // 3. Create a View
    GLView *glView = [[[GLView alloc] initWithFrame:CGRectMake(0, 0, 320, 640)] autorelease];
    //GLView *glView = [[[GLView alloc] initWithFrame:boundsWindow] autorelease];
    [self.view addSubview:glView];
    
    // 4. Create a Renderbuffer
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)glView.layer];
    
    // 5. Create a Framebuffer
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    
    // 6. Draw!
    glClearColor(0.3, 0.6, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 7. Transfer the pixels to our view
    [context presentRenderbuffer:GL_RENDERBUFFER];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end

