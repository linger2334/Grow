//
//  UIViewExtension.m
//  Grow
//
//  Created by 林纲 王 on 14-10-6.
//
//
#import <objc/runtime.h>
#import <QuartzCore/QuartzCore.h>
#import "UIViewExtension.h"
#include "GameManager.h"
#include "PolygonView.h"
#include "LevelEditor.h"


@implementation UIView(Hierarchy)

-(int)getSubviewIndex
{
    return [self.superview.subviews indexOfObject:self];
}

-(void)bringToFront
{
    [self.superview bringSubviewToFront:self];
}

-(void)sendToBack
{
    [self.superview sendSubviewToBack:self];
}

-(void)bringOneLevelUp
{
    int currentIndex = [self getSubviewIndex];
    [self.superview exchangeSubviewAtIndex:currentIndex withSubviewAtIndex:currentIndex+1];
}

-(void)sendOneLevelDown
{
    int currentIndex = [self getSubviewIndex];
    [self.superview exchangeSubviewAtIndex:currentIndex withSubviewAtIndex:currentIndex - 1];
}

-(BOOL)isInFront
{
    return ([self.superview.subviews lastObject]==self);
}

-(BOOL)isAtBack
{
    return ([self.superview.subviews objectAtIndex:0]==self);
}

-(void)swapDepthsWithView:(UIView*)swapView
{
    [self.superview exchangeSubviewAtIndex:[self getSubviewIndex] withSubviewAtIndex:[swapView getSubviewIndex]];
}

@end



@implementation UIView (LevelEditor)

-(void)setHeightLightState:(BOOL)isHeightLight
{
    objc_setAssociatedObject(self, "isHeightLight", [NSNumber numberWithBool:isHeightLight], OBJC_ASSOCIATION_ASSIGN);
}

-(BOOL)getHeightLightState
{
    return [objc_getAssociatedObject(self, "isHeightLight") boolValue];
}

-(void)setTypeName:(NSString*)typeName
{
    objc_setAssociatedObject(self, "name", typeName, OBJC_ASSOCIATION_COPY_NONATOMIC);
}

-(NSString*)getTypeName
{
    return objc_getAssociatedObject(self, "name");
}


-(void)itemAddGestureRecognizerWithTarget:(id)target
{
    UIPanGestureRecognizer* panGestureRecognizer =[[UIPanGestureRecognizer alloc] initWithTarget:target action:@selector(handlePan:)];
    UIRotationGestureRecognizer* rotateRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:target action:@selector(handleRotate:)];
    UITapGestureRecognizer* tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:target action:@selector(handleTap:)];
    tapRecognizer.numberOfTapsRequired = 1;
    UILongPressGestureRecognizer* longPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:target action:@selector(handleLongPress:)];
    longPressRecognizer.minimumPressDuration =0.5;
    //询问代理
    panGestureRecognizer.delegate = target;
    longPressRecognizer.delegate = target;
    
    [self addGestureRecognizer:panGestureRecognizer];
    [self addGestureRecognizer:rotateRecognizer];
    [self addGestureRecognizer:tapRecognizer];
    [self addGestureRecognizer:longPressRecognizer];
    
    [panGestureRecognizer release];
    [rotateRecognizer release];
    [tapRecognizer release];
    [longPressRecognizer release];
    
    //互斥
    [tapRecognizer requireGestureRecognizerToFail:panGestureRecognizer];
    
    [self setUserInteractionEnabled:YES];
}



@end

@implementation ItemView

@synthesize animationGroupCount;
@synthesize bindButton;

-(id)init:(Item&)item
{
    self = [super init];
    const float scrollviewWidth = GameManager::getInstance()->_levelEditor->_myViewController->_scrollView.contentSize.width;
    const float scrollviewHeight = GameManager::getInstance()->_levelEditor->_myViewController->_scrollView.contentSize.height/PAGE_COUNTS;
    const float currentZoomFactor = GameManager::getInstance()->_levelEditor->_myViewController->_scrollView.currentZoomFactor;
    bindButton = nil;
    
    if (self) {
        
        itemtype = item.type;
        isAnimated = item.isAnimated;
        triggerTime = item.triggerTime;
        elapsedTime = item.elapsedTime;
        bindedTriggerID = item.bindedTriggerID;
        isAutoSmoothing = item.isAutoSmoothing;
        _animationControlInstructions = item.animationControlInstructions;
        _animationInfos = item.animationInfos;
        animationGroupCount = item.animationInfos.size();
        float x = item.x*scrollviewWidth;
        float y = (PAGE_COUNTS - item.y)*scrollviewHeight;
        
        self.center = CGPointMake(x, y);
        [self setTag:item.id];
        [self setHeightLightState:NO];
        features = NULL;
        
        std::string imageFilename;
        int levelId = *(GameManager::getInstance()->_fileHandler->_filename.c_str() + strlen("levels/level ")) - ('0' - 0);
        std::string suffix = StringUtils::format("%d.png",levelId);
        switch (itemtype) {
            case Flame_Blue:
                imageFilename = IMAGE_FLAME_BLUE;
                break;
            case Flame_Orange:
                imageFilename = IMAGE_FLAME_ORANGE;
                break;
            case Flame_Violet:
                imageFilename = IMAGE_FLAME_VIOLET;
                break;
            case Flame_White:
                imageFilename = IMAGE_FLAME_WHITE;
                break;;
            case Rock_Circle:
                imageFilename = IMAGE_ROCK_CIRCLE + suffix;
                break;
            case Rock_Cover:
                imageFilename = IMAGE_ROCK_COVER + suffix;
                break;
            case Rock_Crinkle:
                imageFilename = IMAGE_ROCK_CRINKLE + suffix;
                break;
            case Rock_Cross:
                imageFilename = IMAGE_ROCK_CROSS + suffix;
                break;
            case Rock_Dagger:
                imageFilename = IMAGE_ROCK_DAGGER + suffix;
                break;
            case Rock_Ellipse:
                imageFilename = IMAGE_ROCK_ELLIPSE + suffix;
                break;
            case Rock_Mount:
                imageFilename = IMAGE_ROCK_MOUNT + suffix;
                break;
            case Rock_MountInv:
                imageFilename = IMAGE_ROCK_MOUNTINV + suffix;
                break;
            case Rock_Ordinary:
                imageFilename = IMAGE_ROCK_ORDINARY + suffix;
                break;
            case Rock_Ovoid:
                imageFilename = IMAGE_ROCK_OVOID + suffix;
                break;
            case Rock_Pebble:
                imageFilename = IMAGE_ROCK_PEBBLE + suffix;
                break;
            case Rock_Pillar:
                imageFilename = IMAGE_ROCK_PILLAR + suffix;
                break;
            case Rock_Pocket:
                imageFilename = IMAGE_ROCK_POCKET + suffix;
                break;
            case Rock_Rect:
                imageFilename = IMAGE_ROCK_RECT + suffix;
                break;
            case Rock_Trape:
                imageFilename = IMAGE_ROCK_TRAPE + suffix;
                break;
            case Cicada:
            {
                if(item.features){
                    features = new Features_Cicada(*((Features_Cicada*)item.features));
                }else{
                    features = new Features_Cicada();
                }
                
                imageFilename = ((Features_Cicada*)features)->isReversalStatus? IMAGE_CICADA_BLUE : IMAGE_CICADA_RED;
            }
                break;
            case DoubDragon_Anti:
            {
                imageFilename = IMAGE_DOUBDRAGON_ANTI;
                if(item.features){
                    features = new Features_DoubleDragon(*((Features_DoubleDragon*)item.features));
                }else{
                    features = new Features_DoubleDragon();
                }
                
            }
                break;
            case DoubDragon_Clockwise:
            {
                imageFilename = IMAGE_DOUBDRAGON_CLOCKWISE;
                if(item.features){
                    features = new Features_DoubleDragon(*((Features_DoubleDragon*)item.features));
                }else{
                    features = new Features_DoubleDragon();
                }
                
            }
                break;
            case Serpent_:
            {
                imageFilename = IMAGE_SERPENT;
                if (item.features) {
                    features = new Features_Serpent(*((Features_Serpent*)item.features));
                }else{
                    features = new Features_Serpent();
                }
            }
                break;
            case Gear_Button:
            {
                imageFilename = IMAGE_GEARBUTTON;
                if(item.features){
                    Features_GearButton* feat = (Features_GearButton*)item.features;
                    features = new Features_GearButton(*feat);
                    if(feat->bindID != kDefaultGearButtonBindID){
                        GameManager::getInstance()->_levelEditor->_myViewController->_boundButtons.push_back(self);
                    }
                    
                }else{
                    features = new Features_GearButton();
                }
            }
                break;
            case Gear_Gate:
            {
                imageFilename = IMAGE_GEARGATE;
                if (item.features) {
                    features = new Features_GearGate(*((Features_GearGate*)item.features));
                }else{
                    features = new Features_GearGate();
                }
                GameManager::getInstance()->_levelEditor->_myViewController->_gates.push_back(self);
            }
                break;
            case Gear_Reversal:
                imageFilename = IMAGE_GEARREVERSAL;
                break;
            case Barrier_:
                imageFilename = IMAGE_BARRIER_RED;
                break;
            case Decoration_Bridge:
                imageFilename = IMAGE_DECORATION_BRIDGE;
                break;
            case Decoration_Flower:
            {
                imageFilename = IMAGE_DECORATION_FLOWER;
                if (item.features) {
                    Features_DecorationFlower* feat = (Features_DecorationFlower*)item.features;
                    features = new Features_DecorationFlower(*feat);
                }else{
                    features = new Features_DecorationFlower();
                }
            }
                break;
            case Decoration_FlowerInv:
            {
                imageFilename = IMAGE_DECORATION_FLOWERINV;
                if (item.features) {
                    Features_DecorationFlower* feat = (Features_DecorationFlower*)item.features;
                    features = new Features_DecorationFlower(*feat);
                }else{
                    features = new Features_DecorationFlower();
                }
            }
                break;
            case Decoration_Pendant:
                imageFilename = IMAGE_DECORATION_PENDANT;
                break;
            case Sprouts_Dextro:
            {
                imageFilename = IMAGE_SPROUTS_DEXTRO;
                if (item.features) {
                    Features_Sprouts* feat = (Features_Sprouts*)item.features;
                    features = new Features_Sprouts(*feat);
                }else{
                    features = new Features_Sprouts();
                }
            }
                break;
            case Sprouts_Levo:
            {
                imageFilename = IMAGE_SPROUTS_LEVO;
                if (item.features) {
                    Features_Sprouts* feat = (Features_Sprouts*)item.features;
                    features = new Features_Sprouts(*feat);
                }else{
                    features = new Features_Sprouts();
                }
            }
                break;
            case Sprouts_Slope:
            {
                imageFilename = IMAGE_SPROUTS_SLOPE;
                if (item.features) {
                    Features_Sprouts* feat = (Features_Sprouts*)item.features;
                    features = new Features_Sprouts(*feat);
                }else{
                    features = new Features_Sprouts();
                }
            }
                break;
                
            default:
                break;
        }
        
        [self setImage:[UIImage imageNamed:[NSString stringWithUTF8String:imageFilename.c_str()]]];
        self.bounds = CGRectMake(0, 0, self.image.size.width, self.image.size.height);
        self.transform = CGAffineTransformRotate(self.transform,item.angle);
        self.transform = CGAffineTransformScale(self.transform,item.scale,item.scale);
    }
    self.bounds = CGRectMake(0, 0, self.bounds.size.width*currentZoomFactor, self.bounds.size.height*currentZoomFactor);
    self.clipsToBounds = NO;
    return self;
}

-(float)getRotateAngle
{
    float angle = kDefaultAngle;
    float a = self.transform.a;
    float b = self.transform.b;
    float c = self.transform.c;
    float d = self.transform.d;
    
    if(a ==0.0&&b>0.0&&c<0.0&&d==0.0)
    {
        angle = M_PI_2;
    }
    else if(a<0.0&&b==0.0&&c==0.0&&d<0.0)
    {
        angle = M_PI;
    }
    else if(a==0.0&&b<0.0&&c>0.0&&d==0.0)
    {
        angle = 3*M_PI_2;
    }
    else
    {
        angle = atanf(b/a);
        if (angle*b<0.0) {
            angle+=M_PI;
        }
    }
    
    return angle;
}

-(void)createAllPathPoints
{
    std::vector<AnimationInfo> eachGroupCorrespondInfos;
    if(!_animationInfos.empty()){
        for(int subIndex = 0;subIndex<_animationInfos.front().size();subIndex++){
            for (int i = 0; i<animationGroupCount; i++) {
                eachGroupCorrespondInfos.push_back(_animationInfos.at(i).at(subIndex));
            }
            PolygonView* newPathPoint = [self createPathPointWith:eachGroupCorrespondInfos];
            OCScrollView* scrollview = GameManager::getInstance()->_levelEditor->_myViewController->_scrollView;
            [scrollview addGestureRecognizerForCenterView:newPathPoint];
            
            self->_pathPoints.push_back(newPathPoint);
            [scrollview addSubview:newPathPoint];
            [newPathPoint release];
            
            eachGroupCorrespondInfos.clear();
        }
    }
}

-(PolygonView*)createPathPointWith:(std::vector<AnimationInfo>&) eachGroupCorrespondInfos
{
    float currentZoomFactor = GameManager::getInstance()->_levelEditor->_myViewController->_scrollView.currentZoomFactor;
    PolygonView* pathPoint = [[PolygonView alloc] initWithEachGroupCorrespondInfos:eachGroupCorrespondInfos];
    pathPoint->_pathParent = self;
    pathPoint.center = CGPointMake(self.center.x + eachGroupCorrespondInfos.front().position.x*currentZoomFactor,self.center.y - eachGroupCorrespondInfos.front().position.y*currentZoomFactor);
    pathPoint.bounds = CGRectMake(0,0,2*kDefaultPathPointRadius*currentZoomFactor,2*kDefaultPathPointRadius*currentZoomFactor);
    pathPoint.pointType = @"pathpoint";
    pathPoint.tag = _pathPoints.size();
    
    pathPoint.pathNum = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, pathPoint.bounds.size.width, pathPoint.bounds.size.height)] autorelease];
    pathPoint.pathNum.text = [NSString stringWithFormat:@"%d", pathPoint.tag +1];
    pathPoint.pathNum.font = [UIFont systemFontOfSize:kDefaultFontSize*currentZoomFactor];
    pathPoint.pathNum.textAlignment = NSTextAlignmentCenter;
    pathPoint.pathNum.textColor = [UIColor whiteColor];
    [pathPoint addSubview:pathPoint.pathNum];
    
    return pathPoint;
}

-(void)translateAllPathPoints:(CGPoint)translation
{
    for(PolygonView* pathPoint : self->_pathPoints){
        pathPoint.center = CGPointMake(pathPoint.center.x + translation.x,pathPoint.center.y + translation.y);
    }
}

-(void)heightLightPathPoints
{
    for(PolygonView* pathPoint : self->_pathPoints){
        pathPoint.backgroundColor = pathPoint.isSelected? pathPoint.selectedColor : pathPoint.heightLightedColor;
    }
}

-(void)cancelHeightLightPathPoints
{
    for (PolygonView* pathPoint : self->_pathPoints) {
        pathPoint.backgroundColor = pathPoint.isSelected? pathPoint.selectedColor : pathPoint.defaultColor;
    }
}

-(void)insertAnimationGroupControlInstructions
{
    for (int i = 0; i<animationGroupCount; i++) {
        std::string groupName = [[NSString stringWithFormat:@"group%d",i] UTF8String];
        _animationControlInstructions.insert(std::make_pair(groupName,kDefaultAnimatedLoopState));
    }
}

-(BOOL)isBorderCreated
{
    BOOL created = NO;
    for (CALayer* layer in self.layer.sublayers) {
        if ([layer.name isEqualToString:@"boder"]) {
            created = YES;
            break;
        }
    }
    return created;
}

-(void)addBorder
{
    CAShapeLayer* border1 = [CAShapeLayer layer];//便利构造器，autorelease
    [border1 setName:@"border"];
    border1.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
    border1.fillColor = nil;
    border1.lineDashPattern = nil;
    [self.layer addSublayer:border1];
    border1.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
    border1.frame = self.bounds;
    
    CAShapeLayer* border2 = [CAShapeLayer layer];
    [border2 setName:@"border"];
    border2.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
    border2.fillColor = nil;
    border2.lineDashPattern = nil;
    [self.layer addSublayer:border2];
    border2.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
    border2.frame = self.bounds;
    
    CAShapeLayer* border3 = [CAShapeLayer layer];
    [border3 setName:@"border"];
    border3.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
    border3.fillColor = nil;
    border3.lineDashPattern = nil;
    [self.layer addSublayer:border3];
    border3.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
    border3.frame = self.bounds;
    
    CAShapeLayer* border4 = [CAShapeLayer layer];
    [border4 setName:@"border"];
    border4.strokeColor = [UIColor colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1].CGColor;
    border4.fillColor = nil;
    border4.lineDashPattern = nil;
    [self.layer addSublayer:border4];
    border4.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
    border4.frame = self.bounds;
}

-(void)showBorder
{
    for(CAShapeLayer* shapeLayer in self.layer.sublayers)
    {
        if ([shapeLayer.name isEqualToString:@"border"]){
            shapeLayer.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
            [shapeLayer setHidden:NO];
        }
    }
}

-(void)refreshBorder
{
    for(CAShapeLayer* shapeLayer in self.layer.sublayers){
        if ([shapeLayer.name isEqualToString:@"border"]) {
            shapeLayer.path = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
        }
    }
    
}

-(void)hideBorder
{
    for(CAShapeLayer* shapeLayer in self.layer.sublayers)
    {
        if ([shapeLayer.name isEqualToString:@"border"]){
            [shapeLayer setHidden:YES];
        }
    }
}

-(std::string)convertTypeToString
{
    std::string name;
    switch (self->itemtype) {
        case Flame_Blue:
            name = "Flame_Blue";
            break;
        case Flame_Orange:
            name = "Flame_Orange";
            break;
        case Flame_Violet:
            name = "Flame_Violet";
            break;
        case Flame_White:
            name = "Flame_White";
            break;
        case Cicada:
            name = "Cicada";
            break;
        case DoubDragon_Anti:
            name = "DoubDragon_Anti";
            break;
        case DoubDragon_Clockwise:
            name = "DoubDragon_Clockwise";
            break;
        case Serpent_:
            name = "Serpent";
            break;
        case Gear_Button:
            name = "Gear_Button";
            break;
        case Gear_Gate:
            name = "Gear_Gate";
            break;
        case Gear_Reversal:
            name = "Gear_Reversal";
            break;
        case Barrier_:
            name = "Barrier";
            break;
        case Decoration_Bridge:
            name = "Decoration_Bridge";
            break;
        case Decoration_Flower:
            name = "Decoration_Flower";
            break;
        case Decoration_FlowerInv:
            name = "Decoration_FlowerInv";
            break;
        case Decoration_Pendant:
            name = "Decoration_Pendant";
            break;
        case Sprouts_Dextro:
            name = "Sprouts_Dextro";
            break;
        case Sprouts_Levo:
            name = "Sprouts_Levo";
            break;
        case Sprouts_Slope:
            name = "Sprouts_Slope";
            break;
        case Rock_Circle:
            name = "Rock_Circle";
            break;
        case Rock_Cover:
            name = "Rock_Cover";
            break;
        case Rock_Crinkle:
            name = "Rock_Crinkle";
            break;
        case Rock_Cross:
            name = "Rock_Cross";
            break;
        case Rock_Dagger:
            name = "Rock_Dagger";
            break;
        case Rock_Ellipse:
            name = "Rock_Ellipse";
            break;
        case Rock_Mount:
            name = "Rock_Mount";
            break;
        case Rock_MountInv:
            name = "Rock_MountInv";
            break;
        case Rock_Ordinary:
            name = "Rock_Ordinary";
            break;
        case Rock_Ovoid:
            name = "Rock_Ovoid";
            break;
        case Rock_Pebble:
            name = "Rock_Pebble";
            break;
        case Rock_Pillar:
            name = "Rock_Pillar";
            break;
        case Rock_Pocket:
            name = "Rock_Pocket";
            break;
        case Rock_Rect:
            name = "Rock_Rect";
            break;
        case Rock_Trape:
            name = "Rock_Trape";
            break;
        default:
            break;
    }
    return name;
}

-(NSComparisonResult)compareWithID:(ItemView*)other
{
    ItemView* myself = (ItemView*)self;
    NSNumber* myID = [NSNumber numberWithInt:myself.tag];
    NSNumber* otherID = [NSNumber numberWithInt:other.tag];
    
    return [myID compare:otherID];
}

-(void)dealloc
{    
    switch (itemtype) {
        case Cicada:
        {
            if (features) {
                delete (Features_Cicada*)features;
            }
        }
            break;
        case DoubDragon_Anti:
        {
            if (features) {
                delete (Features_DoubleDragon*)features;
            }
        }
            break;
        case DoubDragon_Clockwise:
        {
            if (features) {
                delete (Features_DoubleDragon*)features;
            }
        }
            break;
        case Serpent_:
        {
            if (features) {
                delete (Features_Serpent*)features;
            }
        }
            break;
        case Gear_Button:
        {
            if(features)
                delete (Features_GearButton*)features;
        }
            break;
        case Gear_Gate:
        {
            if (features)
                delete (Features_GearGate*)features;
        }
            break;
        case Decoration_Flower:
        {
            if (features) {
                delete (Features_DecorationFlower*)features;
            }
        }
            break;
        case Decoration_FlowerInv:
        {
            if (features) {
                delete (Features_DecorationFlower*)features;
            }
        }
            break;
        case Sprouts_Dextro:
        {
            if (features) {
                delete (Features_Sprouts*)features;
            }
        }
            break;
        case Sprouts_Levo:
        {
            if (features) {
                delete (Features_Sprouts*)features;
            }
        }
            break;
        case Sprouts_Slope:
        {
            if (features) {
                delete (Features_Sprouts*)features;
            }
        }
            break;
            
        default:
            break;
    }
    
    [super dealloc];
}

@end









