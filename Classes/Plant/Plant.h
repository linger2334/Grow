#ifndef __Grow_Demo__Plant1__
#define __Grow_Demo__Plant1__
#include "common.h"
#include "ContorlPoint.h"
class PlantNode : public Node
{
public:
    typedef std::vector<ContorlPointV2> CPListType ;
    virtual ~PlantNode()
    {
       if(_texture) _texture->release();
    }
    CREATE_FUNC(PlantNode);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawPlant(const Mat4 &transform, uint32_t flags);
    
    void initPlant(std::list<ContorlPointV2> cpList,float cpListUnitHeight = -1,float verticesUnitHeight = -1 );
    void resetStartContorlPoint(ContorlPointV2 cp);
    void setTexture(Texture2D* texture);
    
    void editTopCP(const ContorlPointV2& cp)
    {
        if(_cpList.empty())return;
        _cpList[_cpList.size()-1] = cp;
    }
    
    float  grow(FaceDirection dir,float height);
    float  reGrow(float height);
    
    void addContorlPoint(const ContorlPointV2& cp)
    {
        this->_cpList.push_back(cp);
        _isDirt =true;
    }
    
    Tex2F getTex2FCPLeft(const ContorlPointV2& cp)
    {
        return Tex2F(_textureLeft,cp._height/_texture->getContentSize().height);
    }
    
    Tex2F getTex2FCPRight(const ContorlPointV2& cp)
    {
        return Tex2F(_textureRight,cp._height/_texture->getContentSize().height);
    }
    
    ContorlPointV2& getTopContorlPoint(){return *(_cpList.rbegin());}
    ContorlPointV2& getTopUintContorlPoint(){return _preContorlPoint;}
    ContorlPointV2 getTopUintContorlPointInWorld()
    {
        auto cp = getTopUintContorlPoint();
        cp._point = this->convertToWorldSpace(cp._point);
        return cp;
    }
    float getHeadAnalge(){return  _cpList.rbegin()->_angle;}

    Vec2 getHeadNextPosition(float len,FaceDirection dir);
    Vec2 getHeadNextPositionInWorld(float len,FaceDirection dir){return this->convertToWorldSpace(getHeadNextPosition(len,dir));}
    
    Vec2 getHeadNextPositionByTopCP(float len,FaceDirection dir);
    Vec2 getHeadNextPositionByTopCPInWorld(float len,FaceDirection dir){return this->convertToWorldSpace(getHeadNextPositionByTopCP(len,dir));}
    
  //Vec2 getHeadPosition(){return _preContorlPoint._point;}
    Vec2 getHeadUnitPosition(){return _preContorlPoint._point;}
    Vec2 getHeadUnitPositionInWorld(){return this->convertToWorldSpace(getHeadUnitPosition());}
    Vec2 getHeadPosition(){return _cpList.rbegin()->_point;}
    Vec2 getHeadPositionInWorld(){return this->convertToWorldSpace(getHeadPosition());}
    
    
    void clearCPList(){_cpList.clear();}
    float getTopCpHeight(){return _cpList.rbegin()->_height; }
    ContorlPointV2& getTopCP(){return *(_cpList.rbegin());}
    
    ContorlPointV2 getNextContorlPoint(float len,FaceDirection dir);
    ContorlPointV2 getNextContorlPointInWorld(float len,FaceDirection dir);
    
    ContorlPointV2 getNextContorlPointByTopCp(float len,FaceDirection dir);
    ContorlPointV2 getNextContorlPointByTopCpInWorld(float len,FaceDirection dir);
    
    ContorlPointV2 getContorlPointByHeight(float height,bool isTop=true);
    ContorlPointV2 getContorlPointByHeightInWorld(float height,bool isTop=true);
    
    Vec2  getPositionByHeight(float,bool isTop=true);
    Vec2  getPositionByHeightInWorld(float,bool isTop=true);
    
    float getLengthByCP(ContorlPointV2& cp,bool isTop = true);
    
    
    float getDefaultRadius(){return _preContorlPoint._radius;}
    void  setDefaultRadius(float value){_preContorlPoint._radius = value;}
    
    float getNextZPosition(){return _preContorlPoint._zPosition;}
    void  setNextZposition(float value){_preContorlPoint._zPosition = value;}
    
    float getRotateRadius(){return _preContorlPoint._rotateRadius;}
    void  setRotateRadius(float value){_preContorlPoint._rotateRadius = value;}
    
    void subContorlPoint(int yheight);
    void subTextureHeight(int yheight);
    
    void ensureCapacity(int count);
    void updateVertices();
    void addVertices(const ContorlPointV2& cp);
    void drawHeadRange(const Mat4 &transform, uint32_t flags);
    void saveInfoToString(std::string& out);
    void resetInfoBySaveString(const std::string& in);
    
    void saveInfoToData(cocos2d::Data& out);
    void resetInfoBySaveData(const cocos2d::Data& in);
public:
    CC_SYNTHESIZE(float , _growLength, GrowLength);
    CC_SYNTHESIZE(float , _verticesUnitHeight, VerticesUnitHeight);
    CC_SYNTHESIZE(float , _cpListUnitHeight, CPListUnitHeight);
    
public:
    FaceDirection   _preGrowDirection;
    ContorlPointV2  _preContorlPoint;
    CPListType      _cpList;

    bool            _isDraw;
    
    std::function<float(float length)> _updateCPRadiusCall;
    
    Texture2D*      _texture;
    float           _textureLeft;
    float           _textureRight;
    GLProgram*      _glprogram;
    GLProgramState* _glprogramstate;
    CustomCommand   _customCommand;
    //opengl
    bool            _isDirt;
    V3F_T2F*        _vertices;
    unsigned int    _verticesCount;
    unsigned int    _capacity;
    unsigned int    _capacityAddSteplen;
};

#endif /* defined(__Grow_Demo__Plant1__) */
