#include "GameManager.h"

GameManager* GameManager::_sGameManager = nullptr;
void  GameManager::initGameInfo()
{
    _crashHelper = new CrashTestHelper();
//    pageScene = ScenePageView::create();
//    pageScene->retain();
    _isGameBegining =   false;
    _isGameRuning   =   false;
    
    _sceneGame = nullptr;
    
    auto director = Director::getInstance();
    _visible   = director->getWinSize();
    _mapGridUnitPixelSize  = Size(8,8);
    LayerBorder::initQuadMap();
}