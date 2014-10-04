//
//  Polygon.h
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#ifndef __Grow__Polygon__
#define __Grow__Polygon__

#include <iostream>
#include "ItemModel.h"

class Polygon : public ItemModel
{
public:
    static Polygon* create(Item& item);
    virtual bool init(Item& item);

protected:
    b2Body* createBody() { return nullptr; };
    
};

#endif /* defined(__Grow__Polygon__) */
