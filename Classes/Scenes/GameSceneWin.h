
#ifndef __Grow_beta_test02__GameSceneWin__
#define __Grow_beta_test02__GameSceneWin__
#include "common.h"
#include "CocosGUI.h"

class GameSceneWin : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameSceneWin);
    virtual  bool init();
    void addAnimation();
    void addMenuUI();
protected:
    ui::ScrollView* _textView;
    Vec2 startPosition;
};

#endif /* defined(__Grow_beta_test02__GameSceneWin__) */
