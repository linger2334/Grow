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
    MapLevelEditorMenu = 888
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

class SceneGame: public Layer,public b2ContactListener
{
//
public:
    SceneGame();
    ~SceneGame();

    static Scene* createScene();
    CREATE_FUNC(SceneGame);
    bool init();
    
    ////////////////////////////////////////////////////////////////Begin
    void  initInfo();
    
    ////////////////////////////////////////////////////////////////
    
    void _layoutGameLayer(Node* gameLayer);
    void  MoveDirtLayer(int x);
    void  MovePlant(int x);
    void  MoveLayers(int yLen);
    
    void  moveDownMapGrid(int moveLen);
    void  moveDownMapGrid(float yLen);
    void  moveDownAlphaMask(float yLen);
    void  moveDownGameView(float yLen);
    
    bool  testMapGridCrash(Vec2 point,int type);//
    
    Node* testLight();
    
    ////////////////////////////////////////////////////////////////
    
    void  moveActionCallBack(float dt);
    
    /////////////////////////////////////////////////////////////////
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //    virtual void onTouchCancelled(Touch*,Event*);
    
    void testBorderLine();
    
    
    std::list<Node*>   _linghtList1;
    Sprite*          _leftBorder[2];
    Sprite*          _rightBorder[2];
    
    LightNode2*                 linghtNode;
    MapGrid                     _mapGrid;
    Sprite*                     _dirtSprite;
    Texture2D*                  _textureAlphaMask;
    GameManager*                _gameManager;
    
    Sprite*                     _maskAlphaSprite;
    LayerRollImage*             _backgroundLayer;
    LayerRollImage*             _dirtLayer;
    LayerPlant_1*               _plantLayer;
    LayerBorder*                _borderlayer;
    LayerGoods1*                _goodsLayer;
    LayerRollImage*            _borderLeftLayer;
    LayerRollImage*            _borderRightLayer;
    LayerLight*                _lightLayer;
    
    
    Sprite*             _btnAddSpeed;
    Sprite*             _btnSubSpeed;
    bool                        _isMoving;
    
    std::list<BorderCell>       _borderList;
    std::map<GridCell,bool>     _borders;
    
    std::list<BorderCell>       _borderList1;
    SpriteBatchNode*            _bordersNode;
    Plant_1*                    _plantBorder;
    Node*                       _light;
    Sprite*                     _spLight;
    float                       _growHeight;
    bool                        _isMovingLight;
    Vector<Sprite*>             _linghtList;
    float                       _lightListTopHeight;
    bool                        _lightRuningMove;
    
    Sprite*                     _plangHead;
    Vec2                      _touchPrePoint;
    ////////////////////////////////////////////////////////////////End
    
    //场景初始化
    void initPhysics();
    inline PhysicsHandler* getPhysicsHandler() {return _physicsHandler;}
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    
    void update(float dt);
    
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
    Sprite*     _lightSprite;
    
//////////////////////////////////////////////End
};

#endif