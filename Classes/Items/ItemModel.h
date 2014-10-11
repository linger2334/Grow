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
#include "TypeBase.h"

class ItemModel : public Sprite,public TypeBase
{
public:
    ItemModel();
    virtual ~ItemModel();
    
    static ItemModel* create(Item& item);
    virtual bool init(Item& item);
    
    virtual void createBody(std::vector<b2Body*>& bodies){};
    
    std::function<void()> _collisionCallBack;
    virtual void collisionWithPlant() { };
protected:
    friend class LayerItem;
    
};



#endif /* defined(__Grow__ItemModel__) */
