#ifndef __Grow_Goods_test__LayerMapGrid__
#define __Grow_Goods_test__LayerMapGrid__
#include "common.h"
#include "MapGrid.h"
#include "PlantBase.h"
#include "GameManager.h"
class LayerMapGrid : public GameLayerRollY
{
public:
    CREATE_FUNC(LayerMapGrid);
  
//    void onEnter();
//    void onExit();
    bool init();
    bool initGameInfo();
    bool initWithHeight(float height);
    bool initWithSaveConfig();
    bool saveMapGrid();
    virtual void update(float dt);
    
    void  moveDownMapGrid(int moveLen);
    void  moveDownMapGrid(float yLen);
    void  moveDownAlphaMask(float yLen);

    
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //    virtual void onTouchCancelled(Touch*,Event*);
    
    virtual void moveDown(float yLen);
    
    bool clearGridByTouchPoint(Vec2 pt);
    
    std::list<Vec2>             _clearList;
    Vec2                        _touchPrePoint;
    Texture2D*                  _textureAlphaMask;
    Sprite*                     _maskAlphaSprite;
    MapGrid                     _mapGrid;
    GameManager*                _gameManager;
};

#endif /* defined(__Grow_Goods_test__LayerMapGrid__) */
