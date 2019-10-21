//
//  MBSpineAppDelegate.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#include "MBSpineAppDelegate.h"

#include <vector>
#include <string>
#include "MBSpineMacros.h"
#include <spine/SkeletonTwoColorBatch.h>

USING_NS_CC;
using namespace std;

spine::DebugExtension debugExtension(spine::SpineExtension::getInstance());

MBSpineAppDelegate::MBSpineAppDelegate() {
    
}

MBSpineAppDelegate::~MBSpineAppDelegate() {
    spine::SkeletonBatch::destroyInstance();
    spine::SkeletonTwoColorBatch::destroyInstance();
    debugExtension.reportLeaks();
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
    
    vector<string> searchPath;
    searchPath.push_back("common");
    FileUtils::getInstance()->setSearchPaths(searchPath);
    
    Scene* scene = MBSpineLayer::scene(animationBundlePath, animationName, completionHandler);
    director->runWithScene(TransitionFade::create(0.02, scene));
    
    return true;
}

void MBSpineAppDelegate::applicationDidEnterBackground() {
}

void MBSpineAppDelegate::applicationWillEnterForeground() {
}
