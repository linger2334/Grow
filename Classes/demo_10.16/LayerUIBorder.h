#ifndef __Grow_Goods_test__LayerUIBorder__
#define __Grow_Goods_test__LayerUIBorder__
#include "common.h"
#include "GameManager.h"
#include "PlantBase.h"

class LayerRollImage;
class LayerUIBorder : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerUIBorder);
    virtual bool init();
    
    virtual bool InitGameInfo();
    virtual void moveDown(float yLen);
    LayerRollImage*             _borderLeftLayer;
    LayerRollImage*             _borderRightLayer;
};

#endif /* defined(__Grow_Goods_test__LayerUIBorder__) */
