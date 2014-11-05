#ifndef __Grow_Demo__Plant1__
#define __Grow_Demo__Plant1__
#include "common.h"
#include "CPLineNode.h"
#include "HeadCursor.h"
#include "GameManager.h"

#define MIN_WIDTH 20
#define HEAD_COUNT 60
#define TURN_COUNT 60
#define HEAD_TURN_STEP 30
//#define DRAW_BORDER_POINTS 0



class Plant_1 : public Node
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
    
    HeadCrashContextV2  testHeadCrash();
    
    void editTopCP(const ContorlPointV2& cp)
    {
        _cpLineNode._cpList[_cpLineNode._cpList.size()-1] = cp;
    }
    float  grow(FaceDirection dir,float height);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    void subPlantCP(int yHeight);
    
    void drawHeadRange(const Mat4 &transform, uint32_t flags);


    void  checkHeightSub(float len);
    
    Vec2 getNextGrowUnitLengthPosition(FaceDirection growDir);
    

    HeadCursorV2    _headCur;
    float           _growUnitHeight;
    CustomCommand   _customCommand;
    CPLineNode      _cpLineNode;
    float           _growLength;
    Texture2D*      _tex;
    FaceDirection   _headFaceDir;
    FaceDirection   _preHeadFaceDir;
    FaceDirection   _headDir;
    float           _growLen;
    GLProgramState* _borderProgramState;
    GLProgram*      _glprogram;
    GLProgramState* _glprogramstate;
    
    b2World*      _world;

    float         _headTopHeight;
};

#endif /* defined(__Grow_Demo__Plant1__) */
