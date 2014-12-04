#ifndef __Grow_Demo_test__GameRuningInfo__
#define __Grow_Demo_test__GameRuningInfo__
#include "common.h"
#include "Singleton.hpp"
#include "ContorlPoint.h"
#include <map>
#include "PlantHelper.h"
class GameLayerBase;

typedef   std::function<void(float heigth,int index)>  PlantHeightChangeCallback;
typedef   std::function<void(float heigth,int index)>  PlantCPChangeCallback;
typedef   std::function<void(int state,int oldState,int index)>  PlantChangeStateCallback;
typedef   std::function<void(const GridCell& cell)> ChangeCellCallBack;

struct GamePlantConfig
{
public:
    Texture2D*                 _texturePlantBody;
    Color4B                    _plantColor;
    std::list<Texture2D*>      _texturePlantLeaf;
    std::list<Color4B>         _texturePlantLeafColor;
    std::list<std::string>     _pathPlantGrowMusics;
    std::list<ContorlPointV2>  _plantStartContorlPointList;
    PlantHelper::PlantRadiusHelper _radiusList;
    std::function<float(float length)> _plantRadiusCallBack;
    float                      _growSpeed;
    float                      _reGrowSpeed;
    float                      _textureLeft;
    float                      _textureRight;
    int                        _state;
    Vec2                       _plantNodeInitPosition;
};
class GameRunningConfig : public Singleton<GameRunningConfig>
{
public:
    void  clearInfo();
    CC_SYNTHESIZE(Texture2D*,_textureMapDirt,TextureMapDirt);
    CC_SYNTHESIZE(Texture2D*,_textureMapBorder,TextureMapBorder);
    CC_SYNTHESIZE(Texture2D*,_textureBackground,TextureBackGround);
    CC_SYNTHESIZE(Texture2D*,_textureUIBorder,TextureUIBorder);
   
    CC_SYNTHESIZE(float, _mapDirtImageStartY, MapDirtImageStartY);
    CC_SYNTHESIZE(float, _backgroundImageStartY, BackgroundImageStartY);

    std::vector<ChangeCellCallBack> _changeMapCellCalls;
    std::vector<GamePlantConfig> _plantConfigs;
    std::vector<PlantHeightChangeCallback> _plantHeightChangeCallbacks;
    std::vector<PlantCPChangeCallback> _plantCPChangeCallbacks;
    std::vector<PlantChangeStateCallback> _plantChangeStateCallbacks;
    std::map<int,int>       _lightsInitConfigCount;
public:
    std::vector<GamePlantConfig>& getPlantConfigs(){return _plantConfigs;}
    void setPlantConfigs(const  std::vector<GamePlantConfig>& list){ _plantConfigs = list;}
    
    void setLightInitCount(int plantIndex,int count)
    {
        _lightsInitConfigCount[plantIndex]=count;
    }
    int  getLightInitCount(int plantIndex)
    {
        return _lightsInitConfigCount[plantIndex];
    }
};

class GameSceneMain;
//游戏运行时的 必须信息
class GameRunningInfo : public Singleton<GameRunningInfo>
{
public:
    void initGameRunningInfo()
    {
        _isGameStart = false;
        _isGamePause = false;
        //_gameSceneNode = nullptr;
        _gamePassHeight = 0;
        _isGameEnd = false;
        _plantIsNeedGrow = true;
    }
    void resetGameRunningInfo();
    bool isHasGameLayer(const std::string layerName);
    GameLayerBase* getGameLayer(const std::string layerName);
    void addGameLayer(const std::string layerName,GameLayerBase*);
    void removeGameLayer(const std::string layerName);
    
    void removeAllGameLayer();
    
    bool  isPause(){return _isGamePause;}
    void  pauseGame(){_isGamePause = true;}
    void  reStartGame(){_isGamePause = false;}
    
    bool  isPlantNeedGrow(){ return _plantIsNeedGrow;}
    
    bool isGameStart()
    {
        return _isGameStart;
    }
    
    bool isHashGameScnenNode()
    {
        return _gameSceneNode != nullptr;
    }
    CC_SYNTHESIZE(bool, _isGameStart, IsGameStart);
    CC_SYNTHESIZE(bool, _isGamePause,IsGamePause);
    CC_SYNTHESIZE(bool ,_isGameEnd, IsGameEnd);
    CC_SYNTHESIZE(Node*, _gameSceneNode, GameSceneNode);
    CC_SYNTHESIZE(bool,_plantIsNeedGrow,PlantIsNeedGrow);
public:
    float _gamePassHeight;
    std::map<std::string,GameLayerBase*> _runningLayers;
};

#endif /* defined(__Grow_Demo_test__GameRuningInfo__) */
