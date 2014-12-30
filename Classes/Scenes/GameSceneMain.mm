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
#include "GameLayerUI.h"
#include "GameLayerEffects.h"
#include "PathListHelper.hpp"
#include "LevelManager.h"
#include "GameManager.h"
/////////////////
#include "CocosGUI.h"
#include "LevelsMenu.h"
#include "LevelEditor.h"
///////////////////
using namespace ui;
GameSceneMain::~GameSceneMain()
{
    GameRunningInfo::getInstance()->setGameSceneNode(nullptr);
    CC_SAFE_RELEASE(GameManager::getInstance()->_fileHandler);
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
    GameRunningInfo::getInstance()->setGameSceneNode(static_cast<Node*>(this));
    GameManager::getInstance()->_gameScne = static_cast<Scene*>(this->getParent());
    GameRunningInfo::getInstance()->setIsGameEnd(false);
    CC_SAFE_RETAIN(GameManager::getInstance()->_fileHandler);
//    DrawLayerCallFunc* worldDraw = DrawLayerCallFunc::create();
//    worldDraw->_callBack =  std::bind(&GameSceneMain::onDraw,this,std::placeholders::_1,std::placeholders::_2);
//    worldDraw->setAnchorPoint(Vec2(0.5,0.5f));
//    UIHelper::layout(worldDraw, UIHelper::ViwePoistionType::Center);
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
    GameRunningManager::getInstance()->onGameSceneMainEnterComplete();
}

void GameSceneMain::onEnter()
{
    Layer::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MUSIC);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    GameRunningInfo::getInstance()->setGameSceneNode(this);
    GameRunningManager::getInstance()->onGameSceneMainEnter();
    initLevelEditorMenu();
}
void GameSceneMain::onExit()
{
    Layer::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    GameRunningManager::getInstance()->onGameSceneMainExit();
}
void GameSceneMain::onExitTransitionDidFinish()
{
    Layer::onExitTransitionDidStart();
    GameRunningManager::getInstance()->onGameSceneMainEixtComplete();
}
void GameSceneMain::onDraw(const Mat4 &transform, uint32_t flags)
{
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    GamePhysicalWorld::getInstance()->getBox2dWorld()->DrawDebugData();
    kmGLPopMatrix();
    
}
#include "GameLayerMapBorder.h"
void    GameSceneMain::updateFirstGrowPlant(float dt)
{
//    GameLayerMap::getRunningLayer()->update(dt);
//    GameLayerMapBorder::getRunningLayer()->update(dt);
//    auto layerLight = GameLayerLight::getRunningLayer();
//    if (layerLight->isAllPlantHasLights()) {
//        unschedule(schedule_selector(GameSceneMain::updateFirstGrowPlant));
//        scheduleUpdate();
//        return ;
//    }
//    GameLayerPlant::getRunningLayer()->update(dt);
//    layerLight->update(dt);
//     GamePhysicalWorld::getInstance()->update(dt);
}

void GameSceneMain::initLevelEditorMenu()
{
    Label* levelsLabel = Label::createWithTTF("Levels", "Marker Felt.ttf", 36);
    MenuItem* toLevelsItem = MenuItemLabel::create(levelsLabel, [&](Ref*){
        Director::getInstance()->replaceScene(LevelsMenu::createScene());
    });
    
    Label* editorLabel = Label::createWithTTF("Editor", "Marker Felt.ttf", 36);
    MenuItem* toEditorItem = MenuItemLabel::create(editorLabel,[&](Ref*){
      //  GameManager::getInstance()->navigationTo(LevelEditor::createScene());
        auto manager = GameManager::getInstance();
        manager->releaseGameScene();
        Director::getInstance()->replaceScene(LevelEditor::createScene());
    });
    
    Menu* menu = Menu::create(toLevelsItem,toEditorItem,NULL);
    menu->setPosition(Vec2(VisibleSize.width/2,toEditorItem->getContentSize().height/2));
    menu->alignItemsHorizontallyWithPadding(VisibleSize.width/10);
    GameRunningInfo::getInstance()->getGameSceneNode()->addChild(menu,MapLevelEditorMenu);
    
    Button* addSpeed = Button::create("addspeed.png");
    Button* reduceSpeed = Button::create("reducespeed.png");
    Text* currentSpeed = Text::create(StringUtils::format("%d",(int)(GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed)), "Arial", 24);
    addSpeed->setScale(0.5);
    reduceSpeed->setScale(0.5);
    addSpeed->setPosition(Vec2(0.9*VisibleSize.width,0.05*VisibleSize.height));
    reduceSpeed->setPosition(Vec2(0.8*VisibleSize.width,0.05*VisibleSize.height));
    currentSpeed->setPosition(Vec2(0.85*VisibleSize.width,0.1*VisibleSize.height));
    addSpeed->addTouchEventListener([=](Ref* sender,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            float speed = GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed;
            speed +=5;
            speed = speed>=250? 250 : speed;
            GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed = speed;
            //if (_levelId>2) {
              //  GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(1)._growSpeed = speed;
          //  }
            currentSpeed->setString(StringUtils::format("%d",(int)speed));
        }
    });
    reduceSpeed->addTouchEventListener([=](Ref* sender,Widget::TouchEventType eventType){
        if (eventType == Widget::TouchEventType::ENDED) {
            float speed = GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed;
            speed -= 5;
            speed = speed<=0? 0 : speed;
            GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(0)._growSpeed = speed;
          //  if (_levelId>2) {
             //   GameLayerPlant::getRunningLayer()->getPlantInfoByIndex(1)._growSpeed = speed;
          //  }
            currentSpeed->setString(StringUtils::format("%d",(int)speed));
        }
    });
    
    GameRunningInfo::getInstance()->getGameSceneNode()->addChild(addSpeed,MapLevelEditorMenu);
    GameRunningInfo::getInstance()->getGameSceneNode()->addChild(reduceSpeed,MapLevelEditorMenu);
    GameRunningInfo::getInstance()->getGameSceneNode()->addChild(currentSpeed,MapLevelEditorMenu);
}
