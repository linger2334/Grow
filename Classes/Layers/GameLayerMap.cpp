#include "GameLayerMap.h"
#include "common.h"
#include "GameHeaders.h"
#include "GameRuntime.h"
#include "GameLayerMapBorder.h"
#include "GameLayerExt.h"
#include "GameLayerRollImage.h"
#include "GamePhysicalWorld.h"
#include "GameRunningInfo.h"
class GameLayerMapDirt: public GameLayerRollImage,public GameLayerHelper<GameLayerMapDirt>
{
public:
    CREATE_FUNC(GameLayerMapDirt);
    virtual bool init(){return GameLayerRollImage::init();}
    virtual bool releaseGameInfo()
    {
        GameLayerRollImage::releaseGameInfo();
        return true;
    }
    virtual void moveDown(float yLen)
    {
        GameLayerRollImage::moveDown(yLen);
    }
    virtual bool initGameInfo()
    {
        _clearImages();
        BlendFunc blend = {GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA };
        setBlendFunc(blend);
        this->setImage(GameRunningConfig::getInstance()->getTextureMapDirt());
        this->setImageStartY(GameRunningConfig::getInstance()->getMapDirtImageStartY());
        return true;
    }

};
bool GameLayerMap::init()
{
    GameLayerBase::init();
    
    return true;
}
bool GameLayerMap::initGameInfo()
{
    auto runtime = GameRuntime::getInstance();
    cocos2d::Size visibleSize =runtime->getVisibleSize();
    cocos2d:: Size gridSize =runtime->getMapGridSize();
    cocos2d:: Size  gridUnitSize = runtime->getMapGridUnitPixelSize();
    float maxheigh = runtime->getMapGridMaxHeight();
    int height = maxheigh<=gridSize.height? gridSize.height *2:maxheigh;
    _mapGrid.resize(gridSize.width, height);
    _mapGrid._maxViewHeight = gridSize.height+1;
    _mapGrid.setData(0, 0,255);
    _mapGrid._maxViewHeight = gridSize.height;

    
    _maskSize = runtime->getMapDirtMaskGridSize();
    _textureAlphaMask = new Texture2D();
    _textureAlphaMask->initWithData(_mapGrid._gridData,_maskSize.width*_maskSize.height, Texture2D::PixelFormat::A8,_maskSize.width, _maskSize.height, cocos2d::Size(_maskSize.width,_maskSize.height));
    _textureAlphaMask->autorelease();
    
    _maskAlphaSprite = Sprite::createWithTexture(_textureAlphaMask);
    _maskAlphaSprite->setScale(gridUnitSize.width);
    BlendFunc funcMaskSprite = {GL_ONE,GL_ZERO};
    _maskAlphaSprite->setBlendFunc(funcMaskSprite);
    _maskAlphaSprite->setAnchorPoint(Vec2(0.5,1));
    _maskAlphaSprite->setRotationX(180);
    _maskAlphaSprite->setPosition(Vec2(visibleSize.width*0.5,0));

    
    RenderMaskColorNode* maskOnlyAlpha= RenderMaskColorNode::create(false,false,false,true);
    RenderMaskColorNode* maskOnlyColor= RenderMaskColorNode::create(true,true,true,true);
    
    _layerdirt = GameLayerMapDirt::create();

    _layerdirt->initGameInfo();
  //  _layerdirt->retain();

    _layerBorder = GameLayerMapBorder::create();
    _layerBorder->_mapGrid = &_mapGrid;
    _layerBorder->initGameInfo();
    UIHelper::layout(_layerBorder, UIHelper::ViwePoistionType::BottomCenter);
    addChild(maskOnlyAlpha);
    addChild(_maskAlphaSprite);
    addChild(maskOnlyColor);
    addChild(_layerdirt);
    addChild(_layerBorder,-1);
    _listener = nullptr;

    //事件调度器

    _isFirstMoveTouch = false;
    updateConfigs();

    return true;
}
void GameLayerMap::startListener()
{
    if(!_listener)
    {
       _listener = EventListenerTouchOneByOne::create();
       _listener->onTouchBegan = CC_CALLBACK_2(GameLayerMap::onTouchBegan, this);
       _listener->onTouchEnded = CC_CALLBACK_2(GameLayerMap::onTouchEnded, this);
       _listener->onTouchMoved = CC_CALLBACK_2(GameLayerMap::onTouchMoved, this);
      _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    }
}
void GameLayerMap::stopListener()
{
    if(_listener)
    {
        _eventDispatcher->removeEventListener(_listener);
        _listener = nullptr;
    }
}
void GameLayerMap::updateConfigs()
{

}

void GameLayerMap::update(float dt)
{
    onClearCell();
   _textureAlphaMask->updateWithData(_mapGrid.getDataByOriginY(), 0,0, _maskSize.width, _maskSize.height);
}
#define TOUCH_MIN_LENGTH 32
bool GameLayerMap::onTouchBegan(Touch* touch,Event*)
{
    Point pt = touch->getLocation();
    _touchPrePoint = pt;
    _isFirstMoveTouch = true;
    touchClearGrid(touch->getLocation());
    return true;
}
void  GameLayerMap::onTouchMoved(Touch* touch,Event*)
{
    cocos2d::Point pt = touch->getLocation();
    if (fabs(pt.x-_touchPrePoint.x)<TOUCH_MIN_LENGTH&&
        fabs(pt.y-_touchPrePoint.y)<TOUCH_MIN_LENGTH) return;
    
    if(touchClearGrid(touch->getLocation()))
            _touchPrePoint = pt;
    _isFirstMoveTouch =false;
}
void  GameLayerMap::onTouchEnded(Touch* touch,Event*)
{
       cocos2d::Point pt = touch->getLocation();
    if(_isFirstMoveTouch)
    {
//        if (fabs(pt.x-_touchPrePoint.x)<TOUCH_MIN_LENGTH&&
//            fabs(pt.y-_touchPrePoint.y)<TOUCH_MIN_LENGTH) return;
        GridCell cell = _mapGrid.getMapGridCellByPosition(pt);
       bool flag = clearGridCellBorderByRange(cell,  GameRuntime::getInstance()->getTouchClearGridRadius())>0;
        if(flag)
            _touchPrePoint = pt;
    }
}
void GameLayerMap::moveDown(float yLen)
{
   
    moveDownMapGrid(yLen);
    moveDownAlphaMask(yLen);
   // _layerdirt->moveDown(yLen);
    //if(_layerBorder)_layerBorder->moveDown(yLen);
};
#include <set>
#include "ItemModel.h"
class ExplosionQueryCallback : public b2QueryCallback
{
public:
    
    ExplosionQueryCallback():b2QueryCallback()
    {
    }
    bool ReportFixture(b2Fixture* fixture)
    {
  
        b2Body* body = fixture->GetBody();
        TypeBase* type = (TypeBase*)(body->GetUserData());
        //log("%d",type->getType());
        if (type->getType() ==Item_Type::Polygon_)
        {
            QueryResult.insert(body);
        }
        return true;
    }
    bool IsContain(b2Body* body)
    {
        std::set<b2Body*>::iterator pos;
        pos = QueryResult.find(body);
        return pos != QueryResult.end();
    }
public:
    std::set<b2Body*> QueryResult;
};
void GameLayerMap::updateExcavatePolygon(int x,int y,int width , int height)
{
   // return ;
//    auto physicalWorld = GamePhysicalWorld::getInstance();
//    
//    for (int j = y ; j< y+height; j++) {
//        for (int i = x; i< x+width; i++) {
//            if (_mapGrid.isOutMapGrid(x, y))continue;
//            Vec2 pt = _mapGrid.getPositionByMapGridCell(i,j);
//            if (physicalWorld->isInExcavatePolygon(pt))
//                changeGridCell(i,j, GridType::None);
//        }
//    }
//    _layerBorder->updateBorder(x, y,width, height);
    b2AABB aabb;

    aabb.lowerBound.Set(x*8/PTM_RATIO,y*8/PTM_RATIO);
    aabb.upperBound.Set((x+width)*8/PTM_RATIO,(y+height)*8/PTM_RATIO);
    //aabb.upperBound.Set(500,500);
    auto physicalWorld = GamePhysicalWorld::getInstance();
    
    ExplosionQueryCallback callback;
    physicalWorld->getBox2dWorld()->QueryAABB(&callback, aabb);
    if (callback.QueryResult.size() == 0) {
        return ;
    }
    auto end = callback.QueryResult.end();
    
    for (int j = y ; j< y+height; j++) {
        for (int i = x; i< x+width; i++) {
            if (_mapGrid.isOutMapGrid(x, y))continue;
            Vec2 pt = _mapGrid.getPositionByMapGridCell(i,j);
//            if (physicalWorld->isInExcavatePolygon(pt)) {
//                    changeGridCell(x, y, 0);
//            }
            auto ip = callback.QueryResult.begin();
            while (ip!=end) {
                if(GamePhysicalWorld::testPointInBody(*ip,pt))
                {
                    changeGridCell(i,j, 0);
                    break;
                }
                ip++;
            }
        }
    }
    _layerBorder->updateBorder(x, y-1,width, height+1);
}
void GameLayerMap::moveDownMapGrid(int moveLen)
{
    _mapGrid._startHeight += moveLen;
    if (_mapGrid._startHeight+ _mapGrid._maxViewHeight >= _mapGrid._maxViewHeight+MAX_CRASH_LENGTH) {
        
        memmove(_mapGrid._gridData, _mapGrid._gridData+(_mapGrid._startHeight)*_mapGrid._gridWidth,(_mapGrid._maxViewHeight)*_mapGrid._gridWidth);
        _mapGrid.setData(0,_mapGrid._maxViewHeight,_mapGrid._gridWidth,MAX_CRASH_LENGTH,GridType::Dirt);
//        _gameManager->getLayerBorder()->moveDownGridCell(_mapGrid._startHeight);
      _layerBorder->moveDownGridCell(_mapGrid._startHeight);
        _mapGrid._startHeight=0;
        auto size = GameRuntime::getInstance()->getMapGridSize();
        //_checkClear(0,_mapGrid._maxViewHeight-1,size.width,MAX_CRASH_LENGTH);
        updateExcavatePolygon(0,_mapGrid._maxViewHeight-1,size.width,MAX_CRASH_LENGTH+1);
        
    }
    _layerBorder->suboutViewBorderCell();
}

void GameLayerMap::moveDownMapGrid(float yLen)
{
    _mapGrid._mapGridStarty += yLen;
    int n =  _mapGrid._mapGridStarty/_mapGrid._unitGridSize.height;
    if (n>0)
    {
        _mapGrid._mapGridStarty -= n*_mapGrid._unitGridSize.height;
        moveDownMapGrid(n);
    }
}

void GameLayerMap::moveDownAlphaMask(float yLen)
{
    if(!_maskAlphaSprite)return;
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
///////
void GameLayerMap::onClearCell()
{
    if(_clearCells.empty())return ;
    
    while (!_clearCells.empty()) {
       auto ip = _clearCells.begin();
        changeGridCell(ip->_x,ip->_y,0);
        _clearCells.erase(ip);
    };
}


////////

void GameLayerMap::addCellBorder(const  GridCell& cell )
{
    static GridCell _vgrid[]=
    {
        GridCell(-1,0),GridCell(1,0),GridCell(0,1),GridCell(0,-1),
        GridCell(-1,1),GridCell(-1,-1),GridCell(1,1),GridCell(1,-1)
    };
    for(int i = 0; i<(sizeof(_vgrid)/sizeof(GridCell));i++)
    {
        GridCell tcell =cell+_vgrid[i];
        if(_mapGrid.getValue(tcell._x,tcell._y)==GridType::None)continue;
        _updateBorderList.insert(tcell);
    }
}
void  GameLayerMap::onChangeCellCalls(const GridCell& cell)
{
    auto config = GameRunningConfig::getInstance();
    for(auto call : config->_changeMapCellCalls)
    {
        call(cell);
    }
}
void GameLayerMap::changeGridCell(int x,int y ,unsigned char type)
{
    if(_mapGrid.getValue(x,y)==0 || _mapGrid.isOutMapGrid(x, y))return;
//    if (_layerBorder->isHasBorder(<#const GridCell &cell#>)) {
//       
//    }
    GridCell cell(x,y);
    onChangeCellCalls(cell);
    //GameRunningConfig::getInstance()->getChangeMapCellCall()(cell);
    _layerBorder->removeBorder(cell);
    _mapGrid.setValue(x, y, 0);
}

////////////////////////////////////////////////////
//平滑边缘
void GameLayerMap::_checkClear(int x,int y, int width,int height)
{
    static int _sclear_width =4;
    //  std::list<GridCell> clearlist;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tx,ty;
            tx = x+i; ty = j+y;
            if (_mapGrid.isOutMapGrid(tx, ty)) {
                continue;
            }
            if (_mapGrid.getValue(tx,ty)==GridType::None) {
                continue;
            }
            if(_isNeedDelete(tx, ty, 1, 0, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, 0, 1, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, -1, 1, _sclear_width*2+1, 2)||
               _isNeedDelete(tx, ty, 1, 1, _sclear_width*2+1, 2))
            {
                changeGridCell(tx,ty,GridType::None);
            }
        }
    }
}
 bool GameLayerMap::isNeedDeleteCell(int x,int y)
{
      static int _sclear_width =6;
  return  _isNeedDelete(x, y, 1, 0, _sclear_width*2+1, _sclear_width)||
          _isNeedDelete(x, y, 0, 1, _sclear_width*2+1, _sclear_width)||
          _isNeedDelete(x, y, -1, 1, _sclear_width*2+1, 4)||
          _isNeedDelete(x, y, 1, 1, _sclear_width*2+1, 4);
}
////////////////////////////////////////////////////
bool GameLayerMap::_isNeedDelete(int x,int y ,int stepX,int stepY,int lenLoop,int lenMaxCheck)
{
    int lenMax=0;
    bool flag1=true,flag2=true;
    for (int i = 1; i<lenLoop; i++) {
        int tx,ty;
        tx = x + i * stepX,  ty = y + i * stepY;
        if (flag1)
        {
            bool isOut = _mapGrid.isOutMapGrid(tx, ty);
            if (isOut||_mapGrid.getValue(tx,ty) == GridType::None)
            {
                flag1 =false;
            }
            else lenMax++;
        }
        tx = x + i * (-stepX), ty = y + i * (-stepY);
        if (flag2)
        {
            bool isOut =_mapGrid.isOutMapGrid(tx, ty);
            if (isOut||_mapGrid.getValue(tx,ty) == GridType::None)
            {
                flag2 =false;
            }
            else lenMax++;
        }
        if (lenMax>=lenMaxCheck)
        {
            return false;
        }
    }
    return true;
}
bool  GameLayerMap::touchClearGrid(Point touchPoint)
{
    if (fabs(touchPoint.x-_touchPrePoint.x)<TOUCH_MIN_LENGTH&&
        fabs(touchPoint.y-_touchPrePoint.y)<TOUCH_MIN_LENGTH)
    {
        return false;
    }
    
    GridCell cell = _mapGrid.getMapGridCellByPosition(touchPoint);
    return clearGridCellBorderByRange(cell,  GameRuntime::getInstance()->getTouchClearGridRadius())>0;
    
}
bool GameLayerMap::isCanTouchRangeCell(const GridCell& cell,int radius)
{
  auto   mapcell = _mapGrid.gridCellToInViewCell(cell);
    if (mapcell._y > _mapGrid.getGridMaxViewHeight() - radius) {
        return false;
    }
    return true;
}
int  GameLayerMap::clearGridCellBorderByRange(GridCell cell, int radius)
{
    if (!isCanTouchRangeCell(cell,radius)) {
        return -1;
    }
    int x1,y1;
    x1=cell._x;
    y1=cell._y;
    x1 -= radius;
    y1 -= radius;
    int isClear = -1;
    int radiusEQ = radius*radius;
    std::list<GridCell> clearList;
    int count =0;
    for (; y1<cell._y+radius; y1++) {
        for (x1= cell._x-radius; x1<cell._x+radius; x1++) {
            int w1,h1;
            w1 = abs(x1 - cell._x);
            h1 = abs(y1 - cell._y);
            if (w1*w1 + h1*h1 < radiusEQ)
            {
                count++;
                if (_mapGrid.isOutMapGrid(x1, y1))continue;
                GridCell tempCell(x1,y1);
                if(_mapGrid.getValue(tempCell._x, tempCell._y)==GridType::Dirt)
                {
                    clearList.push_back(tempCell);
                   // changeGridCell(tempCell._x, tempCell._y, GridType::None);
                    isClear = 1;
                }
            }
        }
    }
  // if(clearList.size()>=count-4)return -1;
    auto ip =clearList.begin();
    auto end = clearList.end();
    while (ip!=end) {
        changeGridCell(ip->_x, ip->_y, GridType::None);
       // cellNeedDel(*ip);
        ip++;
    }
#define CLEAR_RANG 6
    if (isClear >0) {
        int xstep= cell._x- radius-CLEAR_RANG,ystep= cell._y- radius-CLEAR_RANG,wstep=radius*2+CLEAR_RANG*2,hstep= radius*2+CLEAR_RANG*2;
        _checkClear(xstep,ystep,wstep,hstep);
        _layerBorder->updateBorder(xstep,ystep,wstep,hstep);
    }
    //_clearCells.insert(clearList.begin(), clearList.end());
    return isClear;
}
bool  GameLayerMap::PointIsInDirtOrOutMapGrid(Vec2 pt)
{
    GridCell cell = _mapGrid.getMapGridCellByPosition(pt);
    return _mapGrid.isOutMapGrid(cell._x, cell._y)||
    _mapGrid.getValue(cell._x, cell._y)==GridType::Dirt;
}
