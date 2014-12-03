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
    void removeBorderLight();
    void removeLightBorderByCell(int tag);
    void showCrashBorderLight();
    
    void onChangeCellCall(const GridCell& cell);
    bool   _islightRuningAction;
    SpriteBatchNode*    _crashBorderBatch;
    std::map<GridCell,Sprite*> _crashBorderLightMap;
    int     _gridWidth;
};
#endif /* defined(__Grow_Beta_01__GameLayerEffects__) */
