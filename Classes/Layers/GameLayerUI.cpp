#include "GameLayerUI.h"
#include "GameRunningInfo.h"
#include "GameRuntime.h"
#include "GameManager.h"
#include "GameLayerPlant.h"
#include "GameRunningManager.h"
#include "GameLayerLight.h"
#include "LevelManager.h"
bool GameLayerUI::init()
{
    GameLayerBase::init();

    return true;
}
bool GameLayerUI::initGameInfo()
{
    GameLayerBase::initGameInfo();
    setContentSize(GameRuntime::getInstance()->getVisibleSize());
    UIHelper::layout(this, UIHelper::ViwePoistionType::BottomCenter);
    _pause_btn = Sprite::create("pause_btn.png");
    _pause_btn->setAnchorPoint(Vec2::ZERO);
    // "Arial"
    _lightCountShow_left = Sprite::create("light_show.png");
    _lightCountShow_right = Sprite::create("light_show.png");
    
    _lightCountShowNumber_left = Label::createWithSystemFont( "",  "Arial",20);
    _lightCountShowNumber_right = Label::createWithSystemFont( "",  "Arial",20);
    
    //    _lightCountShowNumber_left->setAnchorPoint(Vec2::ZERO);
    //    _lightCountShowNumber_left->setAnchorPoint(Vec2(1,0));
    
    Size wSize = GameRuntime::getInstance()->getVisibleSize();
    _lightCountShow_left->setAnchorPoint(Vec2(0.5,0.45));
    _lightCountShow_right->setAnchorPoint(Vec2(0.5,0.45));
    addChild(_pause_btn,1001);
    addChild(_lightCountShowNumber_left,1002);
    addChild(_lightCountShowNumber_right,1002);
    addChild(_lightCountShow_left,1001);
    addChild(_lightCountShow_right,1001);
    _pause_btn->setPosition(Vec2(20,20));
    Size lSize = _lightCountShow_left->getContentSize();
    _lightCountShow_left->setPosition(Vec2(20+lSize.width*0.5,80+lSize.height*0.5));
    _lightCountShow_right->setPosition(Vec2(wSize.width - 20-lSize.width*0.5,80+lSize.height*0.5));
    auto lrect = _lightCountShow_left->getBoundingBox();
    _lightCountShowNumber_left->setPosition(Vec2(lrect.origin.x+lrect.size.width*0.5,lrect.origin.y+lrect.size.height*0.5));
     auto rrect = _lightCountShow_right->getBoundingBox();
    _lightCountShowNumber_right->setPosition(Vec2(rrect.origin.x+rrect.size.width*0.5,rrect.origin.y+rrect.size.height*0.5));
    
    
    _lengthShow_left = Label::createWithSystemFont( "",  "Arial",35);
    _lengthShow_right = Label::createWithSystemFont( "",  "Arial",35);
    _lengthShow_left->setPosition(Vec2(100,wSize.height*0.5));
    _lengthShow_right->setPosition(Vec2(wSize.width- 100,wSize.height*0.5));
    
    _lengthShow_left->setOpacity(255*0.1);
    addChild(_lengthShow_left);
    addChild(_lengthShow_right);
    _pause = Sprite::create("pause.png");
    
    float scale = GameRuntime::getInstance()->getVisibleSize().width/ _pause->getContentSize().width;
    float height =_pause->getContentSize().height;
    _pause->setScale(scale);
    addChild(_pause,1007);
    _pause->setAnchorPoint(Vec2::ZERO);
    _pause->setPosition(Vec2::ZERO);
    
    _layerColor = LayerColor::create(Color4B(0 ,0,0,90));
    addChild(_layerColor,1006);
    
    Size win =  GameRuntime::getInstance()->getVisibleSize();
    float d = 1.0/3.0f;
    float w0 = win.width*0.5;
    float w = win.width*0.5*d;
    _pauseRect = Rect(w0,0,w0,height);
    _toFlower = Rect(w0 ,0,w,height);
    _toLevel = Rect(w0 + w,0,w,height);
    _toLogo  = Rect(w0 + w*2,0,w,height);
    _isTouch = false;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayerUI::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayerUI::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayerUI::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //事件调度器
    //listener->setSwallowTouches(true);
    // schedule(schedule_selector(GameLayerUI::updateLength),1.0f/5.0f);
    _lengthShow_left->setOpacity(0);
    reStartGame();
    
    int count = LevelManager::getInstance()->_levelId == 3 ? 2 : 1;
    if (count<2) {
        _lightCountShowNumber_right->setVisible(false);
        _lightCountShow_right->setVisible(false);
    }
    return true;
}
void  GameLayerUI::showLinght()
{
   // _lengthShow_left->setOpacity(255);
    FadeIn* fadeIn = FadeIn::create(0.5);
     auto dtime = DelayTime::create(1.5);
    //auto opeato = FadeTo::create(0.1, 0);
    FadeOut* fadeout = FadeOut::create(1);
    _lengthShow_left->runAction(Sequence::create(fadeIn,dtime,fadeout ,NULL));
}

void  GameLayerUI::updateLightShow()
{
    auto manager= GameRunningManager::getInstance();

    int lightCount = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(0);
    char buf[256]= {""};
    sprintf(buf,"%d",lightCount);
    _lightCountShowNumber_left->setString(buf);

    float passHeight = manager->getPlantRemoveLightStepHeight(0)/manager->getPlantRemoveLightUnitHeight(0);
    float angle = passHeight*360;
    _lightCountShow_left->setRotation(angle);
    
    
    int count = LevelManager::getInstance()->_levelId == 3 ? 2 : 1;
    if (count >1) {
        int lightCount = GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(1);
        char buf[256]= {""};
        sprintf(buf,"%d",lightCount);
        _lightCountShowNumber_right->setString(buf);
        
        float passHeight = manager->getPlantRemoveLightStepHeight(1)/manager->getPlantRemoveLightUnitHeight(1);
        float angle = passHeight*360;
        _lightCountShow_right->setRotation(angle);
    }

}
void GameLayerUI::update(float dt)
{
    updateLightShow();
    //updateLength(dt);
}
void GameLayerUI::onEnter()
{
    GameLayerBase::onEnter();

}
void GameLayerUI::onExit()
{
    GameLayerBase::onExit();
}
bool GameLayerUI::onTouchBegan(Touch* touch,Event* event)
{
    
    auto gamemanager = GameManager::getInstance();
    Vec2 pt = touch->getLocation();
    if ((!gamemanager->isPause() && _pause_btn->getBoundingBox().containsPoint(pt))||
        gamemanager->isPause())
    {
        return true;
    }

    return false;
}

void GameLayerUI::onTouchMoved(Touch* touch,Event* event)
{
    
}

void GameLayerUI::onTouchEnded(Touch* touch,Event* event)
{
    auto gamemanager = GameManager::getInstance();
    Vec2 pt = touch->getLocation();
    if (!gamemanager->isPause() && _pause_btn->getBoundingBox().containsPoint(pt)) {
        //_pause_btn->setColor(Color3B::YELLOW);
        pauseGame();
        gamemanager->pauseGame();
       
    }
    else if( gamemanager->isPause()&&!_pauseRect.containsPoint(pt))
    {
        gamemanager->reStartGame();
        reStartGame();
    }
    else if( gamemanager->isPause()&&_pauseRect.containsPoint(pt))
    {
        if (_toLogo.containsPoint(pt)) {
            GameManager::getInstance()->navigationTo(SceneLogo);
        }
        else   if (_toFlower.containsPoint(pt)) {
            GameManager::getInstance()->navigationTo(SceneFlower);
            
        }
        else   if (_toLevel.containsPoint(pt)) {
            GameManager::getInstance()->navigationTo(ScenePageView);
        }
    }
}
void  GameLayerUI::pauseGame()
{
    _pause->setVisible(true);
    _layerColor->setVisible(true);
}
void  GameLayerUI::reStartGame()
{
    _pause->setVisible(false);
    _layerColor->setVisible(false);
}