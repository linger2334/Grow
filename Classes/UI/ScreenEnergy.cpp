//
//  ScreenEnergy.cpp
//  Grow-UI
//
//  Created by wlg on 11/17/14.
//
//

#include "ScreenEnergy.h"
#include "GameRunningManager.h"
#include "GameManager.h"
using namespace ui;

bool ScreenEnergy::init()
{
    if (Layer::init()) {
        accumulativeTime_Natural = UserDefault::getInstance()->getIntegerForKey("accumulativeTime_Natural", 0);
        accelerationTime = 0;
        particleDidResume = false;
        //background
        Layout* grayLayer = Layout::create();
        grayLayer->setContentSize(VisibleSize);
        grayLayer->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        grayLayer->setBackGroundColor(Color3B(0,0,0));
        grayLayer->setBackGroundColorOpacity(180);
        grayLayer->setTouchEnabled(true);
        grayLayer->setCascadeOpacityEnabled(true);
        addChild(grayLayer);
        //operation panel
        ImageView* panel = ImageView::create("UI_SpeedUpPanel.png");
        panel->setPosition(Vec2(VisibleSize.width/2,VisibleSize.height/2));
        panel->setTouchEnabled(true);
        panel->setCascadeOpacityEnabled(true);
        addChild(panel);
        
        Size panelSize = panel->getBoundingBox().size;
        //particle
        _particleEffect = ParticleSystemQuad::create();
        _particleEffect->setEmitterMode(cocos2d::ParticleSystem::Mode::GRAVITY);
        _particleEffect->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
        _particleEffect->setSourcePosition(Vec2(panelSize.width/2,panelSize.height/2));//起点
        _particleEffect->setPosVar(Vec2(150,200));
        _particleEffect->cocos2d::ParticleSystem::setTexture(Director::getInstance()->getTextureCache()->addImage("UI_particle_texture.png"));
        _particleEffect->setGravity(Vec2(0,-1));
        _particleEffect->setSpeed(2);//
        _particleEffect->setSpeedVar(0);
        _particleEffect->setTangentialAccel(0);//切向加速度，逆时针方向为正
        _particleEffect->setRadialAccel(0);//径向加速度，向外为正
        _particleEffect->setTotalParticles(200);
        _particleEffect->setDuration(-1);
        _particleEffect->setLife(6*VisibleSize.height/DefiniteSize.height);
        _particleEffect->setLifeVar(0);
        _particleEffect->setAngle(90);//发射角，数学中的角度
        _particleEffect->setAngleVar(60);
        _particleEffect->setStartSize(2);
        _particleEffect->setStartSizeVar(7);
        _particleEffect->setEndSize(20);
        _particleEffect->setEndSizeVar(60);
        _particleEffect->setStartColor(Color4F(1, 1, 1, .7));
        _particleEffect->setStartColorVar(Color4F(0,0,0,.5));
        _particleEffect->setEndColor(Color4F(1,1,1,0));
        _particleEffect->setEndColorVar(Color4F(0,0,0,0));
        _particleEffect->setStartSpin(0);
        _particleEffect->setStartSpinVar(60);
        _particleEffect->setEndSpin(120);
        _particleEffect->setEndSpinVar(240);
        _particleEffect->setEmissionRate(_particleEffect->getTotalParticles()/_particleEffect->getLife());
        BlendFunc blendfunc = {GL_SRC_ALPHA,GL_ONE};
        _particleEffect->setBlendFunc(blendfunc);

        panel->addChild(_particleEffect,-1);
        //label
        Text* speedupLabel = Text::create("speed up", "Arial", 24);
        speedupLabel->setPosition(Vec2(panelSize.width/2,panelSize.height/3.8));
        panel->addChild(speedupLabel);
        
        _waitLabel = Text::create("wait: 60m00s", "Arial", 42);
        _waitLabel->setPosition(Vec2(panelSize.width/2,panelSize.height/2.8));
        panel->addChild(_waitLabel);
        
        Text* notenoughLabel = Text::create("not enough energy", "Arial", 24);
        notenoughLabel->setPosition(Vec2(panelSize.width/2,0.43*panelSize.height));
        panel->addChild(notenoughLabel);
        
        _progressLabel = Text::create("0%","Arial",70);
        _progressLabel->setColor(Color3B(0,0,0));
        _progressLabel->setPosition(Vec2(panelSize.width/2,panelSize.height/1.4));
        panel->addChild(_progressLabel);
        
        //button
        _speedUp = Button::create();
        _speedUp->loadTextures("UI_SpeedUpNormal.png", "UI_SpeedUpPressed.png");
        _speedUp->setPosition(Vec2(panelSize.width/2,panelSize.height/7));
        _speedUp->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            switch (eventType) {
                case Widget::TouchEventType::BEGAN:
                    this->accelerateParticle();
                    break;
                case Widget::TouchEventType::MOVED:
                    if (!_speedUp->isHighlighted()&&!particleDidResume) {
                        this->resumeParticle();
                    }
                    break;
                case Widget::TouchEventType::ENDED:
                    if (!particleDidResume) {
                        this->resumeParticle();
                    }
                    if (progress == 100) {
                        UserDefault::getInstance()->setIntegerForKey("accumulativeTime_Natural", 0);
                        FadeOut* disappear = FadeOut::create(1.5);
                        RemoveSelf* remove = RemoveSelf::create();
                        TargetedAction* particleDisappear = TargetedAction::create(_particleEffect, FadeOut::create(1));
                        TargetedAction* particleRemove = TargetedAction::create(_particleEffect, remove->clone());
                        this->runAction(Spawn::create(Sequence::create(disappear,remove, NULL),Sequence::create(particleDisappear,particleRemove,NULL),NULL));
                    }
                    break;
                case Widget::TouchEventType::CANCELED:
                    if (!particleDidResume) {
                        this->resumeParticle();
                    }
                    if (progress == 100) {
                        UserDefault::getInstance()->setIntegerForKey("accumulativeTime_Natural", 0);
                        FadeOut* disappear = FadeOut::create(1.5);
                        RemoveSelf* remove = RemoveSelf::create();
                        TargetedAction* particleDisappear = TargetedAction::create(_particleEffect, FadeOut::create(1));
                        TargetedAction* particleRemove = TargetedAction::create(_particleEffect, remove->clone());
                        this->runAction(Spawn::create(Sequence::create(disappear,remove, NULL),Sequence::create(particleDisappear,particleRemove,NULL),NULL));
                    }
                    break;
                default:
                    break;
            }
        });
        panel->addChild(_speedUp);
        
        Button* close = Button::create("UI_close.png");
        close->setPosition(Vec2(0.9*panelSize.width,0.95*panelSize.height));
        close->addTouchEventListener([&](Ref* sender,Widget::TouchEventType eventType){
            if (eventType == Widget::TouchEventType::ENDED) {
                if (progress == 100) {
                    UserDefault::getInstance()->setIntegerForKey("accumulativeTime_Natural", 0);
                }else{
                    UserDefault::getInstance()->setIntegerForKey("accumulativeTime_Natural", accumulativeTime_Natural);
                }
                UserDefault::getInstance()->flush();
                FadeOut* disappear = FadeOut::create(1.5);
                RemoveSelf* remove = RemoveSelf::create();
                TargetedAction* particleDisappear = TargetedAction::create(_particleEffect, FadeOut::create(1));
                TargetedAction* particleRemove = TargetedAction::create(_particleEffect, remove->clone());
                this->runAction(Spawn::create(Sequence::create(disappear,remove, NULL),Sequence::create(particleDisappear,particleRemove,NULL),NULL));
            }
        }); 
        panel->addChild(close);
        
        for (int i = 0; i<panel->getChildrenCount(); i++) {
            Node* child = dynamic_cast<Node*>(panel->getChildren().at(i));
            child->setCascadeOpacityEnabled(true);
        }
        setOpacity(0);
        setCascadeOpacityEnabled(true);
        scheduleUpdate();
        return true;
    }
    
    return false;
}

void ScreenEnergy::onEnter()
{
    Layer::onEnter();
    FadeIn* appear = FadeIn::create(1.5);
    runAction(appear);
}

void ScreenEnergy::onExit()
{
    if(progress == 100){
        GameRunningManager::getInstance()->waitAddLightComplete();
    }else{
        GameManager::getInstance()->reStartGame();
        GameLayerUI::getRunningLayer()->noShowWaitLightUI();
    }
    
    Layer::onExit();
}

void ScreenEnergy::accelerateParticle()
{
    _particleEffect->setGravity(Vec2(0,30));
    _particleEffect->setSourcePosition(Vec2(_particleEffect->getSourcePosition().x,_particleEffect->getSourcePosition().y/1.5));
    particleDidResume = false;
}

void ScreenEnergy::resumeParticle()
{
    _particleEffect->setGravity(Vec2(0,-1));
    _particleEffect->setSourcePosition(Vec2(_particleEffect->getSourcePosition().x,_particleEffect->getSourcePosition().y*1.5));
    particleDidResume = true;
}

void ScreenEnergy::update(float dt)
{
    float waittime;
    accumulativeTime_Natural += dt;
    if (_speedUp->isHighlighted()) {
        accelerationTime += dt;
//        waittime = 60 - accelerationTime > 0? 60 - accelerationTime : 0;
//        progress = accelerationTime/60*100.0 < 100.0? accelerationTime/60*100.0 : 100.0;
        waittime = 5.0 -accelerationTime>0? 5.0-accelerationTime : 0;
        progress = accelerationTime/5.0*100 <100.0? accelerationTime/5.0*100 : 100.0;
        
    }else{
        accelerationTime = 0;
        waittime = 60*60 - accumulativeTime_Natural > 0? 60*60 - accumulativeTime_Natural : 0;
        progress = accumulativeTime_Natural/(60*60)*100.0 < 100.0? accumulativeTime_Natural/(60*60)*100.0 : 100.0;
    }
    
    _waitLabel->setString(StringUtils::format("wait: %2dm%2ds",(int)waittime/60,(int)waittime%60));
    _progressLabel->setString(StringUtils::format("%d%%",progress));
    if (progress == 100) {
        unscheduleUpdate();
    }
}

