#ifndef Grow_Demo_test_GameLayerBackground_h
#define Grow_Demo_test_GameLayerBackground_h
#include "common.h"
#include "GameLayerBase.h"
#include "GameLayerRollImage.h"


class GameLayerBackground : public GameLayerRollImage,public GameLayerHelper<GameLayerBackground>
{
public:
    CREATE_FUNC(GameLayerBackground);
    virtual bool init(){return GameLayerRollImage::init();}
    virtual void moveDown(float yLen)
    {
        GameLayerRollImage::moveDown(yLen);
    }
    virtual bool initGameInfo()
    {
        this->setImage(GameRunningConfig::getInstance()->getTextureBackGround());
        this->setImageStartY(GameRunningConfig::getInstance()->getBackgroundImageStartY());
        return true;
    }
};

#endif
