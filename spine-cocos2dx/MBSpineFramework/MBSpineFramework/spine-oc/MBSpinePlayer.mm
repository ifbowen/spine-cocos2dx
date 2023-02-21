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

@interface MBSpinePlayer ()

@property (nonatomic, assign) BOOL isRunning;
@property (nonatomic, strong) UIView *contentView;

@property (nonatomic, copy) NSString *spineSkin;
@property (nonatomic, copy) NSString *spinePath;
@property (nonatomic, copy) NSString *spineName;
@property (nonatomic, copy) NSString *spineAniamtion;
@property (nonatomic, strong) NSMutableArray *skins;
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
        self.skins = @[].mutableCopy;
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

- (void)setSpineLocalSkin:(NSString *)name file:(NSString *)file
{
    if (!name || !file) {
        printf("MBSpinePlayer name or file is nil\n");
        return;
    }
    NSDictionary *skin = @{@"name": name, @"file": file};
    [self.skins addObject:skin];
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
        printf("MBSpinePlayer spineName or spinePath or spineAniamtion is nil\n");
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
    
    for (NSDictionary *skin in self.skins) {
        spineLayer->setSkinFile(std::string([skin[@"name"] UTF8String]), std::string([skin[@"file"] UTF8String]));
    }
    spineLayer->startHandler = [self] (void) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(animationDidStart)]) {
            [self.delegate animationDidStart];
        }
    };
    spineLayer->completeHandler = [self] (void) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(animationDidCompletion)]) {
            [self.delegate animationDidCompletion];
        }
    };
}

- (BOOL)startAnimation
{
    if (self.isRunning) {
        printf("MBSpinePlayer is running\n");
        return NO;
    }
    printf("MBSpinePlayer startAnimation\n");

    self.isRunning = YES;
    
    return [self _start];
}

- (void)stopAnimation
{
    if (!self.isRunning) {
        printf("MBSpinePlayer is not running\n");
        return;
    }
    printf("MBSpinePlayer stopAnimation\n");
    
    self.isRunning = NO;
    
    [self _stop];
}

- (BOOL)_start
{
    auto director = cocos2d::Director::getInstance();
    if (director->getRunningScene()) {
        printf("MBSpinePlayer current running Scene is not completely destroyed. Please call later\n");
        return NO;
    }
    static MBSpineAppDelegate s_sharedApplication;
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    
    MBSpineAppDelegate *delegate = (MBSpineAppDelegate *)app;
    
    delegate->initFinishHandler = [self] (MBSpineLayer* spineLayer) {
        [self setupSpineLayer:spineLayer];
    };
    
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame:[UIScreen mainScreen].bounds
                                         pixelFormat:kEAGLColorFormatRGBA8
                                         depthFormat:cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer:NO
                                          sharegroup:nil
                                       multiSampling:NO
                                     numberOfSamples:0];
    [eaglView setOpaque:NO];
    [eaglView setMultipleTouchEnabled:NO];
    [eaglView setBackgroundColor:UIColor.clearColor];
    [self.contentView addSubview:eaglView];
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void*)eaglView);
    director->setClearColor(cocos2d::Color4F(0, 0, 0, 0));
    director->setOpenGLView(glview);
    if (self.debugEable) {
        director->setDisplayStats(true);
    }
    
    app->run();
    
    return YES;
}

- (void)_stop
{
    auto director = cocos2d::Director::getInstance();
    director->stopAnimation();
    director->end();
    director->purgeCachedData();
    [self _clearData];
}

- (void)_clearData
{
    self.spineSkin = nil;
    self.spinePath = nil;
    self.spineName = nil;
    self.spineAniamtion = nil;
    self.contentView = nil;
    [self.skins removeAllObjects];
}

@end
