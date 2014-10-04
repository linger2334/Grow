//
//  Rock.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Rock.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"

Rock* Rock::create(Item& item)
{
    Rock *rock = new Rock();
    if (rock && rock->init(item))
    {
        rock->autorelease();
        return rock;
    }
    CC_SAFE_DELETE(rock);
    return nullptr;
}

bool Rock::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {
        
        switch (type) {
            case Rock_Circle:
                setTexture(IMAGE_ROCK_CIRCLE);
                break;
            case Rock_Ellipse:
                setTexture(IMAGE_ROCK_ELLIPSE);
                break;
            case Rock_Gray:
                setTexture(IMAGE_ROCK_GRAY);
                break;
            default:
                return false;
        }
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

b2Body* Rock::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getPositionX()/PTM_RATIO,getPositionY()/PTM_RATIO);
    bodyDef.angle = - CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    b2Body* body = GameManager::getInstance()->_sceneGame->world->CreateBody(&bodyDef);
    
    GB2ShapeCache* shapeCache = GB2ShapeCache::getInstance();
    shapeCache->addShapesWithFile("Item_fixtures.plist");
    switch (type) {
        case Rock_Circle:
            shapeCache->addFixturesToBody(body, "Rock_Gray");
            break;
        case Rock_Ellipse:
            shapeCache->addFixturesToBody(body, "Rock_Gray");
            break;
        case Rock_Gray:
            shapeCache->addFixturesToBody(body, "Rock_Gray");
            break;
        default:
            break;
    }
    
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





