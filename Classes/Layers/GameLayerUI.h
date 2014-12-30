#ifndef __Grow_Demo_test__GameLayerUI__
#define __Grow_Demo_test__GameLayerUI__
#include "GameLayerBase.h"
#include "ScreenEnergy.h"

class GameLayerUI : public GameLayerBase ,public  GameLayerHelper<GameLayerUI>
{
public:
    CREATE_FUNC(GameLayerUI);

    virtual bool init();
    virtual bool initGameInfo();
    virtual void onEnter();
    virtual void onExit();
    //
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //virtual void onTouchCancelled(Touch*,Event*);
    void  showLinght();
    virtual void update(float dt);
    virtual void moveDown(float yLen){}
    
    void showWaitLightUI();
    void noShowWaitLightUI()
    {
        _isShowWaitLightUI = false;
    }
    
    void  updateLightShow();
    
    void  pauseGame();
    void  reStartGame();
    int                         _level_id;
    
    Sprite*                     _level_2_btn;
    cocos2d::Rect                        _level_2_Rect;
    Sprite*                     _pause_btn;
    Sprite*                     _lightCountShow_left;
    Label*                      _lightCountShowNumber_left;
//    Sprite*                     _lightCountShow_right;
//    Label*                      _lightCountShowNumber_right;
    Sprite*                     _pause;
    LayerColor*                 _layerColor;
    
    Label*                      _lengthShow_left;
    Label*                      _lengthShow_right;
    bool                        _isTouch;
    
    cocos2d::Rect               _pauseRect;
    cocos2d::Rect               _toLogo;
    cocos2d::Rect               _toFlower;
    cocos2d::Rect               _toLevel;
    cocos2d::Rect               _pauseBtnRect;
    bool                        _isShowWaitLightUI;
   // ScreenEnergy*     _layerAddLights;
};


#endif /* defined(__Grow_Demo_test__GameLayerUI__) */
