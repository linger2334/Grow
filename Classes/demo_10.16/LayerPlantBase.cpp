#include "LayerPlantBase.h"
#include "GameManager.h"
#include "MapGrid.h"
bool LayerPlantBase::init()
{
    GameLayerRollY::init();
    _gameManager = GameManager::getInstance();
    _mapGrid = _gameManager->getMapGrid();
    return true;
}
bool LayerPlantBase::isMapGridCrash(Vec2 pt,bool outRetCrash )
{
    Vec2 pn = pt + Vec2(0,_mapGrid->_mapGridStarty);
    GridCell cell = _mapGrid->getMapGridCellByPosition(pn);

    if(_mapGrid->isOutMapGrid(cell))
    {
        if(outRetCrash)return true;
        else return false;
    }
    
    return _mapGrid->getValue(cell._x, cell._y) ==255;
}

bool LayerPlantBase::isStoneCrash(Vec2 pt)
{
    return _gameManager->getPhysicsHandler()->isPointContact(pt);
}