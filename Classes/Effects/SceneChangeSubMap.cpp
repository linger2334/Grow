#include "SceneChangeSubMap.h"
#include "GameManager.h"
#include "LevelManager.h"
Scene* SceneChangeSubMap::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneChangeSubMap::create();
    scene->addChild(layer);
    return scene;
}

void SceneChangeSubMap::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
//    LevelManager::getInstance()->releaseLevelInfo();
  //  GameManager::getInstance()->navigationToGameScene();
}