
#ifndef __Grow_Beta_01__LightNode__
#define __Grow_Beta_01__LightNode__
#include "common.h"
class LightNode : public Sprite
{
public:
    enum
    {
        ActionRandScale = 100,
        ActionRandFade =101
    };
    enum
    {
        StateNormal,
        StateRandAction,
        StateStop,
    };
    CREATE_FUNC(LightNode);
    virtual bool init();
   // virtual void update(float dt);
    Action* createRandFadeAction();
    void randFadeActionCall();
    void randScaleActionCall();
    Action* createRandScaleAction();
    
    void runRandFadeAction();
    void runRandScaleAction();
    void stopRandFadeAction();
    void stopRandScaleAction();
    void changeLightState(int state);
    
    void stopLightState(int state);
    
public:
    bool _isRandRotate;
    int _state;
};

#endif /* defined(__Grow_Beta_01__LightNode__) */
