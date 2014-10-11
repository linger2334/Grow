#include "UIHelper.h"

namespace UIHelper{
    
    void layoutToCenter(Node* node,bool x,bool y)
    {
        Size visibleSize = Director::getInstance()->getWinSize();
        if(x)node->setPositionX(visibleSize.width/2);
        if(y)node->setPositionY(visibleSize.height/2);
    }
    
}