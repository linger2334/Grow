//
//  CustomAction.cpp
//  GrowUp
//
//  Created by wlg on 10/29/14.
//
//

#include "CustomAction.h"

OvalBy::OvalBy()
{
    
}

OvalBy::~OvalBy()
{
    
}

OvalBy* OvalBy::create(float duration,const OvalConfig& ovalconfig)
{
    OvalBy *pOval = new OvalBy();
    if (pOval && pOval->initWithDuration(duration,ovalconfig))
    {
        pOval->autorelease();
        return pOval;
    }
    CC_SAFE_DELETE(pOval);
    return nullptr;
}

bool OvalBy::initWithDuration(float duration,const OvalConfig& ovalconfig)
{
    if (ActionInterval::initWithDuration(duration)) {
        CCASSERT(ovalconfig.a>0&&ovalconfig.b>0,"a or b is invalid!");
        ovalConfig = ovalconfig;
        isHorizontal = ovalConfig.a > ovalConfig.b;
        return true;
    }
    
    return false;
}

void OvalBy::startWithTarget(Node* target)
{
    ActionInterval::startWithTarget(target);
    startPosition = _target->getPosition();
    center = startPosition + Vec2(ovalConfig.a,0);
    prePosition = startPosition;
}

void OvalBy::update(float time)
{
    if (_target) {
        float phase,x,y,dy,dx,rotation;
        
        if (isHorizontal) {
            phase = M_PI - 2*M_PI*time;
            x = ovalConfig.a*cosf(phase) + center.x;
            y = ovalConfig.b*sinf(phase) + center.y;
        }else{
            phase = -M_PI/2 + 2*M_PI*time;
            x = ovalConfig.a*sinf(phase) + center.x;
            y = ovalConfig.b*cosf(phase) + center.y;
        }
        
        dy = y - prePosition.y;
        dx = x - prePosition.x;
        if (time<0.01) {
            rotation = _target->getRotation();
        }else{
            rotation = CC_RADIANS_TO_DEGREES(M_PI/2 - atan2f(dy,dx));
        }
        
        _target->setPosition(x,y);
        _target->setRotation(rotation);
        //
        prePosition = Vec2(x,y);
    }
}








