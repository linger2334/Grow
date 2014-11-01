
#include "ContorlPoint.h"

Vec2 ContorlPointV2::getTopPositionByLength(float len)const
{
    Vec2 top=_point+Vec2(0,len);
    return getRotatePosition(_point,top,_angle);
}
Vec2  ContorlPointV2::getPositionLeft() const
{
    return getRotatePosition(_point,_radius/2,_angle);
}
Vec2  ContorlPointV2::getPositionRight() const
{
    return getRotatePosition(_point,_radius/2,_angle+180);
}


