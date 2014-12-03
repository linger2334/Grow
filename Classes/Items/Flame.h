//
//  Flame.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#ifndef __Grow__Flame__
#define __Grow__Flame__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class Flame:public ItemModel
{
public:
    Flame();
    ~Flame();
    
    static Flame* create(Item& item);
    bool init(Item& item);
    void createBody();
    void updateFlameCollectedCount();
    
protected:
    friend class LayerItem;
    
};

#endif /* defined(__Grow__Flame__) */
