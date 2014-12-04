#ifndef __Grow_Beta_01__GameLayerUIBorder__
#define __Grow_Beta_01__GameLayerUIBorder__
#include "GameLayerBase.h"
#include "GameLayerRollImage.h"
#include "GamePhysicalWorld.h"
class GameLayerRollImage;
class GameLayerUIBorder : public GameLayerBase ,public GameLayerHelper<GameLayerUIBorder>
{
public:

    CREATE_FUNC(GameLayerUIBorder);
    
    virtual bool init();
    virtual bool initGameInfo();
    virtual bool releaseGameInfo();
    virtual void moveDown(float yLen);
    virtual void addUIBorderBody();
    virtual void update(float dt);
    GameLayerRollImage*             _borderLeftLayer;
    GameLayerRollImage*             _borderRightLayer;
    b2Body* bodys[2];
    b2Body* _testBody;
};

#endif /* defined(__Grow_Beta_01__GameLayerUIBorder__) */
