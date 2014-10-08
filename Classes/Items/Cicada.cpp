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
        
        ActionInterval* shine = FadeTo::create(1,bellyTransparency*255);
        ActionInterval* shine_reverse = FadeTo::create(1, 255);
        _belly->runAction(RepeatForever::create(Sequence::createWithTwoActions(shine, shine_reverse)));
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

void Cicada::createBody(std::vector<b2Body*>& bodies)
{
    b2World* world = GameManager::getInstance()->_sceneGame->world;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()-> convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    b2Body* body= world->CreateBody(&bodyDef);
    
    GB2ShapeCache* shapeCache = GB2ShapeCache::getInstance();
    shapeCache->addShapesWithFile("Item_fixtures.plist");
    shapeCache->addFixturesToBody(body, "Cicada");
    
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

