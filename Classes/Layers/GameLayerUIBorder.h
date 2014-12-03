#ifndef __Grow_Beta_01__GameLayerUIBorder__
#define __Grow_Beta_01__GameLayerUIBorder__
#include "GameLayerBase.h"
#include "GameLayerRollImage.h"
#include "GamePhysicalWorld.h"
class GameLayerRollImage;
class GameLayerUIBorder : public GameLayerBase ,public GameLayerHelper<GameLayerUIBorder>
{
public:
    virtual ~GameLayerUIBorder()
    {
//        for(auto i: bodys)
//        {
//            if(i) GamePhysicalWorld::getInstance()->getBox2dWorld()->DestroyBody(i);
//        }
    }
    CREATE_FUNC(GameLayerUIBorder);
    
    virtual bool init();
    virtual bool initGameInfo();
    virtual bool releaseGameInfo();
    virtual void moveDown(float yLen);
    virtual void addUIBorderBody();
    
    GameLayerRollImage*             _borderLeftLayer;
    GameLayerRollImage*             _borderRightLayer;
    b2Body* bodys[2];
};

#endif /* defined(__Grow_Beta_01__GameLayerUIBorder__) */
