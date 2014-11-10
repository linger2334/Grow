//
//  GearButton.cpp
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#include "GearButton.h"
#include "GameManager.h"
#include "LayerItem.h"
#include "GearGate.h"

GearButton::GearButton():_subject(nullptr),_lamp(nullptr),_sink(nullptr)
{
    
}

GearButton::~GearButton()
{
    
}

GearButton* GearButton::create(Item &item)
{
    GearButton* button = new GearButton();
    if (button&&button->init(item)) {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool GearButton::init(Item &item)
{
    if (ItemModel::init(item)) {
        setTexture("GearButton_Pedestal.png");
        
        _subject = Sprite::create("GearButton_Subject.png");
        _subject->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2 + kDefaultGearButtonSubjectHeight);
        addChild(_subject);
        
        _lamp = Sprite::create("GearButton_BlueLamp.png");
        _lamp->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        _subject->addChild(_lamp);
        //
        setPositionY(getPositionY()-kDefaultGearButtonSubjectHeight/2);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        bindID = kDefaultGearButtonBindID;
        sinkSpeed = kDefaultGearButtonSinkSpeed;
        if(item.features){
            bindID = ((Features_GearButton*)item.features)->bindID;
            sinkSpeed = ((Features_GearButton*)item.features)->sinkSpeed;
        }
        createCollisionAnimation();
        
        _collisionCallBack = std::bind(&GearButton::collisionWithPlant,this,std::placeholders::_1);
        return true;
    }

    return false;
}

void GearButton::createCollisionAnimation()
{
    ActionInterval* squint = FadeTo::create(kDefaultGearButtonSubjectHeight/sinkSpeed,0);
    TargetedAction* lampSquint = TargetedAction::create(_lamp, squint);
    
    ActionInterval* sink = MoveBy::create(kDefaultGearButtonSubjectHeight/sinkSpeed, Vec2(-kDefaultGearButtonSubjectHeight*sinf(CC_DEGREES_TO_RADIANS(getRotation())),-kDefaultGearButtonSubjectHeight*cosf(CC_DEGREES_TO_RADIANS(getRotation()))));
    TargetedAction* subjectSink = TargetedAction::create(_subject, sink);
    
//    ActionInterval* disappear = FadeTo::create(0.5, 0);
//    TargetedAction* lampDisappear = TargetedAction::create(_lamp, disappear->clone());
//    TargetedAction* subjectDisappear = TargetedAction::create(_subject, disappear->clone());
    
    _sink = Spawn::create(lampSquint,subjectSink, NULL);
//    FiniteTimeAction* group2 = Spawn::create(disappear,lampDisappear,subjectDisappear, NULL);
//    CallFunc* remove = CallFunc::create([&](){
//        this->removeFromParent();
//    });
    
    CC_SAFE_RETAIN(_sink);
}

void GearButton::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(getParent()->convertToWorldSpace(getPosition()).x, getParent()->convertToWorldSpace(getPosition()).y);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.userData = this;
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    
    ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "GearButton");
    
}

void GearButton::collisionWithPlant(ItemModel *plantHead)
{
    stopAllActions();
    GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
    _body = nullptr;
    ((LayerItem*)getParent())->getItems().remove(this);
    
    CallFunc* openBindGate = CallFunc::create([&](){
        if(bindID != kDefaultGearButtonBindID){
            GearGate* bindGate = static_cast<GearGate*>(getParent()->getChildByTag(bindID));
            bindGate->openGate();
        }
        CC_SAFE_RELEASE(_sink);
    });
    runAction(Sequence::create(_sink,openBindGate, NULL));
    scheduleUpdate();
    //other effect
    
}

void GearButton::update(float dt)
{
    Vec2 positionInWorld = getParent()->convertToWorldSpace(getPosition());
    if(positionInWorld.y<-getBoundingBox().size.height/2){
        removeFromParent();
    }
    
}



