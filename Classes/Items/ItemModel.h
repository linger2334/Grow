//
//  ItemModel.h
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#ifndef __Grow__ItemModel__
#define __Grow__ItemModel__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Item.h"
#include "Macro.h"

class ItemModel : public Sprite
{
public:
    ItemModel();
    virtual ~ItemModel();
    
    static ItemModel* create(Item& item);
    virtual bool init(Item& item);
    
    Item_Type type;
    
protected:
    friend class LayerItem;
    virtual void createBody(std::vector<b2Body*>& bodies){};
};



#endif /* defined(__Grow__ItemModel__) */
