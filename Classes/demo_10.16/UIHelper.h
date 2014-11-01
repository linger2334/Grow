#ifndef __Grow_Demo__UIHelper__
#define __Grow_Demo__UIHelper__
#include "common.h"
#include "GameManager.h"
namespace UIHelper {
    Menu* createMenu();
    Menu* addBackMenuItem(Menu*);
    
    void layoutToCenter(Node* node,bool x,bool y);
}

#endif /* defined(__Grow_Demo__UIHelper__) */
