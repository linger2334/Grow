#include "LayerMap.h"
#define CENTER_POINT Vec2(_visibleSize.width/2 , _visibleSize.height/2)

#define  VIEWS_COUNT 3
 DrawShape* LayerMap::_drawShapes = nullptr;
 unsigned int LayerMap::_drawShapesCount = 0 ;
bool LayerMap::init()
{
    bool bRet = false;
    do
    {
        bRet = Layer::init();
        if (!bRet)break;
        _visibleSize = Director::getInstance()->getWinSize();
        _mapSize = _visibleSize;
        _mapBlockSize = Vec2(8,8);
        _mapGridStartY = 0;
        _mapGridVisibleWidth = _visibleSize.width/_mapBlockSize.width;
        _mapGridVisibleHeight = _visibleSize.height/_mapBlockSize.height;
        _mapGridMaxHeight = _mapGridVisibleHeight+_mapGridVisibleHeight/2;
        assert(_mapGridMaxHeight%VIEWS_COUNT==0);
        _mapViewGridHeight = _mapGridMaxHeight/VIEWS_COUNT;
        _mapViewGridWidth  = _mapGridVisibleWidth;
        _mapViewHeight     = _mapViewGridHeight*_mapBlockSize.height;
        _mapViewWidth      = _visibleSize.width;
        _passGridHeightLen = 0;
        _spritesPoolMaxCount =0;
        bRet = true;
        initMapInfo();
        _isNeedChack=false;
        //this->scheduleUpdate();
        //schedule(schedule_selector(LayerMap::update),1.0/30);
       // schedule(schedule_selector(LayerMap::_updateClearCheck),1.0/30);
    }while (0);
    return bRet;
}

bool LayerMap::initMapInfo()
{
    bool bRet = true;
    do
    {
        _mapGrid.resize(_mapViewGridWidth, _mapGridMaxHeight);
        _mapGrid.resetGridData(GridType::Dirt);
//        for (int i =0; i<10; i++) {
//            for (int j = 0; j<_mapGrid._width; j++) {
//                _mapGrid._grid[j+i*_mapGrid._width]=GridType::None;
//            }
//        }
        testClear(_mapGridVisibleWidth/2, 0,40);
        LoadTextures();
        //initMapViwes();
        //layoutViews();
   
    }while (0);
    return bRet;
}

bool LayerMap::initMapViwes()
{
    if (!_mapViews.empty()) return true;
    assert(_mapGridMaxHeight%_mapViewGridHeight==0);
    for (int i=0; i<_mapGridMaxHeight/_mapViewGridHeight; i++) {
        RenderTexture* render = RenderTexture::create(_mapViewWidth, _mapViewHeight);
        _mapViews.pushBack(render);
        addChild(render,Zorder::LayerDirt);
    }
    _borderBatch = SpriteBatchNode::create("yunduo24.png");
    addChild(_borderBatch,Zorder::LayerGoods);
//    _mapViews.at(0)->getSprite()->setColor(Color3B(255,0,0));
//    _mapViews.at(1)->getSprite()->setColor(Color3B(0,255,0));
//    _mapViews.at(2)->getSprite()->setColor(Color3B(0,0,255));
//    
}
void   LayerMap::layoutViews()
{
    Point pStart = getCellPoint(0, 0);
    pStart.x= _visibleSize.width/2;
    float addLen = _mapViewHeight/2;
    for (auto i : _mapViews) {
        i->setPosition(pStart+Vec2(0,addLen));
        pStart = pStart + Vec2(0,addLen*2);
    }
}
bool LayerMap::LoadTextures()
{
    auto manager = Director::getInstance()->getTextureCache();
    _textures[GridType::Dirt]=manager->addImage("tudiwenli512.png");
    _textures[GridType::None]=manager->addImage("background3.png");
    _textures[GridType::Border]=manager->addImage("border16.png");
}

void LayerMap::initMapGridInfo()
{
    _mapGrid.resetGridData(GridType::Dirt);
}

 void LayerMap::update(float dt){
     updateMapDirty();
//     static float dtt =0;
//     dtt+=dt;
//     if (dtt>0.2) {
        //  moveMapDown(-1);
       //  dtt=0;
  //   }
 }

void LayerMap::updateMapInView(int x, int y,int width ,int height)
{
    updateMap(x,y-_mapGridStartY,width,height);
}
void LayerMap::updateMap(int x,int y,int width,int height)
{
    if (x<0||x>=_mapGridVisibleWidth||
        y<0 || y>=_mapGridMaxHeight) {
        return;
    }
    return updateMapView(0,0,0,0);
}
//////////////////////////////////////////////////////////////
Sprite* LayerMap::createSpriteByGridCell(int x,int y,bool autoRelease)
{
    unsigned char type = getMapCell(x, y);
    Texture2D* texture =getTextureByGridType(type);
    Size  size =texture->getContentSize();
    int countx=size.width/_mapBlockSize.width;
    int county=size.height/_mapBlockSize.height;
    int ix,iy;
    ix = x%countx;
    iy = (y+_passGridHeightLen)%county;
    Sprite* sp = createSpriteByPool(texture,Rect(ix*8,(county-iy-1)*8,8,8));
    sp->setAnchorPoint(Vec2(0,0));
    if (autoRelease) {
        autoReleaseSpriet(sp);
    }
    return sp;
}
void LayerMap::updateRenderView(int x,int y,int width,int height,int renderIndex)
{
    assert(renderIndex >= 0 && renderIndex < _mapViews.size());
    RenderTexture* render = _mapViews.at(renderIndex);
    width =x+width;
    if (x+width >= _mapViewGridWidth) width =_mapViewGridWidth;
    height = y +height;
     if (y+height >= _mapViewGridHeight) height =_mapViewGridHeight;
    render->begin();
    for (int i = x; i < width; i++) {
        for (int j = y;j < height; j++) {
            GridCell cell =renderViewGridToMainGridCell(i,j,renderIndex);
            Sprite* sp =createSpriteByGridCell(cell._x, cell._y);
            Point spPoint =Vec2(i*_mapBlockSize.width,j*_mapBlockSize.height);
            sp->setPosition(spPoint);
            sp->visit();
        }
    }
    render->end();
}
void LayerMap::updateRenderView(int renderIndex)
{
    updateRenderView(0,0,_mapViewGridWidth,_mapViewGridHeight,renderIndex);
}
void LayerMap::updateMapView(int x,int y,int width,int height)
{
    for (int i = 0; i < 3; i++) {
        updateRenderView(i);
    }
}
void LayerMap::updateMapViewInView(int x,int y,int width,int height)
{
}
void LayerMap::drawToRenderViewBySpriteList(int renderIndex,const Vector<Sprite*>& list)
{
    assert(renderIndex>=0&&renderIndex<_mapViews.size());
    RenderTexture* render = _mapViews.at(renderIndex);
    render->begin();
    for (auto i:list) {
        i->visit();
    }
    render->end();
}

/////////////////////////////////////////////////////////////

 void LayerMap::clearGrid(int x,int y)
{
    int sx,sy;
    int width = 16,height =16;
    sx=x-3;sy=y-3;
//    if (sx < 0) {width += sx; sx = 0;}
//    if (sy < 0) {height += sy; sy = 0;}
//    for (int i = 0; i<width; i++) {
//        for (int j = 0; j < height; j++) {
//            changeGridCell(sx+i, sy+j, GridType::None);
//        }
//    }
    for (int i = 0; i < width/2 ; i++) {
        for (int j = 0 ; j < height/2 ; j++) {
            if (i+j<width*0.75) {
                changeGridCell(x-i, y+j, GridType::None);
                changeGridCell(x+i, y+j, GridType::None);
                changeGridCell(x-i, y-j, GridType::None);
                changeGridCell(x+i, y-j, GridType::None);
            }
        }
    }
   _checkClear(sx-10,sy-10,width+20,height+20);
   // updateBorder(sx-10,sy-10,width+20,height+20);
}
void LayerMap::_updateClearCheck(float dt){
    if (_isNeedChack) {
        _checkClear(0,0,80,120);
        _isNeedChack=false;
    }
}
  bool LayerMap::isBorderCell(int x,int y)
{
    static int vecs[][2]=
    {
        {-1,0},{1,0},
        {0,-1},{0,1},
        {-1,1},{-1,-1},
        {1,1},{1,-1}
    };
    bool bRet=false;
    if(getMapCell(x,y)==GridType::None)return false;
    for (int i=0; i<8; i++) {
        if (isOutMapGrid(x+vecs[i][0], y+vecs[i][1])) {
            continue;
        }
        if(getMapCell(x+vecs[i][0], y+vecs[i][1])==GridType::None)
        {
            bRet=true;
            break;
        }
    }
    return bRet;
}
  void LayerMap::updateBorder(int x,int y, int width,int height)
{
  for (int i = x; i < x+width; i++)
  {
    for (int j = y; j< y+height; j++)
    {
        if (isOutMapGrid(i, j)) {
            continue;
        }
        BorderDirection dir;
       if (isBorder(i, j,GridType::Dirt,dir)) {
            //changeGridCell(i, j, GridType::Border);
           // _borders[GridCell(i,j)]=dir;
           // addBorderNode(i, j);
       }
    }
 }
}

void LayerMap::_checkClear(int x,int y, int width,int height)
{
    static int _sclear_width =3;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tx,ty;
            tx = x+i; ty = j+y;
            if (isOutMapGrid(tx, ty)) {
                 continue;
            }
            if (getMapCell(tx,ty)==GridType::None) {
                continue;
            }
            if(_isNeedDelete(tx, ty, 1, 0, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, 0, 1, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, -1, 1, _sclear_width*2, _sclear_width-1)||
               _isNeedDelete(tx, ty, 1, 1, _sclear_width*2, _sclear_width-1))
            {
                changeGridCell(tx,ty,GridType::None);
            }
        }
    }
}

bool LayerMap::onTouchBegan(Touch *touch, Event *event)
{
   // GridCell cell =getCellByPoint(touch->getLocation());
    //clearGrid(cell._x,cell._y);
    return true;
}

void LayerMap::onTouchMoved(Touch *touch, Event *event)
{
    GridCell cell = getCellByPoint(touch->getLocation());
    clearGrid(cell._x,cell._y);
}

 void LayerMap::onTouchEnded(Touch *touch, Event *event){
     GridCell cell =getCellByPoint(touch->getLocation());
     clearGrid(cell._x,cell._y);
 }
 void LayerMap::onTouchCancelled(Touch*,Event*){}

void LayerMap::beginTouchListen()
{
    schedule(schedule_selector(LayerMap::update), 1.0f/30.0f);
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LayerMap::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerMap::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerMap::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(LayerMap::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LayerMap::stopTouchListen()
{
    unscheduleUpdate() ;
    _eventDispatcher->removeEventListenersForTarget(this);
}

bool LayerMap::isTouchListening()
{
    return isScheduled(schedule_selector(LayerMap::update));
}

bool LayerMap::_isNeedDelete(int x,int y ,int stepX,int stepY,int lenLoop,int lenMaxCheck)
{
    int lenMax=0;
    bool flag1=true,flag2=true;
    for (int i = 1; i<lenLoop; i++) {
        int tx,ty;
        tx = x + i * stepX,  ty = y + i * stepY;
        if (flag1 &&
            !(tx<0 || tx >= _mapGrid._width||
              ty<0 || ty >= _mapGrid._height))
        {
                if (getMapCell(tx,ty) == GridType::None)
                {
                    flag1 =false;
                }
                else lenMax++;
        }
        tx = x + i * (-stepX), ty = y + i * (-stepY);
        if (flag2&&
            !(tx < 0 || tx >= _mapGrid._width||
              ty < 0 || ty >= _mapGrid._height))
        {
            if (getMapCell(tx,ty) == GridType::None)
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


void LayerMap::updateMapDirty()
{
    clearSpriteAutoReleasePool();
    if (_dirtyCellsSet.empty()) {
        return;
    }
    auto ip = _dirtyCellsSet.begin();
    auto end = _dirtyCellsSet.end();
    std::vector<Vector<Sprite*> > dirtySprites(VIEWS_COUNT);
    
    while(ip!=end)
    {
        GridCell cell = *ip;
        int index = getRenderViewIdByGridCell(cell._x, cell._y);
        Sprite* sp=nullptr;
        sp =createSpriteByGridCell(cell._x,cell._y);
        Point pt = gridCellToRenderPoint(cell._x,cell._y,index);
        sp->setPosition(pt);
        unsigned type = getMapCell(cell._x, cell._y);
//        if (getMapCell(cell._x,cell._y)==GridType::None) {
//            BlendFunc func ={GL_ZERO,GL_ZERO};
//            sp->setBlendFunc(func);
//        }
        if (type == GridType::Border) {
            auto it = _borders.find(cell);
            if (it != _borders.end()) {
                BorderDirection dir = (BorderDirection)it->second;
                if(dir == BorderDirection::Right)
                    sp->setColor(Color3B(255,0,0));
                else  if(dir == BorderDirection::Left)
                    sp->setColor(Color3B(0,255,0));
                else  if(dir == BorderDirection::Top)
                    sp->setColor(Color3B(0,0,255));
                else  if(dir == BorderDirection::Bottom)
                    sp->setColor(Color3B(255,255,255));
                else  if(dir == BorderDirection::LeftTop)
                    sp->setColor(Color3B(255,255,0));
                else  if(dir == BorderDirection::LeftBottom)
                    sp->setColor(Color3B(255,0,255));
                else  if(dir == BorderDirection::RightTop)
                    sp->setColor(Color3B(0,255,255));
                else  if(dir == BorderDirection::RightBottom)
                    sp->setColor(Color3B(0,0,0));
            }
        }
        dirtySprites.at(index).pushBack(sp);
        ip++;
    }
    for (int i=0;i<VIEWS_COUNT;i++) {
        if (dirtySprites.at(i).empty())continue;
        drawToRenderViewBySpriteList(i,dirtySprites.at(i));
    }
    dirtySprites.clear();
    _dirtyCellsSet.clear();
}


//////////////
Texture2D* LayerMap::getTextureByGridType(unsigned char type)
{
    Texture2D* retTextrue = nullptr;
    auto ip = _textures.find(type);
    if(ip != _textures.end())retTextrue = ip->second;
    return retTextrue;
}
void LayerMap::resetMap()
{
    _mapGrid.resetGridData(GridType::Dirt);
    updateMap(0,0,_mapGridVisibleWidth,_mapGridVisibleHeight);
}


 void LayerMap::testOutLineInit()
{
    SpriteBatchNode* batch =SpriteBatchNode::create("CloseNormal.png");
    for (int i=0; i<800; i++) {
        Sprite* sp =Sprite::createWithTexture(batch->getTexture());
        batch->addChild(sp);
        sp->setPosition(Vec2(i%20,i%100*8));
    }
    addChild(batch);
}

void LayerMap::initDrawShapes(){
    
    _drawShapes = new DrawShape[DrawShapeType::ShapeEnd];
    _drawShapes[DrawShapeType::Circle]._points = new Vec2[24];
    
}

bool LayerMap::isBorder(int x,int y,unsigned char type,BorderDirection& direction)
{
    static int vecs[][2]=
    {
        {-1,0},{1,0},
        {0,1},{0,-1},
        {-1,1},{-1,-1},
        {1,1},{1,-1}
    };
    bool sets[8];
    memset(sets,false,sizeof(sets));
    bool bRet=false;
    if(getMapCell(x,y)==GridType::None)return false;
    for (int i=0; i<8; i++) {
        if (isOutMapGrid(x+vecs[i][0], y+vecs[i][1])) {
            continue;
        }
        if(getMapCell(x+vecs[i][0], y+vecs[i][1]) == GridType::None)
        {
            bRet=true;
            sets[i]=true;
        }
        
    }
    if(bRet)
    {
        if ((!sets[BorderDirection::Top] &&
            !sets[BorderDirection::Bottom]) &&
            !sets[BorderDirection::Right]) {
            direction =BorderDirection::Left;
        }
        else  if ((!sets[BorderDirection::Top] &&
                 !sets[BorderDirection::Bottom]) &&
                 !sets[BorderDirection::Left]) {
            direction = BorderDirection::Right;
        }
        else  if ((!sets[BorderDirection::Left]&&
                  !sets[BorderDirection::Right])&&
                  !sets[BorderDirection::Bottom]) {
            direction = BorderDirection::Top;
        }
        else  if ((!sets[BorderDirection::Left]&&
                  !sets[BorderDirection::Right])&&
                  !sets[BorderDirection::Top]) {
            direction = BorderDirection::Bottom;
        }
        else  if (sets[BorderDirection::Left]&&
                  sets[BorderDirection::Top]&&
                  sets[BorderDirection::LeftTop]) {
            direction = BorderDirection::LeftTop;
        }
        else  if (sets[BorderDirection::Left]&&
                  sets[BorderDirection::Bottom]&&
                  sets[BorderDirection::LeftBottom]) {
            direction = BorderDirection::LeftBottom;
        }
        else  if (sets[BorderDirection::RightTop]&&
                  sets[BorderDirection::Top]&&
                  sets[BorderDirection::RightTop]) {
            direction = BorderDirection::RightTop;
        }
        else  if (sets[BorderDirection::RightTop]&&
                  sets[BorderDirection::Bottom]&&
                  sets[BorderDirection::RightBottom]) {
            direction = BorderDirection::RightBottom;
        }

    }
    
    return bRet;
}
 void LayerMap::changeGridCell(int x,int y,unsigned char type)
{
    //if (getMapCell(x, y)!= type) {
    if (getMapCell(x, y)==GridType::Dirt) {
      
        
       // delBorderNode(x, y);
        
        
//        auto ip =_borders.find(GridCell(x,y));
//        if (ip !=_borders.end()) {
//            _borders.erase(ip);
//        }
    }
        setMapCell(x, y, type);
       // setDirtyCell(x, y);
    //}
}


void LayerMap::moveMapDown(int len)
{
      int oldGridStartY =_mapGridStartY;
    _mapGridStartY = _mapGridStartY+len;
    
   if (_mapGridStartY<=-_mapViewGridHeight) {
        int viewGridCount =_mapGrid._width*_mapGridVisibleHeight;
        memcpy(_mapGrid._grid, _mapGrid._grid+abs(_mapGridStartY)*_mapGrid._width,viewGridCount);
       memset(_mapGrid._grid+viewGridCount,GridType::Dirt,(_mapGrid._height-abs(_mapGridStartY))* _mapGrid._width);
       // RenderTexture* render =_mapViews.at(0);
      //  _mapViews.eraseObject(render);
      //  _mapViews.pushBack(render);
        _mapGridStartY=0;
        _passGridHeightLen+=_mapViewGridHeight;
      // updateRenderView(0,0,_mapViewGridWidth,_mapViewGridHeight,VIEWS_COUNT-1);
   }
    //updateRenderView(0,abs(oldGridStartY),_mapViewGridWidth,abs(len),VIEWS_COUNT-1);
   // layoutViews();
}


/////////////////////////////////////////////////
Sprite* LayerMap::newSpriteByPool()
{
    Sprite* sp;
    if (_spritesPool.empty()) {
        sp= new Sprite();
    }
    else{
        sp = _spritesPool.back();
        sp->retain();
        _spritesPool.popBack();
    }
    //sp->setColor(Color3B(255,255,255));
    return sp;
}
Sprite* LayerMap::createSpriteByPool(const char* file)
{
    Sprite* sp =newSpriteByPool();
    sp->initWithFile(file);
    sp->setColor(Color3B(255,255,255));
    return sp;
}
Sprite* LayerMap::createSpriteByPool(Texture2D* texture)
{
    Sprite* sp =newSpriteByPool();
    sp->initWithTexture(texture);
    sp->setColor(Color3B(255,255,255));
    return sp;
}
Sprite* LayerMap::createSpriteByPool(Texture2D* texture,Rect rect)
{
    Sprite* sp =newSpriteByPool();
    sp->initWithTexture(texture,rect);
    sp->setColor(Color3B(255,255,255));
    return sp;
}

void  LayerMap::releaseSpriteToPool(Sprite* sprite)
{
    if(_spritesPool.size()<_spritesPoolMaxCount)
    {
        _spritesPool.pushBack(sprite);
    }
    sprite->release();
}

void    LayerMap::autoReleaseSpriet(Sprite* sprite)
{
    if(sprite)
    _spritesAutoReleasePool.pushBack(sprite);
}
void    LayerMap::clearSpriteAutoReleasePool()
{
    for (auto i:_spritesAutoReleasePool) {
        releaseSpriteToPool(i);
    }
    _spritesAutoReleasePool.clear();
}


//////////////////////////////////////////////////


void LayerMap::addBorderNode(int x,int y)
{
    return ;
    Sprite* border = Sprite::createWithTexture(_borderBatch->getTexture());
    GridCell cell(x,y);
    _borderBatch->addChild(border,0,_mapGrid.getIndexForGridCell(cell));
    border->setPosition(getCellPoint(x,y));
 
  //  border->setTag(_mapGrid.getIndexForGridCell(cell));
    _borders.insert(std::make_pair(cell,0));
    
}
void LayerMap::delBorderNode(int x,int y)
{
    return ;
    GridCell cell(x,y);
    auto border =_borderBatch->getChildByTag(_mapGrid.getIndexForGridCell(cell));
    if (border) {
        border->removeFromParent();
    }
    auto ip = _borders.find(cell);
    if (ip!=_borders.end()) {
        _borders.erase(ip);
    }
    
}
void LayerMap::testClear(int x,int y,int len)
{
    int sx,sy;
    int width = len,height =len;
    sx=x-3;sy=y-3;
    for (int i = 0; i < width/2 ; i++) {
        for (int j = 0 ; j < height/2 ; j++) {
            if (i+j<width/2+1) {
                changeGridCell(x-i, y+j-_mapGridStartY, GridType::None);
                changeGridCell(x+i, y+j-_mapGridStartY, GridType::None);
                changeGridCell(x-i, y-j-_mapGridStartY, GridType::None);
                changeGridCell(x+i, y-j-_mapGridStartY, GridType::None);
            }
        }
    }
    _checkClear(sx-10,sy-10,width+20,height+20);
}