//
//  PolygonView.h
//  Grow
//
//  Created by 林纲 王 on 14-10-11.
//
//

#import <UIKit/UIKit.h>

@interface PolygonView : UIView{
    CGPoint originalLocation;
    CGPoint lastLocation;
    BOOL isSelected;
    UIView* activePointView;
    UIView* centerview;
}

@property(nonatomic,copy)UIColor *pointColor;
@property(nonatomic,copy)UIColor *lineColor;
@property(nonatomic,strong)NSArray* pointViews;;

-(id)initWithFrame:(CGRect)frame;

-(void)drawRect:(CGRect)rect;

//-(std::vector<std::pair<float,float>>&)getPoints;
-(NSArray*)getPoints;
-(void)addPointViewFrom:(NSArray*)points;
-(void)createPointViewAtCenter;

+(CGPoint)convertPoint:(CGPoint)point1 fromRect1:(CGSize)rect1 toRect2:(CGSize)rect2;
+(CGPoint)convertCGPoint:(CGPoint)point1 fromRect1:(CGSize)rect1 toRect2:(CGSize)rect2;

@end
