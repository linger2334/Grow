//
//  Dragon.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Dragon.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"

Dragon* Dragon::create(Item& item)
{
    Dragon *dragon = new Dragon();
    if (dragon && dragon->init(item))
    {
        dragon->autorelease();
        return dragon;
    }
    CC_SAFE_DELETE(dragon);
    return nullptr;
}

bool Dragon::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {

        std::string bodyFilename;
        std::string backFilename;
        int rotateDirection;
        float w;
        
        switch (type) {
            case Dragon_Anti:
            {
                bodyFilename = "DragonBody_Anti.png";
                backFilename = "DragonBack_Anti.png";
                rotateDirection = -1;
                w = 360/4.0;
            }
                break;
            case Dragon_Clockwise:
            {
                bodyFilename = "DragonBody_Clockwise.png";
                backFilename = "DragonBack_Clockwise.png";
                rotateDirection = 1;
                w = 360/4.0;
            }
                break;
            default:
                return false;
        }
        
        setTexture(bodyFilename);
        Sprite* dragonBack = Sprite::create(backFilename);
        dragonBack->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        addChild(dragonBack);
        
        ActionInterval* rotation = RotateBy::create(360/w,rotateDirection*360);
        runAction(RepeatForever::create(rotation));
        
        ActionInterval* flickering = FadeTo::create(0.4,50);
        ActionInterval* flickering_reverse = FadeTo::create(0.4, 255);
        ActionInterval* sequence = Sequence::createWithTwoActions(flickering, flickering_reverse);
        dragonBack->runAction(RepeatForever::create(sequence));
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

b2Body* Dragon::createBody()
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
    switch (type) {
        case Dragon_Anti:
            shapeCache->addFixturesToBody(body, "Flame_Blue");
            break;
        case Dragon_Clockwise:
            shapeCache->addFixturesToBody(body, "Flame_Blue");
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