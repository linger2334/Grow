
#include "ContorlPoint.h"

Vec2 ContorlPointV2::getPositionTopByLength(float len)const
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
ContorlPointV2  ContorlPointV2::getNextContorlPointV2(float len)
{
    ContorlPointV2 old = *this;
    old._point = getPositionTopByLength(len);
    old._height+=len;
    return old;
}
ContorlPointV2  ContorlPointV2::getNextContorlPointV2ByRotate(float len,float angle)
{
    ContorlPointV2 old = *this;
    old._point = getPositionTopByLength(len);
    old._height+=len;
    return old;
}
ContorlPointV2  ContorlPointV2::getNextContorlPointV2ByRotateLeft(float len)
{
    ContorlPointV2 old = *this;
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateRadius );
    Vec2 po = getPositionLeftByLength( _rotateRadius);
     old._point = getRotatePosition(po, _point,-rotateAngle);
     old._angle -= rotateAngle;
     old._height+=len;

    return old;
}
ContorlPointV2  ContorlPointV2::getNextContorlPointV2ByRotateRight(float len)
{
    ContorlPointV2 old = *this;
    float rotateAngle = MathHelper::getRotateAngle(len, _rotateRadius );
    Vec2 po = getPositionRightByLength( _rotateRadius);
    old._point = getRotatePosition(po, _point,rotateAngle);
    old._angle += rotateAngle;
    old._height += len;
    return old;
}

//////////////////////////////////////////////
ContorlPointV2 ContorlPointV2::moveNext(float len)
{
    auto old = *this;
    _point = getPositionTopByLength(len);
    _height+=len;
    return old;
}
ContorlPointV2 ContorlPointV2::moveNextByRotate(float len,float angle)
{
    auto old = *this;
    _point = getPositionTopByLength(len);
    _height+=len;
    return old;
}
ContorlPointV2 ContorlPointV2::moveNextByRotateLeft(float len)
{
    auto old = *this;
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateRadius );
    Vec2 po = getPositionLeftByLength( _rotateRadius);
    old._point = getRotatePosition(po, _point,-rotateAngle);
    old._angle -= rotateAngle;
    old._height+=len;
    return old;
}
ContorlPointV2 ContorlPointV2::moveNextByRotateRight(float len)
{
    auto old = *this;
    float rotateAngle = MathHelper::getRotateAngle(len, _rotateRadius );
    Vec2 po = getPositionRightByLength( _rotateRadius);
    _point = getRotatePosition(po, _point,rotateAngle);
    _angle += rotateAngle;
    _height+=len;
    return old;
}


/////
Vec2 ContorlPointV2::getPositionRaycastByLength(float angle,float length) const
{
    Vec2 top = _point + Vec2(0,length);
    return MathHelper::getRotatePosition(_point,top,_angle+angle);
}
