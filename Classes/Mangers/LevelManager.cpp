#include "LevelManager.h"
#include "GameRunningInfo.h"
#include "GameLayers.h"
#include "GameLayerPlant_Level_1.h"
#include "GameLayerPlant_Level_2.h"
#include "GameLayerPlant_level_3.h"
#include "GamePhysicalWorld.h"
#include "GameLayerUIBorder.h"
#include "LayerItem.h"
#include "LevelFileHandler.h"
#include "GameManager.h"
#include "GameRuntime.h"
#include "TuoYuanRotateNode.h"
#include "GameLayerLight.h"
#include "GameLayerUI.h"
#include "GameLayerEffects.h"
#include "PathListHelper.hpp"
#include "GameRunningInfo.h"
#include "GameSceneMain.h"
#include "GameLayerLight.h"
#include "GameRunningInfo.h"
//////////add for UI
#include "StatisticsData.h"

std::map<int,std::string> LevelManager::_mapFiles;
std::map< int,std::vector<int> > LevelManager::_mapGroups;
bool LevelManager::_sIsInitMapInfo = false;
#define ADD_MAP_FILE(ID,FILE) LevelManager::_mapFiles[ID] = FILE;
void LevelManager::initMapsInfo()
{
    if(_sIsInitMapInfo)return;
    _sIsInitMapInfo = true;
    ADD_MAP_FILE(1,"level 1 - 1");
    ADD_MAP_FILE(1,"level 1 - 2");
    ADD_MAP_FILE(2,"level 1 - 3");
    ADD_MAP_FILE(3,"level 1 - 4");
    ADD_MAP_FILE(4,"level 1 - 1");
    ADD_MAP_FILE(5,"level 1 - 3");
    ADD_MAP_FILE(6,"level 1 - 3");
    ADD_MAP_FILE(7,"level 1 - 3");
    ADD_MAP_FILE(8,"level 1 - 3");
    ADD_MAP_FILE(9,"level 1 - 3");
    ADD_MAP_FILE(10,"level 1 - 3");
    ADD_MAP_FILE(11,"level 1 - 3");
    ADD_MAP_FILE(12,"level 1 - 3");
    ADD_MAP_FILE(13,"level 1 - 3");
    ADD_MAP_FILE(14,"level 1 - 3");
    ADD_MAP_FILE(15,"level 1 - 3");
    ADD_MAP_FILE(16,"level 1 - 3");
    ADD_MAP_FILE(17,"level 1 - 3");
    ADD_MAP_FILE(18,"level 1 - 3");
    ADD_MAP_FILE(19,"level 1 - 3");
    {
        std::vector<int> groups;
        groups.push_back(1);
        groups.push_back(2);
        groups.push_back(3);
        groups.push_back(4);
        LevelManager::_mapGroups[1] = groups;
    }
    {
        std::vector<int> groups;
        groups.push_back(1);
        groups.push_back(2);
        groups.push_back(3);
        groups.push_back(4);
        LevelManager::_mapGroups[2] = groups;
    }
    {
        std::vector<int> groups;
        groups.push_back(1);
        groups.push_back(2);
        groups.push_back(3);
        groups.push_back(4);
        LevelManager::_mapGroups[3] = groups;
    }
    {
        std::vector<int> groups;
        groups.push_back(1);
        groups.push_back(2);
        groups.push_back(3);
        groups.push_back(4);
        LevelManager::_mapGroups[4] = groups;
    }
    {
        std::vector<int> groups;
        groups.push_back(1);
        LevelManager::_mapGroups[4] = groups;
    }
}

void LevelManager::selectLevel(int id)
{
    assert(id>0&&id<6);
    _levelId = id;
    _mapGroupId = id ;
    _mapGroupSubId = 0 ;
    _initState = FirstInit;
}
void LevelManager::selectRecordLevel()
{
    _mapGroupSubId = 1 ;
}
bool LevelManager::selectNextSubLevel()
{
    if (_levelId == 5) {
        return false;
    }
    saveLevelInfo();
    _mapGroupSubId++;
    if ( _mapGroupSubId >= LevelManager::_mapGroups[_mapGroupId].size())
    {
        return false;
    }
    _initState = ChangeNextSubMapInit;
    return true;
}
void LevelManager::saveLevelInfo()
{
    auto layerPlant =  GameLayerPlant::getRunningLayer();
    auto config = GameRunningConfig::getInstance();
    auto layerLight = GameLayerLight::getRunningLayer();
    for(int i = 0; i < layerPlant->getPlantCount();i++)
    {
        _subInitPlantPoints[i] = GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(i)->getHeadPosition();
        int count = layerLight->getLightCountByPlantIndex(i);
        config->setLightInitCount(i,count);
    }
}
void LevelManager::initLevelInfoByRecord()
{
    (*GameRunningConfig::getInstance()->getPlantConfigs()[0]._plantStartContorlPointList.begin())._point = Vec2(_subInitPlantPoints[0].x,0);
    if (_levelId > 2) {
        (*GameRunningConfig::getInstance()->getPlantConfigs()[1]._plantStartContorlPointList.begin())._point = Vec2(_subInitPlantPoints[1].x,0);
    }
}
void LevelManager::initLevelInfo()
{
    LevelManager::selectMapFile(_mapGroupId,_mapGroupSubId);
    
    switch (_levelId) {
        case 1:
            initLevelInfo_1();
            break;
        case 2:
            initLevelInfo_2();
            break;
        case 3:
            initLevelInfo_3();
            break;
        case 5:
            initLevelInfo_1();
            break;
        default:
            break;
    }
}
void LevelManager::initLevelInfo_1()
{
    auto conifg = GameRunningConfig::getInstance();
    auto cache = Director::getInstance()->getTextureCache();
    auto back = cache->addImage(GamePaths::_sPathBackground_level_1_1);
    conifg->setTextureBackGround(back);
    conifg->setTextureMapDirt(cache->addImage(GamePaths::_sPathMapDirt_level_1_1));
    conifg->setTextureMapBorder(cache->addImage(GamePaths::_sPathMapBorder_level_1_1));
    conifg->setTextureUIBorder(cache->addImage(GamePaths::_sPathUIBorder_level_1_1));
    
    GamePlantConfig  first;
    first._textureLeft = 0;
    first._textureRight =1;
    first._growSpeed = 60;
    first._reGrowSpeed =100;
    first._plantNodeInitPosition=Vec2(0,0);
    
    ContorlPointV2 cp;
    cp._rotateRadius = 60;
    cp._angle = 0;
    cp._height = 0;
    float x1 = GameRuntime::getInstance()->getVisibleSize().width * 0.5;
    cp._point = Vec2(x1,-1);
    cp._radius = 60;
    cp._zPosition  = 0;
    first._plantStartContorlPointList.push_back(cp);
    auto tex = cache->addImage(GamePaths::_sPathPlantBody_level_1_1);
    Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
    tex->setTexParameters(parms);
    first._texturePlantBody = tex;
    conifg->getPlantConfigs().push_back(first);
}

void LevelManager::initLevelInfo_2()
{
    auto conifg = GameRunningConfig::getInstance();
    auto cache = Director::getInstance()->getTextureCache();
    auto back = cache->addImage(GamePaths::_sPathBackground_level_2_1);
    conifg->setTextureBackGround(back);
    conifg->setTextureMapDirt(cache->addImage(GamePaths::_sPathMapDirt_level_2_1));
    conifg->setTextureMapBorder(cache->addImage(GamePaths::_sPathMapBorder_level_2_1));
    conifg->setTextureUIBorder(cache->addImage(GamePaths::_sPathUIBorder_level_1_1));
    
    GamePlantConfig  first;
    first._textureLeft = 0;
    first._textureRight =1;
    first._growSpeed = 60;
    first._reGrowSpeed =100;
    first._plantNodeInitPosition=Vec2(0,0);
    
    ContorlPointV2 cp;
    cp._rotateRadius = 60;
    cp._angle = 0;
    cp._height = 0;
    float x1= GameRuntime::getInstance()->getVisibleSize().width * 0.5;
    cp._point = Vec2(x1,-1);
    cp._radius = 60;
    cp._zPosition  = 0;
    first._plantStartContorlPointList.push_back(cp);
    auto tex = cache->addImage(GamePaths::_sPathPlantBody_level_1_1);
    Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
    tex->setTexParameters(parms);
    first._texturePlantBody = tex;
    conifg->getPlantConfigs().push_back(first);
}
void LevelManager::initLevelInfo_3()
{
    auto conifg = GameRunningConfig::getInstance();
    auto cache = Director::getInstance()->getTextureCache();
    conifg->setTextureBackGround(cache->addImage(GamePaths::_sPathBackground_level_1_1));
    conifg->setTextureMapDirt(cache->addImage(GamePaths::_sPathMapDirt_level_1_1));
    conifg->setTextureMapBorder(cache->addImage(GamePaths::_sPathMapBorder_level_1_1));
    conifg->setTextureUIBorder(cache->addImage(GamePaths::_sPathUIBorder_level_1_1));
    
    GamePlantConfig  first;
    first._textureLeft = 0;
    first._textureRight =1;
    first._growSpeed = 60;
    first._reGrowSpeed =100;
    first._plantNodeInitPosition=Vec2(0,0);
    
    ContorlPointV2 cp;
    cp._rotateRadius = 50;
    cp._angle = 0;
    cp._height = 0;
    cp._point = Vec2(100,-10);
    cp._radius = 60;
    cp._zPosition  = 0;
    first._plantStartContorlPointList.push_back(cp);
    auto tex = cache->addImage(GamePaths::_sPathPlantBody_level_1_1);
    Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
    tex->setTexParameters(parms);
    first._texturePlantBody = tex;
    conifg->getPlantConfigs().push_back(first);
    first._plantStartContorlPointList.clear();
    cp._point = Vec2(500,-10);
    first._plantStartContorlPointList.push_back(cp);
    conifg->getPlantConfigs().push_back(first);
 
}

void LevelManager::selectMapFile(int groupId,int groupSubId)
{
    if (_levelId < 5) {
        int id = LevelManager::_mapGroups[groupId][groupSubId];
        initMapFile(LevelManager::_mapFiles[id]);
    }
    else{
       // log(GameManager::getInstance()->_levelFileName.c_str());
//        initMapFile(GameManager::getInstance()->_levelFileName);
    }
}
void LevelManager::initMapFile(std::string fileName)
{
    LevelFileHandler* fileHandler = LevelFileHandler::createWithFileName(fileName.c_str());
    GameManager* gameManager = GameManager::getInstance();
    gameManager->_fileHandler = fileHandler;
    gameManager->_fileHandler->retain();
}
GameLayerPlant* LevelManager::createLayerPlantById(int id)
{
    GameLayerPlant* ret ;
    switch(id)
    {
        case 1:
            ret=  static_cast<GameLayerPlant*>(GameLayerPlant_Level_1::create());
            break;
        case 2:
            ret=  static_cast<GameLayerPlant*>(GameLayerPlant_Level_2::create());
            break;
        case 3:
            ret=  static_cast<GameLayerPlant*>(GameLayerPlant_Level_3::create());
            break;
        case 5:
            ret = static_cast<GameLayerPlant*>(GameLayerPlant_Level_1::create());
            break;
         default:
            ret=  static_cast<GameLayerPlant*>(GameLayerPlant_Level_1::create());
            break;
    }
    return ret;
}
void LevelManager::createBasesLayers()
{
    auto gameScene = GameRunningInfo::getInstance()->getGameSceneNode();
    auto back = GameLayerBackground::create();
    auto map = GameLayerMap::create();
    auto layerPlant = createLayerPlantById(_levelId);
    auto uiBorder = GameLayerUIBorder::create();
    auto layerItem = LayerItem::create();
    auto statisticsdata = StatisticsData::create();
    auto ui = GameLayerUI::create();
    auto crashShow = GameLayerPlantCrashEffect::create();
    auto layerLight = GameLayerLight::create();
    //auto layerLandmark = GameLayerShowLandmark::create();
    
    gameScene->addChild(layerLight,SceneGameChildZorder::MapLight);
   // gameScene->addChild(layerLandmark,SceneGameChildZorder::MapItem+1);
    gameScene->addChild(back,SceneGameChildZorder::MapBackground);
    gameScene->addChild(layerPlant,SceneGameChildZorder::MapPlant);
    gameScene->addChild(crashShow,SceneGameChildZorder::MapDirt-1);
    gameScene->addChild(map,SceneGameChildZorder::MapDirtBorder);
    gameScene->addChild(layerItem,SceneGameChildZorder::MapItem);
    gameScene->addChild(uiBorder,SceneGameChildZorder::MapUIBorder);
    gameScene->addChild(ui,SceneGameChildZorder::MapUI);
    gameScene->addChild(layerItem->_layerItemExt,SceneGameChildZorder::MapItemExt);
    gameScene->addChild(statisticsdata);
    
    back->initGameInfo();
    layerPlant->initGameInfo();
    map->initGameInfo();
    uiBorder->initGameInfo();
    ui->initGameInfo();
    crashShow->initGameInfo();
    layerLight->initGameInfo();
    //layerLandmark->initGameInfo();
//    auto size = GameRuntime::getInstance()->getMapGridSize();
//    
//    layerItem->loadItemsAndBodys(0);
//    layerItem->moveDown(0);
//    map->updateExcavatePolygon(0,0,size.width,size.height+MAX_CRASH_LENGTH);
//    map->update(0.1);

}