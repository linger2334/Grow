#ifndef __Grow_Demo_test__GameLayerMapBorder__
#define __Grow_Demo_test__GameLayerMapBorder__
#include "common.h"
#include "GameLayerBase.h"
#include "GameBorderline.h"
#include "GameRuntime.h"
class MapGrid;
class ItemModel;
class GameLayerMapBorder;
class GameLayerMapBorder : public GameLayerBase,public GameLayerHelper<GameLayerMapBorder>
{
public:
    CREATE_FUNC(GameLayerMapBorder);

    virtual void  moveDown(float y);
    
    virtual bool init();
    virtual void update(float dt);
    virtual bool initGameInfo();
    virtual bool releaseGameInfo();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    void checkRemoveBorderCell(const GridCell& cell){
        if(isHasBorder(cell))removeBorder(cell);
    }
    V3F_C4B_T2F_Quad createQuadByBorderCell(const GridCell& cell,BorderMaskType _type,float bHeight);
    bool isHasBorder(const GridCell& cell);
    void removeBorder(const GridCell& cell);
    void addBorderLine(const GameBorderLine& line);
    void clearBorders();
    int createOneQuad();
    void releaseQuad(int index);
    
    void updateQuad(const GridCell& cell,const BorderContext& context);
    V3F_C4B_T2F_Quad*  getQuadPtrByIndex(int index){return &_quads->getQuads()[index];}
    V3F_C4B_T2F_Quad&  getQuadByIndex(int index){return _quads->getQuads()[index];}
    
    void moveDownGridCell(int len);
    void suboutViewBorderCell();
    //  std::list<BorderLine>&& getBorderlines( std::set<BorderCell>&& set);
    std::list<GameBorderLine> getBorderlines( std::set<BorderCell>&& set);
    
    void  addBorderCell(int x,int y);
    void  addBorderCell(BorderCell);
    
    void updateBorder(int x,int y, int width,int height);
    void updateBorderCheckRemove(int x,int y, int width,int height);
    void updateBorder(std::set<BorderCell>&& borders);
    bool isBorderCell(int x,int y,unsigned char type,BorderCell& cellOut);
    
    bool isNewBorderCell(int x,int y,unsigned char type,BorderCell& cellOut);
    
    b2Vec2 getB2Vec2ByCell(const BorderCell& cell);
    void addBorderLineToBox2dWorld( GameBorderLine& line);
    void removeAllBorderLineInBox2dWorld();
    void linkBorderLines(std::list<GameBorderLine>& lineList);
    
    void getTexture();
    void setTexture(Texture2D* texture);
    
    void updateBox2dBorderLine();
public:
    bool        _isBox2DBorderLineDirt;
    float       _moveDownLenght;
    MapGrid*    _mapGrid;
    ItemModel*  _itemBorderLine;
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
    static bool             _isInitQuadMap;
};

#endif /* defined(__Grow_Demo_test__GameLayerMapBorder__) */
