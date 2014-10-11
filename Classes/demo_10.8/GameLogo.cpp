#include "GameLogo.h"
#include "ScenePageView.h"
#include "GameManager.h"
USING_NS_CC;

Scene* GameLogo::createScene()
{
    auto scene = Scene::create();
    auto layer = GameLogo::create();
    scene->addChild(layer);
    return scene;
}

bool GameLogo::init()
{
    
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //背景图片
    auto background = Sprite::create("background.jpg");
    
    background->setScale(0.5, 0.5);
    
    background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    _background = background;
    
    this->addChild(background, 0);
    
    //创建logo
    auto logo = Sprite::create("logo.png");
    
    logo->setScale(0.5, 0.5);
    
    logo->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2+visibleSize.height/8));
    _logo = logo;
    
    _logo->runAction(Sequence::create(ScaleBy::create(10, 1.3), NULL));
    this->addChild(logo,88888888);
    
    //创建圈圈
    auto circle = Sprite::create("circle.png");
    
    circle->setScale(0.5,0.5);
    
    circle->setPosition(Point(visibleSize.width/2,visibleSize.height/2.5));
    //_circle = circle;
    
    this->addChild(circle);
    
    //创建圈圈后的特效
    auto circlebackground = Sprite::create("circlebackground.png");
    
    circlebackground->setScale(0.5,0.5);
    
    circlebackground->setPosition(Point(visibleSize.width/2,visibleSize.height/2.5));
    _circlebackground = circlebackground;
    
    this->addChild(circlebackground);
    
    auto button = Sprite::create("buttonNormal.png");
    
    button->setScale(0.5, 0.5);
    
    button->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2.5));
    button->setScale(0.5,0.5);
    auto rotate = RotateBy::create(5,180);
    button->runAction(RepeatForever::create(rotate));
    
    this->addChild(button);

    EventListenerTouchOneByOne* start_listener = EventListenerTouchOneByOne::create();
    start_listener->setSwallowTouches(true);
    
    start_listener->onTouchBegan = [](Touch* touch,Event* event){
        Point pt = event->getCurrentTarget()->getParent()->convertToNodeSpace(touch->getLocation());
        Sprite* sprite = static_cast<Sprite*>(event->getCurrentTarget());
        if (sprite->getBoundingBox().containsPoint(pt)){
            return true;
        }
        return false;
    };
    auto aa = GameManager::getInstance();
    start_listener->onTouchMoved = [](Touch* touch,Event* event){};

    start_listener->onTouchEnded = [&](Touch* touch,Event* event){

        CallFunc* selectlevel = CallFunc::create([&](){
            
            GameManager::getInstance()->navigationToPageView();
        });
        
        _circlebackground->runAction(Sequence::create(ScaleBy::create(0.5, 1.8), selectlevel,NULL));
  
       _circlebackground->runAction(FadeOut::create(0.5));
//        
//        _background->runAction(FadeOut::create(0.5));
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(start_listener, button);
    return true;
}







