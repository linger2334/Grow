#ifndef __PlantOri__HeadCursor__
#define __PlantOri__HeadCursor__
#include "common.h"
#include "TypeDef.h"
#include "ContorlPoint.h"

///头部
class HeadCursorV2
{
public:
    HeadCursorV2():_topHeight(0){}
    HeadCursorV2(ContorlPointV2 cp,float topHeight,float rotatelength):_cp(cp),_topHeight(topHeight),_rotateLenght(rotatelength){}
    
    ContorlPointV2 getContorlPoint(){return _cp;}
    void setContorlPoint(const ContorlPointV2& cp){_cp =cp;}
    float getTopHeight(){return _topHeight;}
    void setTopHeight(float topHeight){_topHeight=topHeight;}
    float getAngle(){return _cp._angle;}
    float getBottomWidth(){return _cp._radius;}
    
    
    HeadCursorV2 getNextHeadCursor(float len) const;
    HeadCursorV2 getNextHeadCursorByLeft(float len) const;
    HeadCursorV2 getNextHeadCursorByLeft(float len, float rotateRadius) const;
    HeadCursorV2 getNextHeadCursorByRight(float len) const;
    HeadCursorV2 getNextHeadCursorByRight(float len,float rotateRadius) const;
    
    Vec2 getPosition() const ;
    Vec2 getPositionTop() const ;
    Vec2 getPositionBLeft() const;
    Vec2 getPositionBRight() const;
    Vec2 getPositionTLeft() const;
    Vec2 getPositionTRight() const;
    Vec2 getPositionTLeft(float len) const;
    Vec2 getPositionTRight(float len) const;
    
    Vec2 getPositionRaycastByLength(float angle,float length) const;
    
    bool isFaceTop();
    bool isFaceLeft();
    bool isFaceRight();
    FaceDirection getFaceDirection();
    
    ContorlPointV2 moveNext(float len);
    ContorlPointV2 moveNextByRotate(float len,float angle);
    ContorlPointV2 moveNextByRotateLeft(float len);
    ContorlPointV2 moveNextByRotateRight(float len);
    
    float _rotateLenght;
    float _topHeight;
    ContorlPointV2 _cp;
};
class HeadCrashContextV2
{
public:
    HeadCrashContextV2()
    {
    }
    HeadCrashContextV2(unsigned char mask):_crashMask(mask)
    {
    }
    
    bool isCrashTop()
    {
        return _crashMask & HeadCursorPoint::center_top;
    }
    bool isCrashBLeft()
    {
        return _crashMask & HeadCursorPoint::left_bottom;
    }
    bool isCrashBRight()
    {
        return _crashMask & HeadCursorPoint::right_bottom;
    }
    bool  isCrashTLeft()
    {
         return _crashMask & HeadCursorPoint::left_top;
    }
    bool  isCrashTRight()
    {
        return _crashMask & HeadCursorPoint::right_top;
    }
    unsigned char _crashMask;
};


#endif /* defined(__PlantOri__HeadCursor__) */
