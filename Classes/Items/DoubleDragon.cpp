//
//  DoubleDragon.cpp
//  GrowUp
//
//  Created by wlg on 10/22/14.
//
//

#include "DoubleDragon.h"
#include "GameManager.h"
#include "GB2ShapeCache-x.h"
#include "LayerItem.h"

DoubleDragon::DoubleDragon()
{
    
}

DoubleDragon::~DoubleDragon()
{

}

DoubleDragon* DoubleDragon::create(Item &item)
{
    DoubleDragon* doubledragon = new DoubleDragon();
    if(doubledragon&&doubledragon->init(item)){
        doubledragon->autorelease();
        return doubledragon;
    }
    CC_SAFE_DELETE(doubledragon);
    return nullptr;
}

bool DoubleDragon::init(Item &item)
{
    bool result = false;
    if (ItemModel::init(item)) {
        std::string trunkFilename = "DoubDragonTrunk_Anti.png";
        std::string bellyFilename = "DoubDragonBelly_Anti.png";
        float w = kDefaultDragonW;
        float bellyTransparency = kDefaultDragonBellyTransparency;
        int rotateDirection = 1;
        
        switch (_type) {
            case DoubDragon_Anti:
                rotateDirection = -1;
                break;
            case DoubDragon_Clockwise:
                rotateDirection = 1;
                break;
            default:
                break;
        }
        
        
        if(item.features){
            w = ((Features_DoubleDragon*)item.features)->w;
            bellyTransparency = ((Features_DoubleDragon*)item.features)->bellyTransparency;
        }
        
        setTexture(trunkFilename);
        _belly = Sprite::create(bellyFilename);
        _belly->setPosition(getBoundingBox().size.width/2,getBoundingBox().size.height/2);
        addChild(_belly,-1);
        
        if (_type == DoubDragon_Clockwise) {
            setRotation3D(Vec3(0, 180, 0));
        }
        setScale(item.scale);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
    
        ActionInterval* flickering = FadeTo::create(kDefaultFlickeringHalfCycle, 255*bellyTransparency);
        ActionInterval* flickering_reverse = FadeTo::create(kDefaultFlickeringHalfCycle, 255);
        ActionInterval* sequence = Sequence::create(flickering,flickering_reverse, NULL);
        _belly->runAction(RepeatForever::create(sequence));
        
        if (w>0.0f) {
            ActionInterval* rotation = RotateBy::create(2*180/w, 360*rotateDirection);
            runAction(RepeatForever::create(rotation));
        }
        
        result = true;
    }
    
    return result;
}

void DoubleDragon::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = cc_to_b2Vec(getPositionX(), getPositionY());
    bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
    bodyDef.userData = this;
    
    _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
    switch (_type) {
        case DoubDragon_Anti:
            ((LayerItem*)(getParent()))->_fixturesCache->addFixturesToBody(_body, "DoubDragon_Anti");
            break;
        case DoubDragon_Clockwise:
            ((LayerItem*)(getParent()))->_fixturesCache->addFixturesToBody(_body, "DoubDragon_Clockwise");
            break;
        default:
            break;
    }
    
    
    for(b2Fixture* fixture = _body->GetFixtureList();fixture;fixture = fixture->GetNext()){
        b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_circle) {
            shape->m_radius *= getScale();
        }else{
            b2PolygonShape* polygonShape = (b2PolygonShape*)shape;
            int count = polygonShape->GetVertexCount();
            for (int i = 0; i<count; i++) {
                polygonShape->m_vertices[i] *= getScale();
            }
        }
    }
    //
    scheduleUpdate();
}




