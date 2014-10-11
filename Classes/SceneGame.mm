//Begin
#include "SceneGame.h"
#include "UIHelper.h"
#include "LayerBackMask.h"
#include "ContorlPoint.h"
#include "LightNode.h"

#define  MOVE_DOWN_UNIT 60
#define PATH_LAYER_BORDER "layer_border_3.png"

#define EDIT_SPEED_SPED    5
//End

#include "SimpleAudioEngine.h"
#include "StaticData.h"
#include "LevelEditor.h"
#include "LevelsMenu.h"

SceneGame::SceneGame():world(nullptr),_debugDraw(nullptr),_levelFileHandler(nullptr),_layerItem(nullptr),_physicsHandler(nullptr)
{
    
}

SceneGame::~SceneGame()
{
    _gameManager->_sceneGame = nullptr;
    _gameManager->setBox2dWorld(nullptr);
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
    _levelFileHandler = GameManager::getInstance()->_fileHandler;
    CCASSERT(_levelFileHandler!=nullptr,"levelfilehandler is nullptr.Game cannot be run");
    //
    
    initInfo();
    _isMoving=false;
    
    scheduleUpdate();
    
    return true;
}

void SceneGame::initInfo()
{
    //add physics world
    _gameManager  = GameManager::getInstance();
    _gameManager->_sceneGame = this;
    initPhysics();
    _gameManager->setPhysicsHandler(_physicsHandler);
/////////////////////////////////////////////
   
    
    _gameManager->_plantGrowCrashTestCallback = std::bind(&SceneGame::testMapGridCrash,this,std::placeholders::_1,std::placeholders::_2);
    _isMovingLight =false;
    
    cocos2d::Size visibleSize = Director::getInstance()->getWinSize();
    cocos2d:: Size gridSize =_gameManager->getMapGridUnitVisibleSize();
    _mapGrid.resize(gridSize.width, gridSize.height*2);
    _mapGrid.setData(0, 0, 255);
    
    _mapGrid._maxViewHeight = _gameManager->getMapGridUnitVisibleSize().height;
    _mapGrid._unitGridSize = _gameManager->getMapGridUnitSize();
    _mapGrid._sceneGame = this;
    _gameManager->_mapGrid =&_mapGrid;
    
    _textureAlphaMask = new Texture2D();
    _textureAlphaMask->initWithData(_mapGrid._gridData,gridSize.width*(gridSize.height), Texture2D::PixelFormat::A8,gridSize.width, gridSize.height, cocos2d::Size(gridSize.width,gridSize.height));
    _textureAlphaMask->autorelease();
    
    _backgroundLayer  = LayerRollImage::create();
    _backgroundLayer->setImage("background_1_3.jpg");
    _backgroundLayer->setImageStartY(0);
    _layoutGameLayer(_backgroundLayer);
    
    RenderMaskColorNode* maskOnlyAlpha= RenderMaskColorNode::create(false,false,false,true);
    _maskAlphaSprite = Sprite::createWithTexture(_textureAlphaMask);
    _maskAlphaSprite->setScale(_gameManager->getMapGridUnitSize().width,_gameManager->getMapGridUnitSize().height);
    BlendFunc funcMaskSprite = {GL_ONE,GL_ZERO};
    _maskAlphaSprite->setBlendFunc(funcMaskSprite);
    _maskAlphaSprite->setAnchorPoint(Vec2(0.5,1));
    _maskAlphaSprite->setRotationX(180);
    _layoutGameLayer(_maskAlphaSprite);
    
    RenderMaskColorNode* maskOnlyColor= RenderMaskColorNode::create(true,true,true,true);
    
    _dirtLayer  = LayerRollImage::create();
    _dirtLayer->_blendFunc =  {GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA };
    _dirtLayer->setImage("dirt512_3.png");
    _dirtLayer->setImageStartY(0);
    _layoutGameLayer(_dirtLayer);
    
    _borderlayer  = LayerBorder::create();
    _mapGrid._layerBorder = _borderlayer;
    _layoutGameLayer(_borderlayer);
    
    _gameManager->setBox2dWorld(world);
    _borderlayer->_world = world;
    
    _mapGrid.testClear(gridSize.width/2,0,40);
    _borderlayer->updateBorder(0, 0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
    
    _plantLayer = LayerPlant_1::create();
    
    _gameManager->_layerplant = _plantLayer;
    _gameManager->_layerBorder = _borderlayer;
    
    _layoutGameLayer(_plantLayer);
    _mapGrid._layerPlant = _plantLayer;
    
//    _goodsLayer = LayerGoods1::create();取消随机光芒
//    _layoutGameLayer(_goodsLayer);
    ///////////////////////////////////////////////////////////////
//    addChild(_goodsLayer,SceneGameChildZorder::MapGoods);
    addChild(_plantLayer,SceneGameChildZorder::MapBackground+3);
    addChild(_backgroundLayer,SceneGameChildZorder::MapBackground);
    addChild(maskOnlyAlpha,SceneGameChildZorder::MapAlphaDrawing-1);
    addChild(_maskAlphaSprite,SceneGameChildZorder::MapAlphaDrawing);
    addChild(maskOnlyColor,SceneGameChildZorder::MapAlphaDrawing+1);
    addChild(_dirtLayer,SceneGameChildZorder::MapDirt);
    
    addChild(_borderlayer,SceneGameChildZorder::MapDirt+1);
    
    // addChild(_plantLayer,888);
    /////////////////////////////////////////////////////////////////
    // _backgroundLayer->setVisible(false);
    // _dirtLayer->setVisible(false);
    //_borderlayer->setVisible(false);
    _borderLeftLayer = LayerRollImage::create();
    _borderLeftLayer->_itemAnchorPoint =Vec2(0,0);
    _borderLeftLayer->_xPosition = 0;
    _borderLeftLayer->setImage(PATH_LAYER_BORDER);
    _layoutGameLayer(_borderLeftLayer);
    
    
    _borderRightLayer = LayerRollImage::create();
    _borderRightLayer->_itemAnchorPoint =Vec2(1,0);
    _borderRightLayer->_itemIsFlippedx = true;
    _borderRightLayer->_xPosition = 1;
    _borderRightLayer->setImage(PATH_LAYER_BORDER);
    _layoutGameLayer(_borderRightLayer);
    _borderLeftLayer->setImageStartY(0);
    _borderRightLayer->setImageStartY(0);
    /////////////////////////////////////////////////////////////////
    addChild(_borderLeftLayer,888);
    addChild(_borderRightLayer,888);
    /////////////////////////////////////////////////////////////////
    //_borderRightLayer->setVisible(false);
    //_borderLeftLayer->setVisible(false);
    _lightLayer  = LayerLight::create();
    _layoutGameLayer(_lightLayer);
    addChild(_lightLayer,999);
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    
    Button* btnAddSpeed =Button::create("flower.png");
    Button* btnSubSpeed =Button::create("back.png");
    btnAddSpeed->setScale(0.5);
    btnSubSpeed->setScale(0.5);
    
    btnAddSpeed->setAnchorPoint(Vec2(1,0));
    btnAddSpeed->setPosition(Vec2(WinSize.width-10,10));
    btnSubSpeed->setAnchorPoint(Vec2(1,0));
    btnSubSpeed->setPosition(Vec2(WinSize.width-70,10));
    addChild(btnAddSpeed,999);
    addChild(btnSubSpeed,999);
    _plantLayer->setGrowSpeed(60);
    Label*  lable = Label::createWithSystemFont("生长速度: 60","Arial",20);
    
    DrawLayerCallFunc* worldDraw = DrawLayerCallFunc::create();
    worldDraw->_callBack =  std::bind(&SceneGame::onDraw,this);
    addChild(worldDraw,1000);
    addChild(lable,999);
    lable->setAnchorPoint(Vec2(1,0));
    lable->setPosition(Vec2(WinSize.width-90,100));
    
    _lightSprite= Sprite::create("light_big.png");
    addChild(_lightSprite,SceneGameChildZorder::MapAlphaDrawing-3);
    _lightSprite->setVisible(false);
    
    btnAddSpeed->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
                                       {
                                          // testBorderLine();
                                           if(type !=  Widget::TouchEventType::ENDED)
                                               return;
                                           
                                           float speed = _plantLayer->getGrowSpeed();
                                           speed+=EDIT_SPEED_SPED;
                                           if (speed>240) {
                                               speed = 240;
                                           }
                                           _plantLayer->setGrowSpeed(speed);
                                           char buf[40] = {0};
                                           sprintf(buf, "生长速度: %d",(int)speed);
                                           lable->setString(buf);
                                           
                                       });
    btnSubSpeed->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
                                       {
                                           if(type !=  Widget::TouchEventType::ENDED)
                                               return;
                                           float speed = _plantLayer->getGrowSpeed();
                                           speed -=EDIT_SPEED_SPED;
                                           if (speed < 0) {
                                               speed = 0;
                                           }
                                           _plantLayer->setGrowSpeed(speed);
                                           char buf[40] = {0};
                                           sprintf(buf, "生长速度: %d",(int)speed);;
                                           lable->setString(buf);
                                       });
    
    Sprite* ringbottom = Sprite::create("ring_bottom.png");
    Sprite* ringtop    = Sprite::create("ring_top.png");
    ringbottom->setAnchorPoint(Vec2::ZERO);
    ringtop->setAnchorPoint(Vec2::ZERO);
    ringbottom->setPosition(Vec2::ZERO);
    ringtop->setPosition(Vec2::ZERO);
    addChild(ringbottom,1000);
    addChild(ringtop,1001);
    
//add Items
    _layerItem = LayerItem::create();
    _layerItem->setAnchorPoint(Vec2(0.5,0));
    _layerItem->setPosition(Vec2(VisibleSize.width/2,0));
    _layerItem->loadItemsAndBodys();
    this->addChild(_layerItem,MapItem);
    initLevelEditorMenu();
///////////////////////////////////////////////
// add for test
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(_plantLayer->_plant->_headCur.getPosition().x, _plantLayer->getPlantTopHeight());
        bodyDef.angle = - CC_DEGREES_TO_RADIANS(_plantLayer->_plant->_headCur.getAngle());
        bodyDef.linearDamping = 0.3f;
        bodyDef.userData = _plantLayer->_plant;
        b2Body* body = world->CreateBody(&bodyDef);
    
        b2FixtureDef fixtureDef;
        b2PolygonShape headCurShape;
    
        headCurShape.SetAsBox(0.01*WinSize.width/PTM_RATIO,0.01*WinSize.height/PTM_RATIO);
        fixtureDef.shape = &headCurShape;
        fixtureDef.density = 10.0;
        fixtureDef.filter.categoryBits = 0x01;
        fixtureDef.filter.maskBits = 0x02;
        fixtureDef.restitution = 0.1;
        body->CreateFixture(&fixtureDef);
    _physicsHandler->getItemBodies().push_back(body);
////////////////////////////////////////////
}

void SceneGame::initPhysics()
{
    b2Vec2 gravity = b2Vec2(0.0,-0.0);
    world = new b2World(gravity);
    
    _physicsHandler = PhysicsHandler::create(world);
    _physicsHandler->retain();
    
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

void SceneGame::initLevelEditorMenu()
{
    Label* levelsLabel = Label::createWithTTF("Levels", "Marker Felt.ttf", 36);
    MenuItem* toLevelsItem = MenuItemLabel::create(levelsLabel, [&](Ref*){
        CC_SAFE_RELEASE_NULL(_gameManager->_fileHandler);
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
///////////////////////////////////////////////////////////b
    _plantLayer->update(dt);
    _lightLayer->update(dt);
//    _goodsLayer->update(dt);取消随机光芒
    
    // if (_isMoving) {
    if (_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.6) {
        moveDownGameView(_plantLayer->getPlantTopHeight()-_gameManager->_visible.height*0.6);
    }
    else _isMoving =false;
    //  }
    //  else if(_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.65) {
    //     _isMoving=true;
    //}
    
    _textureAlphaMask->updateWithData(_mapGrid.getDataByOriginY(), 0,0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
/////////////////////////////////////////////////////////////////////////
    //位置同步与道具碰撞检测
    _physicsHandler->update(dt);

}

//////////////////////////////////////////////////////////////////////////////////Begin
void SceneGame::moveActionCallBack(float dt)
{
    if (_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.6) {
        moveDownGameView(dt*MOVE_DOWN_UNIT);
    }
}

void SceneGame::_layoutGameLayer(Node* gameLayer)
{
    gameLayer->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
}

void SceneGame::moveDownMapGrid(int moveLen)
{
    
    _mapGrid._startHeight += moveLen;
    
    if (_mapGrid._startHeight+ _mapGrid._maxViewHeight >= _mapGrid._maxViewHeight+50 ) {
        
        memmove(_mapGrid._gridData, _mapGrid._gridData+(_mapGrid._startHeight)*_mapGrid._gridWidth,(_mapGrid._maxViewHeight+20)*_mapGrid._gridWidth);
        _mapGrid.setData(0,_mapGrid._maxViewHeight+20,_mapGrid._gridWidth,50,GridType::Dirt);
        _borderlayer->moveDownGridCell(_mapGrid._startHeight);
        _mapGrid._startHeight=0;
    }
    _borderlayer->suboutViewBorderCell();
}

void SceneGame::moveDownMapGrid(float yLen)
{
    _mapGrid._mapGridStarty += yLen;
    int n =  _mapGrid._mapGridStarty/_mapGrid._unitGridSize.height;
    if (n>0)
    {moveDownMapGrid(n);
        _mapGrid._mapGridStarty -= n*_mapGrid._unitGridSize.height;
    }
}

void SceneGame::moveDownAlphaMask(float yLen)
{
    float y = _maskAlphaSprite->getPositionY();
    y -= yLen;
    _maskAlphaSprite->setPositionY(y);
    
    cocos2d::Size visibleSize = Director::getInstance()->getWinSize();
    if (y <= -_mapGrid._unitGridSize.height)
    {
        int n = fabs(y)/_mapGrid._unitGridSize.height;
        _maskAlphaSprite->setPositionY(y+n*_mapGrid._unitGridSize.height);
    }
}

void SceneGame::moveDownGameView(float yLen)
{
    _backgroundLayer->moveDown(yLen*0.75);
    _dirtLayer->moveDown(yLen);
    moveDownAlphaMask(yLen);
    moveDownMapGrid(yLen);
    _borderlayer->moveDown(yLen);
    _plantLayer->moveDown(yLen);
    _borderLeftLayer->moveDown(yLen*1.3);
    _borderRightLayer->moveDown(yLen*1.3);
//    _goodsLayer->moveDown(yLen);取消随机光芒
    _lightLayer->moveDown(yLen);
/////////////////////add by wlg
    _layerItem->moveDown(yLen);
//////////////////////////////////////////
}
bool  SceneGame::testMapGridCrash(Vec2 point,int type)
{
    if(type==0)
    {
        if (point.x<40|| point.x > VisibleSize.width-40)return true;
        // modify by wlg
        if(_physicsHandler->isPointContact(point))return true;
    }
    Vec2 pn = point + Vec2(0,_mapGrid._mapGridStarty);
    GridCell cell = _mapGrid.getMapGridCellByPosition(pn);
    
    if(_mapGrid.isOutMapGrid(cell))return false;
    
    return _mapGrid.getValue(cell._x, cell._y) ==255;
}
//bool SceneGame::testMapGridCrash(Vec2 point)
//{
//    Vec2 pn =point + Vec2(0,_mapGrid._mapGridStarty);
//    GridCell cell = _mapGrid.getMapGridCellByPosition(pn);
//    if(_mapGrid.isOutMapGrid(cell))return true;
//    return _mapGrid.getValue(cell._x, cell._y) ==255||isPointContact(point);
//}

bool SceneGame::onTouchBegan(Touch* touch,Event* event)
{
    auto pt =touch->getLocation();
    if(pt.x < 60 ||
       pt.x > Director::getInstance()->getWinSize().width-60)return false;
    if(_mapGrid.touchClearGrid(pt))
    {
        //   _borderlayer->updateBorder(0, 0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
    }
    _touchPrePoint = touch->getLocation();
    
    return true;
}

void SceneGame::onTouchMoved(Touch* touch,Event* event)
{
    cocos2d::Point pt = touch->getLocation();
    if (fabs(pt.x-_touchPrePoint.x)>32||
        fabs(pt.y-_touchPrePoint.y)>32) {
        if(_mapGrid.touchClearGrid(touch->getLocation()))
            //  _borderlayer->updateBorder(0, 0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
            _touchPrePoint = pt;
    }
    
}

void SceneGame::onTouchEnded(Touch* touch,Event* event)
{
    
    _plantLayer->_isNeedGrow=true;
}

void SceneGame::testBorderLine()
{
    auto ip = _borderlayer->_borderMap.begin();
    auto end = _borderlayer->_borderMap.end();
    while (ip!=end) {
        Sprite* sp = Sprite::create("border_outlight.png");
        sp->setScale(0.1);
        sp->setOpacity(40);
        addChild(sp,SceneGameChildZorder::MapBackground+1);
        auto  cell = ip->first;
        auto pt = _mapGrid.getPositionByMapGridCell(cell);
        sp->setPosition(pt);
        BlendFunc func = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
        sp->setBlendFunc(func);
        ip++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////End