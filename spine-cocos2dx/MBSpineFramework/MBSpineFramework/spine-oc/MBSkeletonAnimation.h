//
//  MBSkeletonAnimation.h
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/24.
//  Copyright © 2019 inke. All rights reserved.
//

#ifndef MBSkeletonAnimation_h
#define MBSkeletonAnimation_h

#include <spine/spine-cocos2dx.h>

using namespace spine;

class MBSkeletonAnimation : public spine::SkeletonAnimation {
public:
    MBSkeletonAnimation();
    virtual ~MBSkeletonAnimation();
    
    static MBSkeletonAnimation* createWithJsonFile (const std::string& skeletonJsonFile, const std::string& atlasFile, float scale = 1);
    virtual void draw (cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags) override;
        
    cocos2d::Map<std::string, cocos2d::Texture2D*> skinMap;
    
private:
    void replaceSkinImage(Slot *slot);
    
};


#endif /* MBSkeletonAnimation_h */
