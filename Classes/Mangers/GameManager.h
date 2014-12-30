#ifndef __PlantOri__GameManager__
#define __PlantOri__GameManager__
#include "common.h"
#include "GameLayers.h"
#include "Singleton.hpp"
#include "GamePhysicalWorld.h"
#include "LevelFileHandler.h"
#include "PhysicsHandler.h"
#include "GameRunningInfo.h"

class GameSceneMain;
class LayerItem;
class LevelEditor;

enum
{
    SceneNoSelect = -1,
    ScenePageView = 0 ,
    SceneLogo,
    SceneLoding,
    SceneGameScene,
    SceneFlower,
    SceneGameWin,
};
class GameManager :public Singleton<GameManager>
{
public:
    b2World* getBox2dWorld()
    {
        return GamePhysicalWorld::getInstance()->getBox2dWorld();
    }
    PhysicsHandler* getPhysicsHandler()
    {
       return  GamePhysicalWorld::getInstance()->_physicsHandler;
    }
    
    void initGameManagerInfo()
    {
        _fileHandler = nullptr;
        _gameScne = nullptr;
        _gameLevel = 0;
    }
    void  navigationTo(int id);
    void  navigationToGameScene(int level);
    void  navigationToGameScene();
    void  navigationToPasueGameScene();
    void  navigationTo(Scene* scene);
    
    bool  isHasRunningGame()
    {
        return _gameScne != nullptr;
    }
    
    void  releaseGameScene();
    
    void  createNewGameScene();
    
    int   getPauseGameSceneLevel()
    {
        return _gameLevel;
    }
    bool    isPause(){ return GameRunningInfo::getInstance()->isPause(); }
    void    pauseGame();
    void    reStartGame();
    
    bool    isGameStop()
    {
        return  !GameRunningInfo::getInstance()->getIsGameStart();
    }
    void    stopGame()
    {
        GameRunningInfo::getInstance()->setIsGameStart(false);
    }
    void    startGame()
    {
        GameRunningInfo::getInstance()->setIsGameStart(true);
        GameRunningInfo::getInstance()->getGameSceneNode()->scheduleUpdate();
    }
    
    void  safeReleaseFileHandler()
    {
        CC_SAFE_RELEASE_NULL(_fileHandler);
    }
    
    void  clearGameLevelInfo();
    void  clearRunningInfo();
    
    void  saveActions();
    void  reStartActions();
    
    LevelEditor* _levelEditor;
    float editor_width;
    float editor_height;
    float editor_contentscale;
    
    std::string _levelFileName;
    LevelFileHandler* _fileHandler;
    Scene* _gameScne;
    int    _gameLevel;
      Vector<Node*>   _pauseNodes;
};


#endif /* defined(__PlantOri__GameManager__) */
