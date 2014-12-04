//
//  Rock.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Rock.h"
#include "GameManager.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"
#include "LevelManager.h"

Rock::Rock()
{
    
}

Rock::~Rock()
{

}

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
    std::string filename;
    if (ItemModel::init(item)) {
        switch (_type) {
            case Rock_Circle:
                filename = IMAGE_ROCK_CIRCLE;
                break;
            case Rock_Crinkle:
                filename = IMAGE_ROCK_CRINKLE;
                break;
            case Rock_Cross:
                filename = IMAGE_ROCK_CROSS;
                break;
            case Rock_Dagger:
                filename = IMAGE_ROCK_DAGGER;
                break;
            case Rock_Ellipse:
                filename = IMAGE_ROCK_ELLIPSE;
                break;
            case Rock_Mount:
                filename = IMAGE_ROCK_MOUNT;
                break;
            case Rock_MountInv:
                filename = IMAGE_ROCK_MOUNT;
                break;
            case Rock_Ovoid:
                filename = IMAGE_ROCK_OVOID;
                break;
            case Rock_Rect:
                filename = IMAGE_ROCK_RECT;
                break;
            case Rock_Trape:
                filename = IMAGE_ROCK_TRAPE;
                break;
            default:
                return false;
        }
//        filename += StringUtils::format("%d.png",LevelManager::getInstance()->_levelId);
        filename += "1.png";
        setTexture(filename);
        if (_type == Rock_MountInv) {
            setRotation3D(Vec3(0, 180, 0));
        }
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        result = true;
    }else{
        result = false;
    }
    
    return result;
}

void Rock::createBody()
{
    b2World* world = GameManager::getInstance()->getBox2dWorld();
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(getParent()->convertToWorldSpace(getPosition()).x/PTM_RATIO,getParent()->convertToWorldSpace(getPosition()).y/PTM_RATIO);
    bodyDef.angle = - CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.linearDamping = 0.3;
    bodyDef.userData = this;
    _body = world->CreateBody(&bodyDef);
    
    GB2ShapeCache* _fixturesCache = ((LayerItem*)getParent())->_fixturesCache;
    switch (_type) {
        case Rock_Circle:
            _fixturesCache->addFixturesToBody(_body, "Rock_Circle");
            break;
        case Rock_Crinkle:
            _fixturesCache->addFixturesToBody(_body, "Rock_Crinkle");
            break;
        case Rock_Cross:
            _fixturesCache->addFixturesToBody(_body, "Rock_Cross");
            break;
        case Rock_Dagger:
            _fixturesCache->addFixturesToBody(_body, "Rock_Dagger");
            break;
        case Rock_Ellipse:
            _fixturesCache->addFixturesToBody(_body, "Rock_Ellipse");
            break;
        case Rock_Mount:
            _fixturesCache->addFixturesToBody(_body, "Rock_Mount");
            break;
        case Rock_MountInv:
            _fixturesCache->addFixturesToBody(_body, "Rock_MountInv");
            break;
        case Rock_Ovoid:
            _fixturesCache->addFixturesToBody(_body, "Rock_Ovoid");
            break;
        case Rock_Rect:
            _fixturesCache->addFixturesToBody(_body, "Rock_Rect");
            break;
        case Rock_Trape:
            _fixturesCache->addFixturesToBody(_body, "Rock_Trape");
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
}





