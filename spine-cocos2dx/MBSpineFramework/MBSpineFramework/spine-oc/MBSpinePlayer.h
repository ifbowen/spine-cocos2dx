//
//  MBSpinePlayer.h
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/18.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol MBSpinePlayerDelegate <NSObject>

/// 动画开始回调
- (void)animationDidStart;

/// 动画完成回调
- (void)animationDidCompletion;

@end

/**
 注意：
 1、视图即将消失之后不能开启动画
 2、contentView移除之前保证Director已经销毁
 3、Director销毁之后才能开启新动画
 */
@interface MBSpinePlayer : NSObject

@property (nonatomic, weak) id <MBSpinePlayerDelegate> delegate;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)new NS_UNAVAILABLE;

+ (instancetype)player;

/// 设置spine动画承载视图
- (void)setSpineDisplayView:(UIView *)contentView;

/// 设置动画资源
/// @param name 资源名称
/// @param path 资源路径
- (void)setSpineName:(NSString *)name path:(NSString *)path;

/// 设置动画名称
/// @param animation 动画名称
/// @param isLoop 是否循环
- (void)setSpineAnimation:(NSString *)animation loop:(BOOL)isLoop;

/// 局部换肤
/// @param name 需要更换的名称
/// @param file 文件
- (void)setSpineLocalSkin:(NSString *)name file:(NSString *)file;

/// 设置动画皮肤
/// @param skin 皮肤
- (void)setSpineSkinSurface:(NSString *)skin;

/// 设置动画调试状态
/// @param enable yes，开启；no，关闭
- (void)setSpineDebugEnable:(BOOL)enable;

/// 开始，结束需要主动调用停止
/// YES，成功；NO，失败
- (BOOL)startAnimation;

/// 停止
- (void)stopAnimation;

@end

NS_ASSUME_NONNULL_END
