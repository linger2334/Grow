//
//  Cicada.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
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

protected:
    friend class LayerItem;
    b2Body* createBody();
};


#endif /* defined(__Grow__Cicada__) */
