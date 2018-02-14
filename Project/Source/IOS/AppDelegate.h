//
//  AppDelegate.h
//  OpenGL
//
//  Created by Anton Holmquist on 6/11/12.
//  Copyright (c) 2012 Anton Holmquist. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLView.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    GLView* _glView;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;
@property (strong, retain) IBOutlet GLView *glView;

@end
