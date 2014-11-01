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
    Dragon();
    ~Dragon();
    
    static Dragon* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    void collisionWithPlant(ItemModel* plantHead);
protected:
    friend class LayerItem;
    
};



#endif /* defined(__Grow__Dragon__) */
