#include "Plant1.h"
#define CONVERT(T) convertToWorldSpace(T)

#define V2TV3(T) Vec3(T.x,T.y,0)
#define HEAD_COUNT 60
bool Plant_1::init()
{
    Node::init();
   
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE));
      tex = Director::getInstance()->getTextureCache()->addImage("plant1_4_01.png");
    _ignoreAnchorPointForPosition = true;
     //tex = Director::getInstance()->getTextureCache()->addImage("plant1_4_01.png");
    Texture2D::TexParams  parms = {GL_LINEAR, GL_LINEAR, GL_REPEAT,GL_REPEAT};
    tex->setTexParameters(parms);
    tex->retain();
    _cpLineNode = CPLineNode(tex,8);
    _headFaceDir= FaceDirection::FaceTop;
    return true;
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
}

void Plant_1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(Plant_1::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
void Plant_1::onDraw(const Mat4 &transform, uint32_t flags)
{
    //auto pt =this->getPosition();
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    
    drawHead();
    _cpLineNode.onDraw(transform,flags);

}

GrowContext Plant_1::getCanGrowContext()
{
    GrowContext canGrow;
    HeadCrashContextV2 crash = testHeadCrash();
//    if (((!crash.isCrashBLeft()&&!crash.isCrashTLeft())||
//        (_headCur.getAngle() > 80 && crash.isCrashTLeft()&&!crash.isCrashBLeft()))&&_headCur.getAngle() > -130)canGrow._left=true;
//    if (((!crash.isCrashBRight()&&!crash.isCrashTRight())||
//         (_headCur.getAngle() < -40 &&crash.isCrashTRight()&&!crash.isCrashBRight()))&&
//        _headCur.getAngle() < 130) {
//        canGrow._right=true;
//    }
//    if (!crash.isCrashTop()&&!crash.isCrashTLeft()&&!crash.isCrashTRight()) {
//          canGrow._top=true;
//    }
    //modify by wlg 2014.9.22 Angle 原为130
    if (!crash.isCrashBLeft()&&!crash.isCrashTLeft()&&_headCur.getAngle() > -90)
        canGrow._left=true;
    if (!crash.isCrashBRight()&&!crash.isCrashTRight()&&
        _headCur.getAngle() < 90) {
        canGrow._right=true;
    }
    if (!crash.isCrashTop()&&!crash.isCrashTLeft()&&!crash.isCrashTRight()) {
        canGrow._top=true;
    }
    return canGrow;
}
#define HEIDE_STEP 4
int  Plant_1::getHeadTopCrashGridCount(int checkCount,bool crash)
{
    int ret = 0;
    Vec2 top = _headCur.getPositionTop();
    if (crash) {
        for (int i = 1; i < HEIDE_STEP; i++) {
            Vec2 tem = top+Vec2(0,-8);
            if(!_crashTestCallBack(CONVERT(tem)))break;
            top = tem;
        }
    }
    int f = checkCount > 0 ? 1 : -1;
    for (int i = 1; i < abs(checkCount); i++)
    {
        if (crash&&!_crashTestCallBack(CONVERT(top+Vec2(f*i*8,0))))break;
        else if(!crash&&_crashTestCallBack(CONVERT(top+Vec2(f*i*8,0))))break;
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
        _growLength = _growUnitHeight;
        retLen = height-(_growUnitHeight - _growLength);
    }
    else{_growLength += height;}
    
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
       topCp=cp.getContorlPoint();
        topCp._radius =MIN_WIDTH;
    }
   // log(str.c_str());
    
    _cpLineNode._isDirt =true;
    checkHead();
    if(retLen>0)
    {
        _turnLen+=retLen;
        _turnLen*=1.2;
        if (_turnLen>HEAD_COUNT) {
            _turnLen=HEAD_COUNT;
        }
    }
    return retLen;
}


HeadCrashContextV2  Plant_1::testHeadCrash()
{
    
     unsigned char _crashMask = 0;
    if (_crashTestCallBack(CONVERT(_headCur.getPositionBLeft())))_crashMask |= HeadCursorPoint::left_bottom;
    if (_crashTestCallBack(CONVERT(_headCur.getPositionTLeft())))_crashMask |= HeadCursorPoint::left_top;
    if (_crashTestCallBack(CONVERT(_headCur.getPositionBRight())))_crashMask |= HeadCursorPoint::right_bottom;
    if (_crashTestCallBack(CONVERT(_headCur.getPositionTRight())))_crashMask |= HeadCursorPoint::right_top;
    if (_crashTestCallBack(CONVERT(_headCur.getPositionTop())))_crashMask |= HeadCursorPoint::center_top;
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
void Plant_1::drawHead()
{
    if (_cpLineNode._cpList.size()==0) return;

    const auto& cp =*_cpLineNode._cpList.rbegin();
    ContorlPointV2 cp1(cp.getTopPositionByLength(_headCur._topHeight),cp._radius,cp._angle,cp._height);
    

    static Vec3  *headVec = new Vec3[HEAD_COUNT*2];
    static Tex2F *text1 = new Tex2F[HEAD_COUNT*2];
    memset(headVec, 0,sizeof(Vec3)*HEAD_COUNT*2);
    memset(text1, 0,sizeof(Tex2F)*HEAD_COUNT*2);
    HeadCursorV2 headCur(cp,HEAD_COUNT,_headCur._rotateLenght);

    for(int i = 0; i<HEAD_COUNT;i++)
    {
        auto cp2 = headCur.getContorlPoint();
        headVec[i*2] = V2TV3(cp2.getPositionLeft());
        text1[i*2] = this->_cpLineNode.getTex2FCPLeft(cp2);
        headVec[i*2+1] = V2TV3(cp2.getPositionRight());
        text1[i*2+1] = this->_cpLineNode.getTex2FCPRight(cp2);

        if(_headFaceDir == FaceDirection::FaceLeft &&i > HEAD_COUNT- _turnLen )
        {
            headCur.moveNextByRotateLeft(1);
        }
        else if(_headFaceDir== FaceDirection::FaceRight && i> HEAD_COUNT- _turnLen)
        {
            headCur.moveNextByRotateRight(1);
        }
        else if(_headFaceDir== FaceDirection::FaceTop && i >  _turnLen)
        {
            if (_preHeadFaceDir ==  FaceDirection::FaceLeft) {
                headCur.moveNextByRotateLeft(1);
            }
            else if (_preHeadFaceDir ==  FaceDirection::FaceRight)
            {
                headCur.moveNextByRotateRight(1);
            }
        }
        else headCur.moveNext(1);
        
        headCur._cp._radius-= 28.0f/(float)HEAD_COUNT;

    }

    GL::bindTexture2D( _cpLineNode._texture->getName());
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, headVec);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, text1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, HEAD_COUNT*2);
    
}