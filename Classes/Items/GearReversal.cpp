//
//  GearReversal.cpp
//  Grow-LevelEdit
//
//  Created by wlg on 12/25/14.
//
//

#include "GearReversal.h"
#include "GameManager.h"
#include "LayerItem.h"

GearReversal::GearReversal()
{
    
}

GearReversal::~GearReversal()
{
    
}

GearReversal* GearReversal::create(Item& item)
{
    GearReversal* ret = new GearReversal();
    if (ret && ret->init(item)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GearReversal::init(Item &item)
{
    if (ItemModel::init(item)) {
        
        setTexture("GearReversal.png");
        
        setCascadeOpacityEnabled(true);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        return true;
    }
    
    return false;
}

void GearReversal::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(convertToWorldSpaceAR(Vec2::ZERO).x, convertToWorldSpaceAR(Vec2::ZERO).y);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.userData = this;
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    
    ((LayerItem*)getParent()->getUserObject())->_fixturesCache->addFixturesToBody(_body, "GearReversal");
    
}