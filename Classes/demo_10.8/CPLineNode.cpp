#include "CPLineNode.h"
#include "MathHelper.h"


void CPLineNode::ensureCapacity(int count)
{
    if (_cpList.size() <= 0 ) return;
    int size = _cpList.size()*2+((*_cpList.rbegin())._height/_verticesUnitLength)*2;

    if(size > _capacity)
    {
        if (_vertices) {
            delete [] _vertices;
#ifdef DRAW_BORDER_POINTS

            delete [] _borderVerticesLeft;
#endif
        }
        _vertices = new V3F_T2F[size+_capacityAddSteplen];
#ifdef DRAW_BORDER_POINTS
            _borderVerticesLeft = new V3F_C4F[size+_capacityAddSteplen];
#endif
 
        _capacity = size+_capacityAddSteplen;
    }
}
///------------alg 2------------
//叉积
double mult(Point a, Point b, Point c)
{
    return (a.x-c.x)*(b.y-c.y)-(b.x-c.x)*(a.y-c.y);
}

//aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false
bool intersect(Point aa, Point bb, Point cc, Point dd)
{
    if ( std::max(aa.x, bb.x)<std::min(cc.x, dd.x) )
    {
        return false;
    }
    if ( std::max(aa.y, bb.y)<std::min(cc.y, dd.y) )
    {
        return false;
    }
    if ( std::max(cc.x, dd.x)<std::min(aa.x, bb.x) )
    {
        return false;
    }
    if ( std::max(cc.y, dd.y)<std::min(aa.y, bb.y) )
    {
        return false;
    }
    if ( mult(cc, bb, aa)*mult(bb, dd, aa)<0 )
    {
        return false;
    }
    if ( mult(aa, dd, cc)*mult(dd, bb, cc)<0 )
    {
        return false;
    }
    return true;
}
#define V3TV2(T) Vec2(T.x,T.y)
void CPLineNode::addVertices(const ContorlPointV2& cp)
{
    
    Vec2 vl,vr;
    vl = cp.getPositionLeft();
    vr = cp.getPositionRight();
//    if (_verticesCount>2) {
//        if (intersect(V3TV2(_vertices[_verticesCount-2].vertices),V3TV2(_vertices[_verticesCount-1].vertices),vl,vr)) {
//            vl = V3TV2(_vertices[_verticesCount-2].vertices);
//            vr = V3TV2(_vertices[_verticesCount-1].vertices);
//        }
//    }
    
    _vertices[_verticesCount].vertices = MakeVec3(vl,cp._zPosition);
    _vertices[_verticesCount].texCoords = getTex2FCPLeft(cp);
#ifdef DRAW_BORDER_POINTS
    _borderVerticesLeft[_verticesCount/2].vertices = _vertices[_verticesCount].vertices;
    _borderVerticesLeft[_verticesCount/2].colors = Color4F(1,0,0,1);
#endif
    _vertices[_verticesCount+1].vertices = MakeVec3(vr,cp._zPosition);
    _vertices[_verticesCount+1].texCoords = getTex2FCPRight(cp);
    _verticesCount += 2;
}
void CPLineNode::updateVertices()
{
    if (_cpList.size()==0 || !_isDirt)return;
    
    ensureCapacity(1);
    _verticesCount = 0;

    auto pre =_cpList[0];
    for (int i = 0 ; i<_cpList.size(); i++) {

        pre = _cpList[i];
        addVertices(_cpList[i]);
    }
    
}

void CPLineNode::onDraw(const Mat4 &transform, uint32_t flags)
{
    updateVertices();
    if(_verticesCount <= 0 )return;

    
    
   // glBlendFunc(GL_ONE, GL_ZERO);
 
    GL::bindTexture2D( _texture->getName());
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    //glClearColor(, 1, 1, 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(_vertices))+3);

    glDrawArrays(GL_TRIANGLE_STRIP, 0,_verticesCount );

}
void CPLineNode::onDrawBorder(const Mat4 &transform, uint32_t flags)
{
    updateVertices();
    if(_verticesCount <= 0 )return;
    
      glBlendFunc(GL_ONE, GL_ZERO);
  //  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION |GL::VERTEX_ATTRIB_FLAG_COLOR);
  
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), _borderVerticesLeft);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(V3F_C4F), ((float*)(_borderVerticesLeft))+3);
    
    glDrawArrays(GL_POINTS, 0,_verticesCount/2.0f );
}





