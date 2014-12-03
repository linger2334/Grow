//
//  GHContextOverlayView.h
//  GHContextMenu
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, GHContextMenuActionType){
    // Default
    GHContextMenuActionTypePan,
    // Allows tap action in order to trigger an action
    GHContextMenuActionTypeTap
};

@protocol GHContextOverlayViewDataSource;
@protocol GHContextOverlayViewDelegate;

@interface GHContextMenuView : UIView

@property (nonatomic, assign) id<GHContextOverlayViewDataSource> dataSource;
@property (nonatomic, assign) id<GHContextOverlayViewDelegate> delegate;

@property (nonatomic, assign) GHContextMenuActionType menuActionType;
@property (nonatomic) CGPoint longPressLocation;

- (void) longPressDetected:(UIGestureRecognizer*) gestureRecognizer;

@end

@protocol GHContextOverlayViewDataSource <NSObject>

@required
- (NSInteger) numberOfMenuItems;
- (UIImage*) imageForItemAtIndex:(NSInteger) index;

@optional
-(BOOL) shouldShowMenuAtPoint:(CGPoint) point;

@end

@protocol GHContextOverlayViewDelegate <NSObject>

- (void) didSelectItemAtIndex:(NSInteger) selectedIndex forMenuAtPoint:(CGPoint) point;

@end
