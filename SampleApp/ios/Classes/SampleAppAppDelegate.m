//
//  SampleAppAppDelegate.m
//  SampleApp
//
//  Created by PJ Cabrera on 6/26/2012.
//  Copyright 2012 PJ Cabrera. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the project nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#import "SampleAppAppDelegate.h"
#import "EmoViewController.h"
#import "EmoView.h"
#import "EmoEngine_glue.h"
#import "MyUIViewController.h"

@implementation SampleAppAppDelegate

@synthesize navController;
@synthesize viewController;
@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	
    viewController = [[EmoViewController alloc] init];
	viewController.view = [[[EmoView alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	[viewController awakeFromNib];
    
    navController = [[UINavigationController alloc] initWithRootViewController:viewController];
    navController.navigationBarHidden = YES;
	
    [self.window addSubview:[navController view]];
	
	[window makeKeyAndVisible];

	[self.viewController onLoad];
	
    return YES;
}

- (void)showMyUIView
{
    NSLOGI(@"In ObjC showMyUIView");
    MyUIViewController *vc = [[[MyUIViewController alloc] init] autorelease];
    [navController pushViewController:vc animated:YES];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	[self.viewController onLostFocus];
    [self.viewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	[self.viewController onGainedFocus];
    [self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[self.viewController onLostFocus];
    [self.viewController stopAnimation];
	[self.viewController onDispose];
}

- (void)dealloc
{
    [navController release];
    [viewController release];
    [window release];
    [super dealloc];
}

@end
