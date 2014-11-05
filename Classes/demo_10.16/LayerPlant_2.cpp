#include "LayerPlant_2.h"
#include "GameManager.h"
#include "ContorlPoint.h"
#include "LayerLight.h"
#include "TypeBase.h"
#include "Macro.h"
#include "ItemModel.h"
#define MIN_WIDTH 20
#define MAX_WIDTH 70
#define HEAD_HEIGHT 180
#define MOVE_LENGTH 1
#define GROW_HEIGHT 1
#define HEAD_ADD  0.5
#define GROW_STEP 60
#define MINGROW_LEN 5
#define ROTATE_LENGTH 100

#define LEAF_RAND_LENGTH 100
#define LEAF_LENGHT      300

#define CONVERT(T) _plant->convertToWorldSpace(T)

#define MAX_CRASH_RAY_LENGTH  90
#define MIN_CRASH_RAY_LENGTH  15
#define MINCRASH_LENGTH 10

#define MAX_GROW_ANGLE_TOP  90
#define MAX_GROW_ANGLE_LEFT  (-90)
#define MAX_GROW_ANGLE_RIGHT 90


#define CRASH_TEST_LENGTH_2 80
bool LayerPlant_2::init()
{
    LayerPlantBase::init();
    GameManager::getInstance()->_layerplant = this;
    this->_crashBorderBatch = SpriteBatchNode::create("border_outlight.png");
    addChild(_crashBorderBatch);
    BlendFunc func = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
    this->_crashBorderBatch->setBlendFunc(func);
    
    _leafBatch = SpriteBatchNode::create("plant1_leaf_1.png");
    addChild(_leafBatch);
    
    _lightSprite= Sprite::create("light_big.png");
    addChild(_lightSprite);
    _lightSprite->setVisible(false);
    
    _plant = Plant_1::create();
    _plant->initPlantInfo(HeadCursorV2(ContorlPointV2(Vec2(0,0),80,0,0),HEAD_HEIGHT,ROTATE_LENGTH),GROW_HEIGHT);
    addChild(_plant);
    
    _plant->setPosition(Vec2(Director::getInstance()->getWinSize().width*0.5f,0));
    
    _isNeedGrow =true;
    
    _leafTopHeight = _plant->_headCur._cp._height;
    _left =true;
    Size size = GameManager::getInstance()->getVisibleSize();
    //  _plant->setVisible(false);
    _render = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    
    _render->setPosition(Vec2(size.width*0.5,size.height*0.5));
    
    setGrowSpeed(GROW_STEP);
    _islightRuningAction=false;
    _world = GameManager::getInstance()->getBox2dWorld();
    _isCheckAddleaf = false;
    createHeadB2Body();
    updateHeadB2Body();
    
    return true;
}

float LayerPlant_2::getPlantTopHeightInView()
{
    return _plant->convertToWorldSpace(_plant->_headCur.getPosition()).y;
}
void LayerPlant_2::renderPlant()
{
    
    return ;
    _plant->setVisible(true);
    
    _render->beginWithClear(0, 0, 0, 0);
    _plant->visit();
    _render->end();
    _plant->setVisible(false);
}
void LayerPlant_2::onEnter()
{
    GameLayerRollY::onEnter();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("plant_grow_music_1.mp3");
    
    _musicId =CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("plant_grow_music_1.mp3",true);
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.02);
}
void LayerPlant_2::onExit()
{   GameLayerRollY::onExit();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
    
    //if(_plant)_plant->onExit();
}
void  LayerPlant_2::update(float dt)
{
    if(GameManager::getInstance()->isPause())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
        return;
    }
    bool isGrow =false;
    if (_isReGrow) {
        if(reGrow(dt)<=0)
            _isReGrow = false;
        updateHead(dt);
        isGrow = true;
    }
    else
    {
        float growLen = _growSpeed*dt;
        while (growLen>0) {
            growLen = grow2(growLen);
            if(growLen >= 0)
            {
                isGrow =true;
                updateHead(dt);
            }
        }
    }
    
    if (!isGrow && !_islightRuningAction) {
        FadeTo* fadeto = FadeTo::create(5,100);
        FadeTo* fadeto1 = FadeTo::create(5,0);
        Sequence* seq = Sequence::create(fadeto, fadeto1,nullptr);
        _lightSprite->stopAllActions();
        _lightSprite->runAction(RepeatForever::create(seq));
        _islightRuningAction = true;
        showCrashBorderLight();
    }
    else if(isGrow)
    {_lightSprite->stopAllActions();_lightSprite->setVisible(false);
        _islightRuningAction=false;
        removeBorderLight();
    }
    if (isGrow) {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(_musicId);
        updateHeadB2Body();
        renderPlant();
        
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_musicId);
        _musicId =CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("plant_grow_music_1.mp3",true);
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(_musicId);
    }
    
}
void  LayerPlant_2::doReGrow(float speed, float len)
{
    _isReGrow = true;
    _reGrowSpeed = speed;
    _reGrowLength = len;
}
int   LayerPlant_2::reGrow(float dt)
{
    float ret = 0;
    float reGrowLen = _reGrowSpeed*dt;
    if (reGrowLen >= _reGrowLength) {
        reGrowLen = _reGrowLength;
        ret =0;
    }
    else ret =1;
    _reGrowLength -= reGrowLen;
    auto ip = _plant->_cpLineNode._cpList.rbegin();
    auto end = _plant->_cpLineNode._cpList.rend();
    if (ip == end) return -1;
    float height = _plant->_headCur._cp._height;
    while (ip!=end) {
        ip++;
        float theight =ip->_height ;
        if (height - theight < reGrowLen) {
            _plant->_cpLineNode._cpList.pop_back();
        }
        else break;
    }
    auto cp =*_plant->_cpLineNode._cpList.rbegin();
    float radius =_plant->_headCur._cp._radius;
    _plant->_headCur._cp = cp;
    _plant->_headCur._cp._radius = radius;
    return ret;
}
void  LayerPlant_2::checkAddLeaf()
{
    
    float randlen = rand()%LEAF_RAND_LENGTH+LEAF_LENGHT;
    if (_isCheckAddleaf &&  _plant->_headCur._cp._height - _leafTopHeight > 40)
    {
        Sprite* sp = Sprite::createWithTexture(_leafBatch->getTexture());
        _leafBatch->addChild(sp);
        sp->setPosition(_leafPoint);
        sp->setScale(0.0);
        float angle;
        if(_left)
        { sp->setAnchorPoint(Vec2(1,0.5));
            sp->setFlippedX(true);
            angle =  _plant->_headCur._cp._angle+30 + 22 - rand()%20;
            _left =false;
        }
        else
        {
            sp->setAnchorPoint(Vec2(0,0.5));
            angle =   _plant->_headCur._cp._angle-30 + 22 -rand()%20;
            _left = true;
        }
        float time = (_growSpeed - 30.0f)/ 210.0f;
        FadeIn* fadein = FadeIn::create(2- time*1.2);
        sp->setOpacity(0);
        sp->runAction(fadein);
        sp->setRotation(angle);
        
        _leafTopHeight = _plant->_headCur._cp._height;
        sp->setUserData((void*)(int)_leafTopHeight);
        
        _isCheckAddleaf = false;
    }
    if (_plant->_headCur._cp._height - _leafTopHeight > randlen) {
        _leafPoint = _plant->convertToWorldSpace(_plant->_headCur._cp._point);
        _isCheckAddleaf =true;
        _leafCheckAddHeight = _plant->_headCur._cp._height;
    }
}


void LayerPlant_2::getPlantRayCrashLength(float outLens[3])
{
    
    Vec2 cpt = CONVERT(_plant->_headCur._cp.getTopPositionByLength(MAX_CRASH_RAY_LENGTH));
    Vec2 cpo = CONVERT(_plant->_headCur.getPosition());
    static int angles[3] = { -40 , 0 ,40};
    for(int i =0; i < 3; i++)
    {
        Vec2 pt = MathHelper::getRotatePosition(cpo, cpt,angles[i]);
        RayCastCallback cash;
        cash._orign = cpo;
        _world->RayCast(&cash,b2Vec2(cpo.x/PTM_RATIO,cpo.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
        if (cash._crash) outLens[i] = cash._crashLen;
        else outLens[i] =MAX_CRASH_RAY_LENGTH +10;
    }
}

bool    LayerPlant_2::isRayCastCrashStone(Vec2 pt)
{
    RayCastStoneCallback cash;
    Vec2 po = CONVERT( _plant->_headCur.getPosition());
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash;
}
bool    LayerPlant_2::isRayCastCrashMapGrid(Vec2 pt)
{
    RayCastMapGridCallback cash;
    Vec2 po = CONVERT( _plant->_headCur.getPosition());
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash;
}
bool   LayerPlant_2::isRayCastCrash(Vec2 pt)
{
    RayCastCallback cash;
    Vec2 po = CONVERT( _plant->_headCur.getPosition());
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash;
}
bool   LayerPlant_2::isCashPoint(Vec2 pt, int type)
{
    RayCastCallback cash;
    Vec2 po = CONVERT( _plant->_headCur.getPosition());
    _world->RayCast(&cash,b2Vec2(po.x/PTM_RATIO,po.y/PTM_RATIO), b2Vec2(pt.x/PTM_RATIO,pt.y/PTM_RATIO));
    return cash._crash||isMapGridCrash(pt); //isCrashPointGridAndStone(pt);
}

bool   LayerPlant_2::isCrashPointGridAndStone(Vec2 pt)
{
    return isRayCastCrashMapGrid(pt)||isRayCastCrashStone(pt)||_gameManager->_physicsHandler->isPointContact(pt);
}
class QueryAABBCallBack :public b2QueryCallback
{
public:
    virtual bool ReportFixture(b2Fixture* fixture);
};
GrowContext LayerPlant_2::getMinRangeCanGrowContext()
{
    GrowContext canGrow(true);
    
    bool isMapCrashLeft = false,isMapCrashRight = false,
    isStoneCrashleft = false,isStoneCrashRight=false;
    Vec2 leftNext=CONVERT(_plant->getNextGrowUnitLengthPosition(FaceLeft));
    Vec2 rightNext =CONVERT(_plant->getNextGrowUnitLengthPosition(FaceRight));
    isMapCrashLeft = isRayCastCrashMapGrid(leftNext);
    isMapCrashRight = isRayCastCrashMapGrid(rightNext);
    isStoneCrashleft = isStoneCrash(leftNext);
    isStoneCrashRight = isStoneCrash(rightNext);
    
    bool isCrashTop = isCrashPointGridAndStone(CONVERT(_plant->getNextGrowUnitLengthPosition(FaceTop)));
    
    float headAngle = _plant->_headCur.getAngle();
    bool isCrashLeft = isMapCrashLeft||isStoneCrashleft;
    bool isCrashRight = isMapCrashRight || isStoneCrashRight;
    
    if (isCrashLeft)
    {
        canGrow._left = false;
    }
    if (isCrashRight) {
        canGrow._right = false;
    }
    if (isCrashTop) {
        canGrow._top =false;
    }
    
    float angle1 = 90;
    if (isStoneCrashleft&&isStoneCrashRight) {
        angle1 = 130;
        auto  cptop = _plant->_headCur._cp;
        cptop._radius = 40;
        
        bool crashsStone[9] = {0};
        
        Vec2 cpl = cptop.getPositionLeftByLength(60);
        Vec2 cpo = cptop.getContorlPosition();
        // Vec2 cpl = cpo + Vec2(-60,0);
        float stepAnalge = 180.0f /9.0f;
        for (int i = 0 ; i< 9 ; i++) {
            
            Vec2 ptemp = CONVERT(MathHelper::getRotatePosition(cpo, cpl,i*stepAnalge));
            bool isStone = isStoneCrash(ptemp);
            crashsStone[i]=isStone;
        }
        
        int lc =0 ,rc =0;
        bool leftFlag =true;
        bool rightFlag= true;
        for (int i = 0; i<4; i++) {
            if (leftFlag&&!crashsStone[i])lc++;
            else leftFlag = false;
            
            if (rightFlag&&!crashsStone[8-i])rc++;
            else rightFlag = false;
        }
        if (lc>rc&& !isMapCrashLeft)canGrow._left = true;
        else if (rc>= lc && !isMapCrashRight)canGrow._right = true;
    }
    Vec2 cpo = _plant->_headCur.getPosition();
    bool isTestStoneLeft =isStoneCrash(CONVERT(cpo+Vec2(-80,0)));
    bool isTestStoneRight = isStoneCrash(CONVERT(cpo+Vec2(80,0)));
    
    float angleLeft = -angle1;
    if (isTestStoneLeft) {
        angleLeft = -40;
    }
    float angleRight = angle1;
    if (isTestStoneRight)
    {
        angleRight = 40;
    }
    
    
    if( headAngle < angleLeft)
    {
        canGrow._left = false;
    }
    if( headAngle > angleRight)
    {
        canGrow._right = false;
    }
    float  x = CONVERT(_plant->_headCur.getPosition()).x;
    Size size= _gameManager->getVisibleSize();
    float len =200;
    if (size.width<768) {
        len = 170;
    }
    if (x < len)
    {
        if(headAngle <-30)
            canGrow._left = false;
        canGrow._top =false;
    }
    else if(x > size.width - len)
    {
        if( headAngle >30)
            canGrow._right = false;
        canGrow._top =false;
    }
    return canGrow;
}

int   LayerPlant_2::getPlantHeadViewWidthType()
{
    int ret ;
    float  x = CONVERT(_plant->_headCur.getPosition()).x;
    Size size= _gameManager->getVisibleSize();
    if (x < size.width * 0.45 ) ret = -1;
    else if(x>size.width * 0.55 ) ret = 1;
    else ret =0;
    return ret;
}

void LayerPlant_2::getGroeDirListTest( std::vector<FaceDirection>& dirList)
{
    int type = getPlantHeadViewWidthType();
    int listtype = 0;
    float angle =_plant->_headCur.getAngle();
    listtype = angle                                                                                                                                                                                                       <= 0 ? 1:2;
    if (listtype == 1) {
        dirList.push_back(FaceDirection::FaceRight);
        dirList.push_back(FaceDirection::FaceLeft);
    }
    else
    {
        dirList.push_back(FaceDirection::FaceLeft);
        dirList.push_back(FaceDirection::FaceRight);
    }
}

GrowContext LayerPlant_2::getRayGrowLength(float vec[3],float testLength)
{
    GrowContext canGrow(false);
    
    if(vec[FaceLeft] > testLength){ canGrow._left  = true;}
    if(vec[FaceRight] > testLength){ canGrow._right  = true;}
    return canGrow;
}
float LayerPlant_2::grow2(float len)
{
    float retLen = -1;
    
    bool isGrow = false;
    FaceDirection trnDir;
    
    auto minGrow = getMinRangeCanGrowContext();
    std::vector<FaceDirection> growDirList;
    
    float  vecLen[3] = {0};
    GrowContext canGrow(false);
    getPlantRayCrashLength(vecLen);
    getGroeDirListTest(growDirList);
    
    for (int i =0 ; i<4; i++)
    {
        GrowContext grow;
        float len =MAX_CRASH_RAY_LENGTH-1-i*30;
        grow = getRayGrowLength(vecLen,len);
        for(auto& ip :growDirList)
        {
            if(minGrow.isCanGrowByDir(ip)&& grow.isCanGrowByDir(ip))
            {
                isGrow = true;
                trnDir = ip;
                break ;
            }
        }
        if (isGrow )break;
    }
    
    _plant->_headCur._rotateLenght = 60;
    if (isGrow) {
        retLen =_plant->grow(trnDir, len);
        if(retLen>0)
            if (_plant->_headCur._cp._point.y>_plant->_headTopHeight) {
                _plant->_headTopHeight= _plant->_headCur._cp._point.y;
            }
        checkAddLeaf();
        float len =_plant->_headCur._cp._height;
        float testlen =512;
        if (len > testlen) {
            _plant->checkHeightSub(testlen);
            _leafTopHeight-=testlen;
            checkLeafSubHeight(testlen);
            GameManager::getInstance()->getLayerLight()->subLightHeight(testlen);
        }
    }
    else{
        _isNeedGrow =false;
    }
    return retLen;
}

float LayerPlant_2::getPlantTopHeight()
{
    return  _plant->convertToWorldSpace(_plant->_headCur.getContorlPoint()._point).y;
}

void  LayerPlant_2::moveDown(float y)
{
    float oldY = _plant->getPositionY();
    float newY = oldY -y;
    if (newY < -300) {
        _plant->subPlantCP(200);
        newY = newY +200;
    }
    _plant->setPositionY(newY);
    std::list<Node*> removeList ;
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        (*i)->setPositionY((*i)->getPositionY()-y);
        Size wsize =Director::getInstance()->getWinSize();
        if (!(*i)->boundingBox().intersectsRect(Rect(0,0,wsize.width,wsize.height))) {
            removeList.push_back(*i);
        }
    }
    for (auto i:removeList ) {
        i->removeFromParent();
    }
    renderPlant();
}
void   LayerPlant_2::checkLeafSubHeight(float _len)
{
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        
        int height =   (long)((*i)->getUserData());
        (*i)->setUserData((void*)(int)(height-=_len));
    }
}
void  LayerPlant_2::updateHead(float dt)
{
    
    auto rip = _plant->_cpLineNode._cpList.rbegin();
    auto rend =_plant->_cpLineNode._cpList.rend();
    
    if (rip ==rend) {
        return ;
    }
    float  topHeight  = rip->_height;
    while (rip!=rend) {
        //rip->_radius = 100;
        if (topHeight - rip->_height <= 60) {
            rip->_radius = (topHeight - rip->_height)/60 *10;
        }
        else if (topHeight - rip->_height <=660) {
            rip->_radius = (topHeight-60 - rip->_height)/600 * 40.0f+10;
        }
        else if (topHeight - rip->_height > 660) {
            rip->_radius = (topHeight - rip->_height - 660)/400 * 40.0f+50.0f;
            if (rip->_radius >=MAX_WIDTH) {
                rip->_radius  =MAX_WIDTH;
            }
        }
        else break;
        rip->_radius*=2;
        rip++;
    }
    
    auto list = _leafBatch->getChildren();
    for (auto i = list.begin();i!=list.end();i++) {
        
        // int height = reinterpret_cast<int>((int*)(*i)->getUserData());
        float height = (long)(*i)->getUserData();
        if (topHeight - height <= 1000) {
            float scale = (topHeight - height)/1000*0.3 + 0.1;
            (*i)->setScale(scale);
            
        }
        else (*i)->setScale(0.4);
        
        
    }
    
}

void  LayerPlant_2::showCrashBorderLight()
{
    auto gamemanager = GameManager::getInstance();
    LayerBorder* border = gamemanager->getLayerBorder();
    auto pt = _plant->convertToWorldSpace(_plant->_headCur._cp._point);
    auto mapGrid = gamemanager->getMapGrid();
    auto cell = mapGrid->getMapGridCellByPosition(pt);
    int x, y,width,height;
    int rid = 23;
    x = cell._x-rid;
    y = cell._y-rid;
    width = x + 2*rid;
    height = y + 2*rid;
    if (x<0)x=0;
    if (y<0)y=0;
    if (width > mapGrid->_gridWidth)width = mapGrid->_gridWidth;
    if (height > mapGrid->_gridHeight)height = mapGrid->_gridHeight;
    auto end = border->_borderMap.end();
    std::list<GridCell> list;
    for (; y < height; y++) {
        for (int tx = x;tx < width ;tx++) {
            auto gridCell =GridCell(tx,y);
            // log("map %d,%d",gridCell._x,gridCell._y);
            auto ip = border->_borderMap.find(gridCell);
            if (ip!=end) {
                list.push_back(gridCell);
                
            }
        }
    }
    
    for (auto& i : list) {
        Sprite* sp = Sprite::createWithTexture(_crashBorderBatch->getTexture());
        
        auto pt = mapGrid->getPositionByMapGridCell(i);
        pt += Vec2(4,4);
        sp->setPosition(pt);
        sp->setScale(0.15);
        sp->setOpacity(0);
        sp->setUserData((void*)0);
        sp->setTag(i._y*mapGrid->_gridWidth+i._x);
        FadeTo* fadeto = FadeTo::create(2,60);
        sp->runAction(fadeto);
        _crashBorderBatch->addChild(sp);
    }
    
}
void LayerPlant_2::removeBorderLight()
{
    auto& list = _crashBorderBatch->getChildren();
    for (auto& i: list) {
        if ((long)i->getUserData()==100) continue;
        FadeTo* fadeto = FadeTo::create(0.3,0);
        
        CallFuncN* call = CallFuncN::create([](Node* node)
                                            {
                                                node->removeFromParent();
                                            });
        i->setUserData((void*)100);
        Sequence* seq = Sequence::create(fadeto, call,nullptr);
        i->runAction(seq);
    }
}
void LayerPlant_2::removeLightBorderByCell(int tag)
{
    
    auto node = _crashBorderBatch->getChildByTag(tag);
    if (node) {
        node->removeFromParent();
    }
}
void  LayerPlant_2::createHeadB2Body()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(0,0);
    bodyDef.userData = GameManager::getInstance()->_sPlantHeadItemModel;
    b2Body* body = _world->CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    b2PolygonShape shape;
    float len =10.0f/PTM_RATIO;
    b2Vec2 vec[3] ={
        b2Vec2(0,5.0f/PTM_RATIO),
        b2Vec2(-len,-len),
        b2Vec2(len ,-len)
    };
    //b2CircleShape circleShape;
    //circleShape.m_radius = 70/PTM_RATIO;
    shape.Set(vec, 3);
    fixtureDef.shape = &shape;
    fixtureDef.density = 10.0;
    fixtureDef.filter.categoryBits = 0x01;
    fixtureDef.filter.maskBits = 0x02;
    fixtureDef.restitution = 0.1;
    body->CreateFixture(&fixtureDef);
    _headBody = body;
}
void  LayerPlant_2::updateHeadB2Body()
{
    Vec2 cp =CONVERT(_plant->_headCur.getPosition());
    b2Vec2 newpt(cp.x/PTM_RATIO,cp.y/PTM_RATIO);
    float angle = _plant->_headCur.getAngle();
    _headBody->SetTransform(newpt, -CC_DEGREES_TO_RADIANS( angle));
}
