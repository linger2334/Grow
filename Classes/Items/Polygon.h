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
#include "Item.h"
#include "TypeBase.h"


class Polygon : public TypeBase,public Node
{
public:
    Polygon();
    ~Polygon();
    
    static Polygon* create(PolygonInfo& polygonInfo);
    bool init(PolygonInfo& polygonInfo);
    
    CC_SYNTHESIZE_READONLY(int, polygonTag, PolygonTag)
    CC_SYNTHESIZE_READONLY(b2Body*, _body, Body)
protected:
    bool isConvex;
};

#endif /* defined(__Grow__Polygon__) */
