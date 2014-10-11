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

struct Features_Polygon{
    bool isConvex;
    std::vector<std::pair<float,float>> vertexs;
    
    Features_Polygon(bool isConvex,std::vector<std::pair<float,float>>& vertexs);
    Features_Polygon(const Features_Polygon& );
};

struct Item{
    Item_Type type;
    int id;
    float x;
    float y;
    float angle;
    float scale;
    int localZorder;
    bool iscreated;
    void* features;
    
    Item(Item_Type _type,int _id,float _x,float _y,float _angle,float _scale,int _localZorder,bool _iscreated = false,void* _features = nullptr);
    
    Item(const Item& );
    ~Item();
    
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
