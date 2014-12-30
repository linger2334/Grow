//
//  Decoration.cpp
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#include "Decoration.h"
#include "GameManager.h"
#include "LayerItem.h"

Decoration* Decoration::create(Item& item)
{
    Decoration* decoration = new Decoration();
    if (decoration&&decoration->init(item)) {
        decoration->autorelease();
        return decoration;
    }
    
    CC_SAFE_DELETE(decoration);
    return nullptr;
}

bool Decoration::init(Item& item)
{
    if (ItemModel::init(item)) {
        std::string fileName;
        switch (_type) {
            case Decoration_Bridge:
                fileName = IMAGE_DECORATION_BRIDGE;
                break;
            case Decoration_Flower:
                fileName = IMAGE_DECORATION_FLOWER;
                if (item.features) {
                    flowerID = ((Features_DecorationFlower*)item.features)->flowerID;
                }
                break;
            case Decoration_FlowerInv:
                fileName = IMAGE_DECORATION_FLOWER;
                if (item.features) {
                    flowerID = ((Features_DecorationFlower*)item.features)->flowerID;
                }
                setFlippedX(true);
                break;
            case Decoration_Pendant:
                fileName = IMAGE_DECORATION_PENDANT;
                break;
            default:
                break;
        }
        
        setTexture(fileName);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
    
        return true;
    }
    
    return false;
}

void Decoration::createBody()
{
    if (_type == Decoration_Flower || _type == Decoration_FlowerInv) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = cc_to_b2Vec(convertToWorldSpaceAR(Vec2::ZERO).x, convertToWorldSpaceAR(Vec2::ZERO).y);
        bodyDef.angle = -CC_DEGREES_TO_RADIANS(getRotation());
        bodyDef.userData = this;
        _body = GameManager::getInstance()->getBox2dWorld()->CreateBody(&bodyDef);
        
        std::string fixtureName;
        switch (_type) {
            case Decoration_Flower:
                fixtureName = "Decoration_Flower";
                break;
            case Decoration_FlowerInv:
                fixtureName = "Decoration_FlowerInv";
                break;
            default:
                fixtureName = "Decoration_Flower";
                break;
        }
        
        ((LayerItem*)getParent())->_fixturesCache->addFixturesToBody(_body, fixtureName);
    }
}

