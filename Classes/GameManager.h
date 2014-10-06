#ifndef __PlantOri__GameManager__
#define __PlantOri__GameManager__
#include "LayerMap.h"
#include "CrashTestHelper.h"
#include "MapGrid.h"
#include "LevelFileHandler.h"
//#include "ScenePageView.h"
//#include "GameLogo.h"
//#include "GameSceneTest.h"
#include "LayerBorder.h"

class SceneGame;
class LayerPlant_1;
class LightNodeListHelper;
#define MAP_GRID_SUB_COUNT 60
//add for LevelEditor
class LevelEditor;
/////////////////////////////////
class GameManager
{
public:
    GameManager(){
    }
    ~GameManager(){
        CC_SAFE_RELEASE(_fileHandler);
    }
    
  static   GameManager* getInstance()
    {
        if (!_sGameManager)
        {
            _sGameManager = new GameManager();
            _sGameManager->initGameInfo();
        }
        return _sGameManager;

    }
    
  static   void releaseSGameManager()
  {
      if (_sGameManager)
      {
          _sGameManager->releaseGameInfo();
          delete _sGameManager;
      }
  }
    
    LayerMap* getLayerMap(){return _map;}
    
//////////////////////////////////////////////////////////////
    
    void  initGameInfo();
    
    void releaseGameInfo()
    {
        if(_crashHelper)delete _crashHelper;
//
//        if(pageScene)
//        {
//            pageScene->release();
//            pageScene = nullptr;
//        }
    }
    
    
//////////////////////////////////////////////////////////////
    
    void  navigationToPageView()
    {
//        navigationTo(pageScene);
    }
    void  navigationToLogo()
    {
//        navigationTo(GameLogo::create());
    }
    void  navigationToGameTestScene()
    {
//        navigationTo(GameSceneTest::createScene());
    }
    
    void  navigationToFlowerScene()
    {
       // navigationTo(AchievementUi::createScene());
//        Director::getInstance()->replaceScene(AchievementUi::createScene());
       
    }
    void  navigationTo(Scene* scene)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene, Color3B(0,0,0)));
    }

///////////////////////////////////////////////////////////////
    CrashTestHelper* getCrashTestHelper(){return _crashHelper;}
    void    setCrashTestHelper(CrashTestHelper* crash){_crashHelper = crash;}
    MapGrid*  getMapGrid(){return _mapGrid;}
    void      setMapGrid(MapGrid* mapGrid){_mapGrid = mapGrid;}
    
    
//    ScenePageView* getScenePageView(){ return pageScene;}

    cocos2d::Size  getVisibleSize(){return _visible;}
    void  setVisibleSize(cocos2d::Size size){ _visible = size;}
    cocos2d::Size  getPixelSize(){return _pixelSize;}
    void  setPixelSize(cocos2d::Size size){ _pixelSize = size;}
    cocos2d::Size getMapGridUnitVisibleSize()
    {
        return cocos2d::Size(_visible.width / _mapGridUnitPixelSize.width,
                    _visible.height / _mapGridUnitPixelSize.height);
    }
    
    cocos2d::Size  getMapGridUnitSize(){return _mapGridUnitPixelSize;}
    
    SceneGame* getGameScene(){return _sceneGame;}
    
     LayerPlant_1* getLayerPlant(){return _layerplant;}
    LightNodeListHelper* getLightListHelper(){return _lightListHelper;}
    
    
    void  pasueGame();
    
//    ScenePageView*  pageScene;
    
    SceneGame*      _sceneGame;
    
    LightNodeListHelper* _lightListHelper;
    
    bool       _isGameBegining;
    bool       _isGameRuning;
    
    
    cocos2d::Size       _visible;
    cocos2d::Size       _pixelSize;
    cocos2d::Size       _mapGridUnitPixelSize;

    MapGrid*   _mapGrid;
    std::function<bool(cocos2d::Vec3)> _dirtCrashTestCallback;
    
    std::function<bool(cocos2d::Vec2)> _plantGrowCrashTestCallback;
    LayerMap*  _map;
    LayerPlant_1* _layerplant;
    CrashTestHelper* _crashHelper;
    static GameManager* _sGameManager;
    
//add for LevelEditor
    LevelFileHandler* _fileHandler;
    LevelEditor* _levelEditor;
    float editor_width;
    float editor_height;
    float editor_contentscale;
    float scrollViewOffset;
    
};


#endif /* defined(__PlantOri__GameManager__) */
