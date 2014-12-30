#include "GameSceneNavigation.h"
#include "LevelManager.h"
#include "GameManager.h"
 GameSceneNavigation* GameSceneNavigation::createScene()
{
    GameSceneNavigation* ret = new GameSceneNavigation();
    if(ret&&ret->init()){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameSceneNavigation::init()
{
    Scene::init();
    auto levelManager = LevelManager::getInstance();
    int levelId = levelManager->_levelId;
    int levelSubId = levelManager->_mapGroupSubId+1;
    if (levelId>1) {
        levelId =1;
        levelSubId =1;
    }
    auto fileName = StringUtils::format("level_loading_%d-%d.png",levelId,levelSubId);
    
    Sprite* sp = Sprite::create(fileName);
    
    addChild(sp);
    auto vec2 = Director::getInstance()->getWinSize();
    sp->setPosition(vec2*0.5);
    return true;
}
void GameSceneNavigation::onEnter()
{
    Scene::onEnter();
//    auto manager =  GameManager::getInstance();
//    manager->navigationToGameScene();
  //  Scene::onEnter();
}
void GameSceneNavigation::onEnterTransitionDidFinish()
{
    Scene::onEnterTransitionDidFinish();
    CallFunc* call = CallFunc::create( [=]()
    {
        auto manager =  GameManager::getInstance();
        manager->navigationToGameScene();
    });
    this->runAction(call);
}