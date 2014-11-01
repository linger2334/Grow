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
            if (isOut||getValue(tx,ty) == GridType::None)
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
            if (isOut||getValue(tx,ty) == GridType::None)
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
            }
        }
    }
}

void  MapGrid::changeGridCell(int x,int y,unsigned char type)
{
    if (isOutMapGrid(x, y)) {
        return;
    }
    if(getValue(x, y)!=GridType::None&&type==GridType::None)
    {
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
#include "GameManager.h"
bool  MapGrid::touchClearGrid(Point touchPoint)
{
    if(touchPoint.y > Director::getInstance()->getVisibleSize().height-32)return false;
    GridCell cell = getMapGridCellByPosition(touchPoint);
   // log("Touch Clear Grid x: %f , y: %f",touchPoint.x,touchPoint.y);
  // return   clearGridCell(cell._x,cell._y);
    return clearGridCellBorderByRange(cell,8)>0;
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
            }
            
        }
    }
    
//            int x1,y1;
//            GridCell    cell(x,y);
//            x1=cell._x;
//            y1=cell._y;
//            x1-=8;
//            y1=8;
//    
//                for (; y1<cell._y+8; y1++) {
//                    for (x1= cell._x-8; x1<cell._x+8; x1++) {
//                    if (isOutMapGrid(x1, y1))continue;
//                    int w1,h1;
//                    w1 = abs(x1 - cell._x);
//                    h1 = abs(y1 - cell._y);
//                    if (w1*w1 + h1*h1 < 64) {
//                        GridCell tempCell(x1,y1);
//                        if(_layerBorder->isHasBorder(tempCell))
//                        {
//                          changeGridCell(tempCell._x, tempCell._y, GridType::None);
//                           // _mapGrid._layerBorder->removeBorder(tempCell);
//                        }
//                    }
//                }
//            }
    if(clearList.size()>=200)return false;
    
  //  _layerPlant->removeBorderLight();
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

}

int  MapGrid::clearGridCellBorderByRange(GridCell cell, int radius)
{
    int x1,y1;
    x1=cell._x;
    y1=cell._y;
    x1 -= radius;
    y1 = radius;
    int isClear = -1;
    int radiusEQ = radius*radius;
     std::list<GridCell> clearList;
    for (; y1<cell._y+radius; y1++) {
         for (x1= cell._x-radius; x1<cell._x+radius; x1++) {
              if (isOutMapGrid(x1, y1))continue;
                int w1,h1;
                w1 = abs(x1 - cell._x);
                h1 = abs(y1 - cell._y);
                if (w1*w1 + h1*h1 < radiusEQ)
                {
                    GridCell tempCell(x1,y1);
                    if(getValue(tempCell._x, tempCell._y)==GridType::Dirt)
                    {
                        clearList.push_back(tempCell);
//                   // if(_layerBorder->isHasBorder(tempCell))
//                    {
 //                       changeGridCell(tempCell._x, tempCell._y, GridType::None);
                       isClear = 1;
//                    }
                    }
                }
            }
      }
    if(clearList.size()>=180)return -1;
    
    //  _layerPlant->removeBorderLight();
    auto ip =clearList.begin();
    auto end = clearList.end();
    while (ip!=end) {
        changeGridCell(ip->_x, ip->_y, GridType::None);
        ip++;
    }
#define CLEAR_RANG 6
    if (isClear >0) {
        int xstep= cell._x- radius-CLEAR_RANG,ystep= cell._y- radius-CLEAR_RANG,wstep=radius*2+CLEAR_RANG*2,hstep= radius*2+CLEAR_RANG*2;
       _checkClear(xstep,ystep,wstep,hstep);
        _layerBorder->updateBorder(xstep,ystep,wstep,hstep);
    }
    return isClear;
}

