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

Features_Dragon::Features_Dragon(float W,float BackTransparency)
{
    w = W;
    backTransparency = BackTransparency;
}

Item::Item(Item_Type _type,int _id,float _x,float _y,float _angle,float _scale,int _localZorder,bool _iscreated,void* _features)
{
    type = _type;
    id = _id;
    x = _x;
    y = _y;
    angle = _angle;
    scale = _scale;
    localZorder = _localZorder;
    iscreated = _iscreated;
    features = _features;
    if(_features){
        switch(type){
            case Cicada:
            {
                Features_Cicada* feat = new Features_Cicada();
                feat->w = ((Features_Cicada*)_features)->w;
                feat->includedAngle = ((Features_Cicada*)_features)->includedAngle;
                feat->fanningDuration = ((Features_Cicada*)_features)->fanningDuration;
                feat->interval = ((Features_Cicada*)_features)->interval;
                feat->bellyTransparency = ((Features_Cicada*)_features)->bellyTransparency;
                features = feat;
            }
                break;
                
            case Dragon_Anti:
            {
                Features_Dragon* feat = new Features_Dragon();
                feat->w = ((Features_Dragon*)_features)->w;
                feat->backTransparency= ((Features_Dragon*)_features)->backTransparency;
                features = feat;
            }
                break;
                
            case Dragon_Clockwise:
            {
                Features_Dragon* feat = new Features_Dragon();
                feat->w = ((Features_Dragon*)_features)->w;
                feat->backTransparency= ((Features_Dragon*)_features)->backTransparency;
                features = feat;
            }
                
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
    iscreated = other.iscreated;
    features = other.features;
    if(other.features){
        switch(type){
            case Cicada:
            {
                Features_Cicada* feat = new Features_Cicada();
                feat->w = ((Features_Cicada*)other.features)->w;
                feat->includedAngle = ((Features_Cicada*)other.features)->includedAngle;
                feat->fanningDuration = ((Features_Cicada*)other.features)->fanningDuration;
                feat->interval = ((Features_Cicada*)other.features)->interval;
                feat->bellyTransparency = ((Features_Cicada*)other.features)->bellyTransparency;
                features = feat;
            }
                break;
                
            case Dragon_Anti:
            {
                Features_Dragon* feat = new Features_Dragon();
                feat->w = ((Features_Dragon*)other.features)->w;
                feat->backTransparency= ((Features_Dragon*)other.features)->backTransparency;
                features = feat;
            }
                break;
                
            case Dragon_Clockwise:
            {
                Features_Dragon* feat = new Features_Dragon();
                feat->w = ((Features_Dragon*)other.features)->w;
                feat->backTransparency= ((Features_Dragon*)other.features)->backTransparency;
                features = feat;
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
            case Polygon:
            {
                delete (Features_Polygon*)features;
            }
            default:
                break;
        }
    }
}


bool Itemlesser(Item& item1,Item& item2)
{
    return item1.localZorder < item2.localZorder;
}