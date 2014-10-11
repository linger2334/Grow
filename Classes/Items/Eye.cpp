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
        setTexture("Eye.png");
        
        result = true;
    }else{
        result = false;
    }
    
    _collisionCallBack = std::bind(&Eye::collisionWithPlant, this);
    return result;
}

void Eye::createBody(std::vector<b2Body*>& bodies)
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()->convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body = world->CreateBody(&bodyDef);
    
    GB2ShapeCache* shapeCache = GB2ShapeCache::getInstance();
    shapeCache->addShapesWithFile("Item_fixtures.plist");
    
    shapeCache->addFixturesToBody(_body, "Eye");
    
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
    
    bodies.push_back(_body);
}

void Eye::collisionWithPlant()
{
    PhysicsHandler* handler = GameManager::getInstance()->getPhysicsHandler();
    handler->getItemBodies().erase(find(handler->getItemBodies().begin(),handler->getItemBodies().end(),_body));
    handler->getWorld()->DestroyBody(_body);
    
    this->removeFromParent();
    /////other effect
    
    
}