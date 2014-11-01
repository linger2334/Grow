#ifndef __Grow_Demo__PlantBase__
#define __Grow_Demo__PlantBase__
#include "common.h"

class GameLayerRollY : public  Node
{
public:
    virtual bool  init();
    virtual bool  initGameInfo(){return true;}
    virtual void  moveDown(float y) = 0;
    
    virtual void _layoutGameLayer(Node* gameLayer);
};

#endif /* defined(__Grow_Demo__PlantBase__) */
