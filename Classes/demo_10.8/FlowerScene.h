#ifndef __GrowUp__FlowerScene__
#define __GrowUp__FlowerScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


#define winSize Director::getInstance()->getWinSize()
#define visible Director::getInstance()->getVisibleSize()

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class FlowerScene : public Scene
{
    
public:
    CREATE_FUNC(FlowerScene);
    
    bool init();

    
};

#endif