#include "Plant1.h"


#define V2TV3(T) Vec3(T.x,T.y,0)
#define TURN_GROW_LENGTH 6
bool Plant_1::init()
{
    Node::init();
    _tex = Director::getInstance()->getTextureCache()->addImage("test_plant_1024.png");
    Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
    _tex->setTexParameters(parms);

    _tex->retain();
    _cpLineNode.setTesture(_tex);
     auto glprogram = GLProgram::createWithFilenames("ccShader_PositionTexture.vert", "ccShader_PositionTexture.frag");
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    setGLProgramState(_glprogramstate);

    _ignoreAnchorPointForPosition = true;
    
#ifdef DRAW_BORDER_POINTS
    _borderProgramState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_COLOR);
    _borderProgramState->retain();
#endif
   // _tex = nullptr;
    _cpLineNode = CPLineNode(_tex,8);
    _headFaceDir= FaceDirection::FaceTop;
    headVec=nullptr;
    text1 =nullptr;
    _turnSpeed = HEAD_TURN_STEP;
    _isTurnHead =false;
    _turnGrowLength = 0;

    _world = GameManager::getInstance()->getBox2dWorld() ;
    memset(_growLenContext, 0, sizeof(_growLenContext));
    return true;
}
void Plant_1::onEnter()
{

  
}
void Plant_1::onExit()
{

    if (_tex) _tex->release();
}
void Plant_1::subPlantCP(int yHeight)
{
    
    auto ip = _cpLineNode._cpList.begin();
    auto end = _cpLineNode._cpList.end();
    while (ip!=end) {
        if (ip->_point.y<yHeight) {
            ip=_cpLineNode._cpList.erase(ip);continue;
        }
        else break;
        ip++;
    }
    while(ip!=end)
    {
        ip->_point.y -= yHeight;
        ip++;
    }
    _headCur._cp._point.y-= yHeight;
    _headTopHeight -= yHeight;
}

void Plant_1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(Plant_1::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
void Plant_1::onDraw(const Mat4 &transform, uint32_t flags)
{
#ifdef DRAW_BORDER_POINTS
    
    
    _borderProgramState->getGLProgram()->use();
    _borderProgramState->getGLProgram()->setUniformsForBuiltins(transform);
    // _cpLineNode.onDrawBorder(transform,flags);
    drawHeadRange(transform,flags);
#endif
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


    _cpLineNode.onDraw(transform,flags);

}
bool Plant_1::isHeadCanRotate(int  endAngle)
{
    ///////////////////////////////////////////////////////////////////////////////////////////
    bool ret = false;
    int f  =  endAngle < 0 ? -1 :1;
    const ContorlPointV2& cp = _headCur._cp;
    
    Vec2 top ;//=_headCur._cp.getTopPositionByLength(_headCur._topHeight);
    if (endAngle < 0) {
        top =_headCur.getPositionTLeft();
    }
    else if(endAngle >0 )
    {top =_headCur.getPositionTRight();}
    for (int i=0; i<abs(endAngle); i++) {
        if (fabs(f*i+cp._angle)>180) {
            continue;
        }
        Vec2 vt =getRotatePosition(cp._point,top,f*i);
        if (!isCashPoint(CONVERT(vt))) {
            ret =true;
            break;
        }
    }
    return ret;
}
GrowContext Plant_1::getCanGrowContext()
{
    GrowContext canGrow;
    HeadCrashContextV2 crash = testHeadCrash();
    
    bool flag ;
    Vec2 ptHeadO = _headCur.getPosition();
    bool lb,lt,rb,rt;
//    lb = crash.isCrashBLeft();
//    lt = crash.isCrashTLeft();
//    rb = crash.isCrashBRight();
//    rt = crash.isCrashTRight();
    
    bool crashs[9] = {0};
    
    Vec2 cpl = _headCur.getPositionBLeft();
    Vec2 cpo = _headCur.getPosition();
    int type = 0;
    for (int i = 0 ; i< 9 ; i++) {
        if (i<=4) {
            type =1;
        }
        else if(i>=6)
        {
             type =2;
        }
        else type =0;
        crashs[i] =  isCashPoint(CONVERT(MathHelper::getRotatePosition(cpo, cpl,-20+i*26.6f)),type);
    }
    
    canGrow._left=canGrow._right=canGrow._top =true;
    int lc=0,rc=0;
    bool bl=true,br=true;
//    for (int i = 0 ; i<4; i++) {
//        if (bl&&crashs[i]) {
//            //bl = false;
//        }
//        else if(bl)lc++;
//        
//        if (br&&crashs[8-i])
//        {
//            br = false;
//        }
//        else if(br) rc++;
//    }
    for (int i = 0 ; i<4; i++) {
        if (!crashs[i])lc++;
        if (!crashs[8-i])rc++;

    }

    if(lc<2 ||_headCur.getAngle() < -90)
    {
        canGrow._left = false;
    }
     if(rc<2||_headCur.getAngle() > 90)
    {
        canGrow._right = false;
    }
    
   
    lt =isCashPoint(CONVERT(_headCur.getPositionTLeft()),0);
    rt =isCashPoint(CONVERT(_headCur.getPositionTRight()),0);
    bool ct = isCashPoint(CONVERT(_headCur.getPositionTop()),0);
    if ((lt||ct||rt)&&(canGrow._left||canGrow._right))
    {
        canGrow._top = false;
    }

    auto  cptop = _headCur._cp;
    cptop._point = _headCur._cp.getTopPositionByLength(15);
    cptop._radius = 30;
    if(isCashPoint(CONVERT(cptop.getContorlPosition()),0))
    {
       canGrow._top = false;
        // canGrow._left=canGrow._right=c
    }
    if(isCashPoint(CONVERT(cptop.getPositionLeft()),1))
    {
        canGrow._left  = false;
    }
    if(isCashPoint(CONVERT(cptop.getPositionRight()),2))
    {
       canGrow._right  = false;
    }

    return canGrow;
}
class RayCastAnyCallback : public b2RayCastCallback
{
public:
	RayCastAnyCallback()
	{
		_crash = false;
	}
    
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
        _crash = true;
	}
    bool _crash;
};
bool   Plant_1::isCashPoint(Vec2 pt, int type)
{
    
    //  b2RayCastCallback p;
    RayCastAnyCallback cash;
    // Vec2 po =  CONVERT( MathHelper::getRotatePosition(_headCur.getPosition(), _headCur.getPositionBLeft(),-90));
    Vec2 pt1 ;
    if (type==0) {
        pt1 = _headCur.getPosition();
    }
    else if(type ==1)pt1 = _headCur._cp.getPositionLeftByLength(7);
     else if(type ==2)pt1 = _headCur._cp.getPositionRightByLength(7);

    Vec2 po = CONVERT(pt1);
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash||_crashTestCallBack(pt,0);
    // return  _crashTestCallBack(pt,0);
}
bool   Plant_1::isCashPoint(Vec2 pt)
{

  //  b2RayCastCallback p;
    RayCastAnyCallback cash;
   // Vec2 po =  CONVERT( MathHelper::getRotatePosition(_headCur.getPosition(), _headCur.getPositionBLeft(),-90));
    Vec2 po = CONVERT(_headCur.getPosition());
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash||_crashTestCallBack(pt,0);
   // return  _crashTestCallBack(pt,0);
}

#define HEIDE_STEP 4
int  Plant_1::getHeadTopCrashGridCount(int checkCount,bool crash)
{
    int ret = 0;
    Vec2 top = _headCur.getPositionTop();
    if (crash) {
        for (int i = 1; i < HEIDE_STEP; i++) {
            Vec2 tem = top+Vec2(0,-8);
            if(!isCashPoint(CONVERT(tem)))break;
            top = tem;
        }
    }
    int f = checkCount > 0 ? 1 : -1;
    for (int i = 1; i < abs(checkCount); i++)
    {
        if (crash&&!isCashPoint(CONVERT(top+Vec2(f*i*8,0))))break;
        else if(!crash&&isCashPoint(CONVERT(top+Vec2(f*i*8,0))))break;
        ret++;
    }
    return ret;
}
float  Plant_1::grow(FaceDirection dir,float height)
{
    float retLen = -1;
    std::string str="MOVE  ";

    if(_growLength<=0.003)
    {
        addHeadToPlantLine();
    }
    ContorlPointV2& topCp = _cpLineNode.getTopCP();
    HeadCursorV2 cp ;
    if (_growLength+height > _growUnitHeight ) {
        //_growLength = _growUnitHeight;
        retLen = height-(_growUnitHeight - _growLength);
         _growLength = _growUnitHeight;
    }
    else{_growLength += height; retLen =0; }
    
    bool flag = _growLength < _growUnitHeight;
    switch (dir) {
        case  FaceDirection::FaceTop:
            str+="top;";
            if(flag)cp = _headCur.getNextHeadCursor(_growLength);
            else _headCur.moveNext(_growUnitHeight);
            break;
        case  FaceDirection::FaceLeft:
            str+="left;";
            if(flag)cp = _headCur.getNextHeadCursorByLeft(_growLength);
            else _headCur.moveNextByRotateLeft(_growUnitHeight);
            break;
        case  FaceDirection::FaceRight:
            if(flag)cp = _headCur.getNextHeadCursorByRight(_growLength);
            else _headCur.moveNextByRotateRight(_growUnitHeight);
            str+="right;";
            break;
        default:
            assert(0);
            break;
    }
    
    if (_growLength >= _growUnitHeight) {
        _growLength = 0;
    }
    else
    {
      //  this->_cpLineNode.addContorlPoint(cp.getContorlPoint());
       topCp=cp.getContorlPoint();
        topCp._radius =MIN_WIDTH;
    }
    log(str.c_str());


    _cpLineNode._isDirt =true;
    checkHead();

    return retLen;
}

bool Plant_1::checkRotateAngleCrash(float angle,Vec2 pto,Vec2 pt)
{
    int f = angle<0?-1:1;
    bool ret = false;
    for (int i = 0; i< fabs(angle); i+=10) {
        Vec2 rotvec = MathHelper::getRotatePosition(pto, pt, i*f);
         if(!isCrashPointByMap(CONVERT(rotvec)))
         {
             ret =true;
             break;
         }
    }

    return ret;
}
bool Plant_1::isCanGrowTop()
{
    return !isCashPoint(CONVERT(_headCur.getPositionTLeft(20)))&&
    !isCashPoint(CONVERT(_headCur.getPositionTRight(20)));
    
}
HeadCrashContextV2  Plant_1::testHeadCrash()
{
    
     unsigned char _crashMask = 0;
    if (isCashPoint(CONVERT(_headCur.getPositionBLeft())))_crashMask |= HeadCursorPoint::left_bottom;
    if (isCashPoint(CONVERT(_headCur.getPositionTLeft())))_crashMask |= HeadCursorPoint::left_top;
    if (isCashPoint(CONVERT(_headCur.getPositionBRight())))_crashMask |= HeadCursorPoint::right_bottom;
    if (isCashPoint(CONVERT(_headCur.getPositionTRight())))_crashMask |= HeadCursorPoint::right_top;
    if (isCashPoint(CONVERT(_headCur.getPositionTop())))_crashMask |= HeadCursorPoint::center_top;
    return HeadCrashContextV2(_crashMask);
}

 void Plant_1::checkHead()
{
    return ;
    float head = _headCur._cp._height;
    if (head> this->_cpLineNode._texture->getContentSize().height) {
        for (auto& i:_cpLineNode._cpList) {
            i._height-=this->_cpLineNode._texture->getContentSize().height;
        }
        
        _headCur._cp._height-=this->_cpLineNode._texture->getContentSize().height;
    }
}
 void  Plant_1::checkHeightSub(float len)
{
    float head = _headCur._cp._height;
    if (head> len) {
        for (auto& i:_cpLineNode._cpList) {
            i._height-=len;
        }
        
        _headCur._cp._height-=len;
    }
}

void Plant_1::drawHeadRange(const Mat4 &transform, uint32_t flags)
{
     if (_cpLineNode._cpList.size()==0) return;
     V3F_C4F headVec[9];

    Vec2 cpl = _headCur.getPositionBLeft();
    Vec2 cpo = _headCur.getPosition();
    for (int i=0 ; i<9; i++) {
          headVec[i].vertices = V2TV3(MathHelper::getRotatePosition(cpo, cpl,-20+i*26.6f));
        headVec[i].colors = Color4F(1,0,0,1);
    }
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION |GL::VERTEX_ATTRIB_FLAG_COLOR);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), headVec);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), ((float*)(headVec))+3);
    glDrawArrays(GL_LINE_STRIP, 0,9 );
    
}


