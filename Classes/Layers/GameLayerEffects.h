#ifndef __Grow_Beta_01__GameLayerEffects__
#define __Grow_Beta_01__GameLayerEffects__
#include "common.h"
#include "GameLayerBase.h"

class GameLayerPlantCrashEffect : public GameLayerBase,public GameLayerHelper<GameLayerPlantCrashEffect>
{
public:
    CREATE_FUNC(GameLayerPlantCrashEffect);
    virtual bool init();
    virtual bool initGameInfo();
    virtual bool releaseGameInfo();
    virtual void moveDown(float yLen);

    void removeLightBorderByCell(int tag);

    void showCrashBorderLight(int plantIndex = 0);
    void removeBorderLight(int plantIndex = 0);
    void onChangeCellCall(const GridCell& cell);

    std::vector<SpriteBatchNode*>  _crashBorderBatchs;
    std::vector<bool>           _islightRuningActions;

    int     _gridWidth;
};

class GameLayerShowLandmark: public GameLayerBase,public GameLayerHelper<GameLayerShowLandmark>
{
public:
    CREATE_FUNC(GameLayerShowLandmark);
    virtual bool initGameInfo();
    virtual void moveDown(float yLen);
    void addLandmark(int n,Vec2 pt);
    void autoAddLandmark();
    std::map<int,Label*> _landmarks;
};
#endif /* defined(__Grow_Beta_01__GameLayerEffects__) */
