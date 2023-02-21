//
//  MBSpinePlayerViewController.m
//  MBSpinePlayer
//
//  Created by Bowen on 2019/10/19.
//

#import "MBSpinePlayerViewController.h"
#import <MBSpineFramework/MBSpineFramework.h>

@interface MBSpinePlayerViewController () <MBSpinePlayerDelegate>

@property(nonatomic, strong) MBSpinePlayer *player;
@property (nonatomic, strong) UIView *spineView;
@property (nonatomic, assign) BOOL dissAppear;
@property (nonatomic, assign) BOOL isLoop;
@property (nonatomic, assign) NSInteger count;

@end

@implementation MBSpinePlayerViewController

- (UIModalPresentationStyle)modalPresentationStyle
{
    return UIModalPresentationFullScreen;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIView *white = [[UIView alloc] initWithFrame:self.view.bounds];
    white.backgroundColor = UIColor.whiteColor;
    [self.view addSubview:white];
    
//    self.isLoop = YES;
    self.count = 0;
    
    self.spineView = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:self.spineView];
    
    self.player = [[MBSpinePlayer alloc] init];
//    [self.player setSpineDebugEnable:YES];
    self.player.delegate = self;
    [self runSpine];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    printf("MBSpinePlayerViewController viewWillDisappear\n");
    self.dissAppear = YES;
    [self.player stopAnimation];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    printf("MBSpinePlayerViewController viewDidDisappear\n");
}

- (void)runSpine
{
    [self.player setSpineDisplayView:self.spineView];
    NSString *path = [[NSBundle mainBundle] pathForResource:@"common" ofType:nil];
    
    if (self.count % 2 == 0) {
            [self.player setSpineName:@"spineboy" path:path];
            [self.player setSpineAnimation:@"walk" loop:self.isLoop];
//            矩形渲染
            [self.player setSpineLocalSkin:@"gun" file:[NSString stringWithFormat:@"%@/AK47.png", path]];
    } else {
            [self.player setSpineName:@"goblins" path:path];
            [self.player setSpineAnimation:@"walk" loop:self.isLoop];
            [self.player setSpineSkinSurface:@"goblin"];
//             网格渲染
            [self.player setSpineLocalSkin:@"head" file:[NSString stringWithFormat:@"%@/ahead.png", path]];
            [self.player setSpineLocalSkin:@"eyes" file:[NSString stringWithFormat:@"%@/aempty.png", path]];
    }
    
    self.count++;

    [self.player startAnimation];
}

- (void)animationDidStart
{
    
}

- (void)animationDidCompletion
{
    if (!self.isLoop) {
        [self.player stopAnimation];
        __weak typeof(self) weakSelf = self;
        // cocos 里面运行循环时间间隔 1 秒
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            if ( weakSelf && !weakSelf.dissAppear) {
                [weakSelf runSpine];
            }
        });
    }
}

- (void)dealloc
{
    printf("MBSpinePlayerViewController dealloc\n");
}


@end
