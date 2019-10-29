//
//  MBSpineLayer.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#include "MBSpineLayer.h"
#include <spine/SkeletonTwoColorBatch.h>

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
    log("MBSpineLayer dealloc");
    spine::SkeletonBatch::destroyInstance();
    spine::SkeletonTwoColorBatch::destroyInstance();
}

bool MBSpineLayer::init () {
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 50))) return false;
    return true;
}

void MBSpineLayer::onExit() {
    LayerColor::onExit();
}

void MBSpineLayer::setSkinFile(std::string slotName, std::string file) {
    skinMap.insert(slotName, _director->getTextureCache()->addImage(file));
}

void MBSpineLayer::addAnimation() {
    
    std::string jsonPath = this->spinePath + "/" + std::string(this->spineName).append(".json");
    std::string atlasPath = this->spinePath + "/" + std::string(this->spineName).append(".atlas");
    
    _skeletonAnimation = MBSkeletonAnimation::createWithJsonFile(jsonPath, atlasPath, 0.5f);
    
    _skeletonAnimation->setStartListener([this] (TrackEntry* entry) {
        if (this->startHandler) {
            this->startHandler();
        }
    });
    _skeletonAnimation->setCompleteListener([this] (TrackEntry* entry) {
        if (this->completeHandler) {
            this->completeHandler();
        }
       });
    
    _skeletonAnimation->setAnimation(0, this->spineAnimation, this->loop);
    _skeletonAnimation->setPosition(Vec2(_contentSize.width / 2, 0));
    
    _skeletonAnimation->skinMap = this->skinMap;

    if (!this->spineSkin.empty()) {
        _skeletonAnimation->setSkin(this->spineSkin);
    }
    
    _skeletonAnimation->setDebugBonesEnabled(this->debugEnable);
    _skeletonAnimation->setDebugSlotsEnabled(this->debugEnable);
    _skeletonAnimation->setDebugMeshesEnabled(this->debugEnable);
    
    addChild(_skeletonAnimation);
}

void MBSpineLayer::runSpine() {
    addAnimation();
    scheduleUpdate();
}
