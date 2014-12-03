
#ifndef __Grow_beta_test02__SceneChangeSubMap__
#define __Grow_beta_test02__SceneChangeSubMap__
#include "common.h"
class SceneChangeSubMap : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(SceneChangeSubMap);
    virtual void onEnterTransitionDidFinish();
};
#endif /* defined(__Grow_beta_test02__SceneChangeSubMap__) */
