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

@property (nonatomic, assign) BOOL isStart;
@property (nonatomic, copy) NSString *spineName;
@property (nonatomic, copy) NSString *spineAniamtion;
@property (nonatomic, copy) NSString *spinePath;
@property (nonatomic, strong) UIView *contentView;

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

- (void)setSpineDisplayView:(UIView *)contentView
{
    self.contentView = contentView;
}

- (void)setSpineName:(NSString *)name animation:(NSString *)animation path:(NSString *)path;
{
    self.spineName = name;
    self.spineAniamtion = animation;
    self.spinePath = path;
}

- (void)setupSpineLayer:(MBSpineLayer *)spineLayer
{
    spineLayer->spineName = std::string([self.spineName UTF8String]);
    spineLayer->spineAnimation = std::string([self.spineAniamtion UTF8String]);
    spineLayer->spinePath = std::string([self.spinePath UTF8String]);
    spineLayer->completionHandler = [self] (void) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(animationDidComplete)]) {
            [self.delegate animationDidComplete];
        }
    };
}

- (void)startAnimation
{
    if (self.isStart) {
        return;
    }
    printf("MBSpinePlayer startAnimation\n");

    self.isStart = YES;
    
    [self _start];
}

- (void)stopAnimation
{
    if (!self.isStart) {
        return;
    }
    printf("MBSpinePlayer stopAnimation\n");
    
    self.isStart = NO;
    
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
    
    app->run();
}

- (void)_stop
{
    cocos2d::Director::getInstance()->stopAnimation();
    cocos2d::Director::getInstance()->end();
    
    [self clearData];
}

- (void)clearData
{
    self.spinePath = nil;
    self.spineName = nil;
    self.spineAniamtion = nil;
    self.contentView = nil;
}

@end
