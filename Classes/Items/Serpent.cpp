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
#include "GameLayerLight.h"
#include "GameLayerPlant.h"
#define RANGE_RADIUS 200

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
        _hole = Sprite::create("Serpent_Hole_Red.png");
        _hole->setOpacity(0);
        _hole->setCascadeOpacityEnabled(true);
        _hole->setPosition(getBoundingBox().size.width/2, getBoundingBox().size.height/2);
        addChild(_hole);
        
        setStatus(ItemStatus::NormalStatus);
        setCascadeOpacityEnabled(true);
        setScale(item.scale);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        
        absorptionRate = kDefaultSerpentAbsorptionRate;
        holeTransparency = kDefaultSerpentHoleTransparency;
        if (item.features) {
            Features_Serpent* features = (Features_Serpent*)item.features;
            absorptionRate = features->absorptionRate;
            holeTransparency = features->holeTransparency;
        }
        
        /////
        _rangeNode = DrawNode::create();
        //_rangeNode->drawDot(Vec2(0,0), 200, Color4F(1.0f,0.0f,0.0f,0.2f));
        addChild(_rangeNode);
        Vec2 points[36];
        Vec2 center = Vec2::ZERO;
        Vec2 pt = Vec2(-RANGE_RADIUS,0);
        for (int i = 0 ; i < 36; i++) {
            points[i] = MathHelper::getRotatePosition(center, pt, i*10);
        }
        _rangeNode->drawPolygon(points, 36, Color4F(0.0f,0.0f,0.0f,0.0f), 1, Color4F(1.0f,0.0f,0.0f,0.2f));
        auto csize = getBoundingBox().size;
        auto apt = Vec2(0.5,0.5);
        Vec2 point = Vec2(csize.width*apt.x,csize.height*apt.y);
        _rangeNode->setPosition(point);
        _rangeNode->setVisible(false);
        
        result = true;
    }
    
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

void Serpent::switchItemStatus()
{
    std::string filename;
    if (status==ItemStatus::NormalStatus) {
        status = ItemStatus::ReversalStatus;
        filename = "Serpent_Hole_Blue.png";
        //吸光切换为放光
        
    }else{
        status = ItemStatus::NormalStatus;
        filename = "Serpent_Hole_Red.png";
        //放光切换为吸光
        
    }
    
    _hole->stopAllActions();
    ActionInterval* fadeaway = FadeOut::create(kDefaultSwitchStatusInterval);
    CallFunc* changeTexture = CallFunc::create([=](){
        _hole->setTexture(filename);
    });
    ActionInterval* fadeIn = FadeIn::create(kDefaultSwitchStatusInterval);
    Sequence* turnColor = Sequence::create(fadeaway,changeTexture,fadeIn, NULL);
    _hole->runAction(turnColor);
    
}

void Serpent::runMoveAction(float dt)
{
    ItemModel::runMoveAction(dt);
    ActionInterval* flickering = FadeTo::create(kDefaultFlickeringHalfCycle, 255);
    ActionInterval* flickering_reverse = FadeTo::create(kDefaultFlickeringHalfCycle, holeTransparency);
    ActionInterval* flicker = RepeatForever::create(Sequence::create(flickering,flickering_reverse, NULL));
    _hole->runAction(flicker);
}

Spawn* createRemoveAction();
void Serpent::update(float dt)
{
    ItemModel::update(dt);
    bool isNearSerpent = false;
    
    
    Vec2 vPlant =  GameLayerPlant::getRunningLayer()->getPlantNodeByIndex(0)->getHeadPositionInWorld();
    if (UIHelper::getWorldPosition(this).distance(vPlant) < RANGE_RADIUS ) {
        isNearSerpent = true;
    }
    
    if(isNearSerpent)
    {
        _rangeNode->setVisible(true);
        _waitRemoveTime -= dt;
        if(_waitRemoveTime <= 0.0f)
        {
            if (GameLayerLight::getRunningLayer()->getLightCountByPlantIndex(0) > 0) {
                
                if (status == ItemStatus::NormalStatus) {
                    GameLayerLight::getRunningLayer()->removeOneLightUseBezier(0, UIHelper::getWorldPosition(this));
                }
                else
                {
                    GameLayerLight::getRunningLayer()->addOneLightByPlantIndexUseBezier(UIHelper::getWorldPosition(this), 0);
                }
                _lightCount++;
            }
            _waitRemoveTime = 0.8;
            
        }
        
    }
    else
    {
        _rangeNode->setVisible(false);
    }
    if(_lightCount >= 8)
    {
        unscheduleUpdate();
        GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
        _body = nullptr;
        this->runAction(Sequence::create(DelayTime::create(1.5f),createRemoveAction(),RemoveSelf::create(), NULL));
        _rangeNode->setVisible(false);
    }
}

