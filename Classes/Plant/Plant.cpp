#include "Plant.h"
#include "TypeBase.h"
#include "GameHeaders.h"
#include "PathListHelper.hpp"
//#define OUT_GROW_DIR 1


static float PathGetLength(const ContorlPointV2& p1,const ContorlPointV2& p2)
{
    return p1._point.getDistance(p2._point);
}
    

static ContorlPointV2 PathGetPoint(float lenght, const ContorlPointV2& start,const ContorlPointV2& end)
{
        Vec2 tstart = start._point;
        Vec2 tend   = end._point;
        float len = fabs( tstart.getDistance(tend) );
        float d = lenght / len;
        float xlen = (tend.x-tstart.x)*d;
        float ylen = (tend.y-tstart.y)*d;
        
        ContorlPointV2 ret = start;
        ret._point =  Vec2(tstart.x+xlen,tstart.y+ylen);
        return ret;
}

void  PlantNode::initPlant(std::list<ContorlPointV2> cpList,float cpListUnitHeight ,float verticesUnitHeight  )
{
    _cpList.clear();
    if(cpListUnitHeight < 0)cpListUnitHeight = GameRuntime::getInstance()->getPlantCPListUnitHeight();
    if(verticesUnitHeight < 0)verticesUnitHeight = GameRuntime::getInstance()->getPlantVerticesUnitHeight();
    auto cp = cpList.front();

    _verticesUnitHeight = verticesUnitHeight;
    _cpListUnitHeight  = cpListUnitHeight;
    _preContorlPoint = cp;
    _cpList.assign(cpList.begin(),cpList.end());
}
bool PlantNode::init()
{
    Node::init();
    auto glprogram  = GLProgram::createWithByteArrays(ccPositionTexture_vert, ccPositionTexture_frag);
//    auto glprogram  = GLProgram::createWithFilenames("ccShader_PositionTexture.vert", "ccShader_PositionTexture.frag");
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(_glprogramstate);
    _ignoreAnchorPointForPosition = false;
    _texture = nullptr;
    setAnchorPoint(Vec2::ZERO);
    _textureLeft = 0;
    _textureRight = 1;

    //opengl
    _isDirt = false;
    _vertices = nullptr;
    _verticesCount = 0;
    _capacity = 0;
    _capacityAddSteplen = 10;
    _isDraw = true;
    return true;
}

void PlantNode::onEnter()
{
    Node::onEnter();
}

 void PlantNode::setTexture(Texture2D* texture)
{
    if (_texture) {
        _texture->release();
    }
    _texture=texture;
    _texture->retain();
}
void PlantNode::onExit()
{
    Node::onExit();
}

void PlantNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    //return ;
    if (!_isDraw) return ;
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(PlantNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}
void PlantNode::onDraw(const Mat4 &transform, uint32_t flags)
{
   
    getGLProgramState()->getGLProgram()->use();
    getGLProgramState()->getGLProgram()->setUniformsForBuiltins(transform);
    Director::getInstance()->setDepthTest(true);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    onDrawPlant(transform,flags);
    Director::getInstance()->setDepthTest(false);
}


float  PlantNode::grow(FaceDirection dir,float height)
{
    float retLen = 0.0f;
#ifdef OUT_GROW_DIR
    std::string str="MOVE  ";
#endif
//    if(_preGrowDirection != dir)
//    {
//        _growLength=0;
//    }

    if( _growLength <= 0.0003 || _cpList.size()==0)
    {
        auto top = _cpList.back();
        _cpList.push_back(_preContorlPoint);
        _preGrowDirection = dir;
        _preContorlPoint._point = top._point;
        _preContorlPoint._height = top._height;
        _preContorlPoint._angle = top._angle;
    }
    float nowGrowLength = 0.0f;
    if (_growLength + height >= _cpListUnitHeight) {
        retLen = height - (_cpListUnitHeight - _growLength);
        nowGrowLength  = _cpListUnitHeight;
        _growLength = 0;
    }
    else {
        _growLength += height;
        nowGrowLength = _growLength;
        retLen =0;
    }
    
    ContorlPointV2 cp;
    switch (dir) {
        case  FaceDirection::FaceTop:
#ifdef OUT_GROW_DIR
            str+="top;";
#endif
            cp = _preContorlPoint.getNextContorlPointV2(nowGrowLength);
            break;
        case  FaceDirection::FaceLeft:
#ifdef OUT_GROW_DIR
            str+="left;";
#endif
           cp = _preContorlPoint.getNextContorlPointV2ByRotateLeft(nowGrowLength);
            break;
        case  FaceDirection::FaceRight:
             cp = _preContorlPoint.getNextContorlPointV2ByRotateRight(nowGrowLength);
#ifdef OUT_GROW_DIR
            str+="right;";
#endif
            break;
        default:
            assert(0);
            break;
    }
#ifdef OUT_GROW_DIR
    log(str.c_str());
#endif

    
    ContorlPointV2& topCP = *_cpList.rbegin();
    
    topCP = cp;

    _isDirt = true;
    
    return retLen;
}

float  PlantNode::reGrow(float height)
{
  
    auto ip = _cpList.rbegin();
  //  auto end =_cpList.rend();
    if (ip == _cpList.rend()) return -1;
    float topHeight = ip->_height;
    float tlen = 0;
    float angle = 0;
    while (ip!=_cpList.rend()) {
        angle = ip->_angle;
        float theight =ip->_height ;
        if (topHeight - theight < height) {
            _cpList.pop_back();
        }
        else break;
        ip++;
    }

   
    float heighttemp = 0;
    if (ip !=_cpList.rend() ) {
        heighttemp = (topHeight - ip->_height)-height;
        angle = ip->_angle;
    }
    ContorlPointV2 cp;
    
    if (ip != _cpList.rend()) {
        float heightStep = heighttemp;
        float nextangle = angle -  ip->_angle;
        FaceDirection dir;
        if (nextangle < -0.0003) {
            dir = FaceLeft;
        }
        else if (nextangle > 0.0003)
        {
            dir = FaceRight;
        }
        else dir = FaceTop;
        ContorlPointV2 tempcp;
        switch (dir) {
            case FaceTop:
                tempcp =   ip->getNextContorlPointV2(heightStep);
                break;
            case FaceLeft:
                tempcp =   ip->getNextContorlPointV2ByRotateLeft(heightStep);
                break;
            case FaceRight:
                tempcp =   ip->getNextContorlPointV2ByRotateRight(heightStep);
                break;
            default:
                break;
        }
        _cpList.push_back(tempcp);
        cp = *ip;
    }
    else cp = _cpList.back();
    
    _preContorlPoint._point = cp._point;
    _preContorlPoint._height = cp._height;
    _preContorlPoint._angle = cp._angle;
    _preContorlPoint._zPosition = cp._zPosition;
    _isDirt = true;
    return 0;
//    auto cp =*_cpList.rbegin();
//    float radius =_headCur._cp._radius;
//    _headCur._cp = cp;
//    _headCur._cp._radius = radius;
//    return 0;
//    _preContorlPoint._height -= height;
//    if (_cpList.size()<=1) {
//        _cpList[0]._height -= height;
//        return 0;
//    }
//    float top = _cpList[_cpList.size()-1]._height;
//    float stepHeight = 0.0;
//    float needHeight = top - height;
//    auto ip = _cpList.rbegin();
//    
//    while (ip!=_cpList.rend()) {
//        if (ip->_height > needHeight) {
//            ip++;
//            _cpList.pop_back();
//            continue;
//        }
//        else{
//            float t = top - ip->_height;
//            float t1 = ip->_height - needHeight;
//            
//        }
//    }
//    for (int i =0 ; i<_cpList.size(); i++) {
//        if (_cpList[i]._height <= needHeight) {
//            
//        }
//        
//    }
    
}
ContorlPointV2 PlantNode::getNextContorlPoint(float len,FaceDirection dir)
{
    ContorlPointV2 cp;
    switch (dir) {
        case  FaceDirection::FaceLeft:
            cp = _preContorlPoint.getNextContorlPointV2ByRotateLeft(len);
            break;
        case  FaceDirection::FaceRight:
            cp = _preContorlPoint.getNextContorlPointV2ByRotateRight(len);
            break;
        case  FaceDirection::FaceTop:
            cp = _preContorlPoint.getNextContorlPointV2(len);
            break;
        default:
            assert(0);
            break;
    }
    return cp;
}
ContorlPointV2 PlantNode::getNextContorlPointInWorld(float len,FaceDirection dir)
{
    ContorlPointV2 cp =getNextContorlPoint(len,dir);
    cp._point =convertToWorldSpace(cp._point);
    return  cp;
}
ContorlPointV2 PlantNode::getNextContorlPointByTopCp(float len,FaceDirection dir)
{
    ContorlPointV2 cp;
    auto topCp  = getTopCP();
    switch (dir) {
        case  FaceDirection::FaceLeft:
            cp = topCp.getNextContorlPointV2ByRotateLeft(len);
            break;
        case  FaceDirection::FaceRight:
            cp = topCp.getNextContorlPointV2ByRotateRight(len);
            break;
        case  FaceDirection::FaceTop:
            cp = topCp.getNextContorlPointV2(len);
            break;
        default:
            assert(0);
            break;
    }
    return cp;
}
ContorlPointV2 PlantNode::getNextContorlPointByTopCpInWorld(float len,FaceDirection dir)
{
    ContorlPointV2 cp =getNextContorlPointByTopCp(len,dir);
    cp._point =convertToWorldSpace(cp._point);
    return  cp;
}
Vec2 PlantNode::getHeadNextPosition(float len,FaceDirection dir)
{
    return getNextContorlPoint(len,dir)._point;
}
Vec2 PlantNode::getHeadNextPositionByTopCP(float len,FaceDirection dir)
{
    return getNextContorlPointByTopCp(len, dir)._point;
}


ContorlPointV2 PlantNode::getContorlPointByHeight(float height,bool isTop)
{
    ContorlPointV2 ret ;
    PathListHelper::PathList<ContorlPointV2,CPListType > list(&_cpList);
    list.PathGetLength = PathGetLength;
    list.PathGetPoint =  PathGetPoint;
  //  PathListHelper::PathList<Vec2,std::list<Vec2> > list(nullptr);
    list.getTypePointByLength(height,isTop,&ret);
    int index =-1;
    for (int i =0; i<_cpList.size();i++) {
        if (height < _cpList[i]._height && i != 0) {
            index = i-1;
            break;
        }
    }
    if (index>=0) {
        float heightStep = height - _cpList[index]._height;
        float nextangle = _cpList[index+1]._angle -  _cpList[index]._angle;
        FaceDirection dir;
        if (nextangle < -0.0003) {
            dir = FaceLeft;
        }
        else if (nextangle > 0.0003)
        {
            dir = FaceRight;
        }
        else dir = FaceTop;
        switch (dir) {
            case FaceTop:
                ret =   _cpList[index].getNextContorlPointV2(heightStep);
                break;
            case FaceLeft:
                ret =   _cpList[index].getNextContorlPointV2ByRotateLeft(heightStep);
                 break;
            case FaceRight:
                 ret =   _cpList[index].getNextContorlPointV2ByRotateRight(heightStep);
                  break;
            default:
             break;
         }
    }
    return ret;
}
ContorlPointV2 PlantNode::getContorlPointByHeightInWorld(float height,bool isTop)
{
    auto cp = getContorlPointByHeight(height,isTop);
    cp._point = this->convertToWorldSpace( cp._point);
    return cp;
}
void PlantNode::subContorlPoint(int yHeight)
{
    auto ip = _cpList.begin();

    while (ip != _cpList.end()) {
        if (ip->_point.y<yHeight) {ip=_cpList.erase(ip);continue;}
        else break;
      //  ip++;
    }
    while(ip != _cpList.end())
    {
        ip->_point.y -= yHeight;
        ip++;
    }
    _preContorlPoint._point.y-=yHeight;
    _isDirt = true;
}
void PlantNode::subTextureHeight(int yHeight)
{
    auto ip = _cpList.begin();
    while (ip != _cpList.end()) {
        ip->_height -= yHeight;
        ip++;
    }
    _preContorlPoint._height-=yHeight;
}
void PlantNode::onDrawPlant(const Mat4 &transform, uint32_t flags)
{
    updateVertices();
    if(_verticesCount <= 2 )return;

    GL::bindTexture2D( _texture->getName());
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(_vertices))+3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,_verticesCount );
}

void PlantNode::ensureCapacity(int count)
{
    if(count >= _capacity)
    {
        int size = ((count+_capacityAddSteplen)*2);
        if(!_vertices)
        {
            _vertices = (V3F_T2F*)malloc(size*sizeof(V3F_T2F));
        }
        else _vertices = (V3F_T2F*)realloc(_vertices,size*sizeof(V3F_T2F));
        
        _capacity = size;
    }
//    if (_cpList.size() <= 0 ) return;
//    //int size = _cpList.size()*2+((*_cpList.rbegin())._height/_verticesUnitHeight)*2;
//    float height1 = (*_cpList.rbegin())._height;
//     float height2 = (*_cpList.begin())._height;
//    int step = _cpListUnitHeight/ _verticesUnitHeight;
//    int unitCount =  step > 1 ? step : 1;
//    int size =((height1-height2)/unitCount)*2+ _cpList.size()*2;
//    if(size >= _capacity)
//    {
//        if (_vertices) { delete [] _vertices;}
//        _vertices = new V3F_T2F[size+_capacityAddSteplen];
//        _capacity = size+_capacityAddSteplen;
//    }
}
#define V3TV2(T) Vec2(T.x,T.y)
void PlantNode::addVertices(const ContorlPointV2& cp)
{
//    if (_verticesCount >= _capacity) {
//        return ;
//        
//    }
    ensureCapacity(_verticesCount+2);
    Vec2 vl,vr;
    vl = cp.getPositionLeft();
    vr = cp.getPositionRight();
    _vertices[_verticesCount].vertices = MakeVec3(vl,cp._zPosition);
    _vertices[_verticesCount].texCoords = getTex2FCPLeft(cp);
    
    _vertices[_verticesCount+1].vertices = MakeVec3(vr,cp._zPosition);
    _vertices[_verticesCount+1].texCoords = getTex2FCPRight(cp);
    _verticesCount += 2;
}
void PlantNode::updateVertices()
{
    if (_cpList.size() <2 || !_isDirt)return;
    _isDirt = false;
  // ensureCapacity(1);
    _verticesCount = 0;
    
    ContorlPointV2* pre = &_cpList[0];
    int  preIndex = 0;
    float topHeight = _cpList.back()._height;
    for (int i = 0 ; i<_cpList.size(); i++) {
        //addVertices(_cpList[i]);
   
        float heightStep = _cpList[i]._height -   _cpList[preIndex]._height;
        float theigh = 0.0;
        float nextangle = _cpList[i]._angle -   _cpList[preIndex]._angle;
        FaceDirection dir;
        if (nextangle < -0.0003) {
            dir = FaceLeft;
        }
        else if (nextangle > 0.0003)
        {
            dir = FaceRight;
        }
        else dir = FaceTop;
//        ContorlPointV2 tcp = _cpList[i];
//        pre->_radius = _updateCPRadiusCall(topHeight - pre->_height);
//        addVertices(*pre);
//        theigh += _verticesUnitHeight;
        while(theigh < heightStep)
        {
            ContorlPointV2 cp ;
            switch (dir) {
                case FaceTop:
                    cp =   _cpList[preIndex].getNextContorlPointV2(theigh);
                    break;
                case FaceLeft:
                    cp =   _cpList[preIndex].getNextContorlPointV2ByRotateLeft(theigh);
                    break;
                case FaceRight:
                    cp =   _cpList[preIndex].getNextContorlPointV2ByRotateRight(theigh);
                    break;
                default:
                    break;
            }
       
            cp._radius = _updateCPRadiusCall(topHeight - cp._height);
            addVertices(cp);
            theigh += _verticesUnitHeight;
        }
//        _cpList[i]._radius =  _updateCPRadiusCall(topHeight - _cpList[i]._height);
//        addVertices(_cpList[i]);
        preIndex = i;
        //pre = &_cpList[i];
    }
    
}

float PlantNode::getLengthByCP(ContorlPointV2& cp,bool isTop )
{
    auto ip = _cpList.rbegin();
    auto end = _cpList.rend();
    
    auto pre =ip;
    float len = 0;
    while (ip!=end) {
        if (ip->_height < cp._height) {
            break;
        }
        len += pre->_point.distance(ip->_point);
        pre = ip;
        ip++;
    }
    len += pre->_point.distance(cp._point);
    return len;
}

