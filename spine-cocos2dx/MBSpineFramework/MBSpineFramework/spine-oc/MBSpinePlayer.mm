//
//  MBSpinePlayer.m
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/18.
//

#import "MBSpinePlayer.h"
#include "cocos2d.h"
#include "platform/ios/CCDirectorCaller-ios.h"
#include "platform/ios/CCEAGLView-ios.h"
#include "MBSpineAppDelegate.h"

static MBSpineAppDelegate s_sharedApplication;

@interface MBSpinePlayer ()

@property (nonatomic, assign) BOOL isRunning;
@property (nonatomic, strong) UIView *contentView;

@property (nonatomic, copy) NSString *spineSkin;
@property (nonatomic, copy) NSString *spinePath;
@property (nonatomic, copy) NSString *spineName;
@property (nonatomic, copy) NSString *spineAniamtion;
@property (nonatomic, assign) BOOL spineLoop;
@property (nonatomic, assign) BOOL debugEable;

@end

@implementation MBSpinePlayer

+ (instancetype)player {
    static MBSpinePlayer *_player;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (!_player) {
            _player = [[MBSpinePlayer alloc] init];
        }
    });
    return _player;
}

- (void)dealloc
{
    printf("MBSpinePlayer dealloc\n");
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.spineLoop = NO;
        self.debugEable = NO;
    }
    return self;
}

- (void)setSpineDisplayView:(UIView *)contentView
{
    self.contentView = contentView;
}

- (void)setSpineName:(NSString *)name path:(NSString *)path;
{
    self.spineName = name;
    self.spinePath = path;
}

- (void)setSpineAnimation:(NSString *)animation loop:(BOOL)isLoop
{
    self.spineAniamtion = animation;
    self.spineLoop = isLoop;
}

- (void)setSpineSkinSurface:(NSString *)skin
{
    self.spineSkin = skin;
}

- (void)setSpineDebugEnable:(BOOL)enable
{
    self.debugEable = enable;
}

- (void)setupSpineLayer:(MBSpineLayer *)spineLayer
{
    if (!self.spineName || !self.spinePath || !self.spineAniamtion) {
        return;
    }
    
    if (self.spineSkin) {
        spineLayer->spineSkin = std::string([self.spineSkin UTF8String]);
    }
    
    spineLayer->spineName = std::string([self.spineName UTF8String]);
    spineLayer->spinePath = std::string([self.spinePath UTF8String]);
    spineLayer->spineAnimation = std::string([self.spineAniamtion UTF8String]);
    spineLayer->loop = self.spineLoop;
    spineLayer->debugEnable = self.debugEable;
    
    spineLayer->startHandler = [self] (void) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(animationDidStart)]) {
            [self.delegate animationDidStart];
        }
    };
    spineLayer->completeHandler = [self] (void) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(animationDidComplete)]) {
            [self.delegate animationDidComplete];
        }
    };
}

- (void)startAnimation
{
    if (self.isRunning) {
        return;
    }
    printf("MBSpinePlayer startAnimation\n");

    self.isRunning = YES;
    
    [self _start];
}

- (void)stopAnimation
{
    if (!self.isRunning) {
        return;
    }
    printf("MBSpinePlayer stopAnimation\n");
    
    self.isRunning = NO;
    
    [self _stop];
}

- (void)_start
{
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    
    MBSpineAppDelegate *delegate = (MBSpineAppDelegate *)app;
    
    __weak typeof(self) weakSelf = self;
    delegate->initFinishHandler = [weakSelf] (MBSpineLayer* spineLayer) {
        [weakSelf setupSpineLayer:spineLayer];
    };
    
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: kEAGLColorFormatRGBA8
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0];
    [eaglView setOpaque:NO];
    [eaglView setMultipleTouchEnabled:NO];
    [eaglView setBackgroundColor:UIColor.clearColor];
    
    [self.contentView addSubview:eaglView];
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void*)eaglView);
    auto director = cocos2d::Director::getInstance();
    director->setClearColor(cocos2d::Color4F(0, 0, 0, 0));
    director->setOpenGLView(glview);
    
    if (self.debugEable) {
        director->setDisplayStats(true);
    }
    
    app->run();
}

- (void)_stop
{
    cocos2d::Director::getInstance()->stopAnimation();
    cocos2d::Director::getInstance()->end();
    
    // 清除缓存
    cocos2d::Director::getInstance()->purgeCachedData();
    
    [self clearData];
}

- (void)clearData
{
    self.spineSkin = nil;
    self.spinePath = nil;
    self.spineName = nil;
    self.spineAniamtion = nil;
    self.contentView = nil;
}

@end
