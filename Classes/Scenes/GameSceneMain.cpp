#include "GameSceneMain.h"
#include "GameRunningInfo.h"
#include "GameLayers.h"
#include "GameLayerPlant_Level_1.h"
#include "GameLayerPlant_Level_2.h"
#include "GamePhysicalWorld.h"
#include "GameLayerUIBorder.h"
#include "LayerItem.h"
#include "LevelFileHandler.h"
#include "GameManager.h"
#include "GameRuntime.h"
#include "TuoYuanRotateNode.h"
#include "GameLayerLight.h"
#include "LayerLight.h"
#include "GameLayerUI.h"
#include "GameLayerEffects.h"
#include "PathListHelper.hpp"
#include "LevelManager.h"

GameSceneMain::~GameSceneMain()
{
    CC_SAFE_RELEASE(GameManager::getInstance()->_fileHandler);
    GameManager::getInstance()->resetGameContext();
    GameManager::getInstance()->_gameScne = nullptr;
    GameRunningInfo::getInstance()->setGameSceneNode(nullptr);
    GamePhysicalWorld::getInstance()->DestorySingletonObject();
    GameManager::getInstance()->_world = nullptr;
}

Scene* GameSceneMain::createScene()
{
    auto scene = Scene::create();
    auto layer = GameSceneMain::create();
    scene->addChild(layer);
    return scene;
}

bool GameSceneMain::init()
{
    Layer::init();
    CC_SAFE_RETAIN(GameManager::getInstance()->_fileHandler);
    GameManager::getInstance()->_world = GamePhysicalWorld::getInstance()->getBox2dWorld();
    GameManager::getInstance()->_gameScne = static_cast<Scene*>(this->getParent());
    GameRunningInfo::getInstance()->setGameSceneNode(static_cast<Node*>(this));
    GameRunningInfo::getInstance()->setIsGameEnd(false);
//    DrawLayerCallFunc* worldDraw = DrawLayerCallFunc::create();
//    worldDraw->_callBack =  std::bind(&GameSceneMain::onDraw,this,std::placeholders::_1,std::placeholders::_2);
//    addChild(worldDraw,10009);

    return true;
}
void GameSceneMain::update(float dt)
{
    GameRunningManager::getInstance()->update(dt);
}
void GameSceneMain::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC,true);
    scheduleUpdate();
}
void GameSceneMain::onExitTransitionDidFinish()
{
    Layer::onExitTransitionDidStart();
  
}
void GameSceneMain::onEnter()
{
    Layer::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MUSIC);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    
}
void GameSceneMain::onExit()
{
    Layer::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void GameSceneMain::onDraw(const Mat4 &transform, uint32_t flags)
{
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    GamePhysicalWorld::getInstance()->getBox2dWorld()->DrawDebugData();
    kmGLPopMatrix();
    
}


