//
//  PolygonView.m
//  Grow
//
//  Created by 林纲 王 on 14-10-11.
//
//

#import "PolygonView.h"
#include "GameConstants.h"

@implementation PolygonView

@synthesize isSelected;
@synthesize pointType;
@synthesize itemtype;
@synthesize _bindIDs;

-(id)init
{
    self = [super init];
    if (self) {
        self.alpha = 0.8;
        self.isSelected = NO;
        self.isConvex = YES;
        self.itemtype = [NSString stringWithUTF8String:"-1"];
        self._bindIDs = [NSMutableArray array];
        self.backgroundColor = [UIColor blueColor];
        self.defaultColor = self.backgroundColor;
        self.selectedColor = [UIColor greenColor];
        self.lineColor = [UIColor whiteColor];
        self.layer.borderColor = self.lineColor.CGColor;
        self.layer.borderWidth = kDefaultPolygonBorderWidth;
        self.layer.cornerRadius = kDefaultPolygonPointRadius;
        self.clipsToBounds = NO;
    }
    return self;
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.isSelected = NO;
        self.itemtype = [NSString stringWithUTF8String:"Polygon_"];
        self._bindIDs = [NSMutableArray array];
        self.backgroundColor = [UIColor blueColor];
        self.defaultColor = self.backgroundColor;
        self.selectedColor = [UIColor greenColor];
        self.lineColor = [UIColor whiteColor];
        self.layer.borderColor = self.lineColor.CGColor;
        self.layer.borderWidth = kDefaultPolygonBorderWidth;
        self.layer.cornerRadius = kDefaultPolygonPointRadius;
        self.clipsToBounds = NO;
        self.userInteractionEnabled = YES;
    }
    return self;
}

-(id)initWithEachGroupCorrespondInfos:(std::vector<AnimationInfo> &)info
{
    self = [super init];
    if(self){
        self.isSelected = NO;
        self.isConvex = YES;
        self.itemtype = @"-1";
        self._bindIDs = [NSMutableArray array];
        self.backgroundColor = [UIColor redColor];
        self.defaultColor = self.backgroundColor;
        self.selectedColor = [UIColor greenColor];
        self.heightLightedColor = [UIColor orangeColor];
        self.lineColor = [UIColor whiteColor];
        self.layer.borderColor = self.lineColor.CGColor;
        self.layer.borderWidth = kDefaultPathPointBorderWidth;
        self.layer.cornerRadius = kDefaultPathPointRadius;
        self.clipsToBounds = NO;
        self.userInteractionEnabled = YES;
        
        self->_eachGroupCorrespondInfos = info;
        self->_pathParent = nil;
    }
    
    return self;
}

-(NSArray*)getUIVertexs
{
    NSMutableArray* tmp = [NSMutableArray array];
    for(PolygonView* pointview in self.subviews)
    {
        [tmp addObject:[NSValue valueWithCGPoint:CGPointMake(pointview.center.x+self.center.x,pointview.center.y+self.center.y)]];
    }
    return tmp;
}

-(NSArray*)getGlLocalVertexes
{
    //先把平行的点去掉
    NSMutableArray* needDelete = [NSMutableArray array];
    NSMutableArray* UIvertexes = [NSMutableArray arrayWithArray:[self getUIVertexs]];
    float det;
    for (int i = 0; i<UIvertexes.count - 2; i++) {
        det = [self checkDetPointA:[UIvertexes[i] CGPointValue] pointB:[UIvertexes[i+1] CGPointValue] pointC:[UIvertexes[i+2] CGPointValue]];
        if (det == 0) {
            [needDelete addObject:UIvertexes[i+1]];
        }
    }
    det = [self checkDetPointA:[UIvertexes[UIvertexes.count - 2] CGPointValue] pointB:[UIvertexes[UIvertexes.count-1] CGPointValue] pointC:[UIvertexes[0] CGPointValue]];
    if (det == 0) {
        [needDelete addObject:UIvertexes[UIvertexes.count-1]];
    }
    det = [self checkDetPointA:[UIvertexes[UIvertexes.count-1] CGPointValue] pointB:[UIvertexes[0] CGPointValue] pointC:[UIvertexes[1] CGPointValue]];
    if (det == 0) {
        [needDelete addObject:UIvertexes.firstObject];
    }
    [UIvertexes removeObjectsInArray:needDelete];
    
    //再逆时针排列
//    NSMutableArray* localVertexes = [NSMutableArray arrayWithArray:[self reorderVertexesInAntiClockwise:UIvertexes]];
    NSMutableArray* localVertexes = [NSMutableArray arrayWithArray:UIvertexes];
    //再转为右手系本地坐标
    int vertexCount = localVertexes.count;
    for (int i = 0; i < vertexCount; i++) {
        CGPoint point = [localVertexes[i] CGPointValue];
        localVertexes[i] = [NSValue valueWithCGPoint:CGPointMake(point.x - self.center.x, self.center.y - point.y)];
    }
    //最后判断是否凸多边形
    bool isConvex = true;
    det = [self checkDetPointA:[localVertexes[0] CGPointValue] pointB:[localVertexes[1] CGPointValue] pointC:[localVertexes[2] CGPointValue]];
    float newdet;
    for (int i = 1; i<vertexCount-2; i++) {
        newdet = [self checkDetPointA:[localVertexes[i] CGPointValue] pointB:[localVertexes[i+1] CGPointValue] pointC:[localVertexes[i+2] CGPointValue]];
        if (det*newdet<0) {
            isConvex = false;
            break;
        }
    }
    newdet = [self checkDetPointA:[localVertexes[vertexCount-2] CGPointValue] pointB:[localVertexes[vertexCount-1] CGPointValue] pointC:[localVertexes[0] CGPointValue]];
    if (det*newdet<0) isConvex = false;
    newdet = [self checkDetPointA:[localVertexes[vertexCount-1] CGPointValue] pointB:[localVertexes[0] CGPointValue] pointC:[localVertexes[1] CGPointValue]];
    if(det*newdet<0) isConvex = false;
    //
//    if(!isConvex){
//        NSArray* tmp = [NSArray arrayWithArray:localVertexes];
//        NSEnumerator* enum1 =[tmp reverseObjectEnumerator];
//        id vertex;
//        int i = 0;
//        while (vertex = [enum1 nextObject]) {
//            localVertexes[i] = vertex;
//            i++;
//        }
//    }
    
    self.isConvex = isConvex;
    return localVertexes;
}

-(float)checkDetPointA:(CGPoint)pointA pointB:(CGPoint)pointB pointC:(CGPoint)pointC
{
    float result = pointA.x*pointB.y + pointB.x*pointC.y + pointC.x*pointA.y - pointA.y*pointB.x -pointB.y*pointC.x - pointC.y*pointA.x;
    return result;
}

-(NSComparisonResult)compare:(PolygonView *)otherview
{
    PolygonView* tmp = (PolygonView*)self;
    
    NSNumber* number1 = [NSNumber numberWithFloat:tmp.center.x];
    NSNumber* number2 = [NSNumber numberWithFloat:otherview.center.x];
    NSComparisonResult result = [number1 compare:number2];
    
    return result;
}

-(NSMutableArray*)reorderPointViewsInAntiClockwise:(NSMutableArray*)pointviews
{
    int pointviewsCount = [pointviews count];
    
    [pointviews sortUsingSelector:@selector(compare:)];
    NSMutableArray* antiClockViews = [NSMutableArray arrayWithArray:pointviews];

    CGPoint left = ((PolygonView*)pointviews[0]).center;
    CGPoint right = ((PolygonView*)[pointviews lastObject]  ).center;
    
    int leftPos = 1;
    int rightPos = pointviewsCount - 1;
    
    antiClockViews[0] = pointviews[0];
    for(int i =1;i<pointviewsCount-1;i++)
    {
        if ([self checkDetPointA:left pointB:right pointC:((PolygonView*)pointviews[i]).center]<0) {
            antiClockViews[rightPos--] = pointviews[i];
        }else{
            antiClockViews[leftPos++] = pointviews[i];
        }
    }
    antiClockViews[leftPos] = pointviews[pointviewsCount - 1];
    
    return antiClockViews;
}

-(NSMutableArray*)reorderVertexesInAntiClockwise:(NSMutableArray*)vertexes
{
    int vertexCount = [vertexes count];
    [vertexes sortedArrayUsingComparator:^(id obj1,id obj2){
        if ([obj1 CGPointValue].x>[obj2 CGPointValue].x) {
            return (NSComparisonResult)NSOrderedDescending;
        }
        
        if ([obj1 CGPointValue].x<[obj2 CGPointValue].x) {
            return (NSComparisonResult)NSOrderedAscending;
        }
        
        return (NSComparisonResult)NSOrderedSame;
    }];
    
    NSMutableArray* antiVertexes = [NSMutableArray arrayWithArray:vertexes];
    
    CGPoint left = [vertexes.firstObject CGPointValue];
    CGPoint right = [vertexes.lastObject CGPointValue];
    
    int leftPos = 1;
    int rightPos = vertexCount - 1;
    
    for(int i = 1;i<vertexCount - 1;i++){
        if ([self checkDetPointA:left pointB:right pointC:[vertexes[i] CGPointValue]]<0) {
            antiVertexes[rightPos--] = vertexes[i];
        }else{
            antiVertexes[leftPos++] = vertexes[i];
        }
    }
    antiVertexes[leftPos] = vertexes[vertexCount - 1];
    
    return antiVertexes;
}

-(void)dealloc
{
    self._bindIDs = nil;
    [super dealloc];
}

@end
