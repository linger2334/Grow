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
    
        return true;
    }
    
    return false;
}
