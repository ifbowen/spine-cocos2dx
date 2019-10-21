//
//  ViewController.m
//  spine-cocos2dx
//
//  Created by Bowen on 2019/10/19.
//  Copyright © 2019 inke. All rights reserved.
//

#import "ViewController.h"
#import "MBSpinePlayerViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.orangeColor;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    MBSpinePlayerViewController *vc = [MBSpinePlayerViewController new];
    [self.navigationController pushViewController:vc animated:YES];
}

@end

