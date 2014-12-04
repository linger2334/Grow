#ifndef __Grow_Demo_test__GameLayerMap__
#define __Grow_Demo_test__GameLayerMap__
#include "GameLayerBase.h"
#include "MapGrid.h"
class GameLayerRollImage;
class GameLayerMapBorder;
class GameLayerMapDirt;
class GameLayerMap : public GameLayerBase,public GameLayerHelper<GameLayerMap>
{
public:
    CREATE_FUNC(GameLayerMap);
    virtual bool init();
    virtual bool initGameInfo();
    virtual void moveDown(float yLen);
    virtual void update(float dt);
    virtual void updateConfigs();
   
    void  moveDownMapGrid(int moveLen);
    void  moveDownMapGrid(float yLen);
    void  moveDownAlphaMask(float yLen);
    
    bool  PointIsInDirtOrOutMapGrid(Vec2 pt);
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
//    virtual void onTouchCancelled(Touch*,Event*);
    
    void updateExcavatePolygon(int x,int y,int width , int height);
    /////////////////////
    bool  touchClearGrid(Vec2 touchPoint);
    int  clearGridCellBorderByRange(GridCell cell, int radius);

//    bool  clearGridCell(int x,int y);
//    void testClear(int x,int y,int len);
//
    
    void cellNeedDel(const GridCell cell)
    {
        if(_mapGrid.getValue(cell._x,cell._y)!=GridType::None)
            _clearCells.insert(cell);
    }
    
    void addCellBorder(const GridCell& cell);

    void onClearCell();
    bool isNeedDeleteCell(int x,int y);
    void changeGridCell(int x,int y ,unsigned char type);
    bool  _isNeedDelete(int x,int y ,int stepX,int stepY,int lenLoop,int lenMaxCheck);
    void _checkClear(int x,int y, int width,int height);
    
    void  onChangeCellCalls(const GridCell& cell);
//    int clearGridCellBorderByRange(GridCell cell, int Radius);
    
    void startListener();
    void stopListener();
    
public:
    GameLayerMapBorder*   _layerBorder;
    Texture2D*            _textureAlphaMask;
    cocos2d::Size                  _maskSize;
    Sprite*               _maskAlphaSprite;
    MapGrid               _mapGrid;
    GameLayerMapDirt*   _layerdirt;
    
    std::set<GridCell>   _clearCells;
    std::set<GridCell>   _updateBorderList;
    Vec2                 _touchPrePoint;
    bool                 _isFirstMoveTouch;
    EventListenerTouchOneByOne* _listener;
};

#endif /* defined(__Grow_Demo_test__GameLayerMap__) */
