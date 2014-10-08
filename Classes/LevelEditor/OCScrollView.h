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

-(void)drawRect:(CGRect)rect;

@end
