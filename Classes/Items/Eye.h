//
//  Eye.h
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#ifndef __Grow__Eye__
#define __Grow__Eye__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

class Eye : public ItemModel
{
public:
    Eye();
    ~Eye();
    
    static Eye* create(Item& item);
    bool init(Item& item);
    
    void collisionWithPlant(ItemModel* plantHead);
    void createBody();
    
protected:
    friend class LayerItem;
};

#endif /* defined(__Grow__Eye__) */
