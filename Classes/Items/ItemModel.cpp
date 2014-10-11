//
//  ItemModel.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#include "ItemModel.h"
#include "Flame.h"
#include "Rock.h"
#include "Cicada.h"
#include "Dragon.h"
#include "Eye.h"
#include "Polygon.h"

ItemModel::ItemModel()
{
    
}

ItemModel::~ItemModel()
{
    
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
        case Rock_Circle:
            itemModel = Rock::create(item);
            break;
        case Rock_Ellipse:
            itemModel = Rock::create(item);
            break;
        case Rock_Gray:
            itemModel = Rock::create(item);
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
        case Eye:
            itemModel = Eye::create(item);
            break;
        case Polygon:
            itemModel = Polygon::create(item);
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
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}