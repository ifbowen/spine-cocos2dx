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
#include "MBSkeletonAnimation.h"

class MBSpineLayer;

typedef std::function<void()> MBPlayerEventHandler;
typedef std::function<void(MBSpineLayer* spineLayer)> MBSpineLayerInitFinishHandler;

class MBSpineLayer : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* scene (const MBSpineLayerInitFinishHandler& handler);
    
    CREATE_FUNC(MBSpineLayer);
    
    virtual ~MBSpineLayer();
    virtual bool init();
    void onExit();
    void setSkinFile(std::string slotName, std::string file);
    
    std::string spineSkin;
    std::string spineName;
    std::string spinePath;
    std::string spineAnimation;
    
    bool loop;
    bool debugEnable;
    
    MBPlayerEventHandler startHandler;
    MBPlayerEventHandler completeHandler;
    
private:
    void runSpine();
    void addAnimation();
    MBSkeletonAnimation* _skeletonAnimation;
    cocos2d::Map<std::string, cocos2d::Texture2D*> skinMap;

};

#endif /* MBSpineLayer_h_ */
