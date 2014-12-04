#ifndef __PlantOri__MapGrid__
#define __PlantOri__MapGrid__
#include "common.h"
#include "Grid.hpp"

class MapGrid
{
public:
    MapGrid():_gridData(nullptr),_gridWidth(0),_gridHeight(0),_startHeight(0),
    _maxViewHeight(0),_unitGridSize(cocos2d::Size(8,8)),_mapGridStarty(0){}
    ~MapGrid()
    {
        if (_gridData) {
            delete [] _gridData;
        }
    }
    MapGrid(const int width,const int height,const int maxViewHeight):_gridData(nullptr),_startHeight(0),_maxViewHeight(maxViewHeight)
    {
        resize(width,height);
    }

    void resize(const int width,const int height)
    {
        _gridWidth = width;
        _gridHeight = height;
        if(_gridData)
            delete [] _gridData;
        _gridData = new unsigned char[_gridWidth * _gridHeight]();
    }
    void setData(int x, int y,unsigned char data)
    {
      
        for(int i = 0;i<_gridWidth*_gridHeight;i++)_gridData[i]=data;
    }
    void setData(int x,int y,int width ,int height,unsigned char data)
    {
        int ipx,ipy,endx,endy;
        ipx = x, ipy = y, endx = ipx + width, endy = ipy + height;
        if(ipx < 0)ipx = 0;
        if(ipy < 0)ipy = 0;
        if(endx >= _gridWidth)endx = _gridWidth;
        if(endy >= _gridHeight)endy = _gridHeight;
        for(;ipy <= endy; ipy++)
        {
            for(int tipx = ipx ; tipx <= endx; tipx++)
            { _gridData[ipy*_gridWidth+tipx]=data;}
        }
    }
 //////////////////////////////////////////////////////////////////
    const unsigned char   getValueForGridCell(const GridCell& gridCell)const
    {
        return getValue(gridCell._x,gridCell._y);
    }
    
    const unsigned char   getValue(int x,int y)const
    {
        return _gridData[x + y*_gridWidth];
    }
    const unsigned char   getValueCheckRange(int x,int y)const
    {
        if(isOutMapGrid(x,y))return 0;
        
        return _gridData[x + y*_gridWidth];
    }
    
    
    void   setValueForGridCell(const GridCell& gridCell,unsigned char value)const
    {
         setValue(gridCell._x,gridCell._y,value);
    }
    
    void  setValue(int x,int y,unsigned char value)const
    {
         _gridData[x + y*_gridWidth]=value;
    }
    
    const unsigned char&   getValueForGridCellInView(const GridCell& gridCell)const
    {
        return getValue(gridCell._x,gridCell._y);
    }
    
    const unsigned char&   getValueInView(int x,int y)const
    {
        return _gridData[x + (y+_startHeight)*_gridWidth];
    }
    
    void   setValueForGridCellInView(const GridCell& gridCell,unsigned char value)const
    {
        setValue(gridCell._x,gridCell._y,value);
    }
    
    void  setValueInView(int x,int y,unsigned char value)const
    {
        _gridData[x + (y+_startHeight)*_gridWidth]=value;
    }
    
    GridCell gridCellToInViewCell(GridCell cell)
    {
        return gridCellToInViewCell(cell._x,cell._y);
    }
    GridCell gridCellToInViewCell(int x,int y)
    {
        return GridCell(x,y-_startHeight);
    }
    
    GridCell inViewGridCellToCell(GridCell cell)
    {
        return inViewGridCellToCell(cell._x,cell._y);
    }
    GridCell inViewGridCellToCell(int x,int y)
    {
        return GridCell(x,y+_startHeight);
    }
///////////////////////////////////////////////////////////////
    
    GridCell getGridCellByView(int x,int y)
    {
        return  GridCell(x,y+_startHeight);
    }
    GridCell getGridCellByView(const GridCell& viewCell)
    {
        return getGridCellByView(viewCell._x, viewCell._y);
    }
/////////////////////////////////////////////////////////////
    bool isGridCellInView(int x,int y)
    {
        return y >= _startHeight &&
              y < (_startHeight+_maxViewHeight) &&
              x >= 0 &&
              x < _gridWidth;
    }
    bool isGridCellInView(const GridCell& cell)
    {
        return  isGridCellInView(cell._x, cell._y);
    }
    
    bool isOutMapGrid(int x, int y) const 
    {
        return y < 0 ||
        y >= _gridHeight ||
        x < 0 ||
        x >= _gridWidth;
    }
    bool isOutMapGrid(const GridCell& cell)
    {
        return isOutMapGrid(cell._x, cell._y);
    }
    
    bool isViewTopOutMapGrid()
    {
        return isOutMapGrid(0,_startHeight+_maxViewHeight);
    }
    
    bool isNextOriginYOutMapGrid(int x)
    {
        int y = _startHeight + x;
        return x > 0 ? y >= _gridHeight : y < 0 ;
    }
///////////////////////////////////////////////////////////////
    Vec2 getPositionByMapGridCell(int x,int y)
    {
        return Vec2(x*_unitGridSize.width,(y-_startHeight)*_unitGridSize.height-_mapGridStarty);
    }
    Vec2 getPositionByMapGridCell(GridCell cell)
    {
        return getPositionByMapGridCell(cell._x,cell._y);
    }
    
    GridCell getMapGridCellByPosition(float x, float y)
    {
        return GridCell(x/_unitGridSize.width,y/_unitGridSize.height+_startHeight);
    }
    
    GridCell getMapGridCellByPosition(Vec2 point)
    {
        return getMapGridCellByPosition(point.x,point.y);
    }
    
    GridCell getMapGridCellByPositionInView(float x, float y)
    {
        return GridCell(x/_unitGridSize.width,y/_unitGridSize.height);
    }
    
    GridCell getMapGridCellByPositionInView(Vec2 point)
    {
        return getMapGridCellByPosition(point.x,point.y);
    }
    

///////////////////////////////////////////////////////////////
    void  setOriginY(unsigned int newY)
    {
        _startHeight = newY;
    }

    unsigned char*  getDataByOriginY()
    {
        return _gridData+_gridWidth*_startHeight;
    }
    unsigned char*  getDataByRow(int y)
    {
          return _gridData+_gridWidth*y;
    }
//////////////////////////////////////////////////////////////
    unsigned int  getGridWidth(){return _gridWidth;}
    unsigned int  getGridHeight(){return _gridHeight;}
    unsigned int  getGridMaxViewHeight(){return _maxViewHeight;}

//////////////////////////////////////////////////////////////
    
    cocos2d::Size   _unitGridSize;
    float           _mapGridStarty;
    unsigned int    _maxViewHeight;
    unsigned int    _startHeight;
    unsigned int    _gridWidth;
    unsigned int    _gridHeight;
    unsigned char*  _gridData;
};

#endif /* defined(__PlantOri__MapGrid__) */
