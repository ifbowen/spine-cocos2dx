//
//  MBSpineLayer.hpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#ifndef MBSpineLayer_h_
#define MBSpineLayer_h_

#include <stdio.h>
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

typedef std::function<void()> MBPlayerCompletionHandler;

class MBSpineLayer : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* scene (const std::string& bundlePath, const std::string& animationName, const MBPlayerCompletionHandler& handler);
    
    CREATE_FUNC(MBSpineLayer);
    
    virtual ~MBSpineLayer();
    virtual bool init();
    
    void setAnimationBundlePath(const std::string& bundlePath);
    void setAnimationName(const std::string& animationName);
    
    void setPlayerCompleteCallback(const MBPlayerCompletionHandler& handler);
    
private:
    void runSpine();
    void addAnimation();
    
    spine::SkeletonAnimation* _skeletonAnimation;
    std::string _animationBundlePath;
    std::string _animationName;
    MBPlayerCompletionHandler _completionHandler;

};

#endif /* MBSpineLayer_h_ */
