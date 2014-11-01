#include "HeadCursor.h"
#define  TOP_ANGLE 3.0f

/////////////////////////////////////////////////////////
HeadCursorV2 HeadCursorV2::getNextHeadCursor(float len) const
{
    HeadCursorV2 ret;
    ret = *this;
    ret._cp._point = _cp.getTopPositionByLength(len);
    ret._cp._height+=len;
    return ret;
}
HeadCursorV2 HeadCursorV2::getNextHeadCursorByLeft(float len) const
{
    HeadCursorV2 ret;
    ret = *this;
    
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateLenght );
    Vec2 po = _cp.getPositionLeftByLength(_rotateLenght);
    ret._cp._point = getRotatePosition(po, _cp._point,-rotateAngle);
    ret._cp._angle -= rotateAngle;
    
    return ret;
}
HeadCursorV2 HeadCursorV2::getNextHeadCursorByRight(float len) const
{
    HeadCursorV2 ret;
    ret = *this;
    
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateLenght );
    Vec2 po = _cp.getPositionRightByLength(_rotateLenght);
    ret._cp._point = getRotatePosition(po, _cp._point,rotateAngle);
    ret._cp._angle += rotateAngle;
    
    return ret;
}


/////////////////////////////////////////////////////////
Vec2 HeadCursorV2::getPosition() const {return _cp._point;}
Vec2 HeadCursorV2::getPositionTop() const {return _cp.getTopPositionByLength(_topHeight);}
Vec2 HeadCursorV2::getPositionBLeft() const {return _cp.getPositionLeftByLength(_cp._radius/2);}
Vec2 HeadCursorV2::getPositionBRight() const {return _cp.getPositionRightByLength(_cp._radius/2);}
Vec2 HeadCursorV2::getPositionTLeft() const {ContorlPointV2 cp(getPositionTop(),_cp._radius,_cp._angle,_cp._height);
    return cp.getPositionLeft();
}
Vec2 HeadCursorV2::getPositionTRight() const {ContorlPointV2 cp(getPositionTop(),_cp._radius,_cp._angle,_cp._height);
    return cp.getPositionRight();}
Vec2  HeadCursorV2::getPositionTLeft(float len)const {ContorlPointV2 cp(getPositionTop(),_cp._radius,_cp._angle,_cp._height);
    return cp.getPositionLeftByLength(len);}
Vec2  HeadCursorV2::getPositionTRight(float len)const {ContorlPointV2 cp(getPositionTop(),_cp._radius,_cp._angle,_cp._height);
    return cp.getPositionRightByLength(len);}
Vec2 HeadCursorV2::getPositionRaycastByLength(float angle,float length) const
{
    Vec2 cpt = _cp.getTopPositionByLength(length);
    return MathHelper::getRotatePosition(_cp._point,cpt, angle);
}

////////////////////////////////////////////////////
bool HeadCursorV2::isFaceTop(){return getFaceDirection()==FaceDirection::FaceTop;}
bool HeadCursorV2::isFaceLeft(){return getFaceDirection()==FaceDirection::FaceLeft;}
bool HeadCursorV2::isFaceRight(){return getFaceDirection()==FaceDirection::FaceRight;}

FaceDirection HeadCursorV2::getFaceDirection()
{
    FaceDirection dir;
    if (_cp._angle<-TOP_ANGLE) {dir = FaceDirection::FaceLeft;}
    else if(_cp._angle>TOP_ANGLE){dir = FaceDirection::FaceRight;}
    else{dir =FaceDirection::FaceTop;}
    return dir;
}

//////////////////////////////////////////////
ContorlPointV2 HeadCursorV2::moveNext(float len)
{
    _cp._point = _cp.getTopPositionByLength(len);
    _cp._height+=len;
    return _cp;
}
ContorlPointV2 HeadCursorV2::moveNextByRotate(float len,float angle)
{
    _cp._point = _cp.getPositionLeftByLength(_cp._radius);
}
ContorlPointV2 HeadCursorV2::moveNextByRotateLeft(float len)
{
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateLenght );
    Vec2 po = _cp.getPositionLeftByLength(_rotateLenght);
    _cp._point = getRotatePosition(po, _cp._point,-rotateAngle);
    _cp._angle -= rotateAngle;
    _cp._height+=len;
    return _cp;
}
ContorlPointV2 HeadCursorV2::moveNextByRotateRight(float len)
{
    float rotateAngle = MathHelper::getRotateAngle(len,_rotateLenght );
    Vec2 po = _cp.getPositionRightByLength(_rotateLenght);
    _cp._point = getRotatePosition(po, _cp._point,rotateAngle);
    _cp._angle += rotateAngle;
    _cp._height+=len;
    return _cp;
}


