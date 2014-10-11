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
@public void* features;
}

-(id)init:(Item&)item;

-(void)dealloc;

@end



