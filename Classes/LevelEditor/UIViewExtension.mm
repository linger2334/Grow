//
//  UIViewExtension.m
//  Grow
//
//  Created by 林纲 王 on 14-10-6.
//
//
#import <objc/runtime.h>
#import "UIViewExtension.h"
#include "GameManager.h"


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

-(id)init:(Item&)item
{
    self = [super init];
    const float width = GameManager::getInstance()->editor_width;
    const float height = GameManager::getInstance()->editor_height;
    const float contentscale = GameManager::getInstance()->editor_contentscale;
    
    if (self) {
        
        itemtype = item.type;
        float x = item.x*width;
        float y = (PAGE_COUNTS - item.y)*height;

        self.center = CGPointMake(x, y);
        [self setTag:item.id];
        [self setHeightLightState:NO];
        features = NULL;
        
        switch (itemtype) {
            case Flame_Red:
                [self setImage:[UIImage imageNamed:@IMAGE_FLAME_RED]];
                self.bounds = CGRectMake(0, 0, FLAME_RED_WIDTH*contentscale, FLAME_RED_HEIGHT*contentscale);
                break;
            case Flame_Green:
                [self setImage:[UIImage imageNamed:@IMAGE_FLAME_GREEN]];
                self.bounds = CGRectMake(0, 0, FLAME_GREEN_WIDTH*contentscale, FLAME_GREEN_HEIGHT*contentscale);
                break;
            case Flame_Blue:
                [self setImage:[UIImage imageNamed:@IMAGE_FLAME_BLUE]];
                self.bounds = CGRectMake(0, 0, FLAME_BLUE_WIDTH*contentscale, FLAME_BLUE_HEIGHT*contentscale);
                break;
            case Flame_White:
                [self setImage:[UIImage imageNamed:@IMAGE_FLAME_WHITE]];
                self.bounds = CGRectMake(0, 0, FLAME_WHITE_WIDTH*contentscale, FLAME_WHITE_HEIGHT*contentscale);
                break;
            case Flame_Orange:
                [self setImage:[UIImage imageNamed:@IMAGE_FLAME_ORANGE]];
                self.bounds = CGRectMake(0, 0, FLAME_ORANGE_WIDTH*contentscale, FLAME_ORANGE_HEIGHT*contentscale);
                break;
            case Rock_Circle:
                [self setImage:[UIImage imageNamed:@IMAGE_ROCK_CIRCLE]];
                self.bounds = CGRectMake(0, 0, ROCK_CIRCLE_WIDTH*contentscale, ROCK_CIRCLE_HEIGHT*contentscale);
                break;
            case Rock_Ellipse:
                [self setImage:[UIImage imageNamed:@IMAGE_ROCK_ELLIPSE]];
                self.bounds = CGRectMake(0, 0, ROCK_ELLIPSE_WIDTH*contentscale, ROCK_ELLIPSE_HEIGHT*contentscale);
                break;
            case Rock_Gray:
                [self setImage:[UIImage imageNamed:@IMAGE_ROCK_GRAY]];
                self.bounds = CGRectMake(0, 0, ROCK_GRAY_WIDTH*contentscale, ROCK_GRAY_HEIGHT*contentscale);
                break;
            case Cicada:
            {
                [self setImage:[UIImage imageNamed:@IMAGE_CICADA]];
                self.bounds = CGRectMake(0, 0, CICADA_WIDTH*contentscale, CICADA_HEIGHT*contentscale);
                
                if(item.features){
                    features = new Features_Cicada(*((Features_Cicada*)item.features));
                }else{
                    features = new Features_Cicada();
                }
            }
                break;
            case Dragon_Anti:
            {
                [self setImage:[UIImage imageNamed:@IMAGE_DRAGON_ANTI]];
                self.bounds = CGRectMake(0, 0, DRAGON_ANTI_WIDTH*contentscale, DRAGON_ANTI_HEIGHT*contentscale);
                
                if(item.features){
                    features = new Features_Dragon(*((Features_Dragon*)item.features));
                }else{
                    features = new Features_Dragon();
                }
            }
                break;
            case Dragon_Clockwise:
            {
                [self setImage:[UIImage imageNamed:@IMAGE_DRAGON_CLOCKWISE]];
                self.bounds = CGRectMake(0, 0, DRAGON_CLOCKWISE_WIDTH*contentscale, DRAGON_CLOCKWISE_HEIGHT*contentscale);
                
                if(item.features){
                    features = new Features_Dragon(*((Features_Dragon*)item.features));
                }else{
                    features = new Features_Dragon();
                }
            }
                break;
            case Eye:
                [self setImage:[UIImage imageNamed:@IMAGE_EYE]];
                self.bounds = CGRectMake(0, 0, EYE_WIDTH*contentscale, EYE_HEIGHT*contentscale);
                break;
            default:
                break;
        }
        self.transform = CGAffineTransformRotate(self.transform,item.angle);
        self.transform = CGAffineTransformScale(self.transform,item.scale,item.scale);
    }
    
    return self;
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
        case Dragon_Anti:
        {
            if (features) {
                delete (Features_Dragon*)features;
            }
        }
        case Dragon_Clockwise:
        {
            if (features) {
                delete (Features_Dragon*)features;
            }
        }
        default:
            break;
    }
    
    [super dealloc];
}

@end










