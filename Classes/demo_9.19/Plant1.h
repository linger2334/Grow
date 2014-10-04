#ifndef __Grow_Demo__Plant1__
#define __Grow_Demo__Plant1__
#include "Macro.h"
#include "CPLineNode.h"
#include "HeadCursor.h"
#include "GameManager.h"
#include "CrashTestHelper.h"
#define  MIN_WIDTH 30
class GrowContext
{
public:
    GrowContext():_left(false),_right(false),_top(false){}
    GrowContext(bool left,bool right,bool top):_left(left),_right(right),_top(top){}
    bool _left,_right,_top;
};

class Plant_1 : public Node
{
public:
    CREATE_FUNC(Plant_1);
    virtual bool init();
    void initPlantInfo(const HeadCursorV2& head,float growUnitHeight)
    {
        this->_cpLineNode.clear();
        _headCur = head ;
        _growUnitHeight = growUnitHeight;
        _cpLineNode.setVerticesUnitLength(1);
        _cpLineNode._rotateLength = _headCur._rotateLenght;
        addHeadToPlantLine();
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
    
    void  updateHead(float dt);
    void drawHead();
    
    void turnHead(FaceDirection dir)
    {
        _preHeadFaceDir = _headFaceDir;
        _headFaceDir =dir;
        _turnLen = 0;
    }
    
    void checkHead();
    void  checkHeightSub(float len);
    std::function<bool(cocos2d::Vec2)>  _crashTestCallBack;
    HeadCursorV2    _headCur;
    float           _growUnitHeight;
    CustomCommand   _customCommand;
    CPLineNode      _cpLineNode;
    float           _growLength;
    Texture2D*      tex;
    FaceDirection  _headFaceDir;
    FaceDirection  _preHeadFaceDir;
    int            _turnLen;
};

#endif /* defined(__Grow_Demo__Plant1__) */
