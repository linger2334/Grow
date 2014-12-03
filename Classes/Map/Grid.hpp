
#ifndef ProBase_Grid_hpp
#define ProBase_Grid_hpp

struct GridCell
{
    int _x;
    int _y;
    
    GridCell():
    _x(0), _y(0)
    {}
    
    GridCell(int x, int y):
    _x(x), _y(y)
    {}
    
    GridCell& operator=(const GridCell& rhs)
    {
        this->_x = rhs._x;
        this->_y = rhs._y;
        return *this;
    }
    
    bool operator==(const GridCell& rhs) const
    {
        return (_x == rhs._x && _y == rhs._y);
    }
    
    bool operator!=(const GridCell& rhs) const
    {
        return (_x != rhs._x|| _y != rhs._y);
    }
    
    bool operator<(const GridCell& rhs) const
    {
        if (_y != rhs._y)
            return (_y < rhs._y);
        else
            return (_x < rhs._x);
    }
    
    bool operator>(const GridCell& rhs) const
    {
        if (_y != rhs._y)
            return (_y > rhs._y);
        else
            return (_x > rhs._x);
    }
    GridCell operator+(const GridCell& rhs) const
    {
        GridCell cell= *this;
        cell._x += rhs._x;
        cell._y += rhs._y;
        return cell;
    }
    
};

template<class _Type>
class Grid
{
public:
    Grid();
    Grid(const int width,const int height);
    Grid(const Grid& grid);
    Grid& operator = (const Grid& grid);
   // Grid(const Grid&& grid);
    ~Grid();
    void resetGridData(const _Type& data)
    {
        if (!_grid) return;
        memset(_grid,data,_width*_height*sizeof(_Type));
    }
    void resize(const int width,const int height)
    {
        _width = width;
        _height = height;
        
        if(_grid)
        delete [] _grid;
        
        _grid = new _Type[width * height]();
    }
    void setDataByRang(int x,int y,int width,int height,const _Type& data)
    {
        if(x<0)x=0;
        if(y<0)y=0;
        width = x+width;
        height = y+height;
        if(width>=_width)width =_width;
        if(height>=_height)height =_height;
        for(;y<height;y++)
        {
            for(;x<width;x++)
            {
                setValueForGridCell(GridCell(x,y), data);
            }
        }
    }
    const _Type&   getValueForGridCell(const GridCell& gridCell)const
    {
        assert(gridCell._x < _width);
        assert(gridCell._y < _height);
        
        return _grid[gridCell._x + gridCell._y*_width];
    }

    const _Type& getValueForIndex(unsigned int i)const
    {
        assert( i < (_width * _height) );
        
        return _grid[i];
    }
    
    _Type& getValueForGridCell(const GridCell& gridCell)
    {
        assert(gridCell._x < _width);
        assert(gridCell._y < _height);
        
        return _grid[gridCell._x + gridCell._y*_width];
    }
    
    _Type& getValueForIndex(unsigned int i)
    {
        assert( i < (_width * _height) );
        
        return _grid[i];
    }
    
    void setValueForGridCell(const GridCell& gridCell, const _Type& value)
    {
        assert(gridCell._x < _width);
        assert(gridCell._y < _height);
        
        _grid[gridCell._x + gridCell._y*_width] = value;
    }
    
    void setValueForIndex(unsigned int i, const _Type& value)
    {
        assert( i < (_width * _height) );
        
        _grid[i] = value;
    }
    
    unsigned int getIndexForGridCell( const GridCell& g )const
    {
        return (g._x + (g._y*_width));
    }
    GridCell getGridCellFromIndex(unsigned int index)
    {
         assert( index < (_width * _height) );
        return GridCell(index%_width,index/_width);
    }
    void setGridMemoryToZero()
    {
        memset(_grid, 0, sizeof(_Type) * _width*_height);
    }
    _Type* getData()
    {
        return _grid;
    }
public:
    int									_width;
    int									_height;
    _Type*								_grid;
    
};

template <class _Type>
Grid<_Type>::Grid():
_width(0),
_height(0),
_grid(0)
{
}

template <class _Type>
Grid<_Type>::Grid(const int width, const int height):
_width(width),
_height(height),
_grid(new _Type[width * height]())
{
}

template <class _Type>
Grid<_Type>::Grid(const Grid& grid):
_width(grid._width),
_height(grid._height),
_grid(new _Type[grid._width * grid.height]())
{
    memcpy(_grid, grid._grid, (grid._width*grid.height * sizeof(_Type)));
}

template <class _Type>
Grid<_Type>& Grid<_Type>::operator=(const Grid<_Type>& grid)
{
    _width = grid._width;
    _height = grid._height;
    
    _grid = new _Type[grid._width * grid.height]();
    memcpy(_grid, grid._grid, (grid._width*grid.height * sizeof(_Type)));
    
    return *this;
}

template <class _Type>
Grid<_Type>::~Grid()
{
    if (_grid) {
         delete [] _grid;
    }
   
}

#endif
