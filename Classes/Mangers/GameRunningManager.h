#ifndef __Grow_Beta_01__GameRunningContorler__
#define __Grow_Beta_01__GameRunningContorler__
#include "common.h"
class ScreenEnergy;
class GameSceneMain;
class GameRunningManager : public Singleton<GameRunningManager>
{
public:
    GameRunningManager():_isShowPause(false),_layerAddLights(nullptr)
    {
    }
    void  update(float dt);
    void reGrowPlantByIndex(int index);
    void addLightByFlame(Vec2 pt,int count,int plantIndex = 0);
    void updatePlantGrowSpeed();
    void checkRemoveLightNormal();
    bool checkChangeSubMap();
    
    float getPlantRemoveLightStepHeight(int plantIndex);
    
    int  getPlantPassStepCount(int plantIndex);
    void addPlantRemoveLightStepHeight(int plantIndex);
    
    float getPlantRemoveLightUnitHeight(int plantIndex)
    {
        return 300;
    }
    
    void waitAddLight();
    void waitAddLightComplete();
    
    void removeOneLightNormal(int plantIndex = 0);
    void removeSubLight(int plantIndex = 0);
    void removeSubLight(Vec2 pt,int plantIndex = 0);
    void checkNeedMoveDown();
    void updatePlantSpeed();
    
    void onGameSceneMainEnter();
    void onGameSceneMainEnterComplete();
    
    void onGameSceneMainExit();
    void onGameSceneMainEixtComplete();
    
    void initFirstGameLoadPolygon();
    
    void doFirstMapInitAction();
    
    bool   _isShowPause;
    std::map<int,int> _plantsStepHeight;
    ScreenEnergy*     _layerAddLights;
};

#endif /* defined(__Grow_Beta_01__GameRunningContorler__) */
