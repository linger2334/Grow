#ifndef __Grow_Beta_01__GameRunningContorler__
#define __Grow_Beta_01__GameRunningContorler__
#include "common.h"

class GameRunningManager : public Singleton<GameRunningManager>
{
public:
    GameRunningManager():_passHeight(0)
    {
    }
    
    void resetRunningManagerInfo()
    {
        _passHeight = 0;
    }
    void  update(float dt);
    void reGrowPlantByIndex(int index);
    void addLightByFlame(Vec2 pt,int count);
    
    void removeOneLight();
    void removeOneLightNormal();
    void removeSubLight();
    void removeSubLight(Vec2 pt);
    void checkNeedMoveDown();
    void checkChangeSubMap();
    void checkSubLight();
    void updatePlantSpeed();
    
    float  _passHeight;
};

#endif /* defined(__Grow_Beta_01__GameRunningContorler__) */
