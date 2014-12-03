#include "GameLayerUI.h"
#include "GameRunningInfo.h"
#include "GameRuntime.h"
#include "GameManager.h"
#include "GameLayerPlant.h"
#include "LayerLight.h"
bool GameLayerUI::init()
{
    GameLayerBase::init();
    _pause_btn = Sprite::create("pause_btn.png");
    _pause_btn->setAnchorPoint(Vec2::ZERO);
   // "Arial"
    _lightCountShow_left = Sprite::create("light_show.png");
    _lightCountShow_right = Sprite::create("light_show.png");
    
    _lightCountShowNumber_left = Label::createWithSystemFont( "",  "Arial",20);
    _lightCountShowNumber_right = Label::createWithSystemFont( "",  "Arial",20);

    _lightCountShow_left->setAnchorPoint(Vec2::ZERO);
    _lightCountShow_right->setAnchorPoint(Vec2(1,0));
    addChild(_pause_btn,1001);
    addChild(_lightCountShowNumber_left,1002);
    addChild(_lightCountShowNumber_right,1002);
    addChild(_lightCountShow_left,1001);
    addChild(_lightCountShow_right,1001);
    _pause_btn->setPosition(Vec2(20,20));
    _lightCountShow_left->setPosition(Vec2(20,80));
    _lightCountShow_right->setPosition(Vec2(GameRuntime::getInstance()->getVisibleSize().width - 20,80));
    auto lrect = _lightCountShow_left->getBoundingBox();
    _lightCountShowNumber_left->setPosition(Vec2(lrect.origin.x+lrect.size.width*0.5,lrect.origin.y+lrect.size.height*0.5));
    _lightCountShowNumber_right->setPosition(Vec2(GameRuntime::getInstance()->getVisibleSize().width - 22,82));
    _lightCountShow_right->setVisible(false);
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
    
    listener->onTouchBegan = CC_CALLBACK_2(GameLayerUI::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayerUI::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayerUI::onTouchMoved, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //事件调度器
    _pause->setVisible(false);
    _layerColor->setVisible(false);
    return true;
}
void GameLayerUI::update(float dt)
{
    int count = GameLayerPlant::getRunningLayer()->getPlantCount();
    int lightCount = LayerLight::getRunningLayer()->_lightListFirst.size();
    char buf[256]= {""};
    sprintf(buf,"%d",lightCount);
    _lightCountShowNumber_left->setString(buf);
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
    
    Vec2 pt = touch->getLocation();
    if (_pause_btn->getBoundingBox().containsPoint(pt)) {
        _pause_btn->setColor(Color3B::YELLOW);
        _pause->setVisible(true);
        _layerColor->setVisible(true);
        auto gamemanager = GameManager::getInstance();
        if (!gamemanager->isPause()) {
            gamemanager->pauseGame();
            _isTouch = true;;
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        }
        
        return true;
    }
    else if(_isTouch && !_pauseRect.containsPoint(pt)){
        auto gamemanager = GameManager::getInstance();
        if (gamemanager->isPause()) {
            GameManager::getInstance()->reStartGame();
            _pause_btn->setColor(Color3B::WHITE);
            _pause->setVisible(false);
            _layerColor->setVisible(false);
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            _isTouch = false;
            return false;
        }
    }
    
    if (_isTouch) {
        
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
    
    if(pt.x < 60 ||
       pt.x > Director::getInstance()->getWinSize().width-60)return false;
    return true;
}

void GameLayerUI::onTouchMoved(Touch* touch,Event* event)
{
    
}

void GameLayerUI::onTouchEnded(Touch* touch,Event* event)
{
    Vec2 pt = touch->getLocation();
    if (_pause_btn->getBoundingBox().containsPoint(pt)) {
        auto gamemanager = GameManager::getInstance();
        if (!gamemanager->isPause()) {
            gamemanager->pauseGame();
        }
        return ;
    }
}