//
//  Cicada.h
//  Grow
//
//  Created by 林纲 王 on 14-10-8.
//
//

#ifndef __Grow__Cicada__
#define __Grow__Cicada__

#include <iostream>
#include "Box2d/Box2d.h"
#include "Macro.h"
#include "ItemModel.h"

enum class Cicada_State{
    Motionless,
    Moving
};

class Cicada : public ItemModel,public ReversalProtocol
{
public:
    Cicada();
    ~Cicada();
    
    static Cicada* create(Item& item);
    bool init(Item& item);
    
    void createBody();
    void wingStir();
    void wingFanning();
    void onStateChangeToMotionless();
    void onStateChangeToMoving();
    void switchItemStatus();
    void update(float dt);
protected:
    Sprite* _belly;
    Sprite* _wing;
    Vec2 prePosition;
    CC_SYNTHESIZE(float, fanningSpeed, FanningSpeed)
    CC_SYNTHESIZE(bool, autoTurnHead, AutoTurnHeadState)
    CC_SYNTHESIZE(bool, autoFanning, AutoFannningState)
    CC_SYNTHESIZE(Cicada_State, state, CicadaState)
    
};

#endif /* defined(__Grow__Cicada__) */
