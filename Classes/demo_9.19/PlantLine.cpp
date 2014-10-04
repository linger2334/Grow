#include "PlantLine.h"
#include "GameManager.h"
#include "CrashTestHelper.h"
#define MIN_WIDTH 50
#define MAX_WIDTH 100
#define HEAD_HEIGHT 80
#define MOVE_LENGTH 1
#define GROW_HEIGHT 8
#define HEAD_ADD  0.8
bool PlantLine::init()
{
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    _pixelSize = Size(1,1);
    _crashTestHelper = GameManager::getInstance()->getCrashTestHelper();
    _dirt = false;
    initGameInfo();
    _cpUnitHeight = GROW_HEIGHT;
    _verticesStepUnitHeight = 2;
    _growLength = 0;
    _vertices = nullptr;
    _capacity = 0;
    _capacityAddSteplen = 100;
    _isFirstload = true;
    _gridUnitSize = GameManager::getInstance()->getMapGridUnitSize();
    return true;
}
void PlantLine::onEnterTransitionDidFinish()
{
   // schedule(schedule_selector(PlantLine::update),1.0/60);
}
void  PlantLine::onEnter()
{
    if (!_isFirstload) {
       // schedule(schedule_selector(PlantLine::update),1.0/60);
    }
    else _isFirstload = false;
    _texture = Director::getInstance()->getTextureCache()->addImage("plant1_4_01.png");
    Texture2D::TexParams  parms = {GL_LINEAR, GL_LINEAR, GL_REPEAT,GL_REPEAT};
    _texture->setTexParameters(parms);
    _texture->retain();
    //_texture->setAntiAliasTexParameters();
    //_texture->setAliasTexParameters();
    
    _textureHead = Director::getInstance()->getTextureCache()->addImage("plant1_3.png");

    _textureHead->retain();
    Node::onEnter();
}
void  PlantLine::onExit()
{
    //unscheduleUpdate();
     _texture->release();
    _textureHead->release();
    Node::onExit();
}
void  PlantLine::initGameInfo()
{
    _headCur = HeadCursor(ContorlPoint(Vec3(0,0,-10), 80, 0, 0),HEAD_HEIGHT,std::bind(&PlantLine::isCrash,this,std::placeholders::_1));
    addContorlPointEditWidth(_headCur.getContorlPoint(),MIN_WIDTH);

    _headCur._rotateLenght = 80;
}

void PlantLine::resetPlant()
{
    _cpList.clear();
    initGameInfo();
}
void PlantLine::update(float dt)
{
    if(!grow(90))return;
    _dirt=true;
    updateHead(dt);
}

#define HEIDE_STEP 4
int  PlantLine::getHeadTopCrashGridCount(int checkCount,bool crash)
{
    int ret = 0;
    Vec3 top = _headCur.getPositionTop();
    if (crash) {
        for (int i = 1; i < HEIDE_STEP; i--) {
            Vec3 tem = top+Vec3(0,-_gridUnitSize.height,0);
            if(!isCrash(tem))break;
            top = tem;
        }
    }
    int f = checkCount > 0 ? 1 : -1;
    for (int i = 1; i < abs(checkCount); i++)
    {
        if (crash&&!isCrash(top+Vec3(f*i*GROW_HEIGHT,0,0)))break;
        else if(!crash&&isCrash(top+Vec3(f*i*GROW_HEIGHT,0,0)))break;
        ret++;
    }
    return ret;
}

bool PlantLine::grow(float angle)
{
    FaceDirection dir = _headCur.getFaceDirection();
    bool isGrow = true;
    FaceDirection trnDir;
    if (dir == FaceDirection::FaceTop)
    {
        if (isCanGrowTop()) {trnDir=FaceDirection::FaceTop;}
        else
        {
            bool testCrash = true;
            int testCount = 5;
            int minCount = MIN_WIDTH/8;
            for (int i = 0; i < 2 ; i++) {
                int lcount = getHeadTopCrashGridCount(-testCount,testCrash);
                int rcount = getHeadTopCrashGridCount(testCount,testCrash);
                if (testCrash&& lcount < rcount)
                {
                    trnDir = FaceDirection::FaceLeft;
                    break;
                }
                else if (testCrash && lcount  > rcount)
                {
                    trnDir = FaceDirection::FaceRight;
                    break;
                }
                else  if (!testCrash&& lcount < rcount && rcount >= minCount)
                {
                    trnDir = FaceDirection::FaceRight;
                    break;
                }
                else if (!testCrash && lcount  > rcount && lcount >= minCount)
                {
                    trnDir = FaceDirection::FaceLeft;
                    break;
                }
                else if (testCrash && lcount == rcount && lcount == 0)
                {
                    testCount=20;
                    testCrash = false;
                }
                else isGrow =false;
            }
        }
    }
    else if (dir == FaceDirection::FaceLeft)
    {
        if (isCanGrowRight()) {trnDir=FaceDirection::FaceRight;}
        else if (isCanGrowTop()){trnDir=FaceDirection::FaceTop;}
        else if(isCanGrowLeft()){trnDir=FaceDirection::FaceLeft;}
        else isGrow =false;
    }
    else if (dir == FaceDirection::FaceRight)
    {
        if (isCanGrowLeft()) {trnDir=FaceDirection::FaceLeft;}
        else  if (isCanGrowTop()){trnDir=FaceDirection::FaceTop;}
        else if(isCanGrowRight()){trnDir=FaceDirection::FaceRight;}
        else isGrow =false;
    }
    else {assert(0);}
    if (isGrow)
    {
        growOneUnit(trnDir);
    }
    return isGrow;
}
 bool PlantLine::growOneUnit(FaceDirection dir)
{
    std::string str="MOVE  ";
    auto p =_cpList.rbegin()->_point;
    log("%f, %f",p.x,p.y);
    if(_growLength<=0.003)
    {
      
     addContorlPointEditWidth(_headCur.getContorlPoint(),MIN_WIDTH);
    }
    HeadCursor cp ;
    _growLength += _verticesStepUnitHeight;
    bool flag = _growLength < _cpUnitHeight;
    switch (dir) {
        case  FaceDirection::FaceTop:
            str+="top;";
            if(flag)cp = _headCur.getNextHeadCursor(_growLength);
            else _headCur.moveNext(_cpUnitHeight);
            break;
        case  FaceDirection::FaceLeft:
            str+="left;";
            if(flag)cp = _headCur.getNextHeadCursorByLeft(_growLength);
            else _headCur.moveNextByRotateLeft(_cpUnitHeight);
            break;
        case  FaceDirection::FaceRight:
        if(flag)cp = _headCur.getNextHeadCursorByRight(_growLength);
            else _headCur.moveNextByRotateRight(_cpUnitHeight);
            str+="right;";
            break;
        default:
            assert(0);
            break;
    }
    
    if (_growLength >= _cpUnitHeight) {
       // _headCur =cp;
            //addContorlPointEditWidth(_headCur.getContorlPoint(),MIN_WIDTH);
        _growLength = 0;
    }
    else
    {
        _cpList[_cpList.size()-1]=cp.getContorlPoint();
        _cpList[_cpList.size()-1]._radius=MIN_WIDTH;
    }
    
    log(str.c_str());

}
bool PlantLine::isCanGrowLeft()
{
    if (_headCur.getAngle()<-130) return false;
    return !_headCur.isCrashBLeft()&&!_headCur.isCrashTLeft()&&(!_headCur.isCrashTop()||isHeadCanRotate(-130));
    return !_headCur.isCrashBLeft()&&(!_headCur.isCrashTop()||isHeadCanRotate(-130));
    //bool flag=_headCur.isCrashBLeft();
   // return (!_headCur.isCrashBLeft()&&(!_headCur.isCrashTop()||isHeadCanRotate(-70)))||
    //       (_headCur.isCrashBRight()&&_headCur.isCrashTop()&&isHeadCanRotate(-70));
   // return !_headCur.isCrashBLeft()&&(!_headCur.isCrashTop()||(getHeadTopCrashGridCount(-4)<4));
    
}
bool PlantLine::isCanGrowRight()
{
    if (_headCur.getAngle()>130) return false;
    return !_headCur.isCrashBRight()&&!_headCur.isCrashTRight()&&(!_headCur.isCrashTop()||isHeadCanRotate(130));
      return !_headCur.isCrashBRight()&&(!_headCur.isCrashTop()||isHeadCanRotate(130));
    //bool flag=_headCur.isCrashBRight();
    //return (!flag&&(!_headCur.isCrashTop()||isHeadCanRotate(70)))||
    //(flag&&isHeadCanRotate(70));
//    return !_headCur.isCrashBRight()&&(!_headCur.isCrashTop()||(getHeadTopCrashGridCount(4)<4));
}
bool PlantLine::isCanGrowTop()
{
    return !_headCur.isCrashTop()&&!_headCur.isCrashTLeft()&&!_headCur.isCrashTRight();
}
bool PlantLine::isHeadCanRotate(int  endAngle)
{/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool ret = false;
    int f  =  endAngle < 0 ? -1 :1;
    const ContorlPoint& cp = _headCur._cp;
    Vec3 top =_headCur._cp.getTopPositionByLength(_headCur._topHeight);
    for (int i=0; i<abs(endAngle); i++) {
        if (fabs(f*i+cp._angle)>90) {
            continue;
        }
        Vec3 vt =getRotatePosition(cp._point,top,f*i);
        if (!isCrash(vt)) {
            ret =true;
            break;
        }
    }
    return ret;
}

void PlantLine::drawHead()
{
    if (_cpList.size()==0) return;
#define HEAD_COUNT 80
    const auto& cp =*_cpList.rbegin();
    ContorlPoint cp1(cp.getTopPositionByLength(_headCur._topHeight),cp._radius,cp._angle,cp._height);
 
    Vec3 head[4]={cp.getPositionLeft(),cp.getPositionRight(),cp1.getPositionLeftByLength(MIN_WIDTH/2.0f),cp1.getPositionRightByLength(MIN_WIDTH/2.0f)};
    Tex2F text[4]=
    {
       Tex2F(0,1),Tex2F(1,1),Tex2F(0,0),Tex2F(1,0)
    };
    static Vec3  *headVec = new Vec3[HEAD_COUNT*2];
    static Tex2F *text1 = new Tex2F[HEAD_COUNT*2];
    memset(headVec, 0,sizeof(Vec3)*HEAD_COUNT*2);
    memset(text1, 0,sizeof(Tex2F)*HEAD_COUNT*2);
    HeadCursor headCur(cp,HEAD_COUNT,std::bind(&PlantLine::isCrash,this,std::placeholders::_1));
//    headCur._cp._point.y=0;
//    headCur._cp._point.z=-2;
//    headCur._cp._radius=200;
    for(int i = 0; i<HEAD_COUNT;i++)
    {
        auto cp2 = headCur.getContorlPoint();
        headVec[i*2] = cp2.getPositionLeft();
        text1[i*2] = getTex2FCPLeft(cp2);
        headVec[i*2+1] = cp2.getPositionRight();
        text1[i*2+1] = getTex2FCPRight(cp2);
        headCur.moveNext(1);
        headCur._cp._radius-=.6;
       // headCur._cp._angle =0;
       // headCur._cp._point.z =-100;
    }
    
    GL::bindTexture2D( _texture->getName());
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, headVec);

      glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, text1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, HEAD_COUNT*2);
    
}
void  PlantLine::updateHead(float dt)
{
    static float old =0;
    old+=dt;
    if (old>0.3) {
        for (int i =_cpList.size()-1; i>=0 ; i--) {
            if (_cpList[i]._radius>MAX_WIDTH) break;
            _cpList[i]._radius+=HEAD_ADD;
        }
        old=0;
    }
    
}

void PlantLine::ensureCapacity(int count)
{
    int size = _cpList.size()*2;
    if(size > _capacity)
    {
        if (_vertices) {
            delete [] _vertices;
        }
        _vertices = new V3F_T2F[size+_capacityAddSteplen];
        _capacity = size+_capacityAddSteplen;
    }
}


void PlantLine::addVertices(const ContorlPoint& cp)
{
    _vertices[_verticesCount].vertices = convertToPositionInPixel(cp.getPositionLeft());
    _vertices[_verticesCount].texCoords = getTex2FCPLeft(cp);
    
    _vertices[_verticesCount+1].vertices = convertToPositionInPixel(cp.getPositionRight());
    _vertices[_verticesCount+1].texCoords = getTex2FCPRight(cp);
    _verticesCount+=2;
}

void PlantLine::updateVertices(unsigned int start,unsigned int length)
{
    ensureCapacity(1);
    _verticesCount = 0;
    for (int i = 0 ; i<_cpList.size(); i++) {
        addVertices(_cpList[i]);
    }
}
float  PlantLine::getHeadHeight()
{
    return  this->getPositionY()+_headCur.getContorlPoint()._point.y;
    
}
 void PlantLine::subContorlPoint(int y)
{
    int subIndex = -1;
//    
//    for (int i = 0 ; i<_cpList.size(); i++) {
//        if (_cpList[i].getContorlPosition().y>y) {
//            break;
//        }
//        subIndex++;
//    }
    std::vector<ContorlPoint>::const_iterator start =_cpList.begin();
    std::vector<ContorlPoint>::const_iterator end = _cpList.end();
    std::vector<ContorlPoint>::const_iterator ip = _cpList.begin();
    bool  isNeedDel = false;
    while (ip!=end) {
        float left = (*ip).getPositionLeft().y;
        float right = (*ip).getPositionRight().y;
        float max =left>right? left:right;
        if (max >= y )
        {
            isNeedDel=true;
            break;
        }
        ip++;
    }
    if (isNeedDel) {
        _cpList.erase(start,ip);
        float y1 = ip->_point.y;
        for (int i = 0 ; i<_cpList.size(); i++) {
            _cpList[i]._point+=Vec3(0,-y,0);
        }
//        _cpList.
//        (*_cpList.begin())._point.y=0;
//        (*_cpList.begin())._angle=0;
        ContorlPoint cpn = *start;
        cpn._point.y=0;
        cpn._angle = 0;
       
        _cpList.insert(start,cpn);
        this->setPositionY(this->getPositionY()+y);
        _headCur._cp._point+=Vec3(0,-y,0);
        
    }
//    if (subIndex>=0) {
//        std::vector<ContorlPoint>::const_iterator  start =_cpList.begin();
//        std::vector<ContorlPoint>::const_iterator end = start +subIndex;
//        _cpList.erase(start, end);
//        int yn = _cpList[0]._point.y;
//        for (int i = 0 ; i<_cpList.size(); i++) {
//            _cpList[i]._point+=Vec3(0,-y,0);
//        }
//        _headCur._cp._point+=Vec3(0,-y,0);
//    }
}

void PlantLine::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(PlantLine::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    
}
void PlantLine::onDraw(const Mat4 &transform, uint32_t flags)
{
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    if (_dirt) {
        updateVertices(0,_cpList.size());
    }
  
   // Director::getInstance()->setDepthTest(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawHead();

    GL::bindTexture2D( _texture->getName());
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(_vertices))+3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,_verticesCount);
     //Director::getInstance()->setDepthTest(false);
}