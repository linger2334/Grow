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

GearGate::GearGate():_left(nullptr),_leftEye(nullptr),_right(nullptr),_rightEye(nullptr)
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
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        _left = Sprite::create("GearGate_Subject_L.png");
        _left->setCascadeOpacityEnabled(true);
        _leftEye = Sprite::create("GearGate_RedEye_L.png");
        _leftEye->setPosition(77.0/121.0*_left->getBoundingBox().size.width,91.0/198.0*_left->getBoundingBox().size.height);
        _leftEye->setCascadeOpacityEnabled(true);
        _left->addChild(_leftEye);
        
        _right = Sprite::create("GearGate_Subject_L.png");
        _right->setCascadeOpacityEnabled(true);
        _rightEye = Sprite::create("GearGate_RedEye_L.png");
        _rightEye->setPosition(77.0/121.0*_right->getBoundingBox().size.width,91.0/198.0*_right->getBoundingBox().size.height);
        _rightEye->setCascadeOpacityEnabled(true);
        _right->addChild(_rightEye);
        _right->setRotation3D(Vec3(0, 180, 0));

        
        _left->setPosition(getPositionX()-_left->getContentSize().width/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),getPositionY()+_left->getContentSize().width/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())));
        _left->setRotation(getRotation());
        
        _right->setPosition(getPositionX()+_right->getContentSize().width/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),getPositionY()-_right->getContentSize().width/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())));
        _right->setRotation(getRotation());
        
        gap = kDefaultGearGateGap;
        startRate = kDefaultGearGateStartRate;
        if (item.features) {
            gap = ((Features_GearGate*)item.features)->gap;
            startRate = ((Features_GearGate*)item.features)->startRate;
        }
        
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
    
    //
    getParent()->addChild(_left,getLocalZOrder());
    getParent()->addChild(_right,getLocalZOrder());
}

void GearGate::openGate()
{
    GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
    _body = nullptr;
    
    //
    FadeOut* elapse = FadeOut::create(0.2);
    CallFunc* changeTexture_L = CallFunc::create([&](){
        _leftEye->setTexture("GearGate_BlueEye_L.png");
    });
    CallFunc* changeTexture_R = CallFunc::create([&](){
        _rightEye->setTexture("GearGate_BlueEye_L.png");
    });
    FadeIn* show = FadeIn::create(0.2);
    TargetedAction* turnColor_L = TargetedAction::create(_leftEye,Sequence::create(elapse,changeTexture_L,show, NULL));
    TargetedAction* turnColor_R = TargetedAction::create(_rightEye, Sequence::create(elapse->clone(),changeTexture_R,show->clone(), NULL));
    
    TargetedAction* openLeftGate = TargetedAction::create(_left, MoveBy::create(gap/2/startRate, Vec2(-gap/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),gap/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())))));
    TargetedAction* openRightGate = TargetedAction::create(_right,MoveBy::create(gap/2/startRate, Vec2(gap/2*cosf(CC_DEGREES_TO_RADIANS(getRotation())),-gap/2*sinf(CC_DEGREES_TO_RADIANS(getRotation())))));
    
    FiniteTimeAction* group1 = Spawn::create(turnColor_L,turnColor_R, NULL);
    FiniteTimeAction* group2 = Spawn::create(openLeftGate, openRightGate,NULL);
    CallFunc* enableUpdate = CallFunc::create([&](){
        this->scheduleUpdate();
    });
    
    runAction(Sequence::create(group1,group2, enableUpdate,NULL));
}
