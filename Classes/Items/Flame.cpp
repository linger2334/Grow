//
//  Flame.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Flame.h"
#include "GameManager.h"
#include "SceneGame.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"
#include "LayerLight.h"


Flame::Flame()
{
    
}

Flame::~Flame()
{

}

Flame* Flame::create(Item& item)
{
    Flame *flame = new Flame();
    if (flame && flame->init(item))
    {
        flame->autorelease();
        return flame;
    }
    CC_SAFE_DELETE(flame);
    return nullptr;
}

bool Flame::init(Item& item)
{
    bool result;
    if (ItemModel::init(item)) {
        
        switch (_type) {
            case Flame_Red:
                setTexture(IMAGE_FLAME_RED);
                break;
            case Flame_Green:
                setTexture(IMAGE_FLAME_GREEN);
                break;
            case Flame_Blue:
                setTexture(IMAGE_FLAME_BLUE);
                break;
            case Flame_White:
                setTexture(IMAGE_FLAME_WHITE);
                break;
            case Flame_Orange:
                setTexture(IMAGE_FLAME_ORANGE);
                break;
            default:
                return false;
        }
        
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        result = true;
    }else{
        result = false;
    }
    
    _collisionCallBack = std::bind(&Flame::collisionWithPlant, this,std::placeholders::_1);
    return result;
}

void Flame::createBody()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(getParent()->convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body = world->CreateBody(&bodyDef);
    
    GB2ShapeCache* _fixturesCache = ((LayerItem*)getParent())->_fixturesCache;
    switch (_type) {
        case Flame_Red:
            _fixturesCache->addFixturesToBody(_body, "Flame_Red");
            break;
        case Flame_Green:
            _fixturesCache->addFixturesToBody(_body, "Flame_Green");
            break;
        case Flame_Blue:
            _fixturesCache->addFixturesToBody(_body, "Flame_Blue");
            break;
        case Flame_White:
            _fixturesCache->addFixturesToBody(_body, "Flame_White");
            break;
        case Flame_Orange:
            _fixturesCache->addFixturesToBody(_body, "Flame_Orange");
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
#include "LayerPlant.h"
void Flame::collisionWithPlant(ItemModel* plantHead)
{
   // if((int)getUserData()==100)return;
    PhysicsHandler* handler = GameManager::getInstance()->getPhysicsHandler();
    handler->getWorld()->DestroyBody(_body);
    _body = nullptr;
    ((LayerItem*)getParent())->getItems().remove(this);
    stopAllActions();
   // this->removeFromParent();
    /////other effect
    auto fadeout = FadeTo::create(1, 0);
    auto call = CallFuncN::create([=](Node* node)
                                 {
                                      Vec2 pt = getPosition();
                                     node->removeFromParent();
                                    GameManager::getInstance()->getLayerLight()->addOneLight(pt);
                               
                                 });
    Sequence* seq = Sequence::create(fadeout,call,nullptr);
    this->runAction(seq);
  
}



