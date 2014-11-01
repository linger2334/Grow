#include "CPLineNode.h"
#include "MathHelper.h"


void CPLineNode::ensureCapacity(int count)
{
    if (_cpList.size() <= 0 ) return;
    int size = _cpList.size()*2+((*_cpList.rbegin())._height/_verticesUnitLength)*2;

    if(size > _capacity)
    {
        if (_vertices) { delete [] _vertices;}
        _vertices = new V3F_T2F[size+_capacityAddSteplen];
        _capacity = size+_capacityAddSteplen;
    }
}
#define V3TV2(T) Vec2(T.x,T.y)
void CPLineNode::addVertices(const ContorlPointV2& cp)
{
    Vec2 vl,vr;
    vl = cp.getPositionLeft();
    vr = cp.getPositionRight();
    _vertices[_verticesCount].vertices = MakeVec3(vl,cp._zPosition);
    _vertices[_verticesCount].texCoords = getTex2FCPLeft(cp);

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
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(_vertices))+3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,_verticesCount );

}






