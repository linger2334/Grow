//
//  ItemModel.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#include "GameManager.h"
#include "ItemModel.h"
#include "Flame.h"
#include "Rock.h"
#include "Cicada.h"
#include "Dragon.h"
#include "DoubleDragon.h"
#include "Serpent.h"
#include "Eye.h"
#include "Polygon.h"

ItemModel::ItemModel():_body(nullptr),_animatesParallel(nullptr),_animatesCardinalSpline(nullptr),_animatesCustomCurve(nullptr)
{
    
}

ItemModel::~ItemModel()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    if (_body&&world) {
        world->DestroyBody(_body);
        _body = nullptr;
    }
    
    CC_SAFE_RELEASE_NULL(_animatesParallel);
    CC_SAFE_RELEASE_NULL(_animatesCardinalSpline);
    CC_SAFE_RELEASE_NULL(_animatesCustomCurve);
}

ItemModel* ItemModel::create(Item& item)
{
    ItemModel *itemModel = nullptr;
    switch(item.type){
        case Flame_Red:
            itemModel = Flame::create(item);
            break;
        case Flame_Green:
            itemModel = Flame::create(item);
            break;
        case Flame_Blue:
            itemModel = Flame::create(item);
            break;
        case Flame_White:
            itemModel = Flame::create(item);
            break;
        case Flame_Orange:
            itemModel = Flame::create(item);
            break;
        case Cicada:
            itemModel = Cicada::create(item);
            break;
        case Dragon_Anti:
            itemModel = Dragon::create(item);
            break;
        case Dragon_Clockwise:
            itemModel = Dragon::create(item);
            break;
        case DoubDragon_Anti:
            itemModel = DoubleDragon::create(item);
            break;
        case DoubDragon_Clockwise:
            itemModel = DoubleDragon::create(item);
            break;
        case Serpent_:
            itemModel = Serpent::create(item);
            break;
        case Eye_:
            itemModel = Eye::create(item);
            break;
        case Rock_Circle:
            itemModel = Rock::create(item);
            break;
        case Rock_Ellipse:
            itemModel = Rock::create(item);
            break;
        case Rock_Gray:
            itemModel = Rock::create(item);
            break;
        default:
            break;
    }
    
    return itemModel;
}

bool ItemModel::init(Item &item)
{
    bool result;
    if (Sprite::init()) {
        _type = item.type;
        setTag(item.id);
        setPosition(item.x*WinSize.width, item.y*WinSize.height);
        setAnimatedOn(item.isAnimated);
        setTriggerTime(item.triggerTime);
        
        if (isAnimated) {
            animationGroupCount = item.animationInfos.size();
            createAnimates(item.animationInfos,item.animationControlInstructions);
        }
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

void ItemModel::createAnimates(std::vector<std::vector<AnimationInfo>>& animationInfos,std::map<std::string,bool>& animationControlInstructions)
{
    Vector<FiniteTimeAction*> allParallelAnimation;
    Vector<FiniteTimeAction*> eachAnimationGroup;
    ActionInterval* animationGroup = nullptr;
    Vec2 prePosition = getPosition();
    Vec2 deltaPosition = Vec2(0,0);
    float t = 0;
    
    for (int i = 0; i< animationGroupCount; i++) {
        prePosition = getPosition();
        bool isLoop = animationControlInstructions.at(StringUtils::format("group%d",i));
        for (AnimationInfo& info : animationInfos.at(i)) {
            ActionInterval* delay = DelayTime::create(info.waitTime);
            ActionInterval* rotate = RotateBy::create(info.rotation/info.rotationSpeed, info.rotation);
            deltaPosition = getPosition() + info.position - prePosition;
            t = deltaPosition.length()/info.moveSpeed;
            ActionInterval* moveBy = MoveBy::create(t, deltaPosition);
            
            Sequence* singleAnimation = Sequence::create(delay,rotate,moveBy,NULL);
            eachAnimationGroup.pushBack(singleAnimation);
            prePosition = getPosition() + info.position;
        }
        if(eachAnimationGroup.empty()) continue;
        //串联单条动画
        animationGroup = Sequence::create(eachAnimationGroup);
        if (isLoop) {
            animationGroup = RepeatForever::create(animationGroup);
        }
        allParallelAnimation.pushBack(animationGroup);
        eachAnimationGroup.clear();
    }
    //并联组动画
//    _animatesParallel = Spawn::create(allParallelAnimation);
    _animatesParallel = animationGroup;
    CC_SAFE_RETAIN(_animatesParallel);
}

void ItemModel::createCardinalSplineAnimates(std::vector<AnimationInfo> &animationInfos)
{
    Vec2 prePosition = getPosition();
    float duration = 0;
    PointArray* pathPoint = PointArray::create(animationInfos.size()+1);
    pathPoint->addControlPoint(Vec2::ZERO);
    for (AnimationInfo& info : animationInfos) {
        pathPoint->addControlPoint(info.position);
        float time = (getPosition() + info.position).distance(prePosition)/info.moveSpeed;
        duration += time;
        
        prePosition = getPosition() + info.position;
    }
    _animatesCardinalSpline = CardinalSplineBy::create(duration, pathPoint, kDefaultCardinalSplineTension);
    CC_SAFE_RETAIN(_animatesCardinalSpline);
}

void ItemModel::createCustomCurveAnimates(std::vector<AnimationInfo> &animationInfos)
{
    
}

void ItemModel::update(float dt)
{
    float beginningPointY = WinSize.height-getBoundingBox().size.height/2;
    Vec2 pointInGl = getParent()->convertToWorldSpace(getPosition());
    if (pointInGl.y<beginningPointY) {
        if (isAnimated&&_animatesParallel){
            runAction(_animatesParallel);
        }
        unscheduleUpdate();
    }
    
}
