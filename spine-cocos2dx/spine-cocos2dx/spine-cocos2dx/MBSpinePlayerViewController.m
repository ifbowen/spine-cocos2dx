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

@end

@implementation MBSpinePlayerViewController

- (UIModalPresentationStyle)modalPresentationStyle
{
    return UIModalPresentationFullScreen;
}


- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    self.spineView = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:self.spineView];
    
    self.player = [MBSpinePlayer player];
    self.player.delegate = self;
    [self runSpine];

}

- (void)runSpine
{
    [self.player setSpineDisplayView:self.spineView];
    NSString *path = [[NSBundle mainBundle] pathForResource:@"common" ofType:nil];
    [self.player setSpineName:@"spineboy" bundlePath:path];
    [self.player startAnimation];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.player stopAnimation];
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)animationDidComplete {
    
    [self.player stopAnimation];

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self runSpine];
    });
}

- (void)dealloc
{
    [self.player stopAnimation];
    NSLog(@"%s", __func__);
}


@end
