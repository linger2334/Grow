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
#include "LayerItem.h"

Dragon::Dragon()
{
    
}

Dragon::~Dragon()
{

}

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
        
        setAnchorPoint(Vec2(145.0/382,196.0/413));
        if(_type == Dragon_Clockwise)
        {
            setAnchorPoint(Vec2(237.0/382,196.0/413));
        }
        std::string bodyFilename;
        std::string backFilename;
        int rotateDirection;
        float w = kDefaultDragonW;
        float backTransparency = kDefaultDragonBackTransparency;
        
        switch (_type) {
            case Dragon_Anti:
            {
                bodyFilename = "DragonTrunk_Anti.png";
                backFilename = "DragonBack_Anti.png";
                rotateDirection = -1;
            }
                break;
            case Dragon_Clockwise:
            {
                bodyFilename = "DragonTrunk_Clockwise.png";
                backFilename = "DragonBack_Clockwise.png";
                rotateDirection = 1;
            }
                break;
            default:
                return false;
        }
        
        if(item.features){
            Features_Dragon* features = (Features_Dragon*)item.features;
            w = features->w;
            backTransparency = features->backTransparency;
        }
        
        setTexture(bodyFilename);
        Sprite* dragonBack = Sprite::create(backFilename);
        dragonBack->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        addChild(dragonBack);
        
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        ActionInterval* flickering = FadeTo::create(1,255*backTransparency);
        ActionInterval* flickering_reverse = FadeTo::create(1, 255);
        ActionInterval* sequence = Sequence::createWithTwoActions(flickering, flickering_reverse);
        dragonBack->runAction(RepeatForever::create(sequence));
        
        result = true;
    }else{
        result = false;
    }
    
    _collisionCallBack = std::bind(&Dragon::collisionWithPlant, this,std::placeholders::_1);
    return result;
}

void Dragon::createBody()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()->convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body = world->CreateBody(&bodyDef);
    
    switch (_type) {
        case Dragon_Anti:
            ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "Dragon_Anti");
            break;
        case Dragon_Clockwise:
            ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, "Dragon_Clockwise");
            break;
        default:
            break;
    }
    
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

void Dragon::collisionWithPlant(ItemModel* plantHead)
{
    ((LayerItem*)getParent())->getItems().remove(this);
    this->removeFromParent();
    /////////other effect 
    
}