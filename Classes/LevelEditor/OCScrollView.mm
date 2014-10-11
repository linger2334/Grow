//
//  OCScrollView.m
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#import "OCScrollView.h"
#include "Macro.h"

@implementation OCScrollView

@synthesize isButtonHiden;
@synthesize levelName;

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self){
        self.pointColor = [UIColor blueColor];
        self.lineColor = [UIColor whiteColor];
        self.centerViews = [NSMutableArray arrayWithCapacity:10];
        self.centerviewKeys = [NSMutableArray arrayWithCapacity:10];
        self.polygons = [NSMutableDictionary dictionaryWithCapacity:10];
        
    }
    
    return self;
}

-(void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    //红虚线
    if (!isButtonHiden) {
        //
        float lengths[] = {10,10};
        CGContextSetLineDash(context, 0, lengths, 2);
        CGContextSetLineWidth(context, 1);
        CGContextSetRGBStrokeColor(context, 1, 0, 0, 1);
        
        for(int i = 0;i<PAGE_COUNTS;i++)
        {
            CGContextBeginPath(context);
            CGContextMoveToPoint(context,0,i*WinSize.height);
            CGContextAddLineToPoint(context, self.contentSize.width, i*WinSize.height);
            CGContextStrokePath(context);
        }
    }
    //关卡标签
    for(int i = PAGE_COUNTS;i>0;i--)
    {
        CGContextSetLineWidth(context, 3);
        CGContextSetRGBFillColor(context, 1, 1, 1, 1);
        UIFont* font = [UIFont boldSystemFontOfSize:24];
        NSString* currentPageNumber = [[NSString stringWithUTF8String:levelName.c_str()] stringByAppendingFormat:@" - %d",i];
        [currentPageNumber drawAtPoint:CGPointMake(40, 20 +(PAGE_COUNTS - i)*WinSize.height) withFont:font];
    }
    
    
    
}

@end
