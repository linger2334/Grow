#ifndef __PlantOri__PlantLine__
#define __PlantOri__PlantLine__
#include "MathHelper.h"
#include "HeadCursor.h"
#include "CrashTestHelper.h"
#include "ContorlPoint.h"

class PlantLine :public Node
{
public:
    CREATE_FUNC(PlantLine);
    virtual bool init();
    virtual void update(float dt);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    virtual void  onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void  onExit();
    
    void  initGameInfo();
    
    void resetPlant();
    
    bool isCanGrowLeft();
    bool isCanGrowRight();
    bool isCanGrowTop();
    bool isHeadCanRotate(int  endAngle);
    bool growOneUnit(FaceDirection dir);
    bool grow(FaceDirection dir,float lenght);
    bool grow(float angle);
    
    void drawHead();
    void updateHead(float dt);
    
    int  getHeadTopCrashGridCount(int checkCount,bool crash = true);
    void addControlPoint(Vec3 pt,float width,float angle,float height)
    {
        addContorlPoint(ContorlPoint(pt,width,angle,height));
    }
    void addContorlPoint(const ContorlPoint& cp)
    {
        this->_cpList.push_back(cp);
    }
    
    void addContorlPointEditWidth(const ContorlPoint& cp,float radius)
    {
        this->_cpList.push_back(ContorlPoint(cp._point,radius,cp._angle,cp._height));
    }

    void addVertices(const ContorlPoint& cp);
    
    void updateVertices(unsigned int start,unsigned int length);
    void ensureCapacity(int count);
    
    Vec3 convertToPositionInPixel(Vec3 vec)
    {
        return Vec3(vec.x*_pixelSize.width,
                    vec.y*_pixelSize.height,
                    vec.z);
    }
    
    Tex2F getTex2FCPLeft(const ContorlPoint& cp)
    {
        return Tex2F(0,cp._height/_texture->getContentSize().height);
    }
    Tex2F getTex2FCPRight(const ContorlPoint& cp)
    {
        return Tex2F(1,cp._height/_texture->getContentSize().height);
    }
    
    bool isCrash(cocos2d::Vec3 vec)
    {
        return _crashTestHelper->isCrashAbsolute(this->convertToWorldSpace(Vec2(vec.x,vec.y)));
    }
    
    void subContorlPoint(int y);
    
    float  getHeadHeight();
    
    cocos2d::Point           _startPoint;
    bool            _dirt;
    HeadCursor      _headCur;
    cocos2d::Size            _pixelSize;
    Texture2D*      _texture;
    CustomCommand   _customCommand;
    ContorlPoint    _cpHead;
    std::vector<ContorlPoint> _cpList;
    CrashTestHelper*  _crashTestHelper;
    float             _cpUnitHeight;
    float             _verticesStepUnitHeight;
    
    Texture2D*      _textureHead;
    
    float             _growLength;
    //opengl
    unsigned int    _verticesCount;
    unsigned int    _capacity;
    unsigned        _capacityAddSteplen;
    V3F_T2F*        _vertices;
    GLuint          _vao;
    GLuint          _vbo;
    unsigned int    _vboSize;
    cocos2d::Size            _gridUnitSize;
    bool            _isFirstload;
};



#endif /* defined(__PlantOri__PlantLine__) */
