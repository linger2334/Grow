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
        }
        _vertices = new V3F_T2F[size+_capacityAddSteplen];
        _capacity = size+_capacityAddSteplen;
    }
}
void CPLineNode::addVertices(const ContorlPointV2& cp)
{
    _vertices[_verticesCount].vertices = MakeVec3(cp.getPositionLeft(),cp._zPosition);
    _vertices[_verticesCount].texCoords = getTex2FCPLeft(cp);
    
    _vertices[_verticesCount+1].vertices = MakeVec3(cp.getPositionRight(),cp._zPosition);
    _vertices[_verticesCount+1].texCoords = getTex2FCPRight(cp);
    _verticesCount += 2;
}
void CPLineNode::updateVertices()
{
    if (_cpList.size()==0 || !_isDirt)return;
    
    ensureCapacity(1);
    _verticesCount = 0;

    for (int i = 0 ; i<_cpList.size(); i++) {
        addVertices(_cpList[i]);
    }
    
}

void CPLineNode::onDraw(const Mat4 &transform, uint32_t flags)
{
    updateVertices();
    if(_verticesCount <= 0 )return;
   // Director::getInstance()->setDepthTest(true);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL::bindTexture2D( _texture->getName());
   // glEnable(GL_CULL_FACE);
    //////////////////////////
//    V3F_T2F vect[40];
//    int i =0;

//    vect[i++].vertices =Vec3(0,100+8,0);
//    vect[i].texCoords = Tex2F(0,1);
//    vect[i++].vertices =Vec3(100,100+8,0);
//    vect[i].texCoords = Tex2F(1,1);
//    
//    vect[i++].vertices =Vec3(0,100+16,0);
//    vect[i].texCoords = Tex2F(0,1-step*1);
//    vect[i++].vertices =Vec3(100,100+16,0);
//    vect[i].texCoords = Tex2F(0,1-step*1);
//    
//    vect[i++].vertices =Vec3(0,100+24,0);
//    vect[i].texCoords = Tex2F(0,step*2);
//    vect[i++].vertices =Vec3(100,100+24,0);
//    vect[i].texCoords = Tex2F(1,step*2);
//    
//    vect[i++].vertices =Vec3(0,100+32,0);
//    vect[i].texCoords = Tex2F(0,step*1);
//    vect[i++].vertices =Vec3(100,100+32,0);
//    vect[i].texCoords = Tex2F(1,step*1);
//    
//    vect[0].vertices =Vec3(0,100,0);
//    vect[1].vertices =Vec3(100,100,0);
    
//        V3F_T2F vect[128];
//    float len =sqrt(8.0f*8.0f+8.0f*8.0f);
//       float step = len/_texture->getContentSize().height;
//    
//    for(int i =0;i<5;i++)
//    {
//        Vec3 vl =Vec3(100,100-i*8,0);
//        Vec3 vt = vl + Vec3(-32,0,0);
//        //Vec3 v1 =MathHelper::getRotatePosition(vl,vt, 45);
//        vect[i*2].vertices = vt;
//        vect[i*2].texCoords = Tex2F(0,1+i*step);
//        vect[i*2+1].vertices = vl;
//        vect[i*2+1].texCoords = Tex2F(1,1+i*step);
//    }
//    for(int i=0;i<8;i++)
//    {
//       
//        Vec3 vl =Vec3(100+i*8,100+i*8,0);
//        Vec3 vt = vl + Vec3(-32,0,0);
//        Vec3 v1 =MathHelper::getRotatePosition(vl,vt, 45);
//        vect[(i+5)*2].vertices = v1;
//        vect[(i+5)*2].texCoords = Tex2F(0,1-i*step);
//        vect[(i+5)*2+1].vertices = vl;
//        vect[(i+5)*2+1].texCoords = Tex2F(1,1-i*step);
//    }

    //////////////////////////
    
    
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
   // glClearColor(1, 1, 1, 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F), ((float*)(_vertices))+3);

    glDrawArrays(GL_TRIANGLE_STRIP, 0,_verticesCount );
    //Director::getInstance()->setDepthTest(false);
}