//
//  OCScrollView.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//
#include "iostream"
#import <Foundation/Foundation.h>

@interface OCScrollView : UIScrollView

@property(nonatomic,assign)BOOL isButtonHiden;
@property(nonatomic,assign)std::string levelName;
//for polygon
@property(nonatomic,copy)UIColor* pointColor;
@property(nonatomic,copy)UIColor* lineColor;
@property(nonatomic,strong)NSMutableArray* centerViews;
@property(nonatomic,strong)NSMutableArray* centerviewKeys;
@property(nonatomic,strong)NSMutableDictionary* polygons;

-(id)initWithFrame:(CGRect)frame;

-(void)drawRect:(CGRect)rect;

@end
