//
//  PolygonView.m
//  Grow
//
//  Created by 林纲 王 on 14-10-11.
//
//

#import "PolygonView.h"
#include "GameConstants.h"
#include "GameManager.h"
#include "LevelEditor.h"

@implementation PolygonView{
    GameManager* _gameManager;
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        self.pointColor = [UIColor blueColor];
        self.lineColor = [UIColor whiteColor];
        self.clipsToBounds = NO;
        isSelected = false;
        activePointView = nil;
        [self createPointViewAtCenter];
        [self addPointViewFrom:@[[NSValue valueWithCGPoint:CGPointMake(frame.size.width/2 - 100, frame.size.height/2 - 100)],
                                 [NSValue valueWithCGPoint:CGPointMake(frame.size.width/2 - 100, frame.size.height/2 + 100)],
                                 [NSValue valueWithCGPoint:CGPointMake(frame.size.width/2 + 100, frame.size.height/2 + 100)],
                                 [NSValue valueWithCGPoint:CGPointMake(frame.size.width/2 + 100, frame.size.height/2 - 100)]
                                 ]];
    }
    
    _gameManager = GameManager::getInstance();
    [self setUserInteractionEnabled:YES];
    
    return self;
}

-(void)addPointViewFrom:(NSArray *)points
{
    NSMutableArray* tmp = [NSMutableArray array];
    
    int i = 0;
    for(NSValue* point in points)
    {
        UIView* pointViewToAdd = [self createPointView:i at: point.CGPointValue];
        [tmp addObject:pointViewToAdd];
        [self addSubview:pointViewToAdd];
        [pointViewToAdd release];
        
        i++;
    }
    
    self.pointViews = tmp;
}

-(NSArray*)getPoints
{
    NSMutableArray* points = [NSMutableArray array];
    
    for (int i = 0; i<self.pointViews.count; i++) {
        UIView* pointview = [self.pointViews objectAtIndex:i];
        CGPoint point = CGPointMake(pointview.center.x, pointview.center.y);
        [points addObject:[NSValue valueWithCGPoint:point]];
    }
    
    return points;
}

-(void)createPointViewAtCenter
{
    centerview = [[UIView alloc] init];
    centerview.center = CGPointMake(self.frame.size.width/2, self.frame.size.height/2);
    centerview.bounds = CGRectMake(0, 0, 2*kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius);
    centerview.backgroundColor = [UIColor grayColor];
    centerview.layer.borderColor = self.lineColor.CGColor;
    centerview.layer.borderWidth = 2;
    centerview.layer.cornerRadius = kDefaultPolygonPointRadius;
    [self addSubview:centerview];
    [centerview release];
    
    UIPanGestureRecognizer* panGestureRecognizer =[[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [centerview addGestureRecognizer:panGestureRecognizer];
    [panGestureRecognizer release];
    [centerview setUserInteractionEnabled:YES];
    
}

-(UIView*)createPointView:(int)num at:(CGPoint)point
{
    UIView* pointview = [[UIView alloc] init];
    pointview.center = CGPointMake(point.x, point.y);
    pointview.bounds = CGRectMake(0, 0, 2*kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius);
    pointview.alpha = 0.8;
    pointview.backgroundColor = self.pointColor;
    pointview.layer.borderColor = self.lineColor.CGColor;
    pointview.layer.borderWidth = 2;
    pointview.layer.cornerRadius = kDefaultPolygonPointRadius;
    
    UILabel* number = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 2*kDefaultPolygonPointRadius, 2*kDefaultPolygonPointRadius)];
    number.text = [NSString stringWithFormat:@"%d",num];
    number.textColor = [UIColor whiteColor];
    number.backgroundColor = [UIColor clearColor];
    number.font = [UIFont systemFontOfSize:10];
    number.textAlignment = NSTextAlignmentCenter;
    
    [pointview addSubview:number];
    [number release];
    
    UIPanGestureRecognizer* panGestureRecognizer =[[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [pointview addGestureRecognizer:panGestureRecognizer];
    [panGestureRecognizer release];
    [pointview setUserInteractionEnabled:YES];
    
    return pointview;
}

+(CGPoint)convertPoint:(CGPoint)point1 fromRect1:(CGSize)rect1 toRect2:(CGSize)rect2
{
    CGPoint result = CGPointMake(point1.x*rect2.width/rect1.width, point1.y*rect2.height/rect1.height);
    return result;
}

+(CGPoint)convertCGPoint:(CGPoint)point1 fromRect1:(CGSize)rect1 toRect2:(CGSize)rect2
{
    point1.y = rect1.height - point1.y;
    CGPoint result = CGPointMake(point1.x*rect2.width/rect1.width, point1.y*rect2.height/rect1.height);
    return result;
}

#pragma mark -Draw
-(void)drawRect:(CGRect)rect
{
    if (self.pointViews.count<=0) return;
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextClearRect(context, self.frame);
    //画笔颜色
    const CGFloat* components = CGColorGetComponents(self.lineColor.CGColor);
    
    CGFloat red;
    CGFloat green;
    CGFloat blue;
    CGFloat alpha;
    
    if (CGColorGetNumberOfComponents(self.lineColor.CGColor)==2) {
        red = 1;
        green = 1;
        blue = 1;
        alpha = 1;
    }else{
        red = components[0];
        green = components[1];
        blue = components[2];
        alpha = components[3];
        if (alpha<0) alpha = 1;
    }
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    CGContextSetLineWidth(context, 2.0);
    
    UIView* pointview0 = [self.pointViews objectAtIndex:0];
    CGContextMoveToPoint(context, pointview0.center.x, pointview0.center.y);
    
    for(int i =1;i<self.pointViews.count;i++)
    {
        UIView* pointview = [self.pointViews objectAtIndex:i];
        CGContextAddLineToPoint(context, pointview.center.x, pointview.center.y);
    }
    
    CGContextAddLineToPoint(context, pointview0.center.x, pointview0.center.y);
    
    CGContextStrokePath(context);
}

#pragma mark - Touch
//-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    UITouch* touch = [touches anyObject];
//    originalLocation = [touch locationInView:self];
//    lastLocation = originalLocation;
//
//    //在周围寻找选中点
//    for(UIView* pointview in self.pointViews){
//        CGPoint locationInPointView = [pointview convertPoint:originalLocation fromView:self];
//
//        if ([pointview pointInside:locationInPointView withEvent:event]) {
//            activePointView = pointview;
//            activePointView.backgroundColor = [UIColor greenColor];
//            break;
//        }
//    }
//    //如果没有，再判断是否在中心
//    if (!activePointView) {
//        CGPoint locationIncenterview = [centerview convertPoint:originalLocation fromView:self];
//        if ([centerview pointInside:locationIncenterview withEvent:event]) {
//            isSelected = true;
//            centerview.backgroundColor = [UIColor greenColor];
//        }
//    }
//    //两种情况都得取消scrollview滚动
//    UIScrollView* scrollview = (UIScrollView*)[self superview];
//    [scrollview setScrollEnabled:false];
//
//}
//
//-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    UITouch* touch = [touches anyObject];
//    CGPoint currentLocation = [touch locationInView:self];
//
//    if (isSelected) {
//        CGPoint center = self.center;
//        center.x += currentLocation.x - originalLocation.x;
//        center.y += currentLocation.y - originalLocation.y;
//        self.center = center;
//    }else if(activePointView!=centerview){
//        CGPoint center = activePointView.center;
//        center.x += currentLocation.x - lastLocation.x;
//        center.y += currentLocation.y - lastLocation.y;
//        activePointView.center = center;
//    }
//
//    lastLocation = currentLocation;
//    [self setNeedsDisplay];
//
//
//}
//
//-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    UITouch* touch = [touches anyObject];
//    if(touch.tapCount==1){
//        if (isSelected) {
//            isSelected = false;
//            centerview.backgroundColor = [UIColor grayColor];
//        }else if(activePointView){
//            activePointView.backgroundColor = self.pointColor;
//            activePointView = nil;
//        }
//
//        activePointView = nil;
//    }else if (touch.tapCount == 2){
//
//    }
//
//    UIScrollView* scrollview = (UIScrollView*)[self superview];
//    [scrollview setScrollEnabled:true];
//}
//
//-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    if (isSelected) {
//        centerview.backgroundColor = [UIColor grayColor];
//    }else{
//        activePointView.backgroundColor = self.pointColor;
//    }
//    activePointView = nil;
//
//    UIScrollView* scrollview = (UIScrollView*)[self superview];
//    [scrollview setScrollEnabled:true];
//
//}

-(void)handlePan:(UIPanGestureRecognizer*)recognizer
{
    UIScrollView* scrollview = _gameManager->_levelEditor->_myViewController->_scrollView;
    if(recognizer.state == UIGestureRecognizerStateBegan){
        scrollview.scrollEnabled = NO;
    }
    
    CGPoint translation = [recognizer translationInView:self];
    if (recognizer.view == centerview) {
        self.center = CGPointMake(self.center.x + translation.x, self.center.y + translation.y);
    }else{
        recognizer.view.center = CGPointMake(recognizer.view.center.x + translation.x, recognizer.view.center.y + translation.y);
        [self setNeedsDisplay];
    }
    [recognizer setTranslation:CGPointZero inView:self];
    
    if(recognizer.state == UIGestureRecognizerStateEnded){
        scrollview.scrollEnabled = YES;
    }
    
}

-(void)doubleTap:(UITapGestureRecognizer*)recognizer
{
    
    CGPoint clickPoint = [recognizer locationInView:self];
    UIView* newPointView = [self createPointView:self.pointViews.count at:clickPoint];
    [self addSubview:newPointView];
    [newPointView release];
    
    NSMutableArray* tmp = [NSMutableArray arrayWithArray:self.pointViews];
    [tmp addObject:newPointView];
    self.pointViews = tmp;
    
    [self setNeedsDisplay];
}

-(void)dealloc
{
    self.pointViews = nil;
    [super dealloc];
}

//- (void)logTouchInfo:(UITouch *)touch {
//    CGPoint locInSelf = [touch locationInView:self];
//    CGPoint locInWin = [touch locationInView:nil];
//    NSLog(@"    touch.locationInView = {%2.3f, %2.3f}", locInSelf.x, locInSelf.y);
//    NSLog(@"    touch.locationInWin = {%2.3f, %2.3f}", locInWin.x, locInWin.y);
//    NSLog(@"    touch.phase = %d", touch.phase);
//    NSLog(@"    touch.tapCount = %d", touch.tapCount);
//}

@end
