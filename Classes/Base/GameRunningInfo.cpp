#include "GameRunningInfo.h"
#include "GameLayerBase.h"
#include "GameManager.h"

void   GameRunningConfig::clearInfo()
{
    _textureMapDirt=nullptr;
    _textureMapBorder =nullptr ;
    _textureBackground =nullptr;
    _textureUIBorder =nullptr;
    
    _mapDirtImageStartY=0;
    _backgroundImageStartY=0;
    _changeMapCellCalls.clear();
    _plantConfigs.clear();
    _plantHeightChangeCallbacks.clear();
    _plantCPChangeCallbacks.clear();
    _plantChangeStateCallbacks.clear();
}


void GameRunningInfo::resetGameRunningInfo()
{
    _isGameStart = false;
    _isGamePause = false;
    _gamePassHeight = 0;
    _isGameEnd = false;
}


bool GameRunningInfo::isHasGameLayer(const std::string layerName)
{
    return getGameLayer(layerName) == nullptr ? false : true;
}

GameLayerBase* GameRunningInfo::getGameLayer(const std::string layerName)
{
    GameLayerBase* findIt = nullptr;
    auto ip = _runningLayers.find(std::string(layerName));
     if(ip != _runningLayers.end())
     {
         findIt = ip->second;
     }
    return findIt;
}
void GameRunningInfo::addGameLayer(const std::string layerName,GameLayerBase* layer)
{
    if(!layer)return ;
    //layer->retain();
    removeGameLayer(layerName);
    _runningLayers.insert(std::make_pair(layerName, layer));
}
void GameRunningInfo::removeGameLayer(const std::string layerName)
{
    auto ip = _runningLayers.find(std::string(layerName));
    if(ip != _runningLayers.end())
    {
        ip=_runningLayers.erase(ip);
    }
}
void GameRunningInfo::removeAllGameLayer()
{
    _runningLayers.clear();
}