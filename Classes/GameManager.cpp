#include "GameManager.h"
#include "LayerPlant.h"
GameManager* GameManager::_sGameManager = nullptr;
void  GameManager::initGameInfo()
{
    _crashHelper = new CrashTestHelper();
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

int  GameManager::getPlantCount()
{
    if(_nowRuningLevel<=0 || _nowRuningLevel>4)return 0;
    if(_nowRuningLevel== 1)return 1;
    return 2;
}
std::vector<ContorlPointV2>& GameManager::getCPListRefByIndex(int i)
{
    return _layerplant->_plant->_cpLineNode._cpList;
}