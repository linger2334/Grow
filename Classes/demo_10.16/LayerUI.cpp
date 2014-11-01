#include "LayerUI.h"
#include "LayerPlant.h"
#define BACKGROUND_MUSIC "background_music_1.mp3"
bool LayerUI::init()
{
    GameLayerRollY::init();
    _ringbottom = Sprite::create("ring_bottom.png");
    Sprite* ringtop    = Sprite::create("ring_top.png");
    _ringbottom->setAnchorPoint(Vec2::ZERO);
    ringtop->setAnchorPoint(Vec2::ZERO);
    _ringbottom->setPosition(Vec2::ZERO);
    ringtop->setPosition(Vec2::ZERO);
    addChild(_ringbottom,1000);
    addChild(ringtop,1001);
    _pause = Sprite::create("pause.png");
    addChild(_pause,1007);
    _pause->setAnchorPoint(Vec2::ZERO);
    _pause->setPosition(Vec2::ZERO);
    _pause->setVisible(false);
    _layerColor = LayerColor::create(Color4B(0 ,0,0,90));
    addChild(_layerColor,1006);
    _layerColor->setVisible(false);
    
    
        Button* btnAddSpeed =Button::create("flower.png");
        Button* btnSubSpeed =Button::create("back.png");
        btnAddSpeed->setScale(0.5);
        btnSubSpeed->setScale(0.5);
    
        btnAddSpeed->setAnchorPoint(Vec2(1,0));
        btnAddSpeed->setPosition(Vec2(WinSize.width-10,10));
        btnSubSpeed->setAnchorPoint(Vec2(1,0));
        btnSubSpeed->setPosition(Vec2(WinSize.width-70,10));
        addChild(btnAddSpeed,999);
        addChild(btnSubSpeed,999);
        //_plantLayer->setGrowSpeed(60);
        Label*  lable = Label::createWithSystemFont("生长速度: 60","Arial",20);
    
        addChild(lable,999);
        lable->setAnchorPoint(Vec2(1,0));
        lable->setPosition(Vec2(WinSize.width-90,100));
#define EDIT_SPEED_SPED 5
        btnAddSpeed->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
                                           {
                                              // testBorderLine();
                                               if(type !=  Widget::TouchEventType::ENDED)
                                                   return;
    
                                               float speed =   GameManager::getInstance()->getLayerPlant()->getGrowSpeed();
                                               speed+=EDIT_SPEED_SPED;
                                               if (speed>240) {
                                                   speed = 240;
                                               }
                                               GameManager::getInstance()->getLayerPlant()->setGrowSpeed(speed);
                                               char buf[40] = {0};
                                               sprintf(buf, "生长速度: %d",(int)speed);
                                               lable->setString(buf);
    
                                           });
        btnSubSpeed->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
                                           {
                                               if(type !=  Widget::TouchEventType::ENDED)
                                                   return;
                                               float speed =   GameManager::getInstance()->getLayerPlant()->getGrowSpeed();
                                               speed -=EDIT_SPEED_SPED;
                                               if (speed < 0) {
                                                   speed = 0;
                                               }
                                                 GameManager::getInstance()->getLayerPlant()->setGrowSpeed(speed);
                                               char buf[40] = {0};
                                               sprintf(buf, "生长速度: %d",(int)speed);;
                                               lable->setString(buf);
                                           });
    
    return true;
}

bool LayerUI::onTouchBegan(Touch* touch,Event* event)
{
    
    Vec2 pt = touch->getLocation();
    if (_ringbottom->getBoundingBox().containsPoint(pt)) {
        _ringbottom->setColor(Color3B::YELLOW);
        _pause->setVisible(true);
        _layerColor->setVisible(true);
    auto gamemanager = GameManager::getInstance();
        if (!gamemanager->isPause()) {
            gamemanager->pauseGame();
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        }
        
        return true;
    }
    else{
        auto gamemanager = GameManager::getInstance();
        if (gamemanager->isPause()) {
        GameManager::getInstance()->reStartGame();
        _ringbottom->setColor(Color3B::WHITE);
        _pause->setVisible(false);
        _layerColor->setVisible(false);
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        return false;
        }
    }
    
    if(pt.x < 60 ||
       pt.x > Director::getInstance()->getWinSize().width-60)return false;
    return true;
}

void LayerUI::onTouchMoved(Touch* touch,Event* event)
{

    
}

void LayerUI::onTouchEnded(Touch* touch,Event* event)
{
    Vec2 pt = touch->getLocation();
    if (_ringbottom->getBoundingBox().containsPoint(pt)) {
        auto gamemanager = GameManager::getInstance();
        if (!gamemanager->isPause()) {
            gamemanager->pauseGame();
        }
  
        return ;
    }
    
}