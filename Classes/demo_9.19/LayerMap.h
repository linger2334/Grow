#ifndef __TestNewMap__LayerMap__
#define __TestNewMap__LayerMap__
#define CC_CONSTRUCTOR_ACCESS public
#include "Macro.h"
#include "TypeDef.h"
#include "Grid.hpp"
#include <map>
#include <set>
enum  Zorder{
    LayerBackground = 0,
    LayerHideGoods,
    LayerDirt,
    LayerRock,
    LayerGoods,
    LayerUI,
    LayerEffect
};


struct DrawShape
{
    DrawShape():_points(nullptr),_pointCount(0){}
    DrawShape(Vec2* points,unsigned int count):_points(points),_pointCount(count){}
    
    Vec2*        _points;
    unsigned int  _pointCount;
};


class LayerMap : public Layer
{
public:
    CREATE_FUNC(LayerMap);
    virtual bool init();
    virtual void update(float dt) override;
    virtual void onEnter(){
       // updateMapInView(0, 0, 80, 120);
    }
    virtual void onExit(){}
    
////////////////////////////////////////////////////////////////
//////Inline Func
    bool          isOutMapGridInView(int x,int y)
    {
         return x<0||x>=_mapGridVisibleWidth||y<0||y>=_mapGridVisibleHeight;
    }
    bool          isOutMapGrid(int x,int y)
    {
        return x<0||x>=_mapGridVisibleWidth||y<0||y>=_mapGridMaxHeight;
    }
    unsigned char getMapCellInView(int x,int y)
    {
        return getMapCell(x, y-_mapGridStartY);
    }

    void          setMapCellInView(int x,int y,unsigned char value)
    {
        setMapCell(x, y-_mapGridStartY, value);
    }
    unsigned int  getMapMaxHeight()
    {
        return _mapGrid._height;
    }
    unsigned char getMapCell(int x,int y)
    {
        if(isOutMapGrid(x,y))return GridType::None;
        return _mapGrid.getValueForGridCell(GridCell(x,y));
    }
    
    void          setMapCell(int x,int y,unsigned char value)
    {
        if(isOutMapGrid(x,y))return;
        _mapGrid.setValueForGridCell(GridCell(x,y),value);
    }
    
    void          setDirtyCell(int x,int y)
    {
        setDirtyCell(GridCell(x,y));
    }
    void          setDirtyCell(const GridCell& cell)
    {
        if(_dirtyCellsSet.find(cell)==_dirtyCellsSet.end());
        _dirtyCellsSet.insert(cell);
    }
    
    cocos2d::Point getCellPointInView(int x,int y)
    {
        return cocos2d::Point(x*_mapBlockSize.width,y*_mapBlockSize.height);
    }
    cocos2d::Point getCellPoint(int x,int y)
    {
        return getCellPointInView(x,y+_mapGridStartY);
    }
    
    GridCell getCellByPointInView(cocos2d::Point pt)
    {
        return GridCell(pt.x/_mapBlockSize.width,pt.y/_mapBlockSize.height);
    }
    GridCell getCellByPoint(cocos2d::Point pt)
    {
        return GridCell(pt.x/_mapBlockSize.width,pt.y/_mapBlockSize.height-_mapGridStartY);
    }
    
    
    RenderTexture* getRenderByGridCell(int x,int y)
    {
        return _mapViews.at(y/_mapViewGridHeight);
    }
    RenderTexture* getRenderByGridCellInView(int x,int y)
    {
        return _mapViews.at(y-_mapGridStartY/_mapViewGridHeight);
    }
    
    bool isInRenderViewByGridCellInView(int x,int y,int renderId)
    {
        return isInRenderViewByGridCell(x,y-_mapGridStartY,renderId);
    }
    bool isInRenderViewByGridCell(int x,int y,int renderId)
    {
          return y/_mapViewGridHeight==renderId;
    }
   
    int getRenderViewIdByGridCell(int x,int y)
    {
        return y/_mapViewGridHeight;
    }
    
    GridCell getRenderViewStartGridCell(int renderId)
    {
        return GridCell(0,renderId*_mapViewGridHeight);
    }
    
    GridCell renderViewGridToMainGridCell(int x,int y,int renderId)
    {
        return renderViewGridToMainGridCell(GridCell(x,y),renderId);
    }
    
    GridCell renderViewGridToMainGridCell(const GridCell& cell,int renderId)
    {
        return GridCell(cell._x,cell._y+renderId*_mapViewGridHeight);
    }
    
    cocos2d::Point gridCellToRenderPoint(int x,int y,int renderId)
    {
        return Vec2(x*_mapBlockSize.width,(y-renderId*_mapViewGridHeight)*_mapBlockSize.height);
    }
    
////////////////////////////////////////////////////////////////
    void updateMapDirty();
    void moveMapDown(int len);
////////////////////////////////////////////////////////////////
    Texture2D* getTextureByGridType(unsigned char type);
    bool LoadTextures();
    bool initMapInfo();
    bool initMapViwes();
    void initMapGridInfo();
    void updateMap(int x,int y,int width,int height);
    void updateMapInView(int x, int y,int widty ,int height);
    void updateRenderView(int renderIndex);
    void updateMapView(int x,int y,int width,int height);
    void updateMapViewInView(int x,int y,int width,int height);
    void updateRenderView(int x,int y,int width,int height,int renderIndex);
    
    void addBorderNode(int x,int y);
    void delBorderNode(int x,int y);
    
    Sprite* createSpriteByGridCell(int x,int y,bool autoRelease=true);
    Texture2D* createGridCellTexture(int x,int y);
    
    void drawToRenderViewBySpriteList(int renderIndex,const Vector<Sprite*>& list);
    
    
    void         layoutViews();

    //是否相应触摸事件
    void beginTouchListen();
    void stopTouchListen();
    bool isTouchListening();

    //Touch Events
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    virtual void onTouchCancelled(Touch*,Event*);
    
    void changeGridCell(int x,int y,unsigned char type);
    
    void clearGrid(int x,int y);
    void _checkClear(int x,int y, int width,int height);
    void _checkClearnew(int x,int y);
    void updateBorder(int x,int y, int width,int height);
    
    void updateBorderNew(int x,int y,int width,int height);
    bool isBorderCell(int x,int y);

    bool _isNeedDelete(int x,int y ,int stepX,int stepY,int lenLoop,int lenMaxCheck);
    
    void initDrawShapes();
    void drawRandomShape();
    
    void resetMap();
    void testOutLineInit();
    
    bool isBorder(int x,int y,unsigned char type,BorderDirection& dit);
    void _updateClearCheck(float dt);
    
    
    
    
    void testClear(int x,int y,int len);
    //////////////////////////
    Sprite* newSpriteByPool();
    Sprite* createSpriteByPool(const char* file);
    Sprite* createSpriteByPool(Texture2D* texture);
    Sprite* createSpriteByPool(Texture2D* texture,cocos2d::Rect rect);
    
    void    releaseSpriteToPool(Sprite*);
    void    autoReleaseSpriet(Sprite* sprite);
    void    clearSpriteAutoReleasePool();
    /////////////////
    
    cocos2d::Size                   _visibleSize;             //分辨率大小
    cocos2d::Size                   _mapSize;                 //地图的大小
    cocos2d::Size                   _mapBlockSize;            //地图最小块的大小
    int                    _mapViewGridWidth;
    int                    _mapViewGridHeight;
    int                    _mapViewWidth;
    int                    _mapViewHeight;
    int                    _mapGridStartY;
    int                    _mapGridVisibleWidth;
    int                    _mapGridVisibleHeight;
    int                    _mapGridMaxHeight;
    Vector<RenderTexture*> _mapViews;
    Grid<unsigned char>    _mapGrid;
    std::stack<Node*>      _releasePool;
    bool                   _isNeedChack;
    std::map<int,int>      _dirtyCellMap;
    std::set<GridCell>     _dirtyCellsSet;
   
    std::map<int,Texture2D*>           _textures;
    std::map<GridCell,char> _borders;
    
    int                _passGridHeightLen;
   
    Vector<Sprite*>    _spritesPool;
    int                _spritesPoolMaxCount;
    Vector<Sprite*>    _spritesAutoReleasePool;
    static DrawShape* _drawShapes;
    static unsigned int _drawShapesCount;
    SpriteBatchNode*   _borderBatch;
  
};

#endif /* defined(__TestNewMap__LayerMap__) */
