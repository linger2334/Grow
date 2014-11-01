
#ifndef __GrowUp__GameLogo__
#define __GrowUp__GameLogo__

#include "cocos2d.h"

USING_NS_CC;

class GameLogo : public cocos2d::Scene
{
public:
    //GameMenu():m_numSp(20),m_loadedSp(0),loadProgress(NULL){};
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    //void gotoNewLayer();//加载完后的跳转函数
    CREATE_FUNC(GameLogo);
    
    Sprite* _circlebackground;
    Sprite* _logo;
    Sprite* _background;
    
//    virtual bool onTouchBegan(Touch* touch,Event* event);
//    virtual bool onTouchMoved(Touch* touch,Event* event);
//    virtual bool onTouchEnded(Touch* touch,Event* event);
    
private:
//    cocos2d::ProgressTimer* loadProgress;//进度条
//    
//    
//    int m_numSp;//要加载的精灵数目,初始化为 20 张
//    int m_loadedSp;//已加载的精灵数目
};


#endif


