#include "LightListHelper.h"

void  LightListHelper::updateLightPosition(float dt)
{
    for (auto& i: _listList) {
        if (static_cast<int>(i._nowLengthByTop) == static_cast<int>(i._toLengthByTop)) {
            i._speed = 0;
            continue;
        }
        else i._speed += dt*10; 
    }
}
void  LightListHelper::checkLightMoveSpeed(float dt)
{
    for (auto& i: _listList) {
        if (i._nowLengthByTop == i._toLengthByTop) {
            i._speed = 0;
        }
        else
        {
            i._speed += dt*10;
        }
    }
    
}