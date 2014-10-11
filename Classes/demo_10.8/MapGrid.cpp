#include "MapGrid.h"
#include "LayerBorder.h"
#include "SceneGame.h"
#include "LayerPlant.h"
bool MapGrid::_isNeedDelete(int x,int y ,int stepX,int stepY,int lenLoop,int lenMaxCheck)
{
    int lenMax=0;
    bool flag1=true,flag2=true;
    for (int i = 1; i<lenLoop; i++) {
        int tx,ty;
        tx = x + i * stepX,  ty = y + i * stepY;
        if (flag1)//&&
            //  !(tx<0 || tx >= _gridWidth||
            //     ty<0 || ty >= _gridHeight))
        {
            bool isOut =isOutMapGrid(tx, ty);
            if (!isOut&&getValue(tx,ty) == GridType::None)
            {
                flag1 =false;
            }
            else lenMax++;
        }
        tx = x + i * (-stepX), ty = y + i * (-stepY);
        if (flag2)//&&
            //!(tx < 0 || tx >= _gridWidth||
            //  ty < 0 || ty >= _gridHeight))
        {
            bool isOut =isOutMapGrid(tx, ty);
            if (!isOut&&getValue(tx,ty) == GridType::None)
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

void MapGrid::_checkClear(int x,int y, int width,int height)
{
    static int _sclear_width =6;
    //  std::list<GridCell> clearlist;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int tx,ty;
            tx = x+i; ty = j+y;
            if (isOutMapGrid(tx, ty)) {
                continue;
            }
            if (getValue(tx,ty)==GridType::None) {
                continue;
            }
            if(_isNeedDelete(tx, ty, 1, 0, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, 0, 1, _sclear_width*2+1, _sclear_width)||
               _isNeedDelete(tx, ty, -1, 1, _sclear_width*2+1, 4)||
               _isNeedDelete(tx, ty, 1, 1, _sclear_width*2+1, 4))
            {
                changeGridCell(tx,ty,GridType::None);
                // clearlist.push_back(GridCell(tx,ty));
            }
        }
    }
    //    for (auto i:clearlist) {
    //         changeGridCell(i._x,i._y,GridType::None);
    //    }
}

void  MapGrid::changeGridCell(int x,int y,unsigned char type)
{
 
    
    if (isOutMapGrid(x, y)) {
        return;
    }
    if(getValue(x, y)!=GridType::None&&type==GridType::None)
    {
//        Vec2 vecs[4];
//        vecs[0]=getPositionByMapGridCell(x, y);
//        vecs[1] = vecs[0] + Vec2(8,0);
//         vecs[2] = vecs[0] + Vec2(0,8);
//         vecs[3] = vecs[0] + Vec2(8,8);
//        for (auto i:vecs ) {
//            // modify by wlg
//            if (_sceneGame->_physicsHandler->isPointContact(i)) {
//                return ;
//            }
//        }
        GridCell cell(x,y);
        if(_layerBorder->isHasBorder(cell))
        {
            _layerPlant->removeLightBorderByCell(cell._y*_gridWidth+cell._x);
            _layerBorder->removeBorder(cell);
        }
    }
    setValue(x, y, type);
}

bool  MapGrid::isCanTouchClearGrid(int x,int y)
{
    
}
bool  MapGrid::touchClearGrid(Point touchPoint)
{
    GridCell cell = getMapGridCellByPosition(touchPoint);
   return  clearGridCell(cell._x,cell._y);
}
void MapGrid::testClear(int x,int y,int len)
{
    int sx,sy;
    int width = len,height =len;
    sx=x-3;sy=y-3;
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
    _checkClear(sx-20,sy-20,width+10,height+10);
}
bool  MapGrid::clearGridCell(int x,int y)
{
    int sx,sy;
    int width = 16,height =16;
    sx=x-8;sy=y-8;
    
    std::list<GridCell> clearList;
    for (int i = 0; i < width/2 ; i++) {
        for (int j = 0 ; j < height/2 ; j++) {
            
            if (i+j<width*0.75) {
                
                if(getValueCheckRange(x-i, y+j)==GridType::Dirt)
                   clearList.push_back(GridCell(x-i, y+j));
                 if(getValueCheckRange(x+i, y+j)==GridType::Dirt)
                clearList.push_back(GridCell(x+i, y+j));
                 if(getValueCheckRange(x-i, y-j)==GridType::Dirt)
                clearList.push_back(GridCell(x-i, y-j));
                if(getValueCheckRange(x+i, y-j)==GridType::Dirt)
                clearList.push_back(GridCell(x+i, y-j));
//                changeGridCell(x-i, y+j, GridType::None);
//                changeGridCell(x+i, y+j, GridType::None);
//                changeGridCell(x-i, y-j, GridType::None);
//                changeGridCell(x+i, y-j, GridType::None);
            }
        }
    }
    if(clearList.size()>=200)return false;
    
    _layerPlant->removeBorderLight();
    auto ip =clearList.begin();
    auto end = clearList.end();
    while (ip!=end) {
        changeGridCell(ip->_x, ip->_y, GridType::None);
        ip++;
    }
    int xstep= x - 20,ystep= y -20,wstep=40,hstep= 40;
    _checkClear(xstep,ystep,wstep,hstep);
    _layerBorder->updateBorder(xstep,ystep,wstep,hstep);
    return true;
    // updateBorder(sx+xstep,sy+ystep,width+wstep,height+hstep);
     //for (int i=0; i<3; i++) {
     //_checkClear(0,0,_gridWidth,_maxViewHeight);
   // }
    
    //updateBorder(0,0,_gridWidth,_maxViewHeight);
    // return ;
//    std::list<BorderLine> list(BorderLine::getBorderlines(_borderList));
//    BorderCell cellOut;
//    BorderLine& line =*list.begin();
//    
//    
//    Color3B colors[]={Color3B::WHITE,Color3B::RED,Color3B::YELLOW,Color3B::BLUE,Color3B::BLACK,
//        Color3B::GREEN};
//    log("list size %d",list.size());
//    int index=0;
//    //   if(list.size()>0)
//    _bordersNode->removeAllChildren();
//    for (auto& i:list) {
//        //Color3B color = Color3B(rand()%255,rand()%255,rand()%255);
//        for(auto n: i._borderList)
//        {
//            Sprite* sp = Sprite::createWithTexture(_bordersNode->getTexture());
//            _bordersNode->addChild(sp);
//            sp->setPosition(Vec2(n._cell._x*8+4,n._cell._y*8+4));
//            sp->setColor(colors[index]);
//        }
//        index++;
//    }
//    _borderList.clear();
}

