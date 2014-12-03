//
//  OCScrollView.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//
#include <iostream>
#include "Macro.h"
#import "FeaturesPickerController.h"

@class PolygonView;

@interface OCScrollView : UIScrollView<FeaturesPickerDelegate,UIPickerViewDataSource,UIPickerViewDelegate>{
@public    std::list<int>polygontags;
}

@property(nonatomic,assign)BOOL isButtonHiden;
@property(nonatomic,assign)BOOL isPreViewed;
@property(nonatomic,assign)float currentZoomFactor;
@property(nonatomic,assign)std::string levelName;
//for polygon
@property(nonatomic,copy)UIColor* lineColor;
@property(nonatomic,strong)NSMutableArray* centerViews;
@property(nonatomic,strong)NSMutableSet* toDealWithPointView;
//for popover
@property(nonatomic,assign)UIView* longPressedView;
@property(nonatomic,strong)FeaturesPickerController* featuresPicker;
@property(nonatomic,strong)UIPopoverController* featuresPickerPopover;
@property(nonatomic,readonly)UIViewController* animationGroupCountController;
@property(nonatomic,readonly)UIPickerView* animationGroupCountPickerView;

-(id)initWithFrame:(CGRect)frame;
-(void)addGestureRecognizerForCenterView:(PolygonView*)uiview;
-(int)firstUnusedTag;
-(void)createNewPolygonAtCenter;
-(PolygonView*)createNewPolygonOfTag:(int)tag AtCCPercentPosition:(Vec2)position WithCCLocalVertxex:(__Array*)vertexes OfType:(bool)isConvex;

-(PolygonView*)copyPathPoint:(PolygonView*)pathpoint;

-(int)savePolygonsInfoInMemory;

-(void)popupFeaturesPickerPopoverWith:(UIView*)uiview At:(CGPoint)position;

-(void)drawRect:(CGRect)rect;

@end
