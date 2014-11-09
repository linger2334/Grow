//
//  GearGate.cpp
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#include "GearGate.h"
#include "GameManager.h"
#include "LayerItem.h"

GearGate::GearGate()
{
    
}

GearGate::~GearGate()
{
    
}

GearGate* GearGate::create(Item& item)
{
    GearGate* gate = new GearGate();
    if (gate&&gate->init(item)) {
        gate->autorelease();
        return gate;
    }
    
    CC_SAFE_DELETE(gate);
    return nullptr;
}

bool GearGate::init(Item &item)
{
    if (ItemModel::init(item)) {
        _left = Sprite::create("GearGate_Subject_L.png");
        _leftEye = Sprite::create("GearGate_RedEye_L.png");
        _leftEye->setPosition(77.0/121.0*_left->getBoundingBox().size.width,_left->getBoundingBox().size.height/2);
        _left->addChild(_leftEye);
        
        _right = Sprite::create("GearGate_Subject_R.png");
        _rightEye = Sprite::create("GearGate_RedEye_R.png");
        _rightEye->setPosition(44.0/121.0*_right->getBoundingBox().size.width,_right->getBoundingBox().size.height/2);
        _right->addChild(_rightEye);
        
        setContentSize(_left->getContentSize()+_right->getContentSize());
        _left->setPosition(1.0/4.0*getContentSize().width,0.5*getContentSize().height);
        _right->setPosition(3.0/4.0*getContentSize().width,0.5*getContentSize().height);
        addChild(_left);
        addChild(_right);
        
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        gap = kDefaultGearGateGap;
        startRate = kDefaultGearGateStartRate;
        if (item.features) {
            gap = ((Features_GearGate*)item.features)->gap;
            startRate = ((Features_GearGate*)item.features)->startRate;
        }
        
        _collisionCallBack = std::bind(&GearGate::collisionWithPlant,this,std::placeholders::_1);
        return true;
    }
    
    return false;
}

void GearGate::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(getParent()->convertToWorldSpace(getPosition()).x, getParent()->convertToWorldSpace(getPosition()).y);
    bodyDef.angle = -CC_RADIANS_TO_DEGREES(getRotation());
    bodyDef.userData = this;
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    
    ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "GearGate");
}

void GearGate::openGate()
{
    FadeOut* elapse = FadeOut::create(0.2);
    CallFunc* changeTexture_L = CallFunc::create([&](){
        _leftEye->setTexture("GearGate_BlueEye_L.png");
    });
    CallFunc* changeTexture_R = CallFunc::create([&](){
        _rightEye->setTexture("GearGate_BlueEye_R.png");
    });
    FadeIn* show = FadeIn::create(0.2);
    TargetedAction* turnColor_L = TargetedAction::create(_leftEye,Sequence::create(elapse,changeTexture_L,show, NULL));
    TargetedAction* turnColor_R = TargetedAction::create(_rightEye, Sequence::create(elapse->clone(),changeTexture_R,show->clone(), NULL));
    
    TargetedAction* openLeftGate = TargetedAction::create(_left, MoveBy::create(gap/2/startRate, Vec2(gap/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),-gap/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())))));
    TargetedAction* openRightGate = TargetedAction::create(_right,MoveBy::create(gap/2/startRate, Vec2(-gap/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),gap/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())))));
    
    FiniteTimeAction* group1 = Spawn::create(turnColor_L,turnColor_R, NULL);
    FiniteTimeAction* group2 = Spawn::create(openLeftGate, openRightGate,NULL);
    
    runAction(Sequence::create(group1,group2, NULL));
}

void GearGate::collisionWithPlant(ItemModel *plantHead)
{
    GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
    _body = nullptr;
    ((LayerItem*)getParent())->getItems().remove(this);
    
    FadeTo* disappear = FadeTo::create(1, 0);
    CallFunc* remove = CallFunc::create([&](){
        this->removeFromParent();
    });
    runAction(Sequence::create(disappear,remove, NULL));
    //other effect
    
}
