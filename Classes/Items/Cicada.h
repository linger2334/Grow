//
//  Cicada.h
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#ifndef __Grow__Cicada__
#define __Grow__Cicada__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class Cicada : public ItemModel
{
public:
    static Cicada* create(Item& item);
    bool init(Item& item);
    
    void createBody(std::vector<b2Body*>& bodies);
    
protected:
    Sprite* _head;
    Sprite* _belly;
    Sprite* _leftwing;
    Sprite* _rightwing;
    
    float w;
    float includedAngle;
    float fanningDuration;
    float interval;
    float bellyTransparency;
    
};

#endif /* defined(__Grow__Cicada__) */
