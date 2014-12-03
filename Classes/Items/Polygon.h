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
#include "Macro.h"
#include "ItemModel.h"

class Polygon : public ItemModel
{
public:
    Polygon();
    ~Polygon();
    
    static Polygon* create(PolygonInfo& polygonInfo);
    bool init(PolygonInfo& polygonInfo);

protected:
    bool isConvex;
};

#endif /* defined(__Grow__Polygon__) */
