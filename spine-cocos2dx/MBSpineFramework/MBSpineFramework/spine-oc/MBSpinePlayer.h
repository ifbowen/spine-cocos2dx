//
//  MBSpinePlayer.h
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/18.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol MBSpinePlayerDelegate <NSObject>

- (void)animationDidComplete;

@end

/**
 注意：
 1、视图即将消失之后不能开启动画
 2、contentView移除之前保证Director已经销毁
 3、Director销毁之后才能开启新动画
 */
@interface MBSpinePlayer : NSObject

@property (nonatomic, weak) id <MBSpinePlayerDelegate> delegate;

+ (instancetype)player;

/// 设置spine动画承载视图
- (void)setSpineDisplayView:(UIView *)contentView;

/// 设置动画
/// @param name 资源名称
/// @param animation 动画名称
/// @param path 资源路径
- (void)setSpineName:(NSString *)name animation:(NSString *)animation path:(NSString *)path;

/// 开始
- (void)startAnimation;

/// 停止
- (void)stopAnimation;

@end

NS_ASSUME_NONNULL_END
