#include "Plant1.h"
#include "TypeBase.h"
#define CONVERT(T) convertToWorldSpace(T)

#define V2TV3(T) Vec3(T.x,T.y,0)

bool Plant_1::init()
{
    Node::init();
    auto glprogram = GLProgram::createWithFilenames("ccShader_PositionTexture.vert", "ccShader_PositionTexture.frag");
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    setGLProgramState(_glprogramstate);
    _ignoreAnchorPointForPosition = true;
    
#ifdef DRAW_BORDER_POINTS
    _borderProgramState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_COLOR);
    _borderProgramState->retain();
#endif
    
    _tex = nullptr;
    _cpLineNode = CPLineNode(_tex,8);
    _headFaceDir= FaceDirection::FaceTop;
    _world = GameManager::getInstance()->getBox2dWorld() ;
    return true;
}
void Plant_1::onEnter()
{
    if(!_tex)
    {
        _tex = Director::getInstance()->getTextureCache()->addImage("test_plant_1024.png");
        Texture2D::TexParams  parms = { GL_LINEAR,GL_LINEAR, GL_REPEAT,GL_REPEAT};
        _tex->setTexParameters(parms);
        _tex->retain();
        _cpLineNode.setTexture(_tex);
    }
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
        if (ip->_point.y<yHeight) {ip=_cpLineNode._cpList.erase(ip);continue;}
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
    drawHeadRange(transform,flags);
#endif
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //  glBlendFunc(GL_ONE, GL_ZERO);
    _cpLineNode.onDraw(transform,flags);
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
        topCp=cp.getContorlPoint();
        topCp._radius =MIN_WIDTH;
    }
    log(str.c_str());
    _cpLineNode._isDirt =true;
    return retLen;
}
Vec2 Plant_1::getNextGrowUnitLengthPosition(FaceDirection growDir)
{
    Vec2 ret ;
    switch (growDir) {
        case FaceLeft:
            ret = _headCur.getNextHeadCursorByLeft(_growUnitHeight).getPosition();
            break;
        case FaceRight:
              ret = _headCur.getNextHeadCursorByRight(_growUnitHeight).getPosition();
            break;
        case FaceTop:
              ret = _headCur.getNextHeadCursor(_growUnitHeight).getPosition();
            break;
        default:
            log("getNextGrowUnitLengthPosition Dir Error");
            assert(0);
            break;
    }
    return ret;
}
 void  Plant_1::checkHeightSub(float len)
{
    float head = _headCur._cp._height;
    if (head> len) {
        for (auto& i:_cpLineNode._cpList) { i._height-=len; }
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
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), headVec);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), ((float*)(headVec))+3);
    glDrawArrays(GL_LINE_STRIP, 0,9);
    
    V3F_C4F headVec1[6];
    float anglevec[3]={-40,0,40};
    Vec2 cpt = _headCur._cp.getTopPositionByLength(100);
    for (int i=0; i<3; i++) {
        headVec1[i*2].vertices = V2TV3(_headCur.getPosition());
        headVec1[i*2].colors = Color4F(1,0,0,1);
        headVec1[i*2+1].vertices = V2TV3(MathHelper::getRotatePosition(cpo, cpt,anglevec[i]));
        headVec1[i*2+1].colors = Color4F(0,1,0,1);
    }
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), headVec1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), ((float*)(headVec1))+3);
    glDrawArrays(GL_LINES, 0,6);
}


