//Begin
#include "SceneGame.h"
#include "UIHelper.h"
#include "LayerBackMask.h"
#include "ContorlPoint.h"
#include "LayerMapGrid.h"
#include "LayerUI.h"
#include "LayerUIBorder.h"
#define PATH_LAYER_BORDER "layer_border_3.png"
#define EDIT_SPEED_SPED    5
//End

#include "SimpleAudioEngine.h"
#include "LevelEditor.h"
#include "LevelsMenu.h"
#include "SimpleAudioEngine.h"


#define BACKGROUND_MUSIC "background_music_1.mp3"
#define PLANT_GROW_MUSIC "plant_grow_music_1.mp3"

SceneGame::SceneGame():world(nullptr),_debugDraw(nullptr),_levelFileHandler(nullptr),_layerItem(nullptr),_physicsHandler(nullptr)
{
    
}

SceneGame::~SceneGame()
{
    _gameManager->_sceneGame = nullptr;
    _gameManager->setBox2dWorld(nullptr);
    CC_SAFE_RELEASE(_gameManager->_fileHandler);
    CC_SAFE_DELETE(world);
    CC_SAFE_DELETE(_debugDraw);
    CC_SAFE_RELEASE(_physicsHandler);
}


Scene* SceneGame::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneGame::create();
    scene->addChild(layer);
    return scene;
}

bool SceneGame::init()
{

    if ( !Layer::init() ){return false;}
    _gameManager  = GameManager::getInstance();
    _gameManager->_sceneGame = this;
    _gameManager->_fileHandler->retain();
    //levelFileHandler = GameManager::getInstance()->_fileHandler;
//    CCASSERT(_levelFileHandler!=nullptr,"levelfilehandler is nullptr.Game cannot be run");
//    //
//
    _mapHeight = 0;
    initInfo();
//    _isMoving=false;
//    
//    scheduleUpdate();
    
    return true;
}

void SceneGame::initInfo()
{
    initPhysics();
    _gameManager->_world = world;
    _gameManager->setPhysicsHandler(_physicsHandler);
    _layerMapGrid = LayerMapGrid::create();
    _layoutGameLayer(_layerMapGrid);
    
   RenderMaskColorNode* maskOnlyAlpha= RenderMaskColorNode::create(false,false,false,true);
    RenderMaskColorNode* maskOnlyColor= RenderMaskColorNode::create(true,true,true,true);

    _layerBorder  = LayerBorder::create();
    _layoutGameLayer(_layerBorder);
   // _layerPlant = LayerPlant_1::create();
    //_layoutGameLayer(_plantLayer);
   
   
    _layerDirt = LayerDirt::create();
    _layoutGameLayer(_layerDirt);
//    _layerBackground = LayerBackGround::create();
//    _layoutGameLayer(_layerBackground);
    
    _layerBorder = LayerBorder::create();
    _layoutGameLayer(_layerBorder);
    
    _layerui  = LayerUI::create();
   _layoutGameLayer(_layerui);
    
    _layerUIBorder = LayerUIBorder::create();
    _layoutGameLayer(_layerUIBorder);
    
    _layerPlant  = LayerPlant_1::create();
    _layoutGameLayer(_layerPlant);
    
    _layerLight = LayerLight::create();
    _layoutGameLayer(_layerLight);
//    addChild(_layerBackground,SceneGameChildZorder::MapBackground);
    addChild(_layerPlant,SceneGameChildZorder::MapBackground+3);
    addChild(maskOnlyAlpha,SceneGameChildZorder::MapAlphaDrawing-1);
    addChild(_layerMapGrid,SceneGameChildZorder::MapAlphaDrawing);
    addChild(maskOnlyColor,SceneGameChildZorder::MapAlphaDrawing+1);
    addChild(_layerDirt,SceneGameChildZorder::MapDirt);
    addChild(_layerBorder,SceneGameChildZorder::MapDirt+1);
    addChild(_layerUIBorder,SceneGameChildZorder::MapDirt+2);
    addChild(_layerLight,SceneGameChildZorder::MapLight);
    addChild(_layerui,SceneGameChildZorder::MapUI);
    
   // initLevelEditorMenu();
//    _layerItem = LayerItem::create();
//    _layerItem->setAnchorPoint(Vec2(0.5,0));
//    _layerItem->setPosition(Vec2(VisibleSize.width/2,0));
//    _layerItem->loadItemsAndBodys();
//    this->addChild(_layerItem,MapItem);
    //
//
    
    _layerMapGrid->initGameInfo();
    _layerMapGrid->_mapGrid.testClear(40, 0, 40);
    _layerMapGrid->_mapGrid._layerBorder->updateBorder(0, 0, 80, 120);
//    /////////////////////////////////////////////////////////////////
//
//    /////////////////////////////////////////////////////////////////
//
//    _lightLayer  = LayerLight::create();
//    _layoutGameLayer(_lightLayer);
//    addChild(_lightLayer,999);
//
    _layerItem = LayerItem::create();
    _layerItem->setAnchorPoint(Vec2(0.5,0));
    _layerItem->setPosition(Vec2(VisibleSize.width/2,-_mapHeight));
    //首次加载
    _layerItem->loadItemsAndBodys(_mapHeight);
    _gameManager->_layerItem = _layerItem;
      initLevelEditorMenu();
    this->addChild(_layerItem,MapItem);
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    scheduleUpdate();
    DrawLayerCallFunc* worldDraw = DrawLayerCallFunc::create();
    worldDraw->_callBack =  std::bind(&SceneGame::onDraw,this);
    addChild(worldDraw,10009);
////////////////////////////////////////////
    _isReGrow = false;
    _labelHeight = Label::createWithSystemFont("第 0 屏","Arial",24);
    _labelHeight->setPosition(Vec2(10,20));
    _labelHeight->setAnchorPoint(Vec2(0,0));
    addChild(_labelHeight,1000);
   // _layerBackground->setVisible(false);
    
//    Button* btn = Button::create("tree.png");
//    btn->setPosition(Vec2(20,20));
//    addChild(btn,1009);
//    btn->addTouchEventListener([&](Ref*,Widget::TouchEventType type)
//                               {
//                                   if (type == Widget::TouchEventType::ENDED)
//                                   {
//                                       _isReGrow = !_isReGrow;
////                                       for (int j = 0; j<120; j++) {
////                                       for (int i =0 ; i<80; i++) {
////                                           GridCell cell(i,j);
////                                           if(_layerBorder->isHasBorder(cell))
////                                           {
////                                               _layerMapGrid->_mapGrid.changeGridCell(cell._x, cell._y, GridType::None);
////                                              // _layerBorder->removeBorder(cell);
////                                           }
////                                       }
////                                       }
////                                       _layerBorder->updateBorder(0, 0, 80, 120);
//                                   }
//                               }
//
//                               );
    
//    Sprite* sp = Sprite::create("test_plant_1024.png");
//    addChild(sp,10002);
//    sp->setColor(Color3B::RED);
//    _layoutGameLayer(sp);
    addUIBorderBody();
    
    initDirt();

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MUSIC);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4);

    
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    return ;
}
 void SceneGame::onEnter()
{
    Layer::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC,true);
    
  //  CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
 void SceneGame::onExit()
{
    Layer::onExit();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void SceneGame::initPhysics()
{
    b2Vec2 gravity = b2Vec2(0.0,-0.0);
    world = new b2World(gravity);
    
    _physicsHandler = PhysicsHandler::create(world);
    CC_SAFE_RETAIN(_physicsHandler);

    
    world->SetAllowSleeping(false);
    world->SetContinuousPhysics(true);
    world->SetContactListener(_physicsHandler);
    //调试绘图
    _debugDraw = new GLESDebugDraw(PTM_RATIO);
    //调试绘图
    world->SetDebugDraw(_debugDraw);
    uint32 flags = 0;
    flags+=b2Draw::e_shapeBit;
    _debugDraw->SetFlags(flags);
    
}

void SceneGame::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    Layer::draw(renderer,transform,flags);
    
    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
    Director* director = Director::getInstance();
    CCASSERT(nullptr!=director, "Director id null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    _modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SceneGame::onDraw, this);
    renderer->addCommand(&_customCommand);
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void SceneGame::onDraw()
{
  
    Director* director = Director::getInstance();
    CCASSERT(nullptr!=director, "Director id null when seting matrix stack");
    
    Mat4 oldMV;
    oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
    world->DrawDebugData();
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}

void SceneGame::update(float dt)
{
  
   // moveDownGameView(30*dt);
//    static float dttemp =0;
//    if (_isReGrow) {
//        dttemp += dt;
//        if(dttemp>0.2)
//        {
////        ((LayerPlant_1*)_layerPlant)->_plant->_cpLineNode._cpList.pop_back();
////        ((LayerPlant_1*)_layerPlant)->_plant->_headCur._cp =*(((LayerPlant_1*)_layerPlant)->_plant->_cpLineNode._cpList.rbegin());
////        ((LayerPlant_1*)_layerPlant)->updateHead(dt);
////        ((LayerPlant_1*)_layerPlant)->renderPlant();
////            int x = _gameManager->getMapGridUnitVisibleSize().width;
////            int y = _gameManager->getMapGridUnitVisibleSize().height;
////        for (int j = 0; j<y; j++) {
////            for (int i =0 ; i<x; i++) {
////                GridCell cell(i,j);
////                if(_layerBorder->isHasBorder(cell))
////                {
////                    auto tcell =_layerMapGrid->_mapGrid.getGridCellByView(cell);
////                    _layerMapGrid->_mapGrid.changeGridCell(tcell._x, tcell._y, GridType::None);
////                    // _layerBorder->removeBorder(cell);
////                }
////            }
////            
////        }
////        _layerBorder->updateBorder(0, 0, x, y);
//            dttemp = 0;
//        }
//       // return ;
//    }
    
    if (_layerPlant->getPlantTopHeightInView()>_gameManager->_visible.height*0.5) {
        float height =_layerPlant->getPlantTopHeightInView();
               moveDownGameView(height-_gameManager->_visible.height*0.5);
    }
    _layerPlant->update(dt);
   _layerLight->update(dt);
    _layerMapGrid->update(dt);
    //删除道具和多边形
    _layerItem->update(dt);
    _physicsHandler->update(dt);
   // return ;
///////////////////////////////////////////////////////////b

//    Vec2 ptheade =  _plantLayer->_plant->convertToWorldSpace( _plantLayer->_plant->_headCur.getPosition());
//    _headBody->SetTransform(b2Vec2(ptheade.x/PTM_RATIO,ptheade.y/PTM_RATIO), 0);
//   
//    if (_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.6) {
//        moveDownGameView(_plantLayer->getPlantTopHeight()-_gameManager->_visible.height*0.6);
//    }
/////////////////////////////////////////////////////////////////////////
    //位置同步与道具碰撞检测
  // _physicsHandler->update(dt);

}
//
////////////////////////////////////////////////////////////////////////////////////Begin
void SceneGame::_layoutGameLayer(Node* gameLayer)
{
    gameLayer->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
}


void SceneGame::moveDownGameView(float yLen)
{
//    _layerBackground->moveDown(yLen*0.75);
    _layerDirt->moveDown(yLen);
    _layerMapGrid->moveDown(yLen);
    _layerBorder->moveDown(yLen);
    _layerUIBorder->moveDown(yLen*1.2);
    _layerPlant->moveDown(yLen);
    _layerLight->moveDown(yLen);
    _layerItem->moveDown(yLen);
    _mapHeight +=yLen;
    char buf[128];
    sprintf(buf,"第 %d 屏",(int)_mapHeight/1024+1);
    _labelHeight->setString(buf);
/////////////////////add by wlg
    //动态加载
    _layerItem->loadItemsAndBodys(_mapHeight);
//////////////////////////////////////////
}
//bool  SceneGame::testMapGridCrash(Vec2 point,int type)
//{
//    return false;
////    if(type==0)
////    {
////        if (point.x<40|| point.x > VisibleSize.width-40)return true;
////        // modify by wlg
////        if(_physicsHandler->isPointContact(point))return true;
////    }
// //   Vec2 pn = point + Vec2(0,_mapGrid._mapGridStarty);
// //   GridCell cell = _mapGrid.getMapGridCellByPosition(pn);
//    
//   // if(_mapGrid.isOutMapGrid(cell))return true;
//   // if(cell._x<4||cell._x>_mapGrid._gridWidth-4)return true;
//   // return _mapGrid.getValue(cell._x, cell._y) ==255;
//}
//
bool SceneGame::onTouchBegan(Touch* touch,Event* event)
{
    if (!_layerui->onTouchBegan(touch, event)) {
         _layerMapGrid->onTouchBegan(touch, event);
    }
  
    return true;
}

void SceneGame::onTouchMoved(Touch* touch,Event* event)
{
     _layerMapGrid->onTouchMoved(touch, event);
}

void SceneGame::onTouchEnded(Touch* touch,Event* event)
{
    _layerMapGrid->onTouchEnded(touch, event);

  //  _plantLayer->_isNeedGrow=true;
}

void   SceneGame::addUIBorderBody()
{
    float xvec[2]={
        40,
        WinSize.width-40
    };
    static TypeBase dirtLine(111);
    for (int i = 0 ; i<2; i++) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = b2Vec2(0,0);
        bodyDef.userData =GameManager::getInstance()->_sLineItemModel;
        b2Body* body = world->CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        b2ChainShape b2line;
        b2Vec2 verts[2]=
        {
            b2Vec2(xvec[i]/PTM_RATIO,0),
            b2Vec2(xvec[i]/PTM_RATIO,WinSize.height/PTM_RATIO)
        };
        b2line.CreateChain(verts,2);
        fixtureDef.shape = &b2line;
        fixtureDef.filter.groupIndex = -1;
        body->CreateFixture(&fixtureDef);
    }
}
void SceneGame::initLevelEditorMenu()
{
    Label* levelsLabel = Label::createWithTTF("Levels", "Marker Felt.ttf", 36);
    MenuItem* toLevelsItem = MenuItemLabel::create(levelsLabel, [&](Ref*){
        Director::getInstance()->replaceScene(LevelsMenu::createScene());
    });
    
    Label* editorLabel = Label::createWithTTF("Editor", "Marker Felt.ttf", 36);
    MenuItem* toEditorItem = MenuItemLabel::create(editorLabel,[&](Ref*){
        Director::getInstance()->replaceScene(LevelEditor::createScene());
    });
    
    Menu* menu = Menu::create(toLevelsItem,toEditorItem,NULL);
    menu->setPosition(Vec2(VisibleSize.width/2,toEditorItem->getContentSize().height/2));
    menu->alignItemsHorizontallyWithPadding(VisibleSize.width/10);
    this->addChild(menu,MapLevelEditorMenu);
}

void  SceneGame::initDirt()
{
    auto list = _layerItem->getPolygons();
    auto size = _gameManager->getMapGridUnitVisibleSize();
    
    auto map = _gameManager->getMapGrid();
    for (int j = 0 ; j< size.height+50; j++) {
        for (int i = 0; i<size.width; i++) {
                for (auto& ip: list) {
                Vec2 pt = map->getPositionByMapGridCell(i,j);
                b2Vec2 vec(pt.x/PTM_RATIO,pt.y/PTM_RATIO);
                    for (b2Fixture* fixture = ip->getBody()->GetFixtureList(); fixture; fixture=fixture->GetNext()) {
                        if(fixture->TestPoint(vec))
                        {
                            map->changeGridCell(i,j, GridType::None);
                        }
                    }
                    
            }
        }
    }
    _layerBorder->updateBorder(0, 0,size.width, size.height+50);
}
/////////////////////////////////////////////////////////////////////////////////////////////////End