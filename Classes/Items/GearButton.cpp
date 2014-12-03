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
    CC_SAFE_RELEASE_NULL(_sink);
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
        _subject->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        addChild(_subject,-1);
        
        _lamp = Sprite::create("GearButton_BlueLamp.png");
        _lamp->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        _subject->addChild(_lamp);
        
        //
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        bindID = kDefaultGearButtonBindID;
        sinkSpeed = kDefaultGearButtonSinkSpeed;
        if(item.features){
            bindID = ((Features_GearButton*)item.features)->bindID;
            sinkSpeed = ((Features_GearButton*)item.features)->sinkSpeed;
        }
        createCollisionAnimation();
        
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
    
    ((LayerItem*)getParent()->getUserObject())->_fixturesCache->addFixturesToBody(_body, "GearButton");
    
}





