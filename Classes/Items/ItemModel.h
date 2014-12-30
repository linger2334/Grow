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
#include "GameRunningManager.h"

enum class ItemStatus{
    ReversalStatus = -1,
    NormalStatus = 1
    
};

class ReversalProtocol
{
public:
    virtual ~ReversalProtocol() {}
    virtual void switchItemStatus()=0;
    
    CC_SYNTHESIZE(ItemStatus, status, Status)
};

class ItemModel :public TypeBase, public Sprite
{
public:
    ItemModel();
    virtual ~ItemModel();
    
    static ItemModel* create(Item& item);
    virtual bool init(Item& item);
    
    inline void setAnimatedOn(bool isanimated) { isAnimated = isanimated;}
    virtual void createAnimates(std::vector<std::vector<AnimationInfo>>& animationInfos,std::map<std::string,bool>& animtaionControlInstructions);
    virtual void createCardinalSplineAnimates(std::vector<AnimationInfo>& animationInfos);
    virtual void createCustomCurveAnimates(std::vector<AnimationInfo>& animationInfos);
    virtual void createBody(){};
    inline b2Body* getBody() { return _body;}
    inline void setBody(b2Body* body) { _body = body;}
    inline bool isSpecialItem(){
        return (_type==Flame_Blue || _type==Flame_Orange || _type==Flame_Violet || _type==Flame_White || _type==Gear_Button || _type==Gear_Reversal || _type==Decoration_Pendant);
    }
    inline bool canSwitchStatus(){
        return (_type==Cicada || _type==Barrier_ || _type== Serpent_);
    }
    std::function<void(ItemModel*)> _collisionCallBack;
    CC_SYNTHESIZE(float, triggerTime, TriggerTime)
    CC_SYNTHESIZE(float, elapsedTime, ElapsedTime)
    CC_SYNTHESIZE(int,bindedTriggerID,BindedTriggerID)
    CC_SYNTHESIZE(bool, isTriggerSwitchOn, TriggerSwitchState)
    CC_SYNTHESIZE(bool, isAutoSmoothing, AutoSmoothingState)
    
    virtual void runMoveAction(float dt);
    virtual void update(float dt);
    /////add by wzf
    virtual Vec2 getRemovePositionInWorld()
    {
        auto parent = this->getParent();
        Vec2  pt = this->getPosition();
        if(parent)pt = parent->convertToWorldSpace(pt);
        return pt;
    }
    
protected:
    friend class LayerItem;
    b2Body* _body;
    bool isAnimated;
    bool isAniPerformed;
    int animationGroupCount;
    Vector<Action*> _allCyclicAnimations;
    ActionInterval* _animatesParallel;
    ActionInterval* _animatesCardinalSpline;
    ActionInterval* _animatesCustomCurve;
};



#endif /* defined(__Grow__ItemModel__) */
