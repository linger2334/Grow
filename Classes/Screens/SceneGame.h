#ifndef __PlantOri__SceneGame__
#define __PlantOri__SceneGame__
//Begin
#include "LayerMap.h"
#include "PlantLine.h"
#include "GameManager.h"
#include "LayerPlant.h"
#include "BorderLine.h"
#include "LayerBorder.h"
#include "LightNode.h"
#include "LayerGoods.h"
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
    MapLevelEditorMenu = MapGoods + 10,
    MapScrollView = MapLevelEditorMenu + 10
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
#include "CocosGUI.h"

using namespace ui;

#define cc_to_b2Vec(x,y) (b2Vec2((x)/PTM_RATIO,(y)/PTM_RATIO))

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
    
    bool  testMapGridCrash(Vec2 point);//
    
    Node* testLight();
    
    ////////////////////////////////////////////////////////////////
    
    void  moveActionCallBack(float dt);
    
    /////////////////////////////////////////////////////////////////
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //    virtual void onTouchCancelled(Touch*,Event*);
    
    void lightCallback(Node* node);
    void lightCallback1(Node* node);
    void lightCallback2(Node* node);
    
    MapGrid     _mapGrid;
    
    Sprite*     _dirtSprite;
    Texture2D*  _textureAlphaMask;
    
//    PlantLine*   _plant;
    GameManager* _gameManager;
    
    Sprite*     _maskAlphaSprite;
    LayerRollImage* _backgroundLayer;
    LayerRollImage* _dirtLayer;
    LayerPlant_1*   _plantLayer;
    
    LayerBorder*    _borderlayer;
    
    LayerGoods1*     _goodsLayer;
    
    bool            _isMoving;
    
    std::list<BorderCell> _borderList;
    std::map<GridCell,bool> _borders;
    
    std::list<BorderCell> _borderList1;
    MotionStreak *streak;
    SpriteBatchNode*          _bordersNode;
    Plant_1*        _plantBorder;
    Node*           _light;
    Sprite*         _spLight;
    float           _growHeight;
    bool            _isMovingLight;
    Vector<Sprite*> _linghtList;
    float           _lightListTopHeight;
    bool            _lightRuningMove;
    LightNodeListHelper _lightListHelper;
    Sprite*         _plangHead;
    LightNode* lightNode;
    cocos2d::Point           _touchPrePoint;
    ////////////////////////////////////////////////////////////////End
    
    //场景初始化
    void initPhysics();
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    
    void update(float dt);
    
    bool isPointContact(cocos2d::Point ptInGl);
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact,const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact,const b2ContactImpulse* impulse);
    
    //editor
    void initLevelEditorMenu();
    friend class GameManager;
    friend class LayerItem;
    b2World* world;
protected:
    GLESDebugDraw* _debugDraw;
    
    std::vector<b2Body*> _itembodies;//存放道具body
    b2Body* _groundBody;
    
    //碰撞事件元素类型
    typedef std::pair<b2Fixture*,b2Fixture*> MyContact;
    std::set<MyContact> _contacts;
    //调试绘图
    Mat4 _modelViewMV;
    void onDraw();
    CustomCommand _customCommand;
    
private:
    LevelFileHandler* _levelFileHandler;
    LayerItem* _layerItem;
    
};

#endif