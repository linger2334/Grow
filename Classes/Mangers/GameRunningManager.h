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
    
    ////Light
    void addLightByFlame(Vec2 pt,int count,int plantIndex = 0,float waitTime = 0.3);
    void removeOneLightNormal(int plantIndex = 0);
    void removeSubLight(int plantIndex = 0);
    void removeSubLight(Vec2 pt,int plantIndex = 0);
    ///
    void updatePlantGrowSpeed(float dt);
    void checkRemoveLightNormal();
    bool checkChangeSubMap();
    
    float getPlantRemoveLightStepHeight(int plantIndex);
    
    int  getPlantPassStepCount(int plantIndex);
    void addPlantRemoveLightStepHeight(int plantIndex);
    
    float getPlantRemoveLightUnitHeight(int plantIndex)
    {
        return 300;
    }
    
    void setPlantWaiting(int plantIndex,bool isWait);
    
    void waitAddLight();
    void waitAddLightComplete();
    

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
    int               _plantIndex;
};

#endif /* defined(__Grow_Beta_01__GameRunningContorler__) */
