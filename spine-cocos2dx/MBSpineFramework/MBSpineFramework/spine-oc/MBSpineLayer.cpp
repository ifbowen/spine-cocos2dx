//
//  MBSpineLayer.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/17.
//

#include "MBSpineLayer.h"

USING_NS_CC;
using namespace spine;

Scene* MBSpineLayer::scene (const std::string& bundlePath, const std::string& animationName, const MBPlayerCompletionHandler& handler) {
    Scene *scene = Scene::create();
    MBSpineLayer *layer = MBSpineLayer::create();
    scene->addChild(layer);
    
    layer->setPlayerCompleteCallback(handler);
    layer->setAnimationName(animationName);
    layer->setAnimationBundlePath(bundlePath);
    
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
    
    std::string jsonPath = this->_animationBundlePath + "/" + std::string(this->_animationName).append(".json");
    std::string atlasPath = this->_animationBundlePath + "/" + std::string(this->_animationName).append(".atlas");
    
    _skeletonAnimation = SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath, 0.5f);
    
    _skeletonAnimation->setCompleteListener([this] (TrackEntry* entry) {
        if (this->_completionHandler) {
            this->_completionHandler();
        }
       });
    
    _skeletonAnimation->setAnimation(0, "walk", false);
    
    _skeletonAnimation->setPosition(Vec2(_contentSize.width / 2, 0));
    
    addChild(_skeletonAnimation);
}

void MBSpineLayer::runSpine() {
    log("run spine");
    addAnimation();
    scheduleUpdate();
}

void MBSpineLayer::setAnimationBundlePath(const std::string& bundlePath) {
    _animationBundlePath = std::string(bundlePath);
}

void MBSpineLayer::setAnimationName(const std::string& animaitonName) {
    _animationName = std::string(animaitonName);
}

void MBSpineLayer::setPlayerCompleteCallback(const MBPlayerCompletionHandler& handler) {
    _completionHandler = handler;
}
