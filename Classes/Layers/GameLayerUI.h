#ifndef __Grow_Demo_test__GameLayerUI__
#define __Grow_Demo_test__GameLayerUI__
#include "GameLayerBase.h"
class GameLayerUI : public GameLayerBase
{
public:
    CREATE_FUNC(GameLayerUI);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    //
    virtual bool onTouchBegan(Touch*,Event*);
    virtual void onTouchMoved(Touch*,Event*);
    virtual void onTouchEnded(Touch*,Event*);
    //virtual void onTouchCancelled(Touch*,Event*);
    
    virtual void update(float dt);
    virtual void moveDown(float yLen){}
    
    Sprite*                     _pause_btn;
    Sprite*                     _lightCountShow_left;
    Label*                      _lightCountShowNumber_left;
    Sprite*                     _lightCountShow_right;
    Label*                      _lightCountShowNumber_right;
    Sprite*                     _pause;
    LayerColor*                 _layerColor;
    bool                        _isTouch;
    
    cocos2d::Rect                        _pauseRect;
    cocos2d::Rect                        _toLogo;
    cocos2d::Rect                        _toFlower;
    cocos2d::Rect                        _toLevel;
};


#endif /* defined(__Grow_Demo_test__GameLayerUI__) */
