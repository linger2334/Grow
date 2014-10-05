//Begin
#include "SceneGame.h"
#include "UIHelper.h"
#include "LayerBackMask.h"
#include "ContorlPoint.h"
#include "TestAction.h"

#define  MOVE_DOWN_UNIT 240
//End

#include "SimpleAudioEngine.h"
#include "StaticData.h"
#include "LevelEditor.h"
#include "LevelsMenu.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

SceneGame::SceneGame():world(nullptr),_debugDraw(nullptr),_levelFileHandler(nullptr),_layerItem(nullptr)
{
    
}

SceneGame::~SceneGame()
{
    _gameManager->_sceneGame = nullptr;
    CC_SAFE_DELETE(world);
    CC_SAFE_DELETE(_debugDraw);
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
    GameManager::getInstance()->_sceneGame = this;
    //
    
    initInfo();
    _isMoving=false;
    
    scheduleUpdate();
    
    return true;
}

void SceneGame::initInfo()
{

/////////////////////////////////////////////
    _gameManager  = GameManager::getInstance();
    
    _gameManager->_plantGrowCrashTestCallback = std::bind(&SceneGame::testMapGridCrash,this,std::placeholders::_1);
    _isMovingLight =false;
    
    cocos2d::Size visibleSize = Director::getInstance()->getWinSize();
    cocos2d::Size gridSize =_gameManager->getMapGridUnitVisibleSize();
    _mapGrid.resize(gridSize.width, gridSize.height*2);
    _mapGrid.setData(0, 0, 255);
    
    
    _mapGrid._maxViewHeight = _gameManager->getMapGridUnitVisibleSize().height;
    _mapGrid._unitGridSize = _gameManager->getMapGridUnitSize();
    _gameManager->_mapGrid =&_mapGrid;
    
    _textureAlphaMask = new Texture2D();
    _textureAlphaMask->initWithData(_mapGrid._gridData,gridSize.width*(gridSize.height), Texture2D::PixelFormat::A8,gridSize.width, gridSize.height, cocos2d::Size(gridSize.width,gridSize.height));
    _textureAlphaMask->autorelease();
    
    
    _backgroundLayer  = LayerRollImage::create();
    _backgroundLayer->setImage("background_5.png");
    _backgroundLayer->setImageStartY(0);
    this->_layoutGameLayer(_backgroundLayer);
    
    RenderMaskColorNode* maskOnlyAlpha= RenderMaskColorNode::create(false,false,false,true);
    _maskAlphaSprite = Sprite::createWithTexture(_textureAlphaMask);
    _maskAlphaSprite->setScale(_gameManager->getMapGridUnitSize().width,_gameManager->getMapGridUnitSize().height);
    BlendFunc funcMaskSprite = {GL_ONE,GL_ZERO};
    _maskAlphaSprite->setBlendFunc(funcMaskSprite);
    _maskAlphaSprite->setAnchorPoint(Vec2(0.5,1));
    _maskAlphaSprite->setRotationX(180);
    this->_layoutGameLayer(_maskAlphaSprite);
    
    RenderMaskColorNode* maskOnlyColor= RenderMaskColorNode::create(true,true,true,false);
    
    _dirtLayer  = LayerRollImage::create();
    _dirtLayer->_blendFunc =  {GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};
    _dirtLayer->setImage("dirt512_2.png");
    _dirtLayer->setImageStartY(0);
    this->_layoutGameLayer(_dirtLayer);
    
    _borderlayer  = LayerBorder::create();
    _mapGrid._layerBorder = _borderlayer;
    this->_layoutGameLayer(_borderlayer);
    _mapGrid.testClear(gridSize.width/2,0,40);
    _borderlayer->updateBorder(0, 0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
    
    _plantLayer = LayerPlant_1::create();
    _gameManager->_layerplant = _plantLayer;
    this->_layoutGameLayer(_plantLayer);
    
    _goodsLayer = LayerGoods1::create();
    this->_layoutGameLayer(_goodsLayer);
    
    this->addChild(_goodsLayer,SceneGameChildZorder::MapGoods);
    this->addChild(_plantLayer,SceneGameChildZorder::MapAlphaDrawing-5);
    this->addChild(_backgroundLayer,SceneGameChildZorder::MapBackground);
    this->addChild(maskOnlyAlpha,SceneGameChildZorder::MapAlphaDrawing-1);
    this->addChild(_maskAlphaSprite,SceneGameChildZorder::MapAlphaDrawing);
    this->addChild(maskOnlyColor,SceneGameChildZorder::MapAlphaDrawing+1);
    this->addChild(_dirtLayer,SceneGameChildZorder::MapDirt);
    
    this->addChild(_borderlayer,SceneGameChildZorder::MapDirt+1);
    _backgroundLayer->setVisible(false);
//    _dirtLayer->setVisible(false);
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    _lightListHelper._sceneGame =this;
    _gameManager->_lightListHelper = & _lightListHelper;
    for (int i=0; i<20; i++) {
        _lightListHelper.addLight(testLight());
    }
//add Items
    //add physics world
    initPhysics();
    _layerItem = LayerItem::create();
    _layerItem->setAnchorPoint(Vec2(0.5,0));
    _layerItem->setPosition(Vec2(VisibleSize.width/2,0));
    _layerItem->loadItemsAndBodys();
    this->addChild(_layerItem,MapScrollView);
    initLevelEditorMenu();
///////////////////////////////////////////////
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

void SceneGame::initPhysics()
{
    b2Vec2 gravity = b2Vec2(0.0,-0.0);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    world->SetContactListener(this);
    //调试绘图
    _debugDraw = new GLESDebugDraw(PTM_RATIO);
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

//b2Body* SceneGame::createBodyForPlant()
//{
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_dynamicBody;
//    bodyDef.position.Set(_plantLayer->_plant->_headCur.getPosition().x, _plantLayer->getPlantTopHeight());
//    bodyDef.angle = - CC_DEGREES_TO_RADIANS(_plantLayer->_plant->_headCur.getAngle());
//    bodyDef.linearDamping = 0.3f;
//    bodyDef.userData = _plantLayer->_plant;
//    b2Body* body = world->CreateBody(&bodyDef);
//    
//    b2FixtureDef fixtureDef;
//    b2PolygonShape headCurShape;
//    
//    headCurShape.SetAsBox(0.01*WinSize.width/PTM_RATIO,0.01*WinSize.height/PTM_RATIO);
//    fixtureDef.shape = &headCurShape;
//    fixtureDef.density = 10.0;
//    fixtureDef.filter.categoryBits = 0x02;
//    fixtureDef.filter.maskBits = 0x02;
//    fixtureDef.restitution = 0.1;
//    body->CreateFixture(&fixtureDef);
//    
//    return body;
//}


void SceneGame::update(float dt)
{
    if (_isMoving) {
        if (_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.4) {
            moveDownGameView(dt*MOVE_DOWN_UNIT);
        }
        else _isMoving =false;
    }
    else if(_plantLayer->getPlantTopHeight()>_gameManager->_visible.height*0.75) {
        _isMoving=true;
    }
    
    float dt1 = dt;
    if(dt1> 3.0/60.0f) dt1=3.0/60.0f;
    _plantLayer->update(dt1);//植物生长
    
    _lightListHelper.update(dt);
    _textureAlphaMask->updateWithData(_mapGrid.getDataByOriginY(), 0,0, _mapGrid._gridWidth, _mapGrid._maxViewHeight);
    _goodsLayer->update(dt);
    
    //物理世界刷新
    int32 velocityIterations = 8;
    int32 positionIterations = 1;
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = world->GetBodyList(); b; b=b->GetNext()) {
            Node* item = static_cast<Node*>(b->GetUserData());
            cocos2d::Point ptInGL = item->getParent()->convertToWorldSpace(item->getPosition());
        
            b2Vec2 b2Position = b2Vec2(ptInGL.x/PTM_RATIO,ptInGL.y/PTM_RATIO);
            float32 b2Angle = -CC_DEGREES_TO_RADIANS(item->getRotation());
        
        if(item == _plantLayer->_plant)
        {
            b2Angle = -CC_DEGREES_TO_RADIANS(_plantLayer->_plant->_headCur.getAngle());
            ptInGL = _plantLayer->_plant->_headCur.getPosition();
            float headx =ptInGL.x + WinSize.width/2 +  0.04*WinSize.height*sinf(-b2Angle);
            float heady =_plantLayer->getPlantTopHeight()+ 0.04*WinSize.height*cosf(-b2Angle);
            b2Position = b2Vec2(headx/PTM_RATIO,heady/PTM_RATIO);
        }

        b->SetTransform(b2Position, b2Angle);
    }
    
    for(b2Body* body : _itembodies) {
        if (find(_itembodies.begin(),_itembodies.end(),body)!=_itembodies.end()) {
            Sprite* item = static_cast<Sprite*>(body->GetUserData());
            if (item&&item->getParent()->convertToWorldSpace(item->getPosition()).y<-WinSize.height/2) {
                item->removeFromParent();
                _itembodies.erase(find(_itembodies.begin(),_itembodies.end(),body));
                world->DestroyBody(body);
            }
        }
        
    }
    
}

bool SceneGame::isPointContact(cocos2d::Point ptInGl)
{
    b2Vec2 ptInB2 = b2Vec2(ptInGl.x/PTM_RATIO,ptInGl.y/PTM_RATIO);
    for (b2Body* body : _itembodies) {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture=fixture->GetNext()) {
            if(fixture->TestPoint(ptInB2))
            {
                return true;
            }
        }
    }
    return false;
}

void SceneGame::BeginContact(b2Contact* contact)
{
    MyContact myContact(contact->GetFixtureA(),contact->GetFixtureB());
    _contacts.insert(myContact);
}

void SceneGame::EndContact(b2Contact* contact)
{
    MyContact myContact(contact->GetFixtureA(),contact->GetFixtureB());
    _contacts.erase(myContact);
}

void SceneGame::PreSolve(b2Contact* contact,const b2Manifold* oldManifold)
{
    
}

void SceneGame::PostSolve(b2Contact* contact,const b2ContactImpulse* impulse)
{
    
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
    _backgroundLayer->moveDown(yLen*0.2);
    _dirtLayer->moveDown(yLen);
    moveDownAlphaMask(yLen);
    moveDownMapGrid(yLen);
    _borderlayer->moveDown(yLen);
    _plantLayer->moveDown(yLen);
    for (auto i:_lightListHelper._lightList) {
        i._node->setPositionY(i._node->getPositionY()-yLen);
    }
    _goodsLayer->moveDown(yLen);
//add by wlg
    _layerItem->moveDown(yLen);
//////////////
}

bool SceneGame::testMapGridCrash(Vec2 point)
{
    Vec2 pn =point + Vec2(0,_mapGrid._mapGridStarty);
    GridCell cell = _mapGrid.getMapGridCellByPosition(pn);
    if(_mapGrid.isOutMapGrid(cell))return true;
    return _mapGrid.getValue(cell._x, cell._y) ==255||isPointContact(point);
}

bool SceneGame::onTouchBegan(Touch* touch,Event* event)
{
    if(_mapGrid.touchClearGrid(touch->getLocation()))
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

void SceneGame::lightCallback1(Node* node)
{
    _lrTuoyuanConfig config ;
    config.aLength = rand()%30+20;
    config.cLength = 20;
    
    cocos2d::Point s_startPosition = Vec2(0,0);// _target->getParent()->getPosition();//m_sConfig.centerPosition;//中心点坐标
    float a = config.aLength;
    float bx = s_startPosition.x;
    float by = s_startPosition.y;
    float c = config.cLength;
    float x = tuoyuanXat(a, bx, c, 0);//调用之前的坐标计算函数来计算出坐标值
    float y = tuoyuanYat(a, by, c, 0);
    Vec2 po = s_startPosition+ Vec2(x-a, y);
    
    MoveTo* moveTo = MoveTo::create(rand()%10+2,po);
    LRTuoyuanBy* lr = LRTuoyuanBy::actionWithDuration(rand()%20+5, config);
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback1, this));
    Sequence* seq = Sequence::create(moveTo,lr, call,NULL);
    node->runAction(seq);
}

void SceneGame::lightCallback2(Node* node)
{
    ScaleBy* scale = ScaleBy::create(rand()%4+2, 2.0f);
    
    
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback2, this));
    Sequence* seq = Sequence::create(scale, scale->reverse(),DelayTime::create(rand()%20+5), call,NULL);
    node->runAction(seq);
}

Node* SceneGame::testLight()
{
    Color3B color = Color3B(rand()%255,rand()%255,rand()%255);
    Node* light = Node::create();
    light->setAnchorPoint(Vec2(0.5,0.5));
    
    Node* node = Node::create();
    node->setAnchorPoint(Vec2(0.5,0.5));
    
    Sprite* sp1 = Sprite::create("light_1.png");
    sp1->setColor(color);
    sp1->setOpacity(90);
    
    Sprite* sp2 = Sprite::create("light_1.png");
    sp2->setColor(color);
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback, this));//持续若隐若现
    //CallFuncN* call = CallFuncN::create(func);
    sp2->runAction(call);
    
    CallFuncN* call1 = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback1, this));//持续移到某一位置再做椭圆运动
    node->runAction(call1);
    CallFuncN* call2 = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback2, this));//持续放大缩小
    node->runAction(call2);
    
    node->addChild(sp1);
    node->addChild(sp2);
    light->addChild(node);
    return light;
    
}

void SceneGame::lightCallback(Node* node)
{
    node->stopAllActions();
    DelayTime* delayTime =DelayTime::create(rand()%20+3);
    FadeIn* in = FadeIn::create(rand()%4+1);
    FadeOut* out = FadeOut::create(rand()%4+1);
    CallFuncN* call = CallFuncN::create(CC_CALLBACK_1(SceneGame::lightCallback, this));
    Sequence* seq = Sequence::create(delayTime,out,in, call,NULL);
    node->runAction(seq);
}

/////////////////////////////////////////////////////////////////////////////////////////////////End