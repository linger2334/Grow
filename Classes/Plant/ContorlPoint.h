#ifndef __PlantOri__ContorlPoint__
#define __PlantOri__ContorlPoint__
#include <cocos2d.h>
#include "MathHelper.h"
using namespace cocos2d;
class ContorlPointV2
{
public:
    ContorlPointV2(){}
    ContorlPointV2(Vec2 point,float radius,float angle,float height,int zPosition =0):_point(point),_height(height),
    _angle(angle),_radius(radius),_zPosition(zPosition){}
    
    Vec2  getContorlPosition(){return _point;}
    Vec2  getNextContorPosition( float angle);
    
    Vec2  getPositionLeft() const;
    Vec2  getPositionRight() const;
    Vec2  getPostionBLeft(float len) const;
    Vec2  getPostionBRight(float len) const;
    Vec2  getPositionLeftByLength(float len) const
    {
        return getRotatePosition(_point,len,_angle);
    }
    
    Vec2  getPositionRightByLength(float len) const
    {
        return getRotatePosition(_point,len,_angle+180);
    }
    Vec2 getPositionTopByLength(float len) const;
    
    Vec2 getPositionRaycastByLength(float angle,float length) const;

    
    ContorlPointV2 getNextContorlPointV2(float len);
    ContorlPointV2 getNextContorlPointV2ByRotate(float len,float angle);
    ContorlPointV2 getNextContorlPointV2ByRotateLeft(float len);
    ContorlPointV2 getNextContorlPointV2ByRotateRight(float len);
    
    ContorlPointV2 moveNext(float len);
    ContorlPointV2 moveNextByRotate(float len,float angle);
    ContorlPointV2 moveNextByRotateLeft(float len);
    ContorlPointV2 moveNextByRotateRight(float len);
    
    Vec2   _point;
    float  _angle;
    float  _radius;
    float  _height;
    int    _zPosition;
    float  _rotateRadius;
};

#define MakeVec3(T,Z) Vec3(T.x,T.y,Z)
#endif /* defined(__PlantOri__ContorlPoint__) */
