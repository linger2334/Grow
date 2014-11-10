//
//  Decoration.cpp
//  GrowUp
//
//  Created by wlg on 11/7/14.
//
//

#include "Decoration.h"

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
        switch (_type) {
            case Decoration_Bridge:
                setTexture(IMAGE_DECORATION_BRIDGE);
                break;
            case Decoration_Pendant:
                setTexture(IMAGE_DECORATION_PENDANT);
                break;
            default:
                break;
        }
        
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        
        
        scheduleUpdate();
        return true;
    }
    
    return false;
}

void Decoration::update(float dt)
{
    if(!isAniPerformed){
        ItemModel::update(dt);
    }
    
    if(this->getParent()->convertToWorldSpace(getPosition()).y < -getBoundingBox().size.height/2){
        this->removeFromParent();
    }
    
}

