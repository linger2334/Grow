#ifndef __PlantOri__GameManager__
#define __PlantOri__GameManager__
#include "common.h"
#include "GameLayers.h"
#include "Singleton.hpp"
#include "GamePhysicalWorld.h"
#include "LevelFileHandler.h"
#include "PhysicsHandler.h"
#include "GameRunningInfo.h"
#include "Box2d/Box2d.h"

class GameSceneMain;
class LayerItem;
class LevelEditor;

enum
{
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
    GameManager();
    ~GameManager();
    
    b2World* getBox2dWorld()
    {
        return _world;
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
    
    void  releaseRunningGameInfo();
    void  releaseGameScene();
    
    void resetGameContext();
    
    int   getPauseGameSceneLevel()
    {
        return _gameLevel;
    }
    
    LayerItem* getLayerLight();

    
    void  initGameRunningInfo();
    
    bool    isPause(){ return GameRunningInfo::getInstance()->isPause(); }
    void    pauseGame(){GameRunningInfo::getInstance()->pauseGame();}
    void    reStartGame(){GameRunningInfo::getInstance()->reStartGame();}
    
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
    LevelFileHandler* _fileHandler;
    
    Scene* _gameScne;
    int    _gameLevel;
    
////////////add for leveleditor
    LevelEditor* _levelEditor;
    float editor_width;
    float editor_height;
    float editor_contentscale;
    b2World* _world;
    
private:
    class Recycle{
    public:
        ~Recycle()
        {
            if(_sOneObject){
                delete _sOneObject;
                _sOneObject = nullptr;
            }
        }
    };
    static Recycle garbo;
//////////////////////////////////
};


#endif /* defined(__PlantOri__GameManager__) */
