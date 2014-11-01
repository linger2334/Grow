//
//  Cicada.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#include "Cicada.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"

Cicada::Cicada()
{
    
}

Cicada::~Cicada()
{

}

class Cicada* Cicada::create(Item& item)
{
    Cicada *cicada = new Cicada();
    if (cicada && cicada->init(item))
    {
        cicada->autorelease();
        return cicada;
    }
    CC_SAFE_DELETE(cicada);
    return nullptr;
}

bool Cicada::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {
        
        setContentSize(cocos2d::Size(82.96,132.06));
        float ciwidth = getBoundingBox().size.width;
        float ciheight = getBoundingBox().size.height;
        
        _head = Sprite::create("Cicada_Head.png");
        _head->setPosition(ciwidth/2,0.678*ciheight);
        _belly = Sprite::create("Cicada_Belly.png");
        _belly->setPosition(ciwidth/2,23/132.06*ciheight);
        
        _leftwing = Sprite::create("Cicada_Leftwing.png");
        _rightwing = Sprite::create("Cicada_Rightwing.png");
        _leftwing->setAnchorPoint(Vec2(0.5,1));
        _leftwing->setPosition(21/82.96*ciwidth,0.833*ciheight);
        _rightwing->setAnchorPoint(Vec2(0.5,1));
        _rightwing->setPosition(61.96/82.96*ciwidth,0.833*ciheight);
        
        addChild(_head);
        addChild(_belly);
        addChild(_leftwing);
        addChild(_rightwing);
        
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        w = kDefaultCicadaW;
        includedAngle = kDefaultCicadaIncludedAngle;
        fanningDuration = kDefaultCicadaFanningDuration;
        interval = kDefaultCicadaInterval;
        bellyTransparency = kDefaultCicadaBellyTransparency;
        if(item.features){
            Features_Cicada* features = (Features_Cicada*)item.features;
            w = features->w;
            includedAngle = features->includedAngle;
            fanningDuration = features->fanningDuration;
            interval = features->interval;
            bellyTransparency = features->bellyTransparency;
        }
        
        ActionInterval* shine = FadeTo::create(kDefaultFlickeringHalfCycle ,bellyTransparency*255);
        ActionInterval* shine_reverse = FadeTo::create(kDefaultFlickeringHalfCycle, 255);
        _belly->runAction(RepeatForever::create(Sequence::createWithTwoActions(shine, shine_reverse)));
        
//        setTexture("Cicada.png");
        result = true;
    }else{
        result = false;
    }
    
    _collisionCallBack = std::bind(&Cicada::collisionWithPlant, this,std::placeholders::_1);
    return result;
}

void Cicada::createBody()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()-> convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body= world->CreateBody(&bodyDef);
    
    ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "Cicada");
    
    for (b2Fixture* fixture = _body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::Type::e_circle) {
            b2CircleShape* circleShape = (b2CircleShape*)shape;
            circleShape->m_radius *= getScale();
        }else{
            b2PolygonShape* polygonShape = (b2PolygonShape*)shape;
            int count = polygonShape->GetVertexCount();
            for (int i = 0; i<count; i++) {
                polygonShape->m_vertices[i] *= getScale();
            }
        }
    }
    
    b2MassData bodymassData;
    _body->GetMassData(&bodymassData);
    bodymassData.mass *= getScale();
    bodymassData.I *= getScale();
    _body->SetMassData(&bodymassData);
    //
    scheduleUpdate();
}

void Cicada::collisionWithPlant(ItemModel* plantHead)
{
    GameManager::getInstance()->getBox2dWorld()->DestroyBody(_body);
    _body = nullptr;
    ((LayerItem*)getParent())->getItems().remove(this);
    //
    stopAllActions();
    ActionInterval* disappear = FadeTo::create(1, 0);
    CallFunc* remove = CallFunc::create([&](){
        this->removeFromParent();
    });
    runAction(Sequence::create(disappear,remove, NULL));
    
    /////other effect
    GameManager::getInstance()->getLayerPlant()->doReGrow(80, 60);
}

