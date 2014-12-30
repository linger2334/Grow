#ifndef __Grow_beta_test02__GameSceneNavigation__
#define __Grow_beta_test02__GameSceneNavigation__
#include "common.h"

class GameSceneNavigation : public Scene
{
public:
    static  GameSceneNavigation* createScene();
    CREATE_FUNC(GameSceneNavigation);
    bool init();
    virtual void onEnter();
    virtual  void onEnterTransitionDidFinish();
};

#endif /* defined(__Grow_beta_test02__GameSceneNavigation__) */
