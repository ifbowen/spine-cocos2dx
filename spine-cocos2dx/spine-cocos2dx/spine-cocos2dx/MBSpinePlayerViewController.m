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
    
    self.spineView = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:self.spineView];
    
    self.player = [[MBSpinePlayer alloc] init];
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
    [self.player setSpineName:@"spineboy" animation:@"walk" path:path];
    [self.player startAnimation];
}

- (void)animationDidComplete
{
    [self.player stopAnimation];
    
    __weak typeof(self) weakSelf = self;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        if ( weakSelf && !weakSelf.dissAppear) {
            [weakSelf runSpine];
        }
    });
}

- (void)dealloc
{
    printf("MBSpinePlayerViewController dealloc\n");
}


@end
