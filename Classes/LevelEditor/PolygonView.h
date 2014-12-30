//
//  PolygonView.h
//  Grow
//
//  Created by 林纲 王 on 14-10-11.
//
//

#import <UIKit/UIKit.h>
#include "Item.h"

@class ItemView;

@interface PolygonView : UIImageView{
@public std::vector<AnimationInfo> _eachGroupCorrespondInfos;
@public    ItemView* _pathParent;
}

@property(nonatomic,assign)BOOL isSelected;
@property(nonatomic,assign)BOOL isConvex;
@property(nonatomic,copy)NSString* itemtype;
@property(nonatomic,strong)NSMutableArray* _bindIDs;
@property(nonatomic,copy)NSString* pointType;
@property(nonatomic,assign)UILabel* pathNum;
@property(nonatomic,copy)UIColor* defaultColor;
@property(nonatomic,copy)UIColor* selectedColor;
@property(nonatomic,copy)UIColor* heightLightedColor;
@property(nonatomic,copy)UIColor *lineColor;

-(id)init;
-(id)initWithFrame:(CGRect)frame;
-(id)initWithEachGroupCorrespondInfos:(std::vector<AnimationInfo>&)info;

-(NSArray*)getUIVertexs;
-(NSArray*)getGlLocalVertexes;
-(float)checkDetPointA:(CGPoint)pointA pointB:(CGPoint)pointB pointC:(CGPoint)pointC;
-(NSComparisonResult)compare:(PolygonView*)otherview;
-(NSMutableArray*)reorderPointViewsInAntiClockwise:(NSMutableArray*)pointviews;

@end
