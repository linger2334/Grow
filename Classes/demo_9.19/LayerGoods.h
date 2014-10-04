#ifndef __Grow_Demo__LayerGoods__
#define __Grow_Demo__LayerGoods__
#include "Macro.h"
#include "PlantBase.h"

class LayerGoods1 : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerGoods1);
    virtual bool init();
    
    virtual  void moveDown(float yLen);
 
    virtual void update(float dt);
    
    SpriteBatchNode* _goodsBatch;
    
};

#endif /* defined(__Grow_Demo__LayerGoods__) */
