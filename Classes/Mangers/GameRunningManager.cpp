#include "GameRunningManager.h"
#include "GameLayerPlant.h"
#include "Plant.h"
#include "GameRuntime.h"
#include "GamePhysicalWorld.h"
#include "LayerItem.h"
#include "LevelManager.h"
#include "GameLayerLight.h"
#include "ScreenEnergy.h"
#include "GameManager.h"
#include "GameSceneMain.h"
void GameRunningManager::reGrowPlantByIndex(int index)
{
    auto& info  = GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(index);
    info._reGrowNeedLength = 40;
    info._reGrowSpeed = 80;
    info._state = GameLayerPlant::PlantStateReGrowIng;
}
void GameRunningManager::addLightByFlame(Vec2 pt,int count,int plantIndex,float waitTime)
{
    GameLayerLight::getRunningLayer()->addLightsUseBezier(plantIndex, pt, count,waitTime);
}
void GameRunningManager::removeSubLight(int plantIndex)
{
    int size = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(plantIndex);
    if (size == 0) return ;
    int subSize;
    if (size != 1)subSize =  size/2;
    else subSize = size;
    GameLayerLight::getRunningLayer()->removeLightsRandIdUseSacel(plantIndex, subSize);
}
void GameRunningManager::removeSubLight(Vec2 pt,int plantIndex)
{
  int size = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(0);
        GameLayerLight::getRunningLayer()->removeLightsRandIdUseSacel(plantIndex, size/2);
}
void  GameRunningManager::removeOneLightNormal(int plantIndex)
{
    GameLayerLight::getRunningLayer()->removeLights(plantIndex,1);
}

void GameRunningManager::checkNeedMoveDown()
{
    Size  wsize = GameRuntime::getInstance()->getVisibleSize();
    float step = wsize.height * 0.5;
    float height = GameLayerPlant::getRunningLayer()->getPlantMinTopHeightInView();
    auto  runningInfo = GameRunningInfo::getInstance();
    float submapHeight = 10240;
    if(runningInfo->_gamePassHeight + wsize.height >= submapHeight)return;
    if(height > step)
    {
        float stept = height- step;
        if (runningInfo->_gamePassHeight + stept >= submapHeight - wsize.height ){
            stept = submapHeight - wsize.height - runningInfo->_gamePassHeight;
        }
        runningInfo->_gamePassHeight += stept;
        
        LayerItem::getRunningLayer()->loadItemsAndBodys(runningInfo->_gamePassHeight + 1050);
        for (auto i: runningInfo->_runningLayers ) {
            i.second->moveDown(stept );
        }
    }
}

bool GameRunningManager::checkChangeSubMap()
{
    float submapHeight = 10240;
    {
        if(GameLayerPlant::getRunningLayer()->getPlantMinTopHeightInView()+
           GameRunningInfo::getInstance()->_gamePassHeight > submapHeight)
        {
            GameRunningInfo::getInstance()->getGameSceneNode()->unscheduleUpdate();
            if (LevelManager::getInstance()->selectNextSubLevel()) {
                GameManager::getInstance()->releaseGameScene();
                GameManager::getInstance()->navigationTo(SceneLoding);
                return true;
            }
            else
            {
                GameManager::getInstance()->clearGameLevelInfo();
                //GameManager::getInstance()->releaseGameScene();
                GameManager::getInstance()->navigationTo(SceneGameWin);
                return true;
            }
        }
    }
    return false;
}
void  GameRunningManager::update(float dt)
{
    auto runningInfo = GameRunningInfo::getInstance();
    if (!GameManager::getInstance()->isPause()&&
        !runningInfo->isRunningAnimation()) {
//        if (checkChangeSubMap()) {
//            return ;
//        }
       // updatePlantGrowSpeed(dt);
        checkNeedMoveDown();
        checkRemoveLightNormal();
    }
    
    for(auto i: runningInfo->_runningLayers)
    {
        i.second->update(dt);
    }
    
    GamePhysicalWorld::getInstance()->update(dt);
}
void GameRunningManager::updatePlantGrowSpeed(float dt)
{
    auto layerPlant = GameLayerPlant::getRunningLayer();
   // if (_layerAddLights)return ;
    for (int i = 0; i < layerPlant->getPlantCount(); i++) {
        int count  = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(i);
        float speed =  0;
        if(count == 0)
        {
            _plantIndex = i;
            layerPlant->setGrowSpeedByIndex(0, i);
            waitAddLight();
            return ;
        }
        if (count > 0 ) {
           speed = count * 4;
           speed += 30;
           speed = speed > 90 ? 90 : speed;
           //speed=90;
        }
       // else speed = 0;
       // speed = 120;
        float setSpeed = layerPlant->getGrowSpeedByIndex(i);
        setSpeed += speed * dt * 0.2;
        if (setSpeed > speed) {
            setSpeed = speed;
        }
        layerPlant->setGrowSpeedByIndex(setSpeed, i);
    }
}
int GameRunningManager::getPlantPassStepCount(int plantIndex)
{
    auto ip = _plantsStepHeight.find(plantIndex);
    if(ip == _plantsStepHeight.end() )
        return 2;
    else return ip->second;
}
float GameRunningManager::getPlantRemoveLightStepHeight(int plantIndex)
{
    auto runningInfo = GameRunningInfo::getInstance();
    float height = runningInfo->_gamePassHeight;
    float y = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(plantIndex)->getHeadPositionInWorld().y;
    float plantY = y + height;
    float passHeight = getPlantPassStepCount(plantIndex)*getPlantRemoveLightUnitHeight(plantIndex);
    return plantY - passHeight;
}
void  GameRunningManager::addPlantRemoveLightStepHeight(int plantIndex)
{
    int i = getPlantPassStepCount(plantIndex);
    _plantsStepHeight[plantIndex] = ++i;
}
void GameRunningManager::checkRemoveLightNormal()
{
    auto layerPlant = GameLayerPlant::getRunningLayer();
    for (int i = 0; i < layerPlant->getPlantCount(); i++) {
        float stepHeiht = getPlantRemoveLightStepHeight(i);
        float stepUnitHeight = getPlantRemoveLightUnitHeight(i);
        if (stepHeiht >= stepUnitHeight) {
            removeOneLightNormal(i);
            addPlantRemoveLightStepHeight(i);
        }
    }
}

void GameRunningManager::waitAddLight()
{
    GameManager::getInstance()->pauseGame();
    GameLayerUI::getRunningLayer()->showWaitLightUI();
//    if(_layerAddLights)return;
//    _layerAddLights = ScreenEnergy::create();
//    auto node = GameRunningInfo::getInstance()->getGameSceneNode();
//    node->addChild(_layerAddLights,SceneGameChildZorder::MapUI-2);
//    GameLayerUI::getRunningLayer()->pauseGame();
}

void GameRunningManager::waitAddLightComplete()
{
    auto layerLight =  GameLayerLight::getRunningLayer();
    Size size =Director::getInstance()->getWinSize();
    Vec2 pt(size.width*0.5, size.height*0.3);
    auto callFunc = [](GameLayerLight* lightLayer,Vec2 point,int index)
    {
        lightLayer->addOneLightByPlantIndexUseBezier(point,index);
    };
    auto call1 = []()
    {
        GameManager::getInstance()->reStartGame();
        GameLayerUI::getRunningLayer()->noShowWaitLightUI();
//      GameLayerUI::getRunningLayer()->reStartGame();
//      GameRunningManager::getInstance()->_layerAddLights = nullptr;
    };
    Repeat* repeat = Repeat::create(Sequence::create(CallFunc::create(std::bind(callFunc, layerLight,pt,_plantIndex)),DelayTime::create(0.04),nullptr), 10);
    auto seq = Sequence::create(repeat,CallFunc::create(call1), NULL);
    layerLight->runAction(seq);
}

void GameRunningManager::onGameSceneMainEnter()
{
    GameRunningInfo::getInstance()->initGameRunningInfo();
    auto levelManager = LevelManager::getInstance();
    int state = levelManager->_initState;
    levelManager->initLevelInfo();
    if (state == LevelManager::ChangeNextSubMapInit) {
        levelManager->initLevelInfoByRecord();
    }
    levelManager->createBasesLayers();
    initFirstGameLoadPolygon();

    if (state == LevelManager::ChangeNextSubMapInit) {
        GameLayerLight::getRunningLayer()->initLightsBySubMap();
        GameLayerLight::getRunningLayer()->runChangeLightsAction(30,0.5);
        
    }
    GameRunningInfo::getInstance()->getGameSceneNode()->scheduleUpdate();
}
void GameRunningManager::onGameSceneMainEnterComplete()
{
    GameLayerMap::getRunningLayer()->startListener();

    GameSceneMain* main = static_cast<GameSceneMain*>( GameRunningInfo::getInstance()->getGameSceneNode());
    if (main->_isFirstLoad) {
          main->schedule(schedule_selector(GameSceneMain::updateFirstGrowPlant),1.0f/60.0f);
    }
    else
    {
        GameRunningInfo::getInstance()->getGameSceneNode()->scheduleUpdate();
    }

}

void GameRunningManager::onGameSceneMainExit()
{
    auto runningInfo =  GameRunningInfo::getInstance();

    for (auto& i :runningInfo->_runningLayers) {
        i.second->releaseGameInfo();
    }
    GameRunningInfo::getInstance()->removeAllGameLayer();
    GameRunningInfo::getInstance()->_gamePassHeight = 0;
    GameRunningConfig::getInstance()->clearInfo();
//    GameManager::getInstance()->safeReleaseFileHandler();
    _plantsStepHeight.clear();
    GameRunningInfo::getInstance()->_gamePassHeight = 0;
    GameManager::getInstance()->releaseGameScene();
     //CC_SAFE_RELEASE(GameManager::getInstance()->_fileHandler);
}
void GameRunningManager::onGameSceneMainEixtComplete()
{

}
#include "GameLayerMapBorder.h"
void GameRunningManager::initFirstGameLoadPolygon()
{
    auto size = GameRuntime::getInstance()->getMapGridSize();
    auto map = GameLayerMap::getRunningLayer();
    auto layerItem = LayerItem::getRunningLayer();
    layerItem->loadItemsAndBodys(0);
    layerItem->moveDown(0);
    map->updateExcavatePolygon(0,0,size.width,size.height+MAX_CRASH_LENGTH);
    map->update(0.1);
    map->startListener();
    GameLayerMapBorder::getRunningLayer()->update(0.1);
}

void GameRunningManager::doFirstMapInitAction()
{
   
}
void GameRunningManager::setPlantWaiting(int plantIndex,bool isWait)
{
    auto& info = GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(plantIndex);
    if (isWait) {
        info._waitingNumber++;
    }
    else{
        info._waitingNumber--;
        if (info._waitingNumber<0) {
            info._waitingNumber = 0;
        }
    }

    GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(plantIndex)._isNeedWaiting = isWait;
}