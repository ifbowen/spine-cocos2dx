//
//  MBSpineLayer.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#include "MBSpineLayer.h"

USING_NS_CC;
using namespace spine;

Scene* MBSpineLayer::scene (const MBSpineLayerInitFinishHandler& handler) {
    Scene *scene = Scene::create();
    MBSpineLayer *layer = MBSpineLayer::create();
    scene->addChild(layer);
    
    handler(layer);
    
    layer->runSpine();
    
    return scene;
}

MBSpineLayer::~MBSpineLayer () {
    log("dealloc MBSpineLayer");
}

bool MBSpineLayer::init () {
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 50))) return false;
    return true;
}

void MBSpineLayer::addAnimation() {
    
    std::string jsonPath = this->spinePath + "/" + std::string(this->spineName).append(".json");
    std::string atlasPath = this->spinePath + "/" + std::string(this->spineName).append(".atlas");
    
    _skeletonAnimation = SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath, 0.5f);
    
    _skeletonAnimation->setCompleteListener([this] (TrackEntry* entry) {
        if (this->completionHandler) {
            this->completionHandler();
        }
       });
    
    _skeletonAnimation->setAnimation(0, this->spineAnimation, false);
    
    _skeletonAnimation->setPosition(Vec2(_contentSize.width / 2, 0));
    
    addChild(_skeletonAnimation);
}

void MBSpineLayer::runSpine() {
    addAnimation();
    scheduleUpdate();
}

