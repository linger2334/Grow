#ifndef __Grow_Beta_01__LightListHelper__
#define __Grow_Beta_01__LightListHelper__
#include "common.h"
#include "Plant.h"
#include "TuoYuanRotateNode.h"
struct  LightPathContext
{
    TuoYuanRotateNode* _target;
    float _nowLengthByTop;
    float _toLengthByTop;
    float _speed;
};
class LightListHelper
{
public:
    void  updateLightPosition(float dt);
    void  checkLightMoveSpeed(float dt);
    std::list<LightPathContext> _listList;
    PlantNode* node;
};

#endif /* defined(__Grow_Beta_01__LightListHelper__) */
