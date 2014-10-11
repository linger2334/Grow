//
//  Rock.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#ifndef __Grow__Rock__
#define __Grow__Rock__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class Rock : public ItemModel
{
public:
    static Rock* create(Item& item);
    bool init(Item& item);
    
    void createBody(std::vector<b2Body*>& bodies);
    
protected:
    friend class LayerItem;
    
    b2Body* _body;
    
};

#endif /* defined(__Grow__Rock__) */
