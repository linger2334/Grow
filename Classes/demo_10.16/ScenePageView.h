
#ifndef __GrowUp__ScenePageView__
#define __GrowUp__ScenePageView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "AchievementUi.h"

#define winSize Director::getInstance()->getWinSize()
#define visible Director::getInstance()->getVisibleSize()

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class  Page: public PageView
{
public:
    CREATE_FUNC(Page);
    virtual void handleReleaseLogic(Touch *touch);
};

class ScenePageView : public Scene
{

public:
    CREATE_FUNC(ScenePageView);
    
    bool init();
    
    void onButtonClicked(Ref* pSender, Widget::TouchEventType type);
    
    
    
    //Sprite* _imageView;
    
private:
    Page* _pageView;
    
    
    
};


#endif