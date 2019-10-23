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

class MBSpineLayer;

typedef std::function<void()> MBPlayerCompletionHandler;
typedef std::function<void(MBSpineLayer* spineLayer)> MBSpineLayerInitFinishHandler;

class MBSpineLayer : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* scene (const MBSpineLayerInitFinishHandler& handler);
    
    CREATE_FUNC(MBSpineLayer);
    
    virtual ~MBSpineLayer();
    virtual bool init();
    void onExit();
    
    std::string spineName;
    std::string spineAnimation;
    std::string spinePath;
    MBPlayerCompletionHandler completionHandler;
    
private:
    void runSpine();
    void addAnimation();
    spine::SkeletonAnimation* _skeletonAnimation;

};

#endif /* MBSpineLayer_h_ */
