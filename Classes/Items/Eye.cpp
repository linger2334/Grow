//
//  Eye.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Eye.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"

class Eye* Eye::create(Item& item)
{
    Eye *eye = new Eye();
    if (eye && eye->init(item))
    {
        eye->autorelease();
        return eye;
    }
    CC_SAFE_DELETE(eye);
    return nullptr;
}

bool Eye::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {
        setTexture("Eye");
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

b2Body* Eye::createBody()
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
    
    b2FixtureDef fixtureDefL;
    b2CircleShape circle_left;
    circle_left.m_radius = this->getContentSize().height*0.5/PTM_RATIO;
    circle_left.m_p.Set(-0.7*this->getContentSize().height*0.5/PTM_RATIO, 0);
    fixtureDefL.shape = &circle_left;
    fixtureDefL.filter.categoryBits = 0x01;
    fixtureDefL.filter.maskBits = 0x00;
    body->CreateFixture(&fixtureDefL);
    
    b2FixtureDef fixtureDefR;
    b2CircleShape circle_right;
    circle_right.m_radius = this->getContentSize().height*0.5/PTM_RATIO;
    circle_right.m_p.Set(0.7*this->getContentSize().height*0.5/PTM_RATIO, 0);
    fixtureDefR.shape = &circle_right;
    fixtureDefR.filter.categoryBits = 0x01;
    fixtureDefR.filter.maskBits = 0x00;
    body->CreateFixture(&fixtureDefR);
    
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