#ifndef __PlantOri__SceneGame__
#define __PlantOri__SceneGame__
//Begin
#include "GameManager.h"
#include "LayerPlant.h"
#include "BorderLine.h"
#include "LayerBorder.h"
#include "LayerGoods.h"
#include "LightNode2.h"
#include "LayerLight.h"
#include "LayerDirt.h"
#include "LayerPlantBase.h"


enum MapGridType
{
    NoneCell = 0,
    DirtCell = 255,
};

enum SceneGameChildZorder
{
    MapBackground = 0,
    MapPlant = MapBackground+10,
    MapAlphaDrawing = MapPlant+10,
    MapDirt = MapAlphaDrawing+10,
    MapDirtBorder = MapDirt+10,
    MapStone = MapDirtBorder+10,
    MapGoods = MapStone+10,
//////////////////////////////////////道具和关卡编辑器
    MapItem = MapGoods + 10,
    MapUIBorder =MapItem + 10,
    MapLight = MapUIBorder + 10,
    MapUI   =  1000,
    MapLevelEditorMenu
  //  MapLevelEditorMenu = 888
////////////////////////////////////////////
};
//End
#include <iostream>
#include <vector>
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "LevelFileHandler.h"
#include "Macro.h"
#include "Item.h"
#include "LayerItem.h"
#include "PhysicsHandler.h"

class LayerUI;
class LayerUIBorder;
class LayerMapGrid;
class SceneGame: public Layer
{
//
public:
    SceneGame();
    ~SceneGame();
    static Scene* createScene();
    CREATE_FUNC(SceneGame);
    bool init();
    virtual  void update(float dt);
    virtual  void onEnter();
    virtual  void onExit();
    
    ////////////////////////////////////////////////////////////////Begin
    void  initInfo();
    
    ////////////////////////////////////////////////////////////////
    
    void  _layoutGameLayer(Node* gameLayer);
    void  moveDownGameView(float yLen);
    
    
    /////////////////////////////////////////////////////////////////
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    // virtual void onTouchCancelled(Touch*,Event*);
    
    void  addUIBorderBody();
    
    void  initDirt();
    GameManager*                _gameManager;
    LayerUI*                    _layerui;
    LayerUIBorder*              _layerUIBorder;
    LayerMapGrid*               _layerMapGrid;
    LayerRollImage*             _layerBackground;
    LayerRollImage*             _layerDirt;
    LayerPlantBase*               _layerPlant;
    LayerBorder*                _layerBorder;
    LayerGoods1*                _goodsLayer;
    LayerLight*                 _layerLight;
    
    bool                _isReGrow;
    Sprite*                     _btnAddSpeed;
    Sprite*                     _btnSubSpeed;
    
    Label*                      _labelHeight;
    
    float                       _mapHeight;
    b2Body* _headBody;
    ////////////////////////////////////////////////////////////////End
    
    //场景初始化
    void initPhysics();
    inline PhysicsHandler* getPhysicsHandler() {return _physicsHandler;}
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    //editor
    void initLevelEditorMenu();
    friend class GameManager;
    friend class LayerItem;
    
protected:
    b2World* world;
    GLESDebugDraw* _debugDraw;

    //调试绘图
    Mat4 _modelViewMV;
    void onDraw();
    CustomCommand _customCommand;
    
private:
    
    LevelFileHandler* _levelFileHandler;
    LayerItem* _layerItem;
    PhysicsHandler* _physicsHandler;
    TypeBase* plantHead;
////////////////////////////////////Begin
    Sprite*                     _lightSprite;
//////////////////////////////////////////////End
};

#endif