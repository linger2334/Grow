//
//  Cicada.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Cicada.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"

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
        
        ActionInterval* shine = FadeTo::create(1, 50);
        ActionInterval* shine_reverse = FadeTo::create(1, 255);
        _belly->runAction(RepeatForever::create(Sequence::createWithTwoActions(shine, shine_reverse)));
        
        ActionInterval* fan_left = RotateBy::create(0.01, 25);
        _leftwing->runAction(RepeatForever::create(Sequence::create(fan_left,fan_left->reverse(), NULL)));
        ActionInterval* fan_right = RotateBy::create(0.01, -25);
        _rightwing->runAction(RepeatForever::create(Sequence::create(fan_right,fan_right->reverse(), NULL)));
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

void Cicada::createBody(std::vector<b2Body*>& bodies)
{
    b2World* world = GameManager::getInstance()->_sceneGame->world;
    
    b2BodyDef bodyDef_head;
    bodyDef_head.type = b2_dynamicBody;
    bodyDef_head.position = b2Vec2(getParent()-> convertToWorldSpace(_head->getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(_head->getPosition()).y/PTM_RATIO);
    bodyDef_head.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef_head.linearDamping = 0.3;
    bodyDef_head.userData = _head;
    
    b2BodyDef bodyDef_belly = bodyDef_head;
    bodyDef_belly.position = b2Vec2(getParent()-> convertToWorldSpace(_belly->getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(_belly->getPosition()).y/PTM_RATIO);
    bodyDef_belly.userData = _belly;
    
    b2BodyDef bodyDef_leftwing = bodyDef_head;
    bodyDef_leftwing.position = b2Vec2(getParent()-> convertToWorldSpace(_leftwing->getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(_leftwing->getPosition()).y/PTM_RATIO);
    bodyDef_leftwing.userData = _leftwing;
    
    b2BodyDef bodyDef_rightwing = bodyDef_head;
    bodyDef_rightwing.position = b2Vec2(getParent()-> convertToWorldSpace(_rightwing->getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(_rightwing->getPosition()).y/PTM_RATIO);
    bodyDef_rightwing.userData = _rightwing;
    
    std::vector<b2Body*>component;
    b2Body* body_head = world->CreateBody(&bodyDef_head);
    b2Body* body_belly = world->CreateBody(&bodyDef_belly);
    b2Body* body_leftwing = world->CreateBody(&bodyDef_leftwing);
    b2Body* body_rightwing = world->CreateBody(&bodyDef_rightwing);
    
    component.push_back(body_head);
    component.push_back(body_belly);
    component.push_back(body_leftwing);
    component.push_back(body_rightwing);
    
    GB2ShapeCache* shapeCache = GB2ShapeCache::getInstance();
    shapeCache->addShapesWithFile("Item_fixtures.plist");
    shapeCache->addFixturesToBody(body_head,"Cicada_Head");
    shapeCache->addFixturesToBody(body_belly, "Cicada_Belly");
    shapeCache->addFixturesToBody(body_leftwing, "Cicada_Leftwing");
    shapeCache->addFixturesToBody(body_rightwing, "Cicada_Rightwing");
    
    for (b2Body* body : component) {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
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
        body->GetMassData(&bodymassData);
        bodymassData.mass *= getScale();
        bodymassData.I *= getScale();
        body->SetMassData(&bodymassData);
        
        bodies.push_back(body);
    }
}
