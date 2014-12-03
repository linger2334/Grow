#include "GameManager.h"
#include "GameRuntime.h"
#include "GameSceneMain.h"
#include "GameRunningInfo.h"
#include "LevelManager.h"
#include "SceneChangeSubMap.h"
#include "GameRunningManager.h"
#include "GameSceneWin.h"
#include "LayerItem.h"
#include "ScreenLoading.h"
#include "ScreenSelectLevel.h"
#include "ScreenStart.h"
#include "AchievementUi.h"

GameManager::Recycle GameManager::garbo;

GameManager::GameManager():_world(nullptr),_levelEditor(nullptr)
{
    GameRuntime::getInstance()->initGameRuntimeInfo();
    GameRunningInfo::getInstance()->initGameRunningInfo();
}

GameManager::~GameManager()
{
    GameRuntime::getInstance()->DestorySingletonObject();
    GameRunningInfo::getInstance()->DestorySingletonObject();
}

void GameManager::resetGameContext()
{
    LevelManager::getInstance()->releaseLevelInfo();
    if(_gameScne)
    {
//    _gameScne->unscheduleUpdate();
//    _gameScne->removeAllChildren();
    }
    GameRunningInfo::getInstance()->resetGameRunningInfo();
    GameRunningManager::getInstance()->_passHeight = 0;
}

LayerItem* GameManager::getLayerLight()
{
    return LayerItem::getRunningLayer();
}
void  GameManager::navigationTo(int id)
{
    Scene* scene = nullptr;
    switch (id) {
        case ScenePageView:
            scene = ScreenSelectLevel::create();
            break;
        case SceneLogo:
            scene = ScreenStart::createScene();
            break;
        case SceneLoding:
            scene = ScreenLoading::createScene(0);
            break;
        case SceneGameScene:
            scene = GameSceneMain::createScene();
            break;
        case SceneFlower:
            scene = AchievementUi::createScene();
            break;
        case SceneGameWin:
            scene = GameSceneWin::createScene();
            break;
        default:
            break;
    }
    navigationTo(scene);
}
void  GameManager::navigationToPasueGameScene()
{
    navigationTo(_gameScne);
}
void   GameManager::releaseGameScene()
{
    if (_gameScne) {
        _gameScne->release();
        _gameScne = nullptr;
    }
}
void  GameManager::releaseRunningGameInfo()
{
    LevelManager::getInstance()->releaseLevelInfo();
    if(_gameScne)
        _gameScne->removeAllChildren();
}
void  GameManager::navigationToGameScene()
{
    if (!_gameScne) {
        _gameScne = GameSceneMain::createScene();
        LevelManager::getInstance()->initGameScene();
    }
    navigationTo(_gameScne);
}
void  GameManager::navigationToGameScene(int level)
{
    if (!_gameScne) {
        LevelManager::getInstance()->selectLevel(level);
        _gameLevel = level;
    }
    navigationToGameScene();
}

void  GameManager::navigationTo(Scene* scene)
{
    Director::getInstance()->replaceScene(scene);
}

void  GameManager::initGameRunningInfo()
{
    GameRunningManager::getInstance()->_passHeight = 0;
}
