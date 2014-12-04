#ifndef __GrowDemo__GameSceneMain__
#define __GrowDemo__GameSceneMain__
#include "common.h"


class GameRunningInfo;
class GameSceneMain : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameSceneMain);
    virtual ~GameSceneMain();
    virtual  bool init();
    virtual  void update(float dt);
    virtual  void onEnter();
    virtual  void onEnterTransitionDidFinish();
    virtual  void onExit();
    virtual  void onExitTransitionDidFinish();
    void   initLevelEditorMenu();
    void    updateFirstGrowPlant(float dt);
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    //调试绘图
    Mat4 _modelViewMV;
    GameRunningInfo* _gameRunningInfo;
    CustomCommand _customCommand;
};


#endif /* defined(__GrowDemo__GameSceneMain__) */
