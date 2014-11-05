//
//  ItemModel.h
//  Grow
//
//  Created by 林纲 王 on 14-10-4.
//
//

#ifndef __Grow__ItemModel__
#define __Grow__ItemModel__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Item.h"
#include "Macro.h"
#include "TypeBase.h"

class ItemModel :public TypeBase, public Sprite
{
public:
    ItemModel();
    virtual ~ItemModel();
    
    static ItemModel* create(Item& item);
    virtual bool init(Item& item);
    
    inline void setAnimatedOn(bool isanimated) { isAnimated = isanimated;}
    inline void setTriggerTime(float triggertime) { triggerTime = triggertime;}
    virtual void createAnimates(std::vector<std::vector<AnimationInfo>>& animationInfos,std::map<std::string,bool>& animtaionControlInstructions);
    virtual void createCardinalSplineAnimates(std::vector<AnimationInfo>& animationInfos);
    virtual void createCustomCurveAnimates(std::vector<AnimationInfo>& animationInfos);
    virtual void createBody(){};
    inline b2Body* getBody() { return _body;}
    std::function<void(ItemModel*)> _collisionCallBack;
    virtual void collisionWithPlant(ItemModel* plantHead) { };
    
    virtual void update(float dt);
protected:
    friend class LayerItem;
    b2Body* _body;
    bool isAnimated;
    float triggerTime;
    int animationGroupCount;
    Vector<Action*> _allCyclicAnimations;
    ActionInterval* _animatesParallel;
    ActionInterval* _animatesCardinalSpline;
    ActionInterval* _animatesCustomCurve;
};



#endif /* defined(__Grow__ItemModel__) */
