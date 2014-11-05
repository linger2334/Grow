//
//  Serpent.cpp
//  GrowUp
//
//  Created by wlg on 10/22/14.
//
//

#include "Serpent.h"
#include "GameManager.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"
#include "LayerPlant.h"

Serpent::Serpent()
{
    
}

Serpent::~Serpent()
{

}

Serpent* Serpent::create(Item &item)
{
    Serpent* serpent = new Serpent();
    if (serpent&&serpent->init(item)) {
        serpent->autorelease();
        return serpent;
    }
    CC_SAFE_DELETE(serpent);
    return nullptr;
}

bool Serpent::init(Item &item)
{
    bool result = false;
    if (ItemModel::init(item)) {
        
        setTexture("Serpent_Trunk.png");
        _hole = Sprite::create("Serpent_Hole.png");
        _hole->setOpacity(0);
        _hole->setPosition(getBoundingBox().size.width/2, getBoundingBox().size.height/2);
        addChild(_hole);
        
        setScale(item.scale);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        
        absorptionRate = kDefaultSerpentAbsorptionRate;
        holeTransparency = kDefaultSerpentHoleTransparency;
        if (item.features) {
            Features_Serpent* features = (Features_Serpent*)item.features;
            absorptionRate = features->absorptionRate;
            holeTransparency = features->holeTransparency;
        }
        
        result = true;
    }
    
    _collisionCallBack = std::bind(&Serpent::collisionWithPlant,this, std::placeholders::_1);
    
    scheduleUpdate();
    return result;
}

void Serpent::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(getPositionX(), getPositionY());
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.userData = this;
    
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "Serpent");
    
    for (b2Fixture* fixture = _body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_circle) {
            shape->m_radius *= getScale();
        }else{
            b2PolygonShape* polygonShape = (b2PolygonShape*)shape;
            int count = polygonShape->GetVertexCount();
            for (int i = 0; i<count; i++) {
                polygonShape->m_vertices[i] *= getScale();
            }
        }
    }
    
    //
    scheduleUpdate();
}

void Serpent::collisionWithPlant(ItemModel *plantHead)
{
    //
    GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
    _body = nullptr;
    ((LayerItem*)getParent())->getItems().remove(this);
    this->removeFromParent();
    
    ((LayerPlant_1*)(GameManager::getInstance()->getLayerPlant()))->doReGrow(80, 60);
}

void Serpent::update(float delta)
{
    float beginningPointY = WinSize.height-getBoundingBox().size.height/2;
    Vec2 pointInGl = getParent()->convertToWorldSpace(getPosition());
    if (pointInGl.y<beginningPointY) {
        DelayTime* delay = DelayTime::create(triggerTime);
        CallFunc* flick = CallFunc::create([&](){
            ActionInterval* flickering = FadeTo::create(kDefaultFlickeringHalfCycle, 255);
            ActionInterval* flickering_reverse = FadeTo::create(kDefaultFlickeringHalfCycle, holeTransparency);
            ActionInterval* flicker = RepeatForever::create(Sequence::create(flickering,flickering_reverse, NULL));
            _hole->runAction(flicker);
        });

        if (isAnimated&&animationGroupCount!=0) {
            runAction(_animatesParallel);
        }
        runAction(Sequence::create(delay,flick, NULL));

        unscheduleUpdate();
    }
    
    
}