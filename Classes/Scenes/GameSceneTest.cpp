#include "GameSceneTest.h"
Scene* GameSceneTest::createScene()
{
    auto scene = Scene::create();
    auto layer = GameSceneTest::create();
    scene->addChild(layer);
    return scene;
}

bool GameSceneTest::init()
{
    Layer::init();
    //背景图片
    Sprite* background = Sprite::create("testWin.png");
    background->setPosition(VisibleSize.width/2,VisibleSize.height/2);
    background->setScale(VisibleSize.height/background->getContentSize().height);
    this->addChild(background);
    
    return true;
}