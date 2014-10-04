#ifndef __PlantOri__HeadCursor__
#define __PlantOri__HeadCursor__
#include "Macro.h"
#include "TypeDef.h"
#include "ContorlPoint.h"

///三角形头部
class HeadCursor
{
public:
    HeadCursor():_topHeight(0){}
    HeadCursor(ContorlPoint cp,float topHeight,std::function<bool(cocos2d::Vec3)> callback):_cp(cp),_topHeight(topHeight),_crashTestCallBack(callback){}
    
    ContorlPoint getContorlPoint(){return _cp;}
    void setContorlPoint(const ContorlPoint& cp){_cp =cp;}
    float getTopHeight(){return _topHeight;}
    void setTopHeight(float topHeight){_topHeight=topHeight;}
    float getAngle(){return _cp._angle;}
    float getBottomWidth(){return _cp._radius;}
    

    HeadCursor getNextHeadCursor(float len) const;
    HeadCursor getNextHeadCursorByLeft(float len) const;
    HeadCursor getNextHeadCursorByLeft(float len, float rotateRadius) const;
    HeadCursor getNextHeadCursorByRight(float len) const;
    HeadCursor getNextHeadCursorByRight(float len,float rotateRadius) const;
    
    Vec3 getPosition() const ;
    Vec3 getPositionTop() const ;
    Vec3 getPositionBLeft() const;
    Vec3 getPositionBRight() const;
    Vec3 getPositionTLeft() const;
    Vec3 getPositionTRight() const;
    
    
    bool isCrashAny()
    {
        return isCrashTop()||isCrashBLeft()||isCrashBRight();
    }
    bool isCrashTop()
    {
        return _crashTestCallBack(getPositionTop());
    }
    bool isCrashBLeft()
    {
        return _crashTestCallBack(getPositionBLeft());
    }
    bool isCrashBRight()
    {
        return _crashTestCallBack(getPositionBRight());
    }
    bool  isCrashTLeft()
    {
        return _crashTestCallBack(getPositionTLeft());
    }
    bool  isCrashTRight()
    {
        return _crashTestCallBack(getPositionTRight());
    }
    
    bool isHeadCanRotate(int angle);
    bool isFaceTop();
    bool isFaceLeft();
    bool isFaceRight();
    
    FaceDirection getFaceDirection();
    
    ContorlPoint moveNext(float len);
    ContorlPoint moveNextByRotate(float len,float angle);
    ContorlPoint moveNextByRotateLeft(float len);
    ContorlPoint moveNextByRotateRight(float len);
    
    std::function<bool(cocos2d::Vec3)> _crashTestCallBack;
    float _rotateLenght;
    float _topHeight;
    ContorlPoint _cp;
};
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

class HeadCrashTestHelper
{
public:
    HeadCrashTestHelper(const HeadCursor& head):_head(head)
    {
    }
    void  crashTest();
    
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
    const HeadCursor&   _head;
    std::function<bool(cocos2d::Vec3)> _crashTestCallBack;
    unsigned char _crashMask;
};

#endif /* defined(__PlantOri__HeadCursor__) */
