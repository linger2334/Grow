#ifndef __Grow_Demo__PlantBase__
#define __Grow_Demo__PlantBase__
#include "common.h"
class PlantBase
{
    
};

class GameLayerRollY : public  Node
{
public:
    virtual bool  init();
    virtual void  moveDown(float y) = 0;
};

#endif /* defined(__Grow_Demo__PlantBase__) */
