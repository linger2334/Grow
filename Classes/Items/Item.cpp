//
//  Item.cpp
//  GrowUp
//
//  Created by wlg on 9/24/14.
//
//

#include "Item.h"

Features_Cicada::Features_Cicada(float W,float IncludedAngle,float FanningDuration,float Interval,float BellyTransparency)
{
    w = W;
    includedAngle = IncludedAngle;
    fanningDuration = FanningDuration;
    interval = Interval;
    bellyTransparency = BellyTransparency;
}

Features_Cicada::Features_Cicada(const Features_Cicada& other)
{
    w = other.w;
    includedAngle = other.includedAngle;
    fanningDuration = other.fanningDuration;
    interval = other.interval;
    bellyTransparency = other.bellyTransparency;
}

Features_Dragon::Features_Dragon(float W,float BackTransparency)
{
    w = W;
    backTransparency = BackTransparency;
}

Features_Dragon::Features_Dragon(const Features_Dragon& other)
{
    w = other.w;
    backTransparency = other.backTransparency;
}

Features_DoubleDragon::Features_DoubleDragon(float W,float BellyTransparency)
{
    w = W;
    bellyTransparency = BellyTransparency;
}

Features_DoubleDragon::Features_DoubleDragon(const Features_DoubleDragon& other)
{
    w = other.w;
    bellyTransparency = other.bellyTransparency;
}

Features_Serpent::Features_Serpent(float AbsorptionRate,float HoleTransparency)
{
    absorptionRate = AbsorptionRate;
    holeTransparency = HoleTransparency;
}

Features_Serpent::Features_Serpent(const Features_Serpent& other)
{
    absorptionRate = other.absorptionRate;
    holeTransparency = other.holeTransparency;
}

Features_GearButton::Features_GearButton(int _bindID,float _sinkSpeed)
{
    bindID = _bindID;
    sinkSpeed = _sinkSpeed;
}

Features_GearButton::Features_GearButton(const Features_GearButton& other)
{
    bindID = kDefaultGearButtonBindID;
    sinkSpeed = other.sinkSpeed;
}

Features_GearGate::Features_GearGate(int _gap,float _startRate)
{
    gap = _gap;
    startRate = _startRate;
}

Features_GearGate::Features_GearGate(const Features_GearGate& other)
{
    gap = other.gap;
    startRate = other.startRate;
}

PolygonInfo::PolygonInfo(int _tag,Vec2 _position,bool _isConvex,std::vector<Vec2>& _vertexes)
{
    tag = _tag;
    position = _position;
    isConvex = _isConvex;
    vertexes = _vertexes;
}

PolygonInfo::PolygonInfo(const PolygonInfo& other)
{
    tag = other.tag;
    position = other.position;
    isConvex = other.isConvex;
    vertexes = other.vertexes;
}

AnimationInfo::AnimationInfo(float _waitTime,float _rotation,float _rotationSpeed,float _moveSpeed,Vec2 _position)
{
    waitTime = _waitTime;
    rotation = _rotation;
    rotationSpeed = _rotationSpeed;
    moveSpeed = _moveSpeed;
    position = _position;
}

AnimationInfo::AnimationInfo(const AnimationInfo& other)
{
    waitTime = other.waitTime;
    rotation = other.rotation;
    rotationSpeed = other.rotationSpeed;
    moveSpeed = other.moveSpeed;
    position = other.position;
}

Item::Item(Item_Type _type,int _id,float _x,float _y,float _angle,float _scale,int _localZorder,bool _isAnimated,float _triggerTime,std::map<std::string,bool>& _animationControlInstructions, std::vector<std::vector<AnimationInfo>>& _animationInfos,void* _features)
{
    type = _type;
    id = _id;
    x = _x;
    y = _y;
    angle = _angle;
    scale = _scale;
    localZorder = _localZorder;
    isAnimated = _isAnimated;
    triggerTime = _triggerTime;
    animationControlInstructions = _animationControlInstructions;
    animationInfos = _animationInfos;
    features = _features;
    if(_features){
        switch(type){
            case Cicada:
            {
                features = new Features_Cicada(*((Features_Cicada*)_features));
            }
                break;
                
            case Dragon_Anti:
            {
                features = new Features_Dragon(*((Features_Dragon*)_features));
            }
                break;
                
            case Dragon_Clockwise:
            {
                features = new Features_Dragon(*((Features_Dragon*)_features));
            }
                break;
            
            case DoubDragon_Anti:
            {
                features = new Features_DoubleDragon(*((Features_DoubleDragon*)_features));
            }
                break;
            case DoubDragon_Clockwise:
            {
                features = new Features_DoubleDragon(*((Features_DoubleDragon*)_features));
            }
                break;
            case Serpent_:
            {
                features = new Features_Serpent(*((Features_Serpent*)_features));
            }
                break;
            case Gear_Button:
            {
                features = new Features_GearButton(*((Features_GearButton*)_features));
            }
                break;
            case Gear_Gate:
            {
                features = new Features_GearGate(*((Features_GearGate*)_features));
            }
                break;
                
            default:
                break;
        }
    }
}

Item::Item(const Item& other)
{
    type = other.type;
    id = other.id;
    x = other.x;
    y = other.y;
    angle = other.angle;
    scale = other.scale;
    localZorder = other.localZorder;
    isAnimated = other.isAnimated;
    triggerTime = other.triggerTime;
    animationControlInstructions = other.animationControlInstructions;
    animationInfos = other.animationInfos;
    features = other.features;
    if(other.features){
        switch(type){
            case Cicada:
            {
                features = new Features_Cicada(*((Features_Cicada*)other.features));
            }
                break;
                
            case Dragon_Anti:
            {
                features = new Features_Dragon(*((Features_Dragon*)other.features));
            }
                break;
                
            case Dragon_Clockwise:
            {
                features = new Features_Dragon(*((Features_Dragon*)other.features));
            }
                break;
                
            case DoubDragon_Anti:
            {
                features = new Features_DoubleDragon(*((Features_DoubleDragon*)other.features));
            }
                break;
                
            case DoubDragon_Clockwise:
            {
                features = new Features_DoubleDragon(*((Features_DoubleDragon*)other.features));
            }
                break;
            case Serpent_:
            {
                features = new Features_Serpent(*((Features_Serpent*)other.features));
            }
                break;
            case Gear_Button:
            {
                features = new Features_GearButton(*((Features_GearButton*)other.features));
            }
                break;
            case Gear_Gate:
            {
                features = new Features_GearGate(*((Features_GearGate*)other.features));
            }
                break;
                
            default:
                break;
        }
    }
}

Item::~Item()
{
    if(features){
        switch(type){
            case Cicada:
            {
                delete (Features_Cicada*)features;
            }
                break;
                
            case Dragon_Anti:
            {
                delete (Features_Dragon*)features;
            }
                break;
                
            case Dragon_Clockwise:
            {
                delete (Features_Dragon*)features;
            }
                break;
                
            case DoubDragon_Anti:
            {
                delete (Features_DoubleDragon*)features;
            }
                break;
                
            case DoubDragon_Clockwise:
            {
                delete (Features_DoubleDragon*)features;
            }
                break;
                
            case Serpent_:
            {
                delete (Features_Serpent*)features;
            }
                break;
            case Gear_Button:
            {
                delete (Features_GearButton*)features;
            }
                break;
            case Gear_Gate:
            {
                delete (Features_GearGate*)features;
            }
                break;
            default:
                break;
        }
    }
}


bool Itemlesser(Item& item1,Item& item2)
{
    return item1.localZorder < item2.localZorder;
}