//
//  Sprouts.cpp
//  Grow-LevelEdit
//
//  Created by wlg on 12/23/14.
//
//

#include "Sprouts.h"
#include "GameManager.h"

Sprouts::Sprouts():_sproutingAnimate(nullptr)
{
    
}

Sprouts::~Sprouts()
{
    CC_SAFE_RELEASE(_sproutingAnimate);
}

Sprouts* Sprouts::create(Item &item)
{
    Sprouts* ret = new Sprouts();
    if (ret && ret->init(item)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Sprouts::init(Item &item)
{
    if (ItemModel::init(item)) {
        
        float growSpeed = kDefaultSproutsGrowSpeed;
        if (item.features) {
            growSpeed = ((Features_Sprouts*)item.features)->growSpeed;
        }
        createSproutingAnimate(growSpeed);
        setRotation(CC_RADIANS_TO_DEGREES(item.angle));
        setScale(item.scale);
        return true;
    }

    return false;
}

void Sprouts::createSproutingAnimate(float growSpeed)
{
    std::string prefix;
    std::string suffix;
    switch (_type) {
        case Sprouts_Dextro:
            prefix = "1-";
            break;
        case Sprouts_Levo:
            prefix = "2-";
            break;
        case Sprouts_Slope:
            prefix = "3-";
            break;
        default:
            break;
    }
    
    Animation* sproutAnimation = Animation::create();
    for (int i = 1; i<23; i++) {
        suffix = StringUtils::format("%d.png",i);
        if (!prefix.compare("1-") && i>20) break;
        sproutAnimation->addSpriteFrameWithFile(prefix + suffix);
    }
    CCASSERT(growSpeed != 0.0, "growspeed is not valid!");
    sproutAnimation->setDelayPerUnit(1.0/12/growSpeed);
    _sproutingAnimate = Animate::create(sproutAnimation);
    CC_SAFE_RETAIN(_sproutingAnimate);
}

void Sprouts::runSproutingAnimate()
{
    runAction(_sproutingAnimate);
}
