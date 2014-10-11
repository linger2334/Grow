#ifndef __Grow_Demo__Plant1__
#define __Grow_Demo__Plant1__
#include "common.h"
#include "CPLineNode.h"
#include "HeadCursor.h"
#include "GameManager.h"
#include "CrashTestHelper.h"
#include "Box2D/Box2D.h"
#define  MIN_WIDTH 20
#define HEAD_COUNT 60
#define TURN_COUNT 60
#define HEAD_TURN_STEP 30
#define DRAW_BORDER_POINTS 0

#define CONVERT(T) convertToWorldSpace(T)
class GrowContext
{
public:
    GrowContext():_left(false),_right(false),_top(false){}
    GrowContext(bool left,bool right,bool top):_left(left),_right(right),_top(top){}
    bool _left,_right,_top;
};
#include "ItemModel.h"
class Plant_1 : public ItemModel
{
public:
    CREATE_FUNC(Plant_1);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
 
    void initPlantInfo(const HeadCursorV2& head,float growUnitHeight)
    {
        this->_cpLineNode.clear();
        _headCur = head ;
        _growUnitHeight = growUnitHeight;
        _cpLineNode.setVerticesUnitLength(1);
        _cpLineNode._rotateLength = _headCur._rotateLenght;
        addHeadToPlantLine();
        _headTopHeight = _headCur._cp._point.y;
    }
    void addHeadToPlantLine()
    {
        _cpLineNode.addContorlPointEditWidth(_headCur.getContorlPoint(), MIN_WIDTH);
    }
    void  setHeadCursor(const HeadCursorV2& head){_headCur = head;}
    HeadCursorV2& getHeadCursorRef(){return _headCur;}
    
    GrowContext getCanGrowContext();
    
    HeadCrashContextV2  testHeadCrash();
    
    void editTopCP(const ContorlPointV2& cp)
    {
        _cpLineNode._cpList[_cpLineNode._cpList.size()-1] = cp;
    }
    int  getHeadTopCrashGridCount(int checkCount,bool crash);
    float  grow(FaceDirection dir,float height);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    void subPlantCP(int yHeight);

    bool isHeadCanRotate(int  endAngle);
    
    bool checkRotateAngleCrash(float angle,Vec2 pto,Vec2 pt);
    
    void drawHeadRange(const Mat4 &transform, uint32_t flags);
    
    bool   isCashPoint(Vec2 pt);
    bool   isCashPoint(Vec2 pt, int type);
    bool isCrashPointByMap(Vec2 pt)
    {
         return  _crashTestCallBack(pt,1);
    }
    bool isCanGrowTop();
    void  checkHead();
    void  checkHeightSub(float len);
    
    
    std::function<bool(cocos2d::Vec2,int type)>  _crashTestCallBack;
    HeadCursorV2    _headCur;
    float           _growUnitHeight;
    CustomCommand   _customCommand;
    CPLineNode      _cpLineNode;
    float           _growLength;
    Texture2D*      _tex;
    FaceDirection  _headFaceDir;
    FaceDirection  _preHeadFaceDir;
    FaceDirection  _headDir;
    float          _growLen;
    GLProgramState* _borderProgramState;
    GLProgram*      _glprogram;
    GLProgramState* _glprogramstate;
    
    float            _turnLen;
    int            _nowDirLen;
    float          _turnSpeed;
    float         _turnGrowLength;
    bool          _isTurnHead;
    
    b2World*     _world;

    float          _growLenContext[3];
    int             _testGrowLen;
    Vec3  *headVec ;
    Tex2F *text1;
    float       _headTopHeight;
};

#endif /* defined(__Grow_Demo__Plant1__) */
