//
//  Dragon.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#ifndef __Grow__Dragon__
#define __Grow__Dragon__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class Dragon:public ItemModel
{
public:
    static Dragon* create(Item& item);
    bool init(Item& item);
    
    void createBody(std::vector<b2Body*>& bodies);
    void collisionWithPlant();
protected:
    friend class LayerItem;
    b2Body* _body;
    
};



#endif /* defined(__Grow__Dragon__) */
