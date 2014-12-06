//
//  LevelsMenu.h
//  GrowUp
//
//  Created by wlg on 9/16/14.
//
//

#ifndef __GrowUp__LevelsMenu__
#define __GrowUp__LevelsMenu__

#include <iostream>
#include "CocosGUI.h"
#include "Macro.h"

class LevelsMenu:public Layer
{
public:
    LevelsMenu();
    ~LevelsMenu();
    static Scene* createScene();
    CREATE_FUNC(LevelsMenu)
    bool init();
    
    void selectLevel(Ref* sender,cocos2d::ui::ListView::EventType eventtype);
protected:
    char fileExist[1000];
    int consecutiveNumbers;
    int sumCount1;
    int sumCount2;
    int sumCount3;
    int sumCount4;
    
};







#endif /* defined(__GrowUp__LevelsMenu__) */
