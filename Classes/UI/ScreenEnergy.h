//
//  ScreenEnergy.h
//  Grow-UI
//
//  Created by wlg on 11/17/14.
//
//

#ifndef __Grow_UI__ScreenEnergy__
#define __Grow_UI__ScreenEnergy__

#include <iostream>
#include "Macro.h"
#include "ui/CocosGUI.h"

class ScreenEnergy : public Layer
{
public:
    CREATE_FUNC(ScreenEnergy)
    bool init();
    void onEnter();
    void onExit();
    
    void accelerateParticle();
    void resumeParticle();
    inline int getAccumulativeTime() { return accumulativeTime_Natural;}
    void update(float dt);
protected:
    ParticleSystemQuad* _particleEffect;
    ui::Button* _speedUp;
    float accumulativeTime_Natural;
    float accelerationTime;
    int progress;
    bool particleDidResume;
    
    ui::Text* _waitLabel;
    ui::Text* _progressLabel;
    
};

#endif /* defined(__Grow_UI__ScreenEnergy__) */
