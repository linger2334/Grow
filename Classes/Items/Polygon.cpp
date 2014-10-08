//
//  Polygon.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#include "Polygon.h"

class Polygon* Polygon::create(Item& item)
{
    Polygon *ret = new Polygon();
    if (ret && ret->init(item))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Polygon::init(Item& item)
{
    bool result;
    if(ItemModel::init(item))
    {
        
        result = true;
    }else{
        result = false;
    }
    return  result;
}