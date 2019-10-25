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
    
    void setSkinFile (std::string slot, std::string file);
    
    cocos2d::Map<std::string, cocos2d::Sprite*> skinMap;
    
private:
    void replaceSkinImage (cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags, cocos2d::BlendFunc blendFunc, Slot *slot);
    
};


#endif /* MBSkeletonAnimation_h */
