//
//  MBSpineAppDelegate.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#include "MBSpineAppDelegate.h"
#include "MBSpineMacros.h"

USING_NS_CC;
using namespace std;

MBSpineAppDelegate::MBSpineAppDelegate() {
    
}

MBSpineAppDelegate::~MBSpineAppDelegate() {
    
}

void MBSpineAppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool MBSpineAppDelegate::applicationDidFinishLaunching() {
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#endif
    
    Scene* scene = MBSpineLayer::scene(initFinishHandler);
    director->runWithScene(TransitionFade::create(0.02, scene));
    
    return true;
}

void MBSpineAppDelegate::applicationDidEnterBackground() {
}

void MBSpineAppDelegate::applicationWillEnterForeground() {
}
