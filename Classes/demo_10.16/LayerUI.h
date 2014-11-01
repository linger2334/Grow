#ifndef __Grow_Goods_test__LayerUI__
#define __Grow_Goods_test__LayerUI__
#include "common.h"
#include "PlantBase.h"
#include "GameManager.h"

class LayerUI : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerUI);
    
//    virtual void onEnter();
//    virtual void onExit();
    virtual bool init();
//    
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //    virtual void onTouchCancelled(Touch*,Event*);
    
    virtual void moveDown(float yLen){}

    Sprite*                     _pause;
    Sprite*                     _ringbottom;
    LayerColor*                 _layerColor;
};


#endif /* defined(__Grow_Goods_test__LayerUI__) */
