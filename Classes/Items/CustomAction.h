//
//  CustomAction.h
//  GrowUp
//
//  Created by wlg on 10/29/14.
//
//

#ifndef __GrowUp__CustomAction__
#define __GrowUp__CustomAction__

#include <iostream>
#include "Macro.h"

struct OvalConfig {
    float a;
    float b;
};

class OvalBy : public ActionInterval
{
public:
    OvalBy();
    ~OvalBy();
    
    static OvalBy* create(float duration,const OvalConfig& ovalconfig);
    bool initWithDuration(float duration,const OvalConfig& ovalconfig);
    
    virtual void startWithTarget(Node *target);
    virtual void update(float time);
    
    virtual ActionInterval* reverse() const { return nullptr;}
	virtual ActionInterval *clone() const { return nullptr;}
protected:
    bool isHorizontal;
    Vec2 center;
    Vec2 startPosition;
    Vec2 prePosition;
    OvalConfig ovalConfig;
    
};

class CustomCardinalSplineBy : public CardinalSplineBy
{
public:
//    static CustomCardinalSplineBy* create(float duration,cocos2d::PointArray* points,float tension);
//    bool initWithDuration(float duration,cocos2d::PointArray* points,float tension);
    
    
    
    
};








#endif /* defined(__GrowUp__CustomAction__) */
