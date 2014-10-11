#ifndef __Grow_Demo__LayerGoods__
#define __Grow_Demo__LayerGoods__
#include "common.h"
#include "PlantBase.h"

class LayerLight;
class LayerGoods1 : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerGoods1);
    virtual bool init();
    
    virtual  void moveDown(float yLen);
 
    virtual void update(float dt);
    
    SpriteBatchNode* _goodsBatch;
    
    
    LayerLight*     _layerLight;
};

#endif /* defined(__Grow_Demo__LayerGoods__) */
