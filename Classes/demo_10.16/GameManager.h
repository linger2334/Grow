#ifndef __PlantOri__GameManager__
#define __PlantOri__GameManager__

#include "MapGrid.h"
#include "ScenePageView.h"
#include "GameLogo.h"
#include "ContorlPoint.h"
#include "AchievementUi.h"
#include "LayerBorder.h"
#include "Box2D/Box2D.h"
#include "PhysicsHandler.h"

class SceneGame;
class LayerPlantBase;
class LightNodeListHelper;
class LayerLight;
class LayerBorder;
class LayerUI;
class LayerUIBorder;
class LayerMapGrid;
class LayerItem;
#define MAP_GRID_SUB_COUNT 60


//add for LevelEditor
#include "LevelFileHandler.h"
class LevelEditor;
class ItemModel;
class GameManager
{
public:
    GameManager(){
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

    
//////////////////////////////////////////////////////////////
    
    void  initGameInfo();
    
    void releaseGameInfo()
    {

        if(pageScene)
        {
            pageScene->release();
            pageScene = nullptr;
        }
    }
    
    
//////////////////////////////////////////////////////////////
    
    void  navigationToPageView()
    {
        navigationTo(pageScene);
    }
    void  navigationToLogo()
    {
        navigationTo(GameLogo::create());
    }

    void navigationToGameScene();
    
    void  navigationToFlowerScene()
    {
        Director::getInstance()->replaceScene(AchievementUi::createScene());
    }
    void  navigationTo(Scene* scene)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene, Color3B(0,0,0)));
    }

///////////////////////////////////////////////////////////////

    MapGrid*  getMapGrid();
    void      setMapGrid(MapGrid* mapGrid);
    
    ScenePageView* getScenePageView(){ return pageScene;}

    cocos2d::Size  getVisibleSize(){return _visible;}
    void  setVisibleSize( cocos2d::Size size){ _visible = size;}
    cocos2d::Size  getPixelSize(){return _pixelSize;}
    void  setPixelSize( cocos2d::Size size){ _pixelSize = size;}
    cocos2d::Size  getMapGridUnitVisibleSize()
    {
        return  cocos2d::Size(_visible.width / _mapGridUnitPixelSize.width,
                    _visible.height / _mapGridUnitPixelSize.height);
    }
    
    cocos2d::Size  getMapGridUnitSize(){return _mapGridUnitPixelSize;}
    
    SceneGame* getGameScene(){return _sceneGame;}
    
    LayerPlantBase* getLayerPlant(){return _layerplant;}

    
    LightNodeListHelper* getLightListHelper(){return _lightListHelper;}
    
    
    LayerUI*         getLayerUI(){return _layerUI;}
    void             setLayerUI(LayerUI* layerui){_layerUI = layerui;}
    
    LayerUIBorder*   getLayerUIBorder(){return  _layerUIBorder;}
    void             setLayerUIBorder(LayerUIBorder* layerUIBorder)
    { _layerUIBorder = layerUIBorder;}
    
    LayerMapGrid*    getLayerMapGrid(){return _layerMapGrid;}
    void             setLayerMapGrid(LayerMapGrid* layerMapGrid)
    {_layerMapGrid = layerMapGrid;}
    
    
    b2World* getBox2dWorld(){return _world;}
    void     setBox2dWorld(b2World* world){_world = world;}
    
    LayerBorder*  getLayerBorder(){return _layerBorder;}
    void  pasueGame();
    
    LayerLight* getLayerLight(){return _layerLight;}
    
    int  getPlantCount();
    std::vector<ContorlPointV2>& getCPListRefByIndex(int i);
    
    
    
    bool    isPause(){return _isGamePause;}
    void    pauseGame(){_isGamePause = true;}
    void    reStartGame(){_isGamePause = false;}
    
    bool       _isGameBegining;
    bool       _isGameRuning;
    bool       _isGamePause;
    
    
    ScenePageView*  pageScene;
    SceneGame*      _sceneGame;
    LightNodeListHelper* _lightListHelper;

    
    
    cocos2d::Size       _visible;
    cocos2d::Size       _pixelSize;
    cocos2d::Size       _mapGridUnitPixelSize;
    std::function<bool(cocos2d::Vec3)> _dirtCrashTestCallback;
    std::function<bool(cocos2d::Vec2,int type)>  _plantGrowCrashTestCallback;
    std::function<int(cocos2d::Vec2)>  _mapCrashTestCallBack;
    
    LayerPlantBase*     _layerplant;
    LayerLight*         _layerLight;
    LayerBorder*        _layerBorder;
    LayerUI*            _layerUI;
    LayerUIBorder*      _layerUIBorder;
    LayerMapGrid*       _layerMapGrid;
    LayerItem*          _layerItem;
    
    int                 _nowRuningLevel;
    b2World*            _world;
    
    ItemModel*          _sLineItemModel;
    ItemModel*          _sPlantHeadItemModel;
    static GameManager* _sGameManager;
    
    
    //add for LevelEditor
    LevelFileHandler* _fileHandler;
    LevelEditor* _levelEditor;
    float editor_width;
    float editor_height;
    float editor_contentscale;
    
    inline void setPhysicsHandler(PhysicsHandler* physicsHandler) { _physicsHandler = physicsHandler;}
    inline PhysicsHandler* getPhysicsHandler() { return _physicsHandler;}
    PhysicsHandler* _physicsHandler;
    //add for memory release
private:
    class Recycle{
    public:
        ~Recycle()
        {
            if(_sGameManager){
                _sGameManager->releaseGameInfo();
                delete _sGameManager;
            }
        }
    };
    static Recycle garbo;
};


#endif /* defined(__PlantOri__GameManager__) */
