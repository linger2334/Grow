//
//  Flame.cpp
//  Grow
//
//  Created by 林纲 王 on 14-10-3.
//
//

#include "Flame.h"
#include "GameManager.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"
#include "GameRunningInfo.h"
#include "StatisticsData.h"

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
            case Flame_Blue:
                setTexture(IMAGE_FLAME_BLUE);
                break;
            case Flame_Orange:
                setTexture(IMAGE_FLAME_ORANGE);
                break;
            case Flame_Violet:
                setTexture(IMAGE_FLAME_VIOLET);
                break;
            case Flame_White:
                setTexture(IMAGE_FLAME_WHITE);
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
    
    GB2ShapeCache* _fixturesCache = ((LayerItem*)getParent()->getUserObject())->_fixturesCache;
    switch (_type) {
        case Flame_Blue:
            _fixturesCache->addFixturesToBody(_body, "Flame_Blue");
            break;
        case Flame_Orange:
            _fixturesCache->addFixturesToBody(_body, "Flame_Orange");
            break;
        case Flame_Violet:
            _fixturesCache->addFixturesToBody(_body, "Flame_Violet");
            break;
        case Flame_White:
            _fixturesCache->addFixturesToBody(_body, "Flame_White");
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

void Flame::updateFlameCollectedCount()
{
    StatisticsData* totaldata = StatisticsData::getRunningLayer();
    UserDefault* userDefault = UserDefault::getInstance();
    int FlameWhiteCount = userDefault->getIntegerForKey("collectedWhite",0);
    int FlameBlueCount = userDefault->getIntegerForKey("collectedBlue", 0);
    int FlameVioltCount = userDefault->getIntegerForKey("collectedViolet",0);
    int FlameOrangeCount = userDefault->getIntegerForKey("collectedOrange",0);

    switch(_type){
        case Flame_White:
            FlameWhiteCount++;
            totaldata->totalFlame_White++;
            userDefault->setIntegerForKey("collectedWhite", FlameWhiteCount);
            break;
        case Flame_Blue:
            FlameBlueCount++;
            totaldata->totalFlame_Blue++;
            userDefault->setIntegerForKey("collectedBlue", FlameBlueCount);
            break;
        case Flame_Violet:
            FlameVioltCount++;
            totaldata->totalFlame_Violet++;
            userDefault->setIntegerForKey("collectedViolet", FlameVioltCount);
            break;
        case Flame_Orange:
            FlameOrangeCount++;
            totaldata->totalFlame_Orange++;
            userDefault->setIntegerForKey("collectedOrange", FlameOrangeCount);
            break;
            
        default:
            break;
    }

}


