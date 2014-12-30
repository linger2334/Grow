#ifndef __Grow_Beta__GameSceneTest__
#define __Grow_Beta__GameSceneTest__
#include "common.h"
class GameSceneTest : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameSceneTest);
    virtual  bool init();

protected:
};

#endif /* defined(__Grow_Beta__GameSceneTest__) */
