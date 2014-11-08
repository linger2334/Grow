//
//  Item.h
//  GrowUp
//
//  Created by wlg on 8/22/14.
//
//

#ifndef __GrowUp__Item__
#define __GrowUp__Item__

#include <iostream>
#include "Macro.h"

struct Features_Cicada{
    float w;
    float includedAngle;
    float fanningDuration;
    float interval;
    float bellyTransparency;
    Features_Cicada(float W=kDefaultCicadaW,float IncludedAngle=kDefaultCicadaIncludedAngle,float FanningDuration=kDefaultCicadaFanningDuration,float Interval=kDefaultCicadaInterval,float BellyTransparency=kDefaultCicadaBellyTransparency);
    Features_Cicada(const Features_Cicada& );
};

struct Features_Dragon{
    float w;
    float backTransparency;
    Features_Dragon(float W = kDefaultDragonW,float BackTransparency = kDefaultDragonBackTransparency);
    Features_Dragon(const Features_Dragon& );
};

struct Features_DoubleDragon{
    float w;
    float bellyTransparency;
    
    Features_DoubleDragon(float W = kDefaultDragonW,float BellyTransparency=kDefaultDragonBellyTransparency);
    Features_DoubleDragon(const Features_DoubleDragon& );
    
};

struct Features_Serpent{
    float absorptionRate;
    float holeTransparency;
    
    Features_Serpent(float AbsorptionRate = kDefaultSerpentAbsorptionRate, float HoleTransparency = kDefaultSerpentHoleTransparency);
    Features_Serpent(const Features_Serpent& );
};

struct Features_GearButton{
    int bindID;
    float sinkSpeed;
    
    Features_GearButton(int _bindID=kDefaultGearButtonBindID,float _sinkSpeed=kDefaultGearButtonSinkSpeed);
    Features_GearButton(const Features_GearButton&);
};

struct Features_GearGate{
    int gap;
    float startRate;
    
    Features_GearGate(int _gap=kDefaultGearGateGap,float _startRate=kDefaultGearGateStartRate);
    Features_GearGate(const Features_GearGate& );
};

struct PolygonInfo{
    int tag;
    Vec2 position;
    bool isConvex;
    std::vector<Vec2> vertexes;
    
    PolygonInfo(int _tag,Vec2 _position,bool _isConvex,std::vector<Vec2>& _vertexes);
    PolygonInfo(const PolygonInfo& );
};

struct AnimationInfo{
    float waitTime;
    float rotation;
    float rotationSpeed;
    float moveSpeed;
    Vec2 position;

    AnimationInfo(float _waitTime=kDefaultAnimationWaitTime,float _rotation=kDefaultAnimationRotation,float _rotationSpeed=kDefaultAnimationRotationSpeed, float _moveSpeed=kDefaultAnimationMoveSpeed,Vec2 _position= kDefaultAnimationPosition);
    AnimationInfo(const AnimationInfo&);
};

struct Item{
    Item_Type type;
    int id;
    float x;
    float y;
    float angle;
    float scale;
    int localZorder;
    bool isAnimated;
    float triggerTime;
    std::map<std::string,bool> animationControlInstructions;
    std::vector<std::vector<AnimationInfo>> animationInfos;
    void* features;
    
    Item(Item_Type _type,int _id,float _x,float _y,float _angle,float _scale,int _localZorder,bool _isAnimated,float _triggerTime,std::map<std::string,bool>& _animationControlInstrutcions,std::vector<std::vector<AnimationInfo>>& _animationInfos,void* _features = nullptr);
    
    Item(const Item& );
    ~Item();
    //容器中是对象时用,指针时需函数指针
    bool operator<(const Item& value)const
    {
        return id < value.id;
    }
    
    bool operator==(const Item& value)const
    {
        return (type == value.type && id ==value.id && x == value.x && y == value.y && angle == value.angle && scale== value.scale);
    }
    
};



#ifdef __cplusplus
extern "C"{
#endif
    
    bool Itemlesser(Item& item1,Item& item2);

#ifdef __cplusplus
}
#endif

#endif /* defined(__GrowUp__Item__) */
