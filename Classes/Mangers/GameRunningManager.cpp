#include "GameRunningManager.h"
#include "GameLayerPlant.h"
#include "Plant.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "LayerLight.h"
#include "LayerItem.h"
#include "LevelManager.h"
void GameRunningManager::reGrowPlantByIndex(int index)
{
    auto& info  = GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0);
    info._reGrowNeedLength = 40;
    info._reGrowSpeed = 120;
    info._state = GameLayerPlant::PlantStateReGrowIng;
    
}
void GameRunningManager::addLightByFlame(Vec2 pt,int count)
{
    for (int i = 0 ; i < count; i++) {
        LayerLight::getRunningLayer()->addOneLight(pt);
    }
}
void GameRunningManager::removeSubLight()
{
    int size = LayerLight::getRunningLayer()->_lightListFirst.size();
    for (int i =0 ; i < size/2; i++) {
         LayerLight::getRunningLayer()->removeOneLight(Vec2(0,0));
    }
}
void GameRunningManager::removeSubLight(Vec2 pt)
{
    int size = LayerLight::getRunningLayer()->_lightListFirst.size();
    for (int i =0 ; i < size/2; i++) {
        LayerLight::getRunningLayer()->removeOneLight(pt);
    }
}
void  GameRunningManager::removeOneLightNormal()
{
    LayerLight::getRunningLayer()->removeOneLightNormal();
}
void GameRunningManager::removeOneLight()
{
    LayerLight::getRunningLayer()->removeOneLight(Vec2(0,0));
}
void GameRunningManager::checkChangeSubMap()
{
    
}
void GameRunningManager::checkSubLight()
{
    
}
void GameRunningManager::updatePlantSpeed()
{
    int count  = LayerLight::getRunningLayer()->_lightListFirst.size();
    if(count == 0)
    {
        GameRunningInfo::getInstance()->setIsGameEnd(true);
    }
    
    float speed = count * 4;
    speed+= 30;
    speed = speed>90?90:speed;
    GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed = speed;
}
void GameRunningManager::checkNeedMoveDown()
{
      Size  wsize= GameRuntime::getInstance()->getVisibleSize();
    float step = wsize.height*0.5;
    float height = GameLayerPlant::getRunningLayer()->getPlantMinTopHeightInView();
         auto  runningInfo = GameRunningInfo::getInstance();
  
    if(runningInfo->_gamePassHeight + wsize.height >= 10240)return;
    if(height > step)
    {
        float stept = height- step;
        if (runningInfo->_gamePassHeight+ stept >= 10240 - wsize.height ) {
            stept = 10240 - wsize.height - runningInfo->_gamePassHeight;
        }
        runningInfo->_gamePassHeight += stept;
        //GameLayerPlant::getRunningLayer()->moveDown(height- 300 );
        LayerItem::getRunningLayer()->loadItemsAndBodys(runningInfo->_gamePassHeight+350);
        for (auto i: runningInfo->_runningLayers ) {
            i.second->moveDown(stept );
        }
         _passHeight += stept;
        if (_passHeight > 300) {
            removeOneLightNormal();
            _passHeight = 0;
        }
    }
}

void  GameRunningManager::update(float dt)
{
    //if(!GameManager::getInstance()->isGameStop())
    {
//    if(GameLayerPlant::getRunningLayer()->getPlantMinTopHeightInView()+
//       GameRunningInfo::getInstance()->_gamePassHeight >= 1024*10-3)//1024*10-3
//       
//    {
//        LevelManager::getInstance()->selectNextMap();
//        return;
//    }
  
    int count  = LayerLight::getRunningLayer()->_lightListFirst.size();
//    if(count == 0)
//    {
//        GameRunningInfo::getInstance()->setIsGameEnd(true);
//    }
//    
//    float speed = count * 4;
//    speed+= 30;
//    speed = speed>90?90:speed;
//          speed =70;
//    GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed = speed;

    checkNeedMoveDown();
    }
    for(auto i: GameRunningInfo::getInstance()->_runningLayers)
    {
        i.second->update(dt);
    }
    
    GamePhysicalWorld::getInstance()->update(dt);
}