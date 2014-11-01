#ifndef __Grow_Demo__CPLineNode__
#define __Grow_Demo__CPLineNode__
#include "common.h"
#include "HeadCursor.h"
#include "MathHelper.h"
#include "ContorlPoint.h"
#define DEFAUT_ADDLEN 100
struct V3F_C4F
{
    //! vertices (3F)
    Vec3     vertices;            // 12 bytes
    //! colors (4B)
    Color4F      colors;              // 4 bytes
};

class CPLineNode
{
public:
    CPLineNode():_texture(nullptr),_vertices(nullptr),_capacity(0),_verticesUnitLength(1),
    _capacityAddSteplen(DEFAUT_ADDLEN),_verticesCount(0),_borderVerticesLeft(nullptr),_isDirt(false){
     
    }
    CPLineNode(Texture2D* texture,int unitLength):_borderVerticesLeft(nullptr),_texture(texture),_vertices(nullptr),_capacity(0),_verticesUnitLength(unitLength),_isDirt(false),
    _capacityAddSteplen(DEFAUT_ADDLEN),_verticesCount(0){}
    ~CPLineNode(){}
    
    void  setTexture(Texture2D* texture){this->_texture = texture; }
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawBorder(const Mat4 &transform, uint32_t flags);
    
    void addContorlPoint(const ContorlPointV2& cp)
    {
        this->_cpList.push_back(cp);
        _isDirt =true;
    }
    void addContorlPointEditWidth(const ContorlPointV2& cp,float radius)
    {
        this->_cpList.push_back(ContorlPointV2(cp._point,radius,cp._angle,cp._height,cp._zPosition));
        _isDirt =true;
    }
    Tex2F getTex2FCPLeft(const ContorlPointV2& cp)
    {
        return Tex2F(0,cp._height/_texture->getContentSize().height);
    }
    Tex2F getTex2FCPRight(const ContorlPointV2& cp)
    {
        return Tex2F(1,cp._height/_texture->getContentSize().height);
    }
    
    void  setVerticesUnitLength(float len)
    {
        _verticesUnitLength = len;
        _verticesUnitLengthAngle = MathHelper::getRotateAngle(_verticesUnitLength,_rotateLength);
    }
    void clear()
    {
        _cpList.clear();
    }
    ContorlPointV2& getTopCP()
    {
        return *(_cpList.rbegin());
    }
    
    void ensureCapacity(int count);
    void updateVertices();
    void addVertices(const ContorlPointV2& cp);
    void subContorlPoint(int y);
public:
    std::vector<ContorlPointV2> _cpList;
    Texture2D*      _texture;
    float           _verticesUnitLength;
    float           _verticesUnitLengthAngle;
    
    float           _rotateLength;
    bool            _isDirt;
    //opengl
    unsigned int    _verticesCount;
    unsigned int    _capacity;
    unsigned        _capacityAddSteplen;
    V3F_T2F*        _vertices;
    V3F_C4F*        _borderVerticesLeft;

    GLuint          _vao;
    GLuint          _vbo;
    unsigned int    _vboSize;
    bool            _isFirstload;
};

#endif /* defined(__Grow_Demo__CPLineNode__) */
