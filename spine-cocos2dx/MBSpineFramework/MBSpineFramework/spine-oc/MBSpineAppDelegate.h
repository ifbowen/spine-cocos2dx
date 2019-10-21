//
//  MBSpineAppDelegate.hpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#ifndef MBSpineAppDelegate_h_
#define MBSpineAppDelegate_h_

#include <stdio.h>
#include "cocos2d.h"
#include "MBSpineLayer.h"

class MBSpineAppDelegate: private cocos2d::Application {
public:
    MBSpineAppDelegate ();
    virtual ~MBSpineAppDelegate();
    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    
    MBSpineLayerInitFinishHandler initFinishHandler;
    
};

#endif /* MBSpineAppDelegate_h_ */
