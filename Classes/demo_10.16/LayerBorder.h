#ifndef __Grow_Demo__LayerBorder__
#define __Grow_Demo__LayerBorder__
#include "common.h"
#include "PlantBase.h"
#include "TypeDef.h"
#include "Grid.hpp"
#include "MapGrid.h"
#include <map>
#include "Box2D/Box2D.h"

class BorderContext
{
public:
    enum
    {
        NormalBorder,
        OnlyQuad,
    };
    BorderContext():_type(NormalBorder),_quadIndex(0),_quadStartHeight(0){}
    BorderContext(BorderMaskType mask,float startHeight,int quadIndex ,char type = NormalBorder):_maskType(mask),_type(type),_quadIndex(quadIndex),_quadStartHeight(startHeight){}
    
    float getTopHeight;
    char _type;
    BorderMaskType _maskType;
    float _quadStartHeight;
    int _quadIndex;
};

class QuadVec
{
public:
    Vec3 lb,lt,rb,rt;
};

class LayerBorder : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerBorder);
    virtual void  moveDown(float y);
    
    virtual bool init();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    void checkRemoveBorderCell(const GridCell& cell){
        if(isHasBorder(cell))removeBorder(cell);
    }
    V3F_C4B_T2F_Quad createQuadByBorderCell(const GridCell& cell,BorderMaskType _type,float bHeight);
    bool isHasBorder(const GridCell& cell);
    void removeBorder(const GridCell& cell);
    void addBorderLine(const BorderLine& line);
    
    int createOneQuad();
    void releaseQuad(int index);
    
    void updateQuad(const GridCell& cell,const BorderContext& context);
    V3F_C4B_T2F_Quad*  getQuadPtrByIndex(int index){return &_quads->getQuads()[index];}
    V3F_C4B_T2F_Quad&  getQuadByIndex(int index){return _quads->getQuads()[index];}
    
    void moveDownGridCell(int len);
    void suboutViewBorderCell();
   //  std::list<BorderLine>&& getBorderlines( std::set<BorderCell>&& set);
    std::list<BorderLine> getBorderlines( std::set<BorderCell>&& set);
    
    void  addBorderCell(int x,int y);
    void  addBorderCell(BorderCell);
    
    void updateBorder(int x,int y, int width,int height);
    bool isBorderCell(int x,int y,unsigned char type,BorderCell& cellOut);
   
    void addBorderLineToBox2dWorld(const BorderLine& line);
    void removeAllBorderLineInBox2dWorld();
    void linkBorderLines(std::list<BorderLine>& lineList);
    
    void getTexture();
    void setTexture(Texture2D* texture);
    
public:
    
    float       _moveDownLenght;
    MapGrid*    _mapGrid;
  
    b2World*    _world;
    std::vector<int> _quadsPool;
    std::list<b2Body*> _bodies;
    CustomCommand   _customCommand;
    TextureAtlas*   _quads;
    std::multimap<GridCell,BorderContext> _borderMap;
    
    static int initQuadMap();
    static V3F_C4B_T2F_Quad _sQuadMap;
    static std::map<BorderMaskType,QuadVec> _sQuadVecMap;
    static V3F_C4B_T2F_Quad _sRawQuad;
};


#endif /* defined(__Grow_Demo__LayerBorder__) */