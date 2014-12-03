//
//  OCScrollView.m
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#import "OCScrollView.h"
#include "Macro.h"
#include "GameConstants.h"
#include "GameManager.h"
#include "UIViewExtension.h"
#include "PolygonView.h"
#include "LevelEditor.h"
#include "b2Separator.h"
#include "PropertyController.h"

@implementation OCScrollView{
    GameManager* _gameManager;
    float width;
    float height;
}

@synthesize isButtonHiden;
@synthesize isPreViewed;
@synthesize currentZoomFactor;
@synthesize levelName;
@synthesize animationGroupCountController;
@synthesize animationGroupCountPickerView;

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self){
        self.lineColor = [UIColor whiteColor];
        _gameManager = GameManager::getInstance();
        width = _gameManager->editor_width;
        height = _gameManager->editor_height;
        self.currentZoomFactor = 1.0;
        
        self.centerViews = [NSMutableArray arrayWithCapacity:3];
        self.toDealWithPointView =[NSMutableSet set];
    }
    [self addGestureRecognizerForScrollView];
    return self;
}

-(void)createNewPolygonAtCenter
{
    //中心点
    CGPoint center = CGPointMake(width/2*currentZoomFactor, self.contentOffset.y + height/2);
    PolygonView* centerview = [[PolygonView alloc] initWithFrame:CGRectMake(center.x - kDefaultPolygonPointRadius*currentZoomFactor, center.y - kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor)];
    centerview.bounds = CGRectMake(-kDefaultPolygonPointRadius*currentZoomFactor, -kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor);
    centerview.backgroundColor = [UIColor grayColor];
    centerview.defaultColor = centerview.backgroundColor;
    centerview.pointType = @"centerview";
    centerview.tag = [self firstUnusedTag];
    polygontags.push_back(centerview.tag);
    
    [self addGestureRecognizerForCenterView:centerview];
    [self addSubview:centerview];
    [self.centerViews addObject:centerview];
    [centerview release];
    
    //默认四个子节点
    for(int i = 0;i<4;i++){
        float dx,dy;
        switch (i) {
            case 0:
                dx = - kDefaultNewPolygonHalfWidth;
                dy = - kDefaultNewPolygonHalfHeight;
                break;
            case 1:
                dx = - kDefaultNewPolygonHalfWidth;
                dy = kDefaultNewPolygonHalfHeight;
                break;
            case 2:
                dx = kDefaultNewPolygonHalfWidth;
                dy = kDefaultNewPolygonHalfHeight;
                break;
            case 3:
                dx = kDefaultNewPolygonHalfWidth;
                dy = - kDefaultNewPolygonHalfHeight;
                break;
            default:
                break;
        }
        dx *= currentZoomFactor;
        dy *= currentZoomFactor;
        PolygonView* pointview = [self createPointViewAtLocalPoint:CGPointMake(dx,dy)];
        [centerview addSubview:pointview];
        [pointview release];
        
    }
    
    [self setNeedsDisplay];
}

-(PolygonView*)createNewPolygonOfTag:(int)tag AtCCPercentPosition:(cocos2d::Vec2)position WithCCLocalVertxex:(cocos2d::__Array *)vertexes OfType:(bool)isConvex
{
    PolygonView* centerview = [[PolygonView alloc] initWithFrame:CGRectMake((position.x*width-kDefaultPolygonPointRadius)*currentZoomFactor, ((PAGE_COUNTS-position.y)*height - kDefaultPolygonPointRadius)*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor)];
    centerview.bounds = CGRectMake(-kDefaultPolygonPointRadius*currentZoomFactor, -kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor);
    centerview.backgroundColor = [UIColor grayColor];
    centerview.defaultColor = centerview.backgroundColor;
    centerview.pointType = @"centerview";
    centerview.tag = tag;
    polygontags.push_back(centerview.tag);
    centerview.isConvex = isConvex;
    
    [self addGestureRecognizerForCenterView:centerview];
    [self addSubview:centerview];
    [self.centerViews addObject:centerview];
    [centerview release];
    
    Ref* vertexElement;
    CCARRAY_FOREACH(vertexes, vertexElement){
        Vec2 pos = PointFromString(static_cast<__String*>(vertexElement)->getCString());
        pos.y *= -1;
        PolygonView* pointview = [self createPointViewAtLocalPoint:CGPointMake(pos.x*currentZoomFactor,pos.y*currentZoomFactor)];
        [centerview addSubview:pointview];
        [pointview release];
    }
    
    [self setNeedsDisplay];
    return centerview;
}


-(int)firstUnusedTag
{
    int firstUnusedTag = 1000;
    int lastTag = 1000;
    polygontags.sort();
    for (int i : polygontags) {
        if(i - lastTag >1){
            firstUnusedTag = lastTag + 1;
            break;
        }
        lastTag++;
    }
    if (firstUnusedTag==1000) {
        firstUnusedTag = lastTag + 1;
    }
    return firstUnusedTag;
}

-(PolygonView*)createPointViewAtLocalPoint:(CGPoint)localPoint
{
    PolygonView* newPointView = [[PolygonView alloc] init];
    newPointView.center = localPoint;
    newPointView.bounds = CGRectMake(0, 0, 2*kDefaultPolygonPointRadius*currentZoomFactor, 2*kDefaultPolygonPointRadius*currentZoomFactor);
    newPointView.pointType = @"pointview";
    
    return newPointView;
}

-(void)createNewPointViewAtUIPosition:(CGPoint)position
{
    PolygonView* firstcenterview = [self.centerViews objectAtIndex:0];
    PolygonView* firstpointview = [firstcenterview.subviews firstObject];
    PolygonView* secondpointview = [firstcenterview.subviews objectAtIndex:1];
    CGPoint firstpoint = CGPointMake(firstcenterview.center.x + firstpointview.center.x,firstcenterview.center.y + firstpointview.center.y);
    CGPoint secondpoint = CGPointMake(firstcenterview.center.x+secondpointview.center.x, firstcenterview.center.y+secondpointview.center.y);
    
    float distance = [self distanceFromPoint:position ToLineWithPointPre:firstpoint andPointNext:secondpoint];
    int idx = 0;
    
    for(PolygonView* centerview in self.centerViews){
        NSArray* vertexes = [centerview getUIVertexs];
        for (int i = 0; i<vertexes.count; i++) {
            float newdistance = [self distanceFromPoint:position ToLineWithPointPre:[vertexes[i] CGPointValue] andPointNext:[vertexes[(i+1)%vertexes.count] CGPointValue]];
            if (newdistance<distance) {
                distance = newdistance;
                idx = i;
                firstcenterview = centerview;
            }
        }
    }
    
    PolygonView* newPointView = [self createPointViewAtLocalPoint:[firstcenterview convertPoint:position fromView:self]];
    [firstcenterview insertSubview:newPointView atIndex:idx+1];

    [newPointView release];
}

-(void)createNewPathPointForItemView:(ItemView*)itemview
{    
    AnimationInfo newPathPointInfo;
    newPathPointInfo.waitTime = kDefaultAnimationWaitTime;
    newPathPointInfo.rotation = kDefaultAnimationRotation;
    newPathPointInfo.rotationSpeed = kDefaultAnimationRotationSpeed;
    newPathPointInfo.moveSpeed = kDefaultAnimationMoveSpeed;
    newPathPointInfo.position = kDefaultAnimationPosition;
    std::vector<AnimationInfo> eachGroupCorrespondInfos(itemview.animationGroupCount,newPathPointInfo);
    eachGroupCorrespondInfos.front().position = Vec2(0,itemview.image.size.height);
    
    PolygonView* newPathPoint = [itemview createPathPointWith:eachGroupCorrespondInfos];
    [self addGestureRecognizerForCenterView:newPathPoint];
    
    itemview->_pathPoints.insert(itemview->_pathPoints.end(), newPathPoint);
    [self addSubview:newPathPoint];
    [newPathPoint release];
    
    //主动显示选中状态
    newPathPoint.isSelected = YES;
    newPathPoint.backgroundColor = newPathPoint.selectedColor;
    [_toDealWithPointView addObject:newPathPoint];
    if([self isScrollEnabled]) [self setScrollEnabled:NO];
    
}

-(PolygonView*)copyPathPoint:(PolygonView*)pathpoint
{
    ItemView* itemview = pathpoint->_pathParent;
    std::vector<AnimationInfo> eachGroupCorrespondInfos(pathpoint->_eachGroupCorrespondInfos);
    eachGroupCorrespondInfos.front().position = Vec2((pathpoint.center.x-itemview.center.x)/currentZoomFactor + 2*kDefaultPathPointRadius,(itemview.center.y-pathpoint.center.y)/currentZoomFactor - 2*kDefaultPathPointRadius);
    PolygonView* newPathPoint = [itemview createPathPointWith:eachGroupCorrespondInfos];
    [self addGestureRecognizerForCenterView:newPathPoint];
    
    itemview->_pathPoints.push_back(newPathPoint);
    [self addSubview:newPathPoint];
    [newPathPoint release];
    
    return newPathPoint;
}

-(float)distanceBetweenPoint1:(CGPoint)point1 andPoint2:(CGPoint)point2
{
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return sqrtf(dx*dx + dy*dy);
}

-(float)distanceFromPoint:(CGPoint)point ToLineWithPointPre:(CGPoint)pointPre andPointNext:(CGPoint)pointNext
{
    float deltaX = pointPre.x - pointNext.x;
    float deltaY = pointPre.y - pointNext.y;
    float distance;
    if (deltaX == 0) {
        if (point.y>=MIN(pointPre.y, pointNext.y)&&point.y<=MAX(pointPre.y, pointNext.y)) {
            distance = fabsf(point.x - pointPre.x);
            return distance;
        }
    }else if(deltaY == 0){
        if (point.x>=MIN(pointPre.x, pointNext.x)&&point.x<=MAX(pointPre.x, pointNext.x)) {
            distance = fabsf(point.y - pointPre.y);
            return distance;
        }
    }else{
        float k = deltaY/deltaX;
        float yIntercept = pointPre.y - k*pointPre.x;
        
        float kOrthogonal = -1/k;
        float yInterceptOrthogonal = point.y - kOrthogonal*point.x;
        
        float x = (yInterceptOrthogonal - yIntercept)/(k - kOrthogonal);
        float y = k*x + yIntercept;
        
        if(x>=MIN(pointPre.x, pointNext.x)&&x<=MAX(pointPre.x, pointNext.x)&&y>=MIN(pointPre.y, pointNext.y)&&y<=MAX(pointPre.y, pointNext.y)){
            distance = sqrtf((point.x-x)*(point.x-x) + (point.y-y)*(point.y-y));
            return distance;
        }
    }
    
    distance = MIN([self distanceBetweenPoint1:point andPoint2:pointPre], [self distanceBetweenPoint1:point andPoint2:pointNext]);
    
    return distance;
}


-(void)addGestureRecognizerForCenterView:(PolygonView*)uiview
{
    UIPanGestureRecognizer* panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(CenterViewPaning:)];
    [uiview addGestureRecognizer:panRecognizer];
    [panRecognizer release];
    
    UITapGestureRecognizer* tapRecgonzier = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(CenterViewSingleTap:)];
    tapRecgonzier.numberOfTapsRequired = 1;
    [uiview addGestureRecognizer:tapRecgonzier];
    [tapRecgonzier release];
    
}

-(void)addGestureRecognizerForScrollView
{
    UITapGestureRecognizer* doubleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(OCScrollViewDoubleTap:)];
    doubleTapRecognizer.numberOfTapsRequired = 2;
    doubleTapRecognizer.numberOfTouchesRequired = 1;
    [self addGestureRecognizer:doubleTapRecognizer];
    [doubleTapRecognizer release];
    
}

//
-(void)CenterViewPaning:(UIPanGestureRecognizer*)recognizer
{
    CGPoint translation = [recognizer translationInView:self];

    if (!self.isScrollEnabled) {
        //移动多边形和路径点
        for(PolygonView* polygonview in self.toDealWithPointView){
            if ([polygonview.pointType isEqualToString:@"pointview"]) {
                PolygonView* centerview = (PolygonView*)polygonview.superview;
                if (centerview.isSelected) continue;
            }else if([polygonview.pointType isEqualToString:@"pathpoint"]){
                ItemView* parent = polygonview->_pathParent;
                if ([parent getHeightLightState]) continue;
            }
            polygonview.center = CGPointMake(polygonview.center.x + translation.x,polygonview.center.y + translation.y);
        }
        
        //移动道具
        std::set<ItemView*>& itemviews = GameManager::getInstance()->_levelEditor->_myViewController->_toDealWith;
        for(std::set<ItemView*>::iterator it = itemviews.begin();it!=itemviews.end();it++){
            ItemView* item = *it;
            item.center = CGPointMake(item.center.x + translation.x, item.center.y + translation.y);
            [item translateAllPathPoints:translation];
        }
    }
    
    [recognizer setTranslation:CGPointZero inView:self];
    
    [self setNeedsDisplay];
}

-(void)CenterViewSingleTap:(UITapGestureRecognizer*)recognizer
{
    PolygonView* centerview = (PolygonView*)recognizer.view;
    centerview.isSelected = !centerview.isSelected;
    
    if ([centerview.pointType isEqualToString:@"centerview"]) {
        centerview.backgroundColor = centerview.isSelected? centerview.selectedColor : centerview.defaultColor;
    }else if ([centerview.pointType isEqualToString:@"pathpoint"]){
        centerview.backgroundColor = centerview.isSelected? centerview.selectedColor : ([centerview->_pathParent getHeightLightState]? centerview.heightLightedColor : centerview.defaultColor);
    }
    
    centerview.isSelected? [_toDealWithPointView addObject:centerview] : [_toDealWithPointView removeObject:centerview];
    
    if (_toDealWithPointView.count>0||_gameManager->_levelEditor->_myViewController->_toDealWith.size()>0) {
        [self setScrollEnabled:NO];
    }else{
        [self setScrollEnabled:YES];
    }
    
}

-(void)OCScrollViewDoubleTap:(UITapGestureRecognizer*)recognizer
{
    CGPoint position = [recognizer locationInView:self];
    if (self.centerViews.count != 0) {
        for (PolygonView* centerview in self.centerViews) {
            CGPoint localPoint = [centerview convertPoint:position fromView:self];
            if (CGRectContainsPoint(centerview.bounds, localPoint)) return;
            
            for (PolygonView* pointview in centerview.subviews) {
                if (CGRectContainsPoint(pointview.frame, localPoint)) return;
            }
        }
        
        [self createNewPointViewAtUIPosition:position];
        
        [self setNeedsDisplay];
    }
    
}

-(int)savePolygonsInfoInMemory
{
    __Dictionary* polygonsDict = _gameManager->_fileHandler->_polygonsDict;
    std::vector<b2Vec2> b2vertexTest;
    b2Separator* separator = new b2Separator();
    int result = 0;
    
    for (PolygonView* polygon in self.centerViews) {
        NSMutableArray* vertexes = [NSMutableArray arrayWithArray:[polygon getGlLocalVertexes]];
        //转为动画坐标
        int vertexCount = vertexes.count;
        for(int i = 0;i<vertexCount;i++){
            vertexes[i] = [NSValue valueWithCGPoint:CGPointMake([vertexes[i] CGPointValue].x/currentZoomFactor, [vertexes[i] CGPointValue].y/currentZoomFactor)];
        }
        
        if (vertexCount>b2_maxPolygonVertices) {
            result = -1;
            break;
        }
        //
        if (!polygon.isConvex) {

            for(int i = 0;i<vertexCount;i++){
                b2vertexTest.push_back(cc_to_b2Vec([vertexes[i] CGPointValue].x, [vertexes[i] CGPointValue].y));
            }
            
            int ret = separator->Validate(b2vertexTest);
            b2vertexTest.clear();
            if (ret!=0) {
                result = 1;
                break;
            }
        }
        //
        std::string mainKey = StringUtils::format("polygon%d",polygon.tag-1000);
        __Dictionary* propertyDict = __Dictionary::create();
        
        __String* position = __String::createWithFormat("{%f,%f}",polygon.center.x/self.contentSize.width,PAGE_COUNTS - polygon.center.y/(self.contentSize.height/PAGE_COUNTS));
        __Bool* isConvex = __Bool::create(polygon.isConvex);
        __Array* Vertexes = __Array::createWithCapacity(vertexes.count);
        for (NSValue* vertex in vertexes) {
            __String* point = __String::createWithFormat("{%f,%f}",[vertex CGPointValue].x,[vertex CGPointValue].y);
            Vertexes->addObject(point);
        }
        
        propertyDict->setObject(position, "position");
        propertyDict->setObject(isConvex, "isConvex");
        propertyDict->setObject(Vertexes, "vertexes");
        
        polygonsDict->setObject(propertyDict, mainKey);
    }
    
    delete separator;
    return result;
}


-(void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    //红虚线
    if (!isButtonHiden) {
        //
        const CGFloat lengths[] = {static_cast<float>(10.0*currentZoomFactor),static_cast<float>(10.0*currentZoomFactor)};
        CGContextSetLineDash(context, 0.0, lengths,2);
        CGContextSetLineWidth(context, 1);
        CGContextSetRGBStrokeColor(context, 1, 0, 0, 1);
        
        for(int i = 0;i<PAGE_COUNTS;i++)
        {
            CGContextBeginPath(context);
            CGContextMoveToPoint(context,0,i*self.contentSize.height/PAGE_COUNTS);
            CGContextAddLineToPoint(context, self.contentSize.width, i*self.contentSize.height/PAGE_COUNTS);
            CGContextStrokePath(context);
        }
        
        CGContextSetLineDash(context, 0, nil, 0);
    }
    //关卡标签
    for(int i = PAGE_COUNTS;i>0;i--)
    {
        CGContextSetRGBFillColor(context, 1, 1, 1, 1);
        UIFont* font = [UIFont boldSystemFontOfSize:24*currentZoomFactor];
        NSString* currentPageNumber = [[NSString stringWithUTF8String:levelName.c_str()] stringByAppendingFormat:@" - %d",i];
        [currentPageNumber drawAtPoint:CGPointMake(40*currentZoomFactor, 20*currentZoomFactor +(PAGE_COUNTS - i)*self.contentSize.height/PAGE_COUNTS) withFont:font];
    }
    //draw polygons
    const CGFloat* components = CGColorGetComponents(self.lineColor.CGColor);
    
    CGFloat red;
    CGFloat green;
    CGFloat blue;
    CGFloat alpha;
    
    if (CGColorGetNumberOfComponents(self.lineColor.CGColor) == 2) {
        red = 1;
        green = 1;
        blue = 1;
        alpha = 1;
    }else{
        red = components[0];
        green = components[1];
        blue = components[2];
        alpha = components[3];
        if (alpha) alpha = 1;
    }
    
    CGContextSetRGBStrokeColor(context, red, green, blue,alpha);
    CGContextSetLineCap(context, kCGLineCapSquare);
    CGContextSetLineJoin(context, kCGLineJoinRound);
    CGContextSetLineWidth(context, kDefaultPolygonLineWidth*currentZoomFactor);
    
    for (PolygonView* centerview in self.centerViews) {
        NSArray* vertexs = [centerview getUIVertexs];
        CGPoint startPoint = [[vertexs objectAtIndex:0] CGPointValue];
        
        CGContextBeginPath(context);
        CGContextMoveToPoint(context, startPoint.x, startPoint.y);
        for (int i = 1; i<vertexs.count; i++) {
            CGPoint point = [[vertexs objectAtIndex:i] CGPointValue];
            CGContextAddLineToPoint(context,point.x, point.y);
        }
        CGContextClosePath(context);
        CGContextStrokePath(context);
    }
    
    //draw animation path point
//
//    UIBezierPath* bezierPath = [UIBezierPath bezierPath];
//    [bezierPath setLineCapStyle:kCGLineCapSquare];
//    [bezierPath setLineJoinStyle:kCGLineJoinRound];
//    [bezierPath setLineWidth:kDefaultPathPointBorderWidth];
//    
//    [[UIColor whiteColor] setStroke];
//    
//    NSMutableParagraphStyle* style = [[[NSMutableParagraphStyle alloc] init] autorelease];
//    style.alignment = NSTextAlignmentCenter;
//    
//    NSDictionary* attribute = [NSDictionary dictionaryWithObjectsAndKeys:[UIFont systemFontOfSize:kDefaultFontSize], NSFontAttributeName, style,NSParagraphStyleAttributeName, nil];
//
//    for(ItemView* item : _gameManager->_levelEditor->_myViewController->itemViews){
//        if([item getHeightLightState])
//            [[UIColor greenColor] setFill];
//        else
//            [[UIColor redColor] setFill];
//        
//        if(!item->_animationInfos.empty()){
//            //
//            for(AnimationInfo& info : item->_animationInfos){
//                CGPoint position = CGPointMake(item.center.x + info.position.x,item.center.y - info.position.y);
//                [bezierPath moveToPoint:CGPointMake(position.x+kDefaultPathPointRadius,position.y)];
//                [bezierPath addArcWithCenter:position radius:kDefaultPathPointRadius startAngle:0 endAngle:2*M_PI clockwise:YES];
//                [bezierPath stroke];
//                [bezierPath fill];
//                [bezierPath removeAllPoints];
//            }
//            //
//            int i = 1;
//            for(AnimationInfo& info : item->_animationInfos){
//                CGPoint position = CGPointMake(item.center.x + info.position.x,item.center.y - info.position.y);
//                NSString* pathTag = [NSString stringWithFormat:@"%d",i];
//                [pathTag drawInRect:CGRectMake(position.x-kDefaultPathPointRadius, position.y-kDefaultPathPointRadius, 2*kDefaultPathPointRadius, 2*kDefaultPathPointRadius) withAttributes:attribute];
//                i++;
//            }
//        }
//    }
    
    
}

#pragma mark popover
-(void)popupFeaturesPickerPopoverWith:(UIView*)uiview At:(CGPoint)position
{
    self.longPressedView = uiview;
    if (!self.featuresPicker) {
        self.featuresPicker = [[[FeaturesPickerController alloc] initWithStyle:UITableViewStylePlain] autorelease];
        self.featuresPicker._delegate = self;
        self.featuresPickerPopover = [[[UIPopoverController alloc] initWithContentViewController:self.featuresPicker] autorelease];
    }
    
    [self.featuresPickerPopover presentPopoverFromRect:uiview.frame inView:self permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

-(void)optionSelected:(NSString *)option
{
    int count = _gameManager->_levelEditor->_myViewController->itemViews.size();
    if ([option isEqualToString:@"upper"]) {
        [self insertSubview:self.longPressedView atIndex:count-1];
        [self.featuresPickerPopover dismissPopoverAnimated:YES];
    }else if ([option isEqualToString:@"lowest"]){
        [self insertSubview:self.longPressedView atIndex:0];
        [self.featuresPickerPopover dismissPopoverAnimated:YES];
    }else if([option isEqualToString:@"property"]){
        PropertyController* propertyController = [[[PropertyController alloc] init:static_cast<ItemView*>(self.longPressedView)] autorelease];
        propertyController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
        propertyController.modalPresentationStyle = UIModalPresentationFormSheet;
        [self.featuresPickerPopover dismissPopoverAnimated:YES];
        [_gameManager->_levelEditor->_myViewController presentModalViewController:propertyController animated:YES];
    }else if([option isEqualToString:@"addPathPoint"]){
        [self.featuresPickerPopover dismissPopoverAnimated:YES];
        [self addPathPointOnTouch:(ItemView*)self.longPressedView];
    }
    
    
}

#pragma mark animationGroupCount
-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

-(NSInteger)pickerView:(UIPickerView*)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return 10;
}

-(NSString*)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [NSString stringWithFormat:@"%d",row];
}

-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    
}

-(void)addPathPointOnTouch:(ItemView*)itemview
{
    if (itemview.animationGroupCount == 0) {
        animationGroupCountController = [[[UIViewController alloc] init] autorelease];
        animationGroupCountController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
        animationGroupCountController.modalPresentationStyle = UIModalPresentationFormSheet;
        
        UILabel* textLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0.15*width, 0.05*height, 0.4*width, 0.1*height)] autorelease];
        textLabel.text = @"您要创建多少组动画？";
        textLabel.font = [UIFont systemFontOfSize:1.5*kDefaultFontSize];
        textLabel.textAlignment = NSTextAlignmentCenter;
        textLabel.textColor = [UIColor blackColor];
        [animationGroupCountController.view addSubview:textLabel];
        
        animationGroupCountPickerView = [[[UIPickerView alloc] initWithFrame:CGRectMake(0.2*width, 0.1*height, 0.3*width, 0.1*height)] autorelease];
        animationGroupCountPickerView.dataSource = self;
        animationGroupCountPickerView.delegate = self;
        animationGroupCountPickerView.showsSelectionIndicator = YES;
        [animationGroupCountPickerView selectRow:1 inComponent:0 animated:YES];
        [animationGroupCountController.view addSubview:animationGroupCountPickerView];

        UIButton* OkButton = [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
        OkButton.frame = CGRectMake(0.2*width, 0.3*height, 0.1*width, 0.1*height);
        [OkButton setTitle:@"OK" forState:UIControlStateNormal];
        [OkButton addTarget:self action:@selector(animationGroupCountSelectedOK) forControlEvents:UIControlEventTouchUpInside];
        [animationGroupCountController.view addSubview:OkButton];
        
        UIButton* CancelButton = [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
        CancelButton.frame = CGRectMake(0.4*width, 0.3*height, 0.1*width, 0.1*height);
        [CancelButton setTitle:@"Cancel" forState:UIControlStateNormal];
        [CancelButton addTarget:self action:@selector(animationGroupCountSelectedCancel) forControlEvents:UIControlEventTouchUpInside];
        [animationGroupCountController.view addSubview:CancelButton];
        
        [_gameManager->_levelEditor->_myViewController presentModalViewController:animationGroupCountController animated:YES];
    }else{
        [self createNewPathPointForItemView:itemview];
    }
}

-(void)animationGroupCountSelectedOK
{
    int selectedCount = [animationGroupCountPickerView selectedRowInComponent:0];
    if (selectedCount>0) {
        ItemView* itemview = static_cast<ItemView*>(self.longPressedView);
        itemview.animationGroupCount = selectedCount;
        [self createNewPathPointForItemView:itemview];
        [itemview insertAnimationGroupControlInstructions];
    }
    [animationGroupCountController dismissViewControllerAnimated:YES completion:nil];
}

-(void)animationGroupCountSelectedCancel
{
    [animationGroupCountController dismissViewControllerAnimated:YES completion:nil];
}


-(void)dealloc
{
    self.centerViews = nil;
    self.toDealWithPointView = nil;
    self.featuresPicker = nil;
    self.featuresPickerPopover = nil;
    [super dealloc];
}



@end
