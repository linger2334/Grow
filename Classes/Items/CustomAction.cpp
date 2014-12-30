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

CustomCardinalSplineBy* CustomCardinalSplineBy::create(float duration, cocos2d::PointArray *points, std::vector<float>& dutyRatio,float tension)
{
    CustomCardinalSplineBy* ret = new CustomCardinalSplineBy();
    if (ret && ret->initWithDuration(duration, points,dutyRatio, tension)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CustomCardinalSplineBy::initWithDuration(float duration, cocos2d::PointArray *points, std::vector<float>& dutyRatio,float tension)
{
    if (CardinalSplineBy::initWithDuration(duration, points, tension)) {
        _dutyRatio = dutyRatio;
        pointCount = dutyRatio.size()-1;
        return true;
    }
    return false;
}

void CustomCardinalSplineBy::update(float time)
{
    ssize_t p;
    float lt;
    
    if (time == 1)
    {
        p = _points->count() - 1;
        lt = 1;
    }
    else
    {
        for (int i =0; i<pointCount; i++) {
            if (time>=_dutyRatio.at(i) && time<_dutyRatio.at(i+1)) {
                p = i;
                lt = (time-_dutyRatio.at(i))/(_dutyRatio.at(i+1)-_dutyRatio.at(i));
                break;
            }
        }
    }
    
    Vec2 pp0 = _points->getControlPointAtIndex(p-1);
    Vec2 pp1 = _points->getControlPointAtIndex(p+0);
    Vec2 pp2 = _points->getControlPointAtIndex(p+1);
    Vec2 pp3 = _points->getControlPointAtIndex(p+2);
    
    Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, _tension, lt);
    
#if CC_ENABLE_STACKABLE_ACTIONS
    Node *node = _target;
    Vec2 diff = node->getPosition() - _previousPosition;
    if( diff.x !=0 || diff.y != 0 ) {
        _accumulatedDiff = _accumulatedDiff + diff;
        newPos = newPos + _accumulatedDiff;
    }
#endif
    
    this->updatePosition(newPos);
}








