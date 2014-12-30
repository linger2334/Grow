//
//  UIViewExtension.h
//  Grow
//
//  Created by 林纲 王 on 14-10-6.
//
//

#import <Foundation/Foundation.h>
#include "iostream"
#include "Item.h"
#import "PolygonView.h"

@interface UIView(Hierarchy)

-(int)getSubviewIndex;

-(void)bringToFront;

-(void)sendToBack;

-(void)bringOneLevelUp;

-(void)sendOneLevelDown;

-(BOOL)isInFront;

-(BOOL)isAtBack;

-(void)swapDepthsWithView:(UIView*)swapView;

@end



@interface UIView(LevelEditor)

-(void)setHeightLightState:(BOOL)isHeightLight;

-(BOOL)getHeightLightState;

-(void)setTypeName:(NSString*) typeName;

-(NSString*)getTypeName;


-(void)itemAddGestureRecognizerWithTarget:(id)target;

@end

@interface ItemView : UIImageView{
@public Item_Type itemtype;
@public bool isAnimated;
@public float triggerTime;
@public float elapsedTime;
@public int bindedTriggerID;
@public bool isAutoSmoothing;
@public std::map<std::string,bool> _animationControlInstructions;
@public std::vector<std::vector<AnimationInfo>> _animationInfos;
@public std::vector<PolygonView*> _pathPoints;
@public void* features;
}
//第一次添加路径点时使用
@property(nonatomic,assign)int animationGroupCount;
@property(nonatomic,assign)ItemView* bindButton;

-(id)init:(Item&)item;

-(float)getRotateAngle;

-(BOOL)isBorderCreated;
-(void)addBorder;
-(void)showBorder;
-(void)refreshBorder;
-(void)hideBorder;
//
-(void)insertAnimationGroupControlInstructions;

-(void)createAllPathPoints;
-(PolygonView*)createPathPointWith:(std::vector<AnimationInfo>&)eachGroupCorrespondInfos;
-(void)translateAllPathPoints:(CGPoint)translation;
-(void)heightLightPathPoints;
-(void)cancelHeightLightPathPoints;
-(std::string)convertTypeToString;

-(NSComparisonResult)compareWithID:(ItemView*)other;
-(void)dealloc;

@end



