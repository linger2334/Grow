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
        
        float ciwidth = getBoundingBox().size.width;
        float ciheight = getBoundingBox().size.height;
        
        Sprite* head = Sprite::create("Cicada_Head.png");
        head->setPosition(ciwidth/2,0.678*ciheight);
        Sprite* belly = Sprite::create("Cicada_Belly.png");
        belly->setPosition(ciwidth/2,23/132.06*ciheight);
        Sprite* leftwing = Sprite::create("Cicada_Leftwing.png");
        Sprite* rightwing = Sprite::create("Cicada_Right.png");
        leftwing->setAnchorPoint(Vec2(0.5,1));
        leftwing->setPosition(21/82.96*ciwidth,0.833*ciheight);
        rightwing->setAnchorPoint(Vec2(0.5,1));
        rightwing->setPosition(61.96/82.96*ciwidth,0.833*ciheight);
        
        addChild(head);
        addChild(belly);
        addChild(leftwing);
        addChild(rightwing);
        
        ActionInterval* shine = FadeTo::create(1, 50);
        ActionInterval* shine_reverse = FadeTo::create(1, 255);
        belly->runAction(RepeatForever::create(Sequence::createWithTwoActions(shine, shine_reverse)));
        
        ActionInterval* fan_left = RotateBy::create(0.01, 25);
        leftwing->runAction(RepeatForever::create(Sequence::create(fan_left,fan_left->reverse(), NULL)));
        ActionInterval* fan_right = RotateBy::create(0.01, -25);
        rightwing->runAction(RepeatForever::create(Sequence::create(fan_right,fan_right->reverse(), NULL)));
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

b2Body* Cicada::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getPositionX()/PTM_RATIO,getPositionY()/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    b2Body* body = GameManager::getInstance()->_sceneGame->world->CreateBody(&bodyDef);
    
    GB2ShapeCache* shapeCache = GB2ShapeCache::getInstance();
    shapeCache->addShapesWithFile("Item_fixtures.plist");
    shapeCache->addFixturesToBody(body,"Rock_Blue");
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
    
    return body;
}