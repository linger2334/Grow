#include "GameManager.h"
#include "LayerPlant.h"
#include "ItemModel.h"
#include "LayerMapGrid.h"

GameManager* GameManager::_sGameManager = nullptr;
GameManager::Recycle GameManager::garbo;
void  GameManager::initGameInfo()
{
    _sLineItemModel = new ItemModel();
    _sLineItemModel->setType(111);
    _sPlantHeadItemModel = new ItemModel();
    _sPlantHeadItemModel->setType(99);
    
    pageScene = ScenePageView::create();
    pageScene->retain();
    _isGameBegining =   false;
    _isGameRuning   =   false;
    _sceneGame = nullptr;
    auto director = Director::getInstance();
    _visible   = director->getWinSize();
    _mapGridUnitPixelSize  = Size(8,8);
    LayerBorder::initQuadMap();
}
MapGrid*  GameManager::getMapGrid(){return &_layerMapGrid->_mapGrid;}

int  GameManager::getPlantCount()
{
    if(_nowRuningLevel<=0 || _nowRuningLevel>4)return 0;
    if(_nowRuningLevel== 1)return 1;
    return 2;
}
std::vector<ContorlPointV2>& GameManager::getCPListRefByIndex(int i)
{
   return ((LayerPlant_1*)_layerplant)->_plant->_cpLineNode._cpList;
}