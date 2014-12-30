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
#include "StatisticsData.h"
#include "PlantLeafs.h"
std::map<int,std::string> LevelManager::_mapFiles;
std::map< int,std::vector<int> > LevelManager::_mapGroups;
std::vector<std::vector<std::string> > LevelManager::_sMapConfigInfos;
std::vector<PlantConfigInfo>   LevelManager::_sPlantConfigInfos;
bool LevelManager::_sIsInitMapInfo = false;
#define ADD_MAP_FILE(ID,FILE) LevelManager::_mapFiles[ID] = FILE;
void LevelManager::initMapsInfo()
{
    if(_sIsInitMapInfo)return;
    _sIsInitMapInfo = true;
    ADD_MAP_FILE(0,"level 1 - 1");
    ADD_MAP_FILE(1,"level 1 - 1");
    ADD_MAP_FILE(2,"level 1 - 2");
    ADD_MAP_FILE(3,"level 1 - 3");
    ADD_MAP_FILE(4,"level 1 - 4");
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
        LevelManager::_mapGroups[1] = {1,2,3,4};
    }
    {
        LevelManager::_mapGroups[2] = {1,2,3,4};
    }
    {
        LevelManager::_mapGroups[3] = {1,2,3,4};
    }
    {
        LevelManager::_mapGroups[4]  = {1,2,3,4};
    }

    
    LevelManager::_sMapConfigInfos =
    {
        {   GamePaths::_sPathBackground_level_1_1,
            GamePaths::_sPathMapDirt_level_1_1,
            GamePaths::_sPathMapBorder_level_1_1,
            GamePaths::_sPathUIBorder_level_1_1
        },
        {   GamePaths::_sPathBackground_level_2_1,
            GamePaths::_sPathMapDirt_level_2_1,
            GamePaths::_sPathMapBorder_level_2_1,
            GamePaths::_sPathUIBorder_level_2_1
        },
        {   GamePaths::_sPathBackground_level_3_1,
            GamePaths::_sPathMapDirt_level_3_1,
            GamePaths::_sPathMapBorder_level_3_1,
            GamePaths::_sPathUIBorder_level_3_1
        },
        {   GamePaths::_sPathBackground_level_1_1,
            GamePaths::_sPathMapDirt_level_1_1,
            GamePaths::_sPathMapBorder_level_1_1,
            GamePaths::_sPathUIBorder_level_1_1
        },
    };

    float x = Director::getInstance()->getWinSize().width*0.5;
///////////////////////////////
    LevelManager::_sPlantConfigInfos =
    {
        ////id - 0
        {
            {
                {
                    {x,0.0f},40.0f
                },
            },
            GamePaths::_sPathPlantBody_level_1_1,
            {"plant1_leaf_2.png","plant1_leaf_3.png","plant1_leaf_4.png"},
            {
                {
                    {0.0f,0.1f},{80.0f,20.0f},{400.0f,60.0f},{600.0f,75.0f}
                }
            },
            {
                {
                    {0.0f,0.0f},{100.0f,0.1f},{300.0f,0.2f},{600.0f,0.3f}
                }
            },
            201.0f,
            51.0f
        },
        ////id - 1
        {
            {
                {
                    {x,0.0f},40.0f
                },
            },
            GamePaths::_sPathPlantBody_level_2_1,
            {"plant2_leaf_1.png"},
            {
                {
                    {0.0f,0.1f},{80.0f,20.0f},{400.0f,60.0f},{600.0f,75.0f}
                }
//                {
//                    {0.0f,1.0f},{80.0f,20.0f},{400.0f,80.0f},{600.0f,90.0f}
//                }
            },
            {
                {
                    {0.0f,0.0f},{100.0f,0.1f},{300.0f,0.18f},{600.0f,0.26f}
                }
            },
            31.0f,
            11.0f
        },
        
        ////id - 2
        
        {
            {
                {
                    {x,0.0f},40.0f
                },
            },
            GamePaths::_sPathPlantBody_level_2_1,
            {"plant2_leaf_1.png"},
            {
                {
                    {0.0f,0.0f},{80.0f,20.0f},{400.0f,60.0f},{600.0f,75.0f}
                }
            },
            {
                {
                    {0.0f,0.0f},{100.0f,0.1f},{300.0f,0.2f},{600.0f,0.3f}
                }
            },
            71.0f,
            21.0f
        },

        
        ////id - 3
        {
            {
                {
                    {x,0.0f},30.0f
                },
            },
            GamePaths::_sPathPlantBody_level_1_1,
            {"plant1_leaf_2.png","plant1_leaf_3.png","plant1_leaf_4.png"},
            {
                {
                    {0.0f,1.0f},{80.0f,20.0f},{400.0f,50.0f},{600.0f,60.0f}
                }
            },
            {
                {
                    {0.0f,0.0f},{100.0f,0.1f},{300.0f,0.2f},{600.0f,0.3f}
                }
            },
            201.0f,
            51.0f
        },
        
        ////id - 4
        {
            {
                {
                    {x,0.0f},30.0f
                },
            },
            GamePaths::_sPathPlantBody_level_1_1,
            {"plant1_leaf_2.png","plant1_leaf_3.png","plant1_leaf_4.png"},
            {
                {
                    {0.0f,0.1f},{80.0f,20.0f},{400.0f,60.0f},{600.0f,75.0f}
                }
            },
            {
                {
                    {0.0f,0.0f},{100.0f,0.1f},{300.0f,0.2f},{600.0f,0.3f}
                }
            },
            201.0f,
            51.0f
        }
    };
}
void LevelManager::selectConfigInfo(int id)
{
    auto conifg = GameRunningConfig::getInstance();
    auto& mapInfo = _sMapConfigInfos[id];
    auto cache = Director::getInstance()->getTextureCache();
    conifg->setTextureBackGround(cache->addImage(mapInfo[0]));
    conifg->setTextureMapDirt(cache->addImage(mapInfo[1]));
    conifg->setTextureMapBorder(cache->addImage(mapInfo[2]));
    conifg->setTextureUIBorder(cache->addImage(mapInfo[3]));
}
void LevelManager::selectLevel(int id)
{
    assert(id>0&&id<5);
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
   // LevelManager::selectMapFile(_mapGroupId,_mapGroupSubId);
    
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
        case 4:
            initLevelInfo_4();
            break;
        case 5:
            initLevelInfo_1();
            break;
        default:
            break;
    }
}

GamePlantConfig&  LevelManager::initPlantConfig(int runningConfigId,int idConfig)
{
    auto& info = LevelManager::_sPlantConfigInfos[idConfig];
    auto conifg = GameRunningConfig::getInstance();
    auto cache = Director::getInstance()->getTextureCache();
    GamePlantConfig  first;
    first._textureLeft = info._textureLeft;
    first._textureRight = info._textureRight;
    first._growSpeed = info._growSpeed;
    first._reGrowSpeed = info._reGrowSpeed;
    first._plantNodeInitPosition = info._plantNodeInitPosition;
    first._plantStartContorlPointList = info._plantStartContorlPointList;
    
    auto tex = cache->addImage(info._pathPlangBody);
    Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
    tex->setTexParameters(parms);
    first._texturePlantBody = tex;
    
    first._leafsRadiusList = info._leafsRadiusList;
    first._radiusList = info._bodyRadiusList;
    
    first._leafHeightStep = info._leafHeightStep;
    first._leafHeightRange = info._leafHeightRange;
    first._texturePlantLeaf = info._pathPlantLeaf;
    conifg->getPlantConfigs().reserve(runningConfigId+1);
    auto& plantConfig = conifg->getPlantConfigs();
    int size = plantConfig.size();
    for (int i = size; i < runningConfigId+1; i++) {
        plantConfig.push_back(GamePlantConfig());
    }
    plantConfig[runningConfigId] = first;
    
    return *(&plantConfig[runningConfigId]);
}
GamePlantConfig&  LevelManager::initPlantConfig(int runningConfigId,int idConfig,Vec2 pt)
{
    auto& config = initPlantConfig(runningConfigId,idConfig);
    (config._plantStartContorlPointList.rbegin())->_point = pt;
    return config;
}
GamePlantConfig&  LevelManager::initPlantConfig(int runningConfigId,int idConfig,ContorlPointV2 cp)
{
    auto& config = initPlantConfig(runningConfigId,idConfig);
    config._plantStartContorlPointList = {cp};
    return config;
}
void LevelManager::initLevelInfo_1()
{
    selectConfigInfo(0);
    initPlantConfig(0,0);
}

void LevelManager::initLevelInfo_2()
{
    selectConfigInfo(1);
    initPlantConfig(0,1);
}
void LevelManager::initLevelInfo_3()
{
    selectConfigInfo(2);
    initPlantConfig(0,2);
    initPlantConfig(1,3);
}
void LevelManager::initLevelInfo_4()
{
    selectConfigInfo(3);
    initPlantConfig(0,3);
}

void LevelManager::selectMapFile(int groupId,int groupSubId)
{
    if (_levelId < 5) {
        int id = LevelManager::_mapGroups[groupId][groupSubId];
        initMapFile(LevelManager::_mapFiles[id]);
    }
    else{
        
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
        case 4:
            ret=  static_cast<GameLayerPlant*>(GameLayerPlant_Level_1::create());
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
    //gameScene->addChild(layerLandmark,SceneGameChildZorder::MapItem+1);
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
    //auto size = GameRuntime::getInstance()->getMapGridSize();
    //
    //layerItem->loadItemsAndBodys(0);
    //layerItem->moveDown(0);
    //map->updateExcavatePolygon(0,0,size.width,size.height+MAX_CRASH_LENGTH);
    //map->update(0.1);

}