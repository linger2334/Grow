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

struct Item{
    Item_Type type;
    int id;
    float x;
    float y;
    float angle;
    float scale;
    int localZorder;
    bool iscreated;
    float speed;
    void* features;
    
    Item(Item_Type _type,int _id,float _x,float _y,float _angle,float _scale,int _localZorder,bool _iscreated = false,float _speed = 0.0,void* _features = nullptr)
    {
        type = _type;
        id = _id;
        x = _x;
        y = _y;
        angle = _angle;
        scale = _scale;
        localZorder = _localZorder;
        iscreated = _iscreated;
        speed = _speed;
        features = _features;
    }
    
    bool operator<(const Item& value)const
    {
        return id < value.id;
    }
    
    bool operator==(const Item& value)const
    {
        return (type == value.type && id ==value.id && x == value.x && y == value.y && angle == value.angle && scale== value.scale);
    }
    
};

struct Features_Cicada{
    float includedAngle;
    float interval;
    float bellTransparency;
};

struct Features_Polygon{
    std::set<std::pair<float,float>> vertexBuffer;
};

#ifdef __cplusplus
extern "C"{
#endif
    
    bool Itemlesser(Item& item1,Item& item2);

#ifdef __cplusplus
}
#endif

#endif /* defined(__GrowUp__Item__) */
